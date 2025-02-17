/* -*- c++ -*- */
/*
 * Copyright 2022 KH.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <gnuradio/io_signature.h>
#include "stream_to_vector_64bit_impl.h"

namespace gr {
  namespace mikes_oot {

    stream_to_vector_64bit::sptr
    stream_to_vector_64bit::make(size_t itemsize, size_t nitems_per_block)
    {
      return gnuradio::make_block_sptr<stream_to_vector_64bit_impl>(
        itemsize, nitems_per_block);
    }


    /*
     * The private constructor
     */
    stream_to_vector_64bit_impl::stream_to_vector_64bit_impl(size_t itemsize, size_t nitems_per_block)
      : gr::sync_decimator("stream_to_vector_64bit",
              gr::io_signature::make(1, 1, itemsize),
              gr::io_signature::make(1, 1, itemsize*nitems_per_block), nitems_per_block)
    {}

    /*
     * Our virtual destructor.
     */
    stream_to_vector_64bit_impl::~stream_to_vector_64bit_impl()
    {
    }

    int
    stream_to_vector_64bit_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      size_t block_size = output_signature()->sizeof_stream_item(0);

      const char* in = (const char*)input_items[0];
      char* out = (char*)output_items[0];

      memcpy(out, in, noutput_items * block_size);
      
      return noutput_items;
    }

  } /* namespace mikes_oot */
} /* namespace gr */
