/* -*- c++ -*- */
/*
 * Copyright 2022 KH.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <gnuradio/io_signature.h>
#include "vector_to_streams_64bit_impl.h"

namespace gr {
  namespace mikes_oot {

    vector_to_streams_64bit::sptr
    vector_to_streams_64bit::make(size_t itemsize, size_t nstreams)
    {
      return gnuradio::make_block_sptr<vector_to_streams_64bit_impl>(
        itemsize, nstreams);
    }


    /*
     * The private constructor
     */
    vector_to_streams_64bit_impl::vector_to_streams_64bit_impl(size_t itemsize, size_t nstreams)
      : gr::sync_block("vector_to_streams_64bit",
              gr::io_signature::make(1, 1, nstreams * itemsize),
              gr::io_signature::make(nstreams, nstreams, itemsize))
    {}

    /*
     * Our virtual destructor.
     */
    vector_to_streams_64bit_impl::~vector_to_streams_64bit_impl()
    {
    }

    int
    vector_to_streams_64bit_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
    
      size_t itemsize = output_signature()->sizeof_stream_item(0);
      int nstreams = output_items.size();

      const char* in = (const char*)input_items[0];
      char** outv = (char**)&output_items[0];

      for (int i = 0; i < noutput_items; i++) {
        for (int j = 0; j < nstreams; j++) {
            memcpy(outv[j], in, itemsize);
            outv[j] += itemsize;
            in += itemsize;
        }
      }
      
      return noutput_items;
    }

  } /* namespace mikes_oot */
} /* namespace gr */