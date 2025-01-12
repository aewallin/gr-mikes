/* -*- c++ -*- */
/*
 * Copyright 2022 KH.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_MIKES_OOT_PHASE_TRACKER_PLL_IMPL_H
#define INCLUDED_MIKES_OOT_PHASE_TRACKER_PLL_IMPL_H

#include <gnuradio/mikes_oot/phase_tracker_pll.h>

namespace gr {
  namespace mikes_oot {

    class phase_tracker_pll_impl : public phase_tracker_pll
    {
     private:
        float d_locksig, d_lock_threshold;
        float d_phase, d_freq;
        float d_alpha, d_beta;
        float d_max_freq, d_min_freq;
        float d_loop_bw, d_damping;
        float d_kp, d_ki, d_kii, d_kd;
        
        std::string d_gain_set;
       
        // IIR taps as stdlib vectors
        std::vector<double> d_filter_b;
        std::vector<double> d_filter_a;
        
        // History of input & output to loop filter
        float d_error_pp = 0.0, d_error_p = 0.0;
        float d_filter_pp = 0.0, d_filter_p = 0.0, d_filter = 0.0;
        
        // History for PIID
        float d_fi = 0.0, d_fii = 0.0, d_fd = 0.0;
        float d_fi_p = 0.0;
        float d_fii_p = 0.0, d_fii_pp = 0;
        float d_fd_p = 0.0;
       
        float mod_2pi(float in);
        float phase_detector(gr_complex sample, float ref_phase);
        
        void update_gains();
        void advance_loop();
        
        void advance_loop(float error);
        void phase_wrap();
        void frequency_limit();
        
        void loop_filter_gr(float error);
        void loop_filter_leadlag(float error);
        void loop_filter_kiid(float error);
        
        bool lock_detector(void);
        
        // Function pointer to loop filter, Gnuradio's version by default, other options are PIID or 2nd order filters
        void (phase_tracker_pll_impl::*fp_loop_filter)(float);

     public:
      phase_tracker_pll_impl(float w, float max_freq, float min_freq, float damp_c,
                             std::string gain_set, float alpha, float beta, float kp,
                             float ki, float kii, float kd, float lock_thr,
                             const std::vector<double>& filter_b,
                             const std::vector<double>& filter_a);
                             
      ~phase_tracker_pll_impl();
      
      float set_lock_threshold();

      void set_loop_bandwidth(float bw);
      void set_damping_factor(float df);
      void set_alpha(float alpha);
      void set_beta(float beta);
      
      void set_kp(float kp);
      void set_ki(float ki);
      void set_kii(float kii);
      void set_kd(float kd);
      
      void set_frequency(float freq);
      void set_phase(float phase);
      void set_min_freq(float freq);
      void set_max_freq(float freq);
      
      void set_taps(const std::vector<double>& filter_b, const std::vector<double>& filter_a);

      float get_loop_bandwidth() const;
      float get_damping_factor() const;
      float get_alpha() const;
      float get_beta() const;
      
      float get_kp() const;
      float get_ki() const;
      float get_kii() const;
      float get_kd() const;
      
      float get_frequency() const;
      float get_phase() const;
      float get_min_freq() const;
      float get_max_freq() const;

      // Where all the action really happens
      int work(
              int noutput_items,
              gr_vector_const_void_star &input_items,
              gr_vector_void_star &output_items
      );
    };

  } // namespace mikes_oot
} // namespace gr

#endif /* INCLUDED_MIKES_OOT_PHASE_TRACKER_PLL_IMPL_H */
