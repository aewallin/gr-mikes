/* -*- c++ -*- */
/*
 * Copyright 2023 KH.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <gnuradio/math.h>
#include <gnuradio/io_signature.h>
#include <iostream>
#include <iomanip>
#include <numeric>


#include "freq_counter_impl.h"

//  Fancy way of getting constants
//  constexpr double am_pi = std::acos(-1);
//  constexpr double am_2pi = 2.0*std::acos(-1);

constexpr double m_pi = 3.141592653589793238462643;
constexpr double m_2pi = 2.0*3.141592653589793238462643;

namespace gr {
namespace mikes_oot {

using input_type = gr_complex;
using output_type = double;
freq_counter::sptr freq_counter::make(size_t vec_len,
                                                              int samp_rate,
                                                              float retune_threshold,
                                                              bool auto_tune,
                                                              int tune_avg,
                                                              int uhd_channel,
                                                              bool baseband) {
    return gnuradio::make_block_sptr<freq_counter_impl>(vec_len,
                                                                    samp_rate,
                                                                    retune_threshold,
                                                                    auto_tune,
                                                                    tune_avg,
                                                                    uhd_channel,
                                                                    baseband);
}



/*
 * The private constructor
 */
freq_counter_impl::freq_counter_impl(size_t vec_len,
                                                              int samp_rate,
                                                              float retune_threshold,
                                                              bool auto_tune,
                                                              int tune_avg,
                                                              int uhd_channel,
                                                              bool baseband)
    : gr::sync_block("freq_counter",
                     gr::io_signature::make( 1  /* min inputs */,
                                             1  /* max inputs */,
                                             sizeof(input_type)*vec_len),
                     gr::io_signature::makev(1,  // min outputs
                                             8,  // max outputs
                                             { sizeof(output_type),  // 8 outputs
                                               sizeof(output_type),
                                               sizeof(output_type),
                                               sizeof(output_type),
                                               sizeof(output_type),
                                               sizeof(output_type),
                                               sizeof(output_type),
                                               sizeof(output_type)})),
                                               d_vlen(vec_len),
                                               d_samp_rate(samp_rate),
                                               d_retune_threshold(retune_threshold),
                                               d_auto_tune(auto_tune),
                                               d_tune_avg(tune_avg),
                                               d_uhd_channel(uhd_channel),
                                               d_baseband(baseband),
                                               d_f_lo(0.0),
                                               d_i_avg(0),
                                               d_f_pi_y(0.0),
                                               d_f_lambda(0.0),
                                               d_f_omega_w(0.0),
                                               d_omegaC(0.0),
                                               d_omegaD(0.0) {
    // Make sure that we have access to 2-1 = 1 vectors of past data
    this->set_history(2);
    message_port_register_out(pmt::intern("retune_cmd"));

    // Averaging time in s
    d_tau = static_cast<double>(d_vlen)/static_cast<double>(d_samp_rate);
    double d_tau0 = static_cast<double>(1.0) / static_cast<double>(d_samp_rate);


    // Memory allocations
    // Omega counter window
    d_omega_win = new double[d_vlen];
    d_omega_winC = new double[d_vlen];  // Danielson (27)
    d_omega_winD = new double[d_vlen];  // Danielson (28)
    d_delta_omega = 12.0/((d_tau0)*static_cast<double>(d_vlen)*(static_cast<double>(d_vlen)-1.0)*(static_cast<double>(d_vlen)+1.0));  // Danielson (21)

    d_phi = new double[d_vlen*2];  // Phase
    i_n_uw = new int[d_vlen*2];    // Unwrapped phase

    // omega counter window
    for (size_t i = 0; i < d_vlen; i++) {
        d_omega_win[i] = (static_cast<double>(-(static_cast<double>(d_vlen)-1.0)/2.0 + static_cast<int>(i)))*d_delta_omega;  // Danielson (21)
        d_omega_winC[i] = 1.0;
        d_omega_winD[i] = static_cast<double>(static_cast<int>(i))/(static_cast<double>(d_vlen) - 1.0);  // note difference from Danielson (28)!
    }

    /* debug output
    std::cout<<"lambda weight"<<d_tau<<" "<<d_lambda_weight<<std::endl;
    std::cout<<"omega prefactor"<<d_tau<<" "<< d_delta_omega<<std::endl;  // d_tau=1, d_delta_omega=1.2e-11
    std::cout<<"omega win[0] "<< d_omega_win[0] <<std::endl;
    std::cout<<"omega win[N-1] "<< d_omega_win[d_vlen-1] <<std::endl;
    */
}  // end constructor


