/* -*- c++ -*- */
/*
 * Copyright 2023 KH.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <memory>

#ifndef INCLUDE_GNURADIO_MIKES_OOT_FREQ_COUNTER_ALL_WEIGHTS_H_
#define INCLUDE_GNURADIO_MIKES_OOT_FREQ_COUNTER_ALL_WEIGHTS_H_

#include <gnuradio/mikes_oot/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
namespace mikes_oot {

/*!
 * \brief Computes frequencies with all methods simultaneously
 * \ingroup mikes_oot
 *
 */
class MIKES_OOT_API freq_counter_all_weights : virtual public gr::sync_block {
 public:
    typedef std::shared_ptr<freq_counter_all_weights> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of
     * mikes_oot::freq_counter_all_weights.
     *
     * To avoid accidental use of raw pointers, mikes_oot::freq_counter_all_weights's
     * constructor is in a private implementation
     * class. mikes_oot::freq_counter_all_weights::make is the public interface for
     * creating new instances.
     */
    static sptr make(size_t vec_len,
                     int samp_rate,
                     float retune_threshold,
                     bool auto_tune,
                     int tune_avg,
                     int uhd_channel,
                     bool baseband);
};

}  // namespace mikes_oot
}  // namespace gr

#endif  // INCLUDE_GNURADIO_MIKES_OOT_FREQ_COUNTER_ALL_WEIGHTS_H_
