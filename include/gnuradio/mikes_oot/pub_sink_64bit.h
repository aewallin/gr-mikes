/* -*- c++ -*- */
/*
 * Copyright 2022 KH.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_MIKES_OOT_PUB_SINK_64BIT_H
#define INCLUDED_MIKES_OOT_PUB_SINK_64BIT_H

#include <gnuradio/mikes_oot/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace mikes_oot {

    /*!
     * \brief <+description of block+>
     * \ingroup mikes_oot
     *
     */
    class MIKES_OOT_API pub_sink_64bit : virtual public gr::sync_block
    {
     public:
      typedef std::shared_ptr<pub_sink_64bit> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of mikes_oot::pub_sink_64bit.
       *
       * To avoid accidental use of raw pointers, mikes_oot::pub_sink_64bit's
       * constructor is in a private implementation
       * class. mikes_oot::pub_sink_64bit::make is the public interface for
       * creating new instances.
       */
      static sptr make(size_t itemsize,
                       size_t vlen,
                       char* address,
                       int timeout = 100,
                       bool pass_tags = false,
                       int hwm = -1,
                       const std::string& key = "",
                       bool drop_on_hwm = true);
      
      virtual std::string last_endpoint() = 0;
      
    };

  } // namespace mikes_oot
} // namespace gr

#endif /* INCLUDED_MIKES_OOT_PUB_SINK_64BIT_H */