/*
 * Our virtual destructor.
 */
freq_counter_impl::~freq_counter_impl() {}

int freq_counter_impl::work(int noutput_items,
                                        gr_vector_const_void_star& input_items,
                                        gr_vector_void_star& output_items) {
    auto in = static_cast<const input_type*>(input_items[0]);
    auto out_pi_y = static_cast<output_type*>(output_items[0]);
    auto out_lambda = static_cast<output_type*>(output_items[1]);
    auto out_omega_w = static_cast<output_type*>(output_items[2]);

    if (noutput_items > 1)
      std::cout << "noutput_items > 1: " << noutput_items << std::endl;

    auto out_omega_C = static_cast<output_type*>(output_items[3]);
    auto out_omega_D = static_cast<output_type*>(output_items[4]);
    auto out_rf_LO = static_cast<output_type*>(output_items[5]);

    for (int i_input = 0; i_input < noutput_items; i_input++) {
    // Read tags from input
    // d_tags.clear();
    get_tags_in_window(d_tags, 0, 0, d_vlen);

    for (const auto& tag : d_tags) {
        if (pmt::symbol_to_string(tag.key) == std::string("rx_freq"))
            d_f_lo = pmt::to_double(tag.value);
    }

    // Compute phase d_phi = atan2( iq.imag, iq.real )
    freq_counter_impl::data_arg(d_phi, in, 2*d_vlen, i_input*d_vlen);
    // Unwrap, and count 2pi cycles
    i_ncycles = count_unwrap(d_phi+d_vlen-1, d_vlen+1, i_n_uw);
    // Pi-counter output, New version, at the end of the window
    d_f_pi_y = (i_ncycles + (-d_phi[d_vlen-1]+d_phi[2*d_vlen-1])/(m_2pi))/ 
                ((static_cast<double>(d_vlen))/static_cast<double>(d_samp_rate));

    i_ncycles = count_unwrap(d_phi, 2*d_vlen, i_n_uw);  // Lambda counter, for whole 2tau length dataset

    double lambda_f = 0.0;  // fractional cycles
    long int lambda_i = 0;  // integer cycles
    lambda_f = -std::accumulate(d_phi, d_phi+d_vlen, 0.0);  // negative weight, first half of window
    lambda_f = std::accumulate(d_phi+d_vlen, d_phi+2*d_vlen, lambda_f);  // positive weight, second half of window

    for (size_t i_vec = 0; i_vec < d_vlen; i_vec++) {
      lambda_i += i_n_uw[d_vlen+i_vec] - i_n_uw[d_vlen-1-i_vec];
    }
    // sum of integer and fractional cycles
    d_f_lambda  = ((static_cast<double>(d_samp_rate) ) / (static_cast<double>(d_vlen*d_vlen)) )*(static_cast<double>(lambda_i) + lambda_f/  m_2pi);

    // Omega weight
    d_omegaCi = 0.0;
    d_omegaDi = 0.0;
    d_omegaCf = 0.0;
    d_omegaDf = 0.0;
    double omega_f = 0.0;
    double omega_i = 0.0;
    // Compute weighted sum for f_omega, C_omega, D_omega
    for (size_t i_vec = 0; i_vec < d_vlen; i_vec++) {
        // normalize to d_vlen-1 value!?
        d_omegaCf += d_omega_winC[i_vec]*(d_phi[i_vec+d_vlen]  - d_phi[d_vlen-1]);
        d_omegaCi += d_omega_winC[i_vec]*(i_n_uw[i_vec+d_vlen] - i_n_uw[d_vlen-1]);

        d_omegaDf += d_omega_winD[i_vec]*(d_phi[i_vec+d_vlen]  - d_phi[d_vlen-1]);
        d_omegaDi += d_omega_winD[i_vec]*(i_n_uw[i_vec+d_vlen] - i_n_uw[d_vlen-1]);

        omega_f += d_omega_win[i_vec]*(d_phi[i_vec+d_vlen]  -  d_phi[d_vlen]);  // raw phase
        omega_i += d_omega_win[i_vec]*(i_n_uw[i_vec+d_vlen] - i_n_uw[d_vlen]);  // unwraps
    }

    // sum of fractional and integer parts
    d_f_omega_w = omega_i + (omega_f/m_2pi);
    d_omegaC = d_omegaCi + (d_omegaCf/m_2pi);
    d_omegaD = d_omegaDi + (d_omegaDf/m_2pi);

    if (d_auto_tune) {
        if (d_i_avg < d_tune_avg) {
            d_f_sum += d_f_pi_y;
            d_i_avg += 1;
        } else {
        d_f_sum /= d_tune_avg;
        std::cout << "Average if freq: " << d_f_sum <<std::endl;
        if (d_f_sum > d_retune_threshold) {
            std::cout << "Retuning" << std::endl;
            // send command as tag
            d_tune_dict = pmt::dict_add(d_tune_dict, pmt::string_to_symbol("freq"), pmt::from_double(d_f_sum + d_f_lo));
            d_tune_dict = pmt::dict_add(d_tune_dict, pmt::string_to_symbol("chan"), pmt::from_double(d_uhd_channel));
            d_tune_dict = pmt::dict_add(d_tune_dict, pmt::string_to_symbol("tag"), pmt::from_long(1));  // This is needed due to bug in UHD
            message_port_pub(pmt::intern("retune_cmd"), d_tune_dict);
        } else {
            std::cout << "Frequency difference below threshold, not retuning." << std::endl;
        }
        d_i_avg = 0;
        d_f_sum = 0.0;
        }
    }

    // Output in Hz
    if (d_baseband) {
        // Baseband frequencies
        out_pi_y[i_input] = d_f_pi_y;
        out_lambda[i_input] = d_f_lambda;
        out_omega_w[i_input] = d_f_omega_w;
    } else {
        // Full RF frequencies
        out_pi_y[i_input] = d_f_pi_y + d_f_lo;
        out_lambda[i_input] = d_f_lambda + d_f_lo;
        out_omega_w[i_input] = d_f_omega_w;  // Removed + d_f_lo
    }
    out_rf_LO[i_input] = d_f_lo;
    out_omega_C[i_input] = d_omegaC;
    out_omega_D[i_input] = d_omegaD;
    }  // loop noutput_items

    return noutput_items;
}

