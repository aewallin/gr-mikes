/* -*- c++ -*- */
/*
 * Copyright 2023 KH.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_MIKES_OOT_FREQ_COUNTER_ALL_WEIGHTS_IMPL_H
#define INCLUDED_MIKES_OOT_FREQ_COUNTER_ALL_WEIGHTS_IMPL_H

#include <gnuradio/mikes_oot/freq_counter_all_weights.h>

namespace gr {
namespace mikes_oot {

class freq_counter_all_weights_impl : public freq_counter_all_weights
{
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
    double * d_lambda_win;
    double * d_t_fit;
    double * d_phi;
    double * d_phi_uw;
    int *    i_n_uw;
    double * d_pi_y;
      
    double d_c0, d_c1, d_cov00, d_cov01, d_cov11, d_sumsq;
    
    double d_tau;
    double d_delta_omega;
    
    // Quantization correction factor
    double d_omega_corr;
    
    double d_lambda_sum;
    double d_omega_sum;
    
    double d_f_lo;
    
    int d_sym_offset;
    
    double d_lambda_weight;
    
    int d_i_avg;
    int i_ncycles;
    double d_f_pi_y;
    double d_f_pi_w;
    double d_f_lambda;
    double d_f_omega_lsq;
    double d_f_omega_w;
    double d_omegaCi;
    double d_omegaDi;
    double d_omegaCf;
    double d_omegaDf;
    double d_omegaC;
    double d_omegaD;

    double d_f_sum;
      
    pmt::pmt_t d_tune_dict = pmt::make_dict();
      
    std::vector<tag_t> d_tags;
      
    void data_arg(double *phi, const gr_complex *iq, size_t nitems, size_t offset);
    void unwrap(double *phi, double *phi_uw, size_t nitems);
    int count_unwrap(double *phi, size_t nitems, int *n);
    void diff(const double *data_in, double *data_out, size_t nitems, size_t offset);

public:
    freq_counter_all_weights_impl(size_t vec_len,
                                  int samp_rate,
                                  float retune_threshold,
                                  bool auto_tune,
                                  int tune_avg,
                                  int uhd_channel,
                                  bool baseband);
    ~freq_counter_all_weights_impl();

    // Where all the action really happens
    int work(int noutput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items);
};

} // namespace mikes_oot
} // namespace gr

#endif /* INCLUDED_MIKES_OOT_FREQ_COUNTER_ALL_WEIGHTS_IMPL_H */
