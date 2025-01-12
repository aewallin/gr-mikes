/* -*- c++ -*- */
/*
 * Copyright 2022 KH.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <gnuradio/io_signature.h>
#include "phase_tracker_pll_impl.h"
#include <gnuradio/math.h>
#include <gnuradio/sincos.h>
#include <iostream>

#define GR_M_TWOPI (2.0f * GR_M_PI)

namespace gr {
  namespace mikes_oot {
    
    // Added input parameter datatypes
    phase_tracker_pll::sptr
    phase_tracker_pll::make(float w, float max_freq, float min_freq, float damp_c,
                            std::string gain_set, float alpha, float beta,
                            float kp, float ki, float kii, float kd, float lock_thr,
                            const std::vector<double>& filter_b,
                            const std::vector<double>& filter_a)
    {
      return gnuradio::make_block_sptr<phase_tracker_pll_impl>(
        w, max_freq, min_freq, damp_c, gain_set, alpha, beta, kp, ki, kii, kd, lock_thr, filter_b, filter_a);
    }

    // Four outputs: gr_complex, gr_complex, float, short
    static int ios[] = { sizeof(gr_complex), sizeof(gr_complex), sizeof(float), sizeof(short) };
    static std::vector<int> iosig(ios, ios + sizeof(ios) / sizeof(int));
    
    /*
     * The private constructor
     */
    phase_tracker_pll_impl::phase_tracker_pll_impl(float w, float max_freq, float min_freq, float damp_c,
                                                   std::string gain_set, float alpha, float beta,
                                                   float kp, float ki, float kii, float kd, float lock_thr,
                                                   const std::vector<double>& filter_b,
                                                   const std::vector<double>& filter_a)
      : gr::sync_block("phase_tracker_pll",
              gr::io_signature::make(1, 1, sizeof(gr_complex)), // One input of type gr_complex
              gr::io_signature::makev(1, 4, iosig)), // Four outputs, see above
        d_locksig(0.0),
        d_lock_threshold(lock_thr),
        d_phase(0.0),
        d_freq(0.0),
        d_alpha(alpha),
        d_beta(beta),
        d_max_freq(max_freq),
        d_min_freq(min_freq),
        d_loop_bw(w),
        d_damping(damp_c),
        d_kp(kp),
        d_ki(ki),
        d_kii(kii),
        d_kd(kd),
        d_gain_set(gain_set),
        d_filter_b(filter_b),
        d_filter_a(filter_a)
        
    {
        set_loop_bandwidth(w);
    }

    /*
     * Our virtual destructor.
     */
    phase_tracker_pll_impl::~phase_tracker_pll_impl()
    {
    }

    int
    phase_tracker_pll_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      
      const gr_complex* iptr = (gr_complex*)input_items[0];
      //gr_complex* optr = (gr_complex*)output_items[0];
      gr_complex* refout_optr = (gr_complex*)output_items[0];
      gr_complex* carrier_optr = output_items.size() >= 2 ? (gr_complex*)output_items[1] : NULL;
      float* error_optr = output_items.size() >= 3 ? (float*)output_items[2] : NULL;
      float* lock_optr = output_items.size() >= 4 ? (float*)output_items[3] : NULL;

      float error;
      float t_imag, t_real;
      gr_complex iq_sample;
      int size = noutput_items;
      
      while (size-- > 0) {
      
        gr::sincosf(d_phase, &t_imag, &t_real);
        *refout_optr++ = gr_complex(t_real, t_imag);

        //fast_cc_multiply(*carrier_optr, *iptr, gr_complex(t_real, -t_imag));
        *carrier_optr++ = *iptr * gr_complex(t_real, -t_imag);

        error = phase_detector(*iptr++, d_phase);
        *error_optr++ = error;
	
    	// iq_sample = *iptr;

        // d_locksig = d_locksig * (1.0 - d_alpha) + d_alpha * (iq_sample.real() * t_real + iq_sample.imag() * t_imag);

        //ADD: Test for lock_thr
        *lock_optr++ = short( 0 );

        advance_loop(error);
        phase_wrap();
        frequency_limit();
    }

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }
  
  void phase_tracker_pll_impl::advance_loop(float error)
  {
    //d_freq = d_freq + d_beta * error;
    //d_phase = d_phase + d_freq + d_alpha * error;
    (this->*fp_loop_filter)(error);
  }
   
  void phase_tracker_pll_impl::loop_filter_gr(float error)
  {
    d_freq = d_freq + d_beta * error;
    d_phase = d_phase + d_freq + d_alpha * error;
  }
  
  void phase_tracker_pll_impl::loop_filter_leadlag(float error)
  {
    d_filter = (1/d_filter_a[0])*( d_filter_b[0]*error + d_filter_b[1]*d_error_p + d_filter_b[2]*d_error_pp - d_filter_a[1]*d_filter_p - d_filter_a[2]*d_filter_pp );
    
    d_phase = d_phase + d_filter;
    
    d_filter_pp = d_filter_p;
    d_filter_p = d_filter;
    
    d_error_pp = d_error_p;
    d_error_p = error;
  }
  
  void phase_tracker_pll_impl::loop_filter_kiid(float error)
  {
  
      d_fi = d_ki  * (error + d_error_p + d_fi_p); // I
      d_fii = d_kii * (error + 2*d_error_p + d_error_pp + 2*d_fii_p - d_fii_pp); // II
      d_fd = d_kd  * (error - d_error_p - d_fd_p); // D
  
      d_filter = d_kp  * error + // P
                 d_fi + d_fii + d_fd;
      
      
      d_phase = d_phase + d_filter;
      
      d_error_pp = d_error_p;
      d_error_p = error;
      
      d_fi_p = d_fi;
      
      d_fii_pp = d_fii_p;
      d_fii_p = d_fii;
      
      d_fd_p = d_fd;
      
  }

  void phase_tracker_pll_impl::phase_wrap()
  {
    while (d_phase > (2 * GR_M_PI))
        d_phase -= 2 * GR_M_PI;
    while (d_phase < (-2 * GR_M_PI))
        d_phase += 2 * GR_M_PI;
  }

  void phase_tracker_pll_impl::frequency_limit()
  {
    if (d_freq > d_max_freq)
        d_freq = d_max_freq;
    else if (d_freq < d_min_freq)
        d_freq = d_min_freq;
  }
    
  float phase_tracker_pll_impl::phase_detector(gr_complex sample, float ref_phase)
  {
    float sample_phase;
    sample_phase = gr::fast_atan2f(sample.imag(), sample.real());
    return mod_2pi(sample_phase - ref_phase);
  }
  
  float phase_tracker_pll_impl::mod_2pi(float in)
  {
    if (in > GR_M_PI){
        return in - GR_M_TWOPI;}
    else if (in < -GR_M_PI){
        return in + GR_M_TWOPI;}
    else
        return in;
  }
  
  bool phase_tracker_pll_impl::lock_detector(void)
  {
    return (fabsf(d_locksig) > d_lock_threshold);
  }

  void phase_tracker_pll_impl::update_gains(void)
  {
    std::cout<<"Gains updated\n";
    std::cout<<"Loop type: "<<d_gain_set<<"\n";
    if(d_gain_set == std::string("loop_bw"))
    {
      std::cout<<"GR loop filter in use\n";
      
      fp_loop_filter = &gr::mikes_oot::phase_tracker_pll_impl::loop_filter_gr;
      
      float denom = (1.0 + 2.0 * d_damping * d_loop_bw + d_loop_bw * d_loop_bw);
      d_alpha = (4 * d_damping * d_loop_bw) / denom;
      d_beta = (4 * d_loop_bw * d_loop_bw) / denom;
    }
    else if(d_gain_set == std::string("alpha_beta"))
    {
      std::cout<<"GR loop filter in use, using alpha/beta-set\n";
      std::cout<<d_alpha<<" "<<d_beta<<std::endl;
      
      fp_loop_filter = &gr::mikes_oot::phase_tracker_pll_impl::loop_filter_gr;
      
      //float denom = (1.0 + 2.0 * d_damping * d_loop_bw + d_loop_bw * d_loop_bw);
      //d_alpha = (4 * d_damping * d_loop_bw) / denom;
      //d_beta = (4 * d_loop_bw * d_loop_bw) / denom;
    }
    else if(d_gain_set == std::string("piid"))
    {
        std::cout<<"PIID filter in use"<<std::endl;
        fp_loop_filter = &gr::mikes_oot::phase_tracker_pll_impl::loop_filter_kiid;
        
        d_error_p = 0;
        d_error_pp = 0;
        
        d_fi_p = 0.0;
        d_fii_p = 0.0;
        d_fii_pp = 0;
        d_fd_p = 0.0;
    }
    else if(d_gain_set == std::string("leadlag_2nd_order"))
    {
        std::cout<<"2nd order lead lag filter in use"<<std::endl;
        fp_loop_filter = &gr::mikes_oot::phase_tracker_pll_impl::loop_filter_leadlag;
        
        d_filter_p = 0;
        d_filter_pp = 0;
        d_error_p = 0;
        d_error_pp = 0;
    }
  }

  // Various set()s and get()s
  
  void phase_tracker_pll_impl::set_taps(const std::vector<double>& filter_b, const std::vector<double>& filter_a)
  {
      d_filter_b = filter_b;
      d_filter_a = filter_a;
      update_gains();
  }

  void phase_tracker_pll_impl::set_loop_bandwidth(float bw)
  {
    d_loop_bw = bw;
    update_gains();
  }

  void phase_tracker_pll_impl::set_damping_factor(float df)
  {
    d_damping = df;
    update_gains();
  }

  void phase_tracker_pll_impl::set_alpha(float alpha)
  {
    d_alpha = alpha;
    update_gains();
  }

  void phase_tracker_pll_impl::set_beta(float beta)
  { 
   d_beta = beta;
   update_gains();
  }
  
  void phase_tracker_pll_impl::set_kp(float kp)
  { 
   d_kp = kp;
   update_gains();
  }
  
  void phase_tracker_pll_impl::set_ki(float ki)
  { 
   d_ki = ki;
   update_gains();
  }
  
  void phase_tracker_pll_impl::set_kii(float kii)
  { 
   d_kii = kii;
   update_gains();
  }
  
  void phase_tracker_pll_impl::set_kd(float kd)
  { 
   d_kd = kd;
   update_gains();
  }

 void phase_tracker_pll_impl::set_frequency(float freq)
  {
    d_freq = freq;
  }

  void phase_tracker_pll_impl::set_phase(float phase)
  {
    d_phase = phase;
  }

  void phase_tracker_pll_impl::set_min_freq(float freq)
  {
    d_min_freq = freq;
  }

  void phase_tracker_pll_impl::set_max_freq(float freq)
  {
    d_min_freq = freq;
  }


  float phase_tracker_pll_impl::get_loop_bandwidth() const
  {
    return d_loop_bw;
  }

  float phase_tracker_pll_impl::get_damping_factor() const
  {
    return d_loop_bw;
  }

  float phase_tracker_pll_impl::get_alpha() const
  {
    return d_alpha;
  }

  float phase_tracker_pll_impl::get_beta() const
  {
    return d_beta;
  }
  
  float phase_tracker_pll_impl::get_kp() const
  {
    return d_kp;
  }
  
  float phase_tracker_pll_impl::get_ki() const
  {
    return d_ki;
  }
  
  float phase_tracker_pll_impl::get_kii() const
  {
    return d_kii;
  }

  float phase_tracker_pll_impl::get_kd() const
  {
    return d_kd;
  }

  float phase_tracker_pll_impl::get_frequency() const
  {
    return d_freq;
  }

  float phase_tracker_pll_impl::get_phase() const
  {
    return d_phase;
  }

  float phase_tracker_pll_impl::get_min_freq() const
  {
    return d_min_freq;
  }

  float phase_tracker_pll_impl::get_max_freq() const
  {
    return d_max_freq;
  }

  float phase_tracker_pll_impl::set_lock_threshold(float threshold)
  {
    return d_min_freq;
  }

  } /* namespace mikes_oot */
} /* namespace gr */