void freq_counter_impl::data_arg(double *phi,
                const gr_complex *iq, size_t nitems, size_t offset) {
  // phi is always of length d_vlen,
  // but iq is in general noutput_items * d_vlen * 2
  // so index accordingly
  for (size_t i = 0; i < nitems; i++) {
    phi[i] = std::atan2(static_cast<double>(iq[offset+i].imag()),
                        static_cast<double>(iq[offset+i].real()));
  }
}

// not used
/*
void freq_counter_impl::unwrap(double *phi, double *phi_uw,
                                                        size_t nitems) {
  double d_term, c_term = 0.0;
  phi_uw[0] = phi[0];
  for (size_t i = 1; i < nitems; i++) {
    d_term = phi[i] - phi[i-1];
    if (d_term > m_pi) {
      c_term -= m_2pi;
    } else if (d_term < -m_pi) {
      c_term += m_2pi;
    }
    phi_uw[i] = phi[i] + c_term;
  }
}
*/

int freq_counter_impl::count_unwrap(double *phi,
                                                size_t nitems, int *n) {
  double d_term;
  int n_uw = 0;
  n[0] = 0;
  for (size_t i = 1; i < nitems; i++) {
    d_term = phi[i] - phi[i-1];
    if (d_term > m_pi) {
      n_uw -= 1;
    } else if (d_term < -m_pi) {
      n_uw += 1;
    }
    n[i] = n_uw;
  }
  return n_uw;
}


} /* namespace mikes_oot */
} /* namespace gr */
