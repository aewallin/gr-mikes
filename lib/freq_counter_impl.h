/* -*- c++ -*- */
/*
 * Copyright 2023 KH.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <vector>
#include <string>

#ifndef LIB_FREQ_COUNTER_IMPL_H_
#define LIB_FREQ_COUNTER_IMPL_H_

#include <gnuradio/mikes_oot/freq_counter.h>

namespace gr {
namespace mikes_oot {

class freq_counter_impl : public freq_counter {
 private:
    const size_t d_vlen;
    const int d_samp_rate;
    float d_retune_threshold;
    bool d_auto_tune;
    int d_tune_avg;
    int d_uhd_channel;
    std::string d_mode;
    bool d_baseband;
    double * d_omega_win;
    double * d_omega_winC;
    double * d_omega_winD;
    double * d_phi; // phase of input complex number
    double * d_abs; // norm of input complex number
    int *    i_n_uw;
    double d_tau;
    double d_delta_omega;
    double d_f_lo;
    int d_i_avg;
    int i_ncycles;
    double d_f_pi;  // Pi-counter output
    double d_f_lambda;  // Lambda-counter output
    double d_f_omega;  // Omega-counter output
    double d_omegaCi;
    double d_omegaDi;
    double d_omegaCf;
    double d_omegaDf;
    double d_omegaC;
    double d_omegaD;
    double d_phase; // phase output
    double d_abs_mean; // mean(norm)
    double d_min_snr; // minimum SNR
    double d_f_sum;
    pmt::pmt_t d_tune_dict = pmt::make_dict();
    std::vector<tag_t> d_tags;
    void data_arg(double *phi, double *absZ, const gr_complex *iq,
                    size_t nitems, size_t offset);
    // void unwrap(double *phi, double *phi_uw, size_t nitems);
    int count_unwrap(double *phi, size_t nitems, int *n);

 public:
    freq_counter_impl(size_t vec_len,
                                  int samp_rate,
                                  float retune_threshold,
                                  bool auto_tune,
                                  int tune_avg,
                                  int uhd_channel,
                                  bool baseband);
    ~freq_counter_impl();
    // Where all the action really happens
    int work(int noutput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items);
};

}  // namespace mikes_oot
}  // namespace gr

#endif  // LIB_FREQ_COUNTER_IMPL_H_
