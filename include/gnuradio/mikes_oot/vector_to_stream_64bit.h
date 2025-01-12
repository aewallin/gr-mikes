/* -*- c++ -*- */
/*
 * Copyright 2022 KH.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_MIKES_OOT_VECTOR_TO_STREAM_64BIT_H
#define INCLUDED_MIKES_OOT_VECTOR_TO_STREAM_64BIT_H

#include <gnuradio/mikes_oot/api.h>
#include <gnuradio/sync_interpolator.h>

namespace gr {
  namespace mikes_oot {

    /*!
     * \brief <+description of block+>
     * \ingroup mikes_oot
     *
     */
    class MIKES_OOT_API vector_to_stream_64bit : virtual public gr::sync_interpolator
    {
     public:
      typedef std::shared_ptr<vector_to_stream_64bit> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of mikes_oot::vector_to_stream_64bit.
       *
       * To avoid accidental use of raw pointers, mikes_oot::vector_to_stream_64bit's
       * constructor is in a private implementation
       * class. mikes_oot::vector_to_stream_64bit::make is the public interface for
       * creating new instances.
       */
      static sptr make(size_t itemsize, size_t nitems_per_block);
    };

  } // namespace mikes_oot
} // namespace gr

#endif /* INCLUDED_MIKES_OOT_VECTOR_TO_STREAM_64BIT_H */
