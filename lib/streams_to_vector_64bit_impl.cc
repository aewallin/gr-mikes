/* -*- c++ -*- */
/*
 * Copyright 2022 KH.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <gnuradio/io_signature.h>
#include "streams_to_vector_64bit_impl.h"

namespace gr {
  namespace mikes_oot {

    streams_to_vector_64bit::sptr
    streams_to_vector_64bit::make(size_t itemsize, size_t nstreams)
    {
      return gnuradio::make_block_sptr<streams_to_vector_64bit_impl>(
        itemsize, nstreams);
    }


    /*
     * The private constructor
     */
    streams_to_vector_64bit_impl::streams_to_vector_64bit_impl(size_t itemsize, size_t nstreams)
      : gr::sync_block("streams_to_vector_64bit",
              gr::io_signature::make(nstreams, nstreams, itemsize),
              gr::io_signature::make(1, 1, nstreams*itemsize))
    {}

    /*
     * Our virtual destructor.
     */
    streams_to_vector_64bit_impl::~streams_to_vector_64bit_impl()
    {
    }

    int
    streams_to_vector_64bit_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      
      size_t itemsize = input_signature()->sizeof_stream_item(0);
      int nstreams = input_items.size();

      const char** inv = (const char**)&input_items[0];
      char* out = (char*)output_items[0];

      for (int i = 0; i < noutput_items; i++) {
        for (int j = 0; j < nstreams; j++) {
            memcpy(out, inv[j], itemsize);
            inv[j] += itemsize;
            out += itemsize;
        }
    }
      
      return noutput_items;
    }

  } /* namespace mikes_oot */
} /* namespace gr */
