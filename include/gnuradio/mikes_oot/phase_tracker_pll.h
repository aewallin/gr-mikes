/* -*- c++ -*- */
/*
 * Copyright 2022 KH.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_MIKES_OOT_PHASE_TRACKER_PLL_H
#define INCLUDED_MIKES_OOT_PHASE_TRACKER_PLL_H

#include <gnuradio/mikes_oot/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace mikes_oot {

    /*!
     * \brief <+description of block+>
     * \ingroup mikes_oot
     *
     */
    class MIKES_OOT_API phase_tracker_pll : virtual public gr::sync_block
    {
     public:
      typedef std::shared_ptr<phase_tracker_pll> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of mikes_oot::phase_tracker_pll.
       *
       * To avoid accidental use of raw pointers, mikes_oot::phase_tracker_pll's
       * constructor is in a private implementation
       * class. mikes_oot::phase_tracker_pll::make is the public interface for
       * creating new instances.
       */
      static sptr make(float w, float max_freq, float min_freq, float damp_c,
                       std::string gain_set, float alpha, float beta,
                       float kp, float ki, float kii, float kd, float lock_thr,
                       const std::vector<double>& filter_b,
                       const std::vector<double>& filter_a);
      
      // All these are virtual ???
      virtual bool lock_detector(void) = 0;
      virtual float set_lock_threshold() = 0;

      virtual void set_loop_bandwidth(float bw) = 0;
      virtual void set_damping_factor(float df) = 0;
      virtual void set_alpha(float alpha) = 0;
      virtual void set_beta(float beta) = 0;
      virtual void set_frequency(float freq) = 0;
      virtual void set_phase(float phase) = 0;
      virtual void set_min_freq(float freq) = 0;
      virtual void set_max_freq(float freq) = 0;

      virtual void set_kp(float kp) = 0;
      virtual void set_ki(float ki) = 0;
      virtual void set_kii(float kii) = 0;
      virtual void set_kd(float kd) = 0;

      virtual void set_taps(const std::vector<double>& filter_b,
                            const std::vector<double>& filter_a) = 0;

      virtual float get_loop_bandwidth() const = 0;
      virtual float get_damping_factor() const = 0;
      virtual float get_alpha() const = 0;
      virtual float get_beta() const = 0;
      virtual float get_frequency() const = 0;
      virtual float get_phase() const = 0;
      virtual float get_min_freq() const = 0;
      virtual float get_max_freq() const = 0;
      
    };

  } // namespace mikes_oot
} // namespace gr

#endif /* INCLUDED_MIKES_OOT_PHASE_TRACKER_PLL_H */
