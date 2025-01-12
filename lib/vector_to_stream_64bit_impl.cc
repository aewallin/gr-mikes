/* -*- c++ -*- */
/*
 * Copyright 2022 KH.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <gnuradio/io_signature.h>
#include "vector_to_stream_64bit_impl.h"

namespace gr {
  namespace mikes_oot {


    vector_to_stream_64bit::sptr
    vector_to_stream_64bit::make(size_t itemsize, size_t nitems_per_block)
    {
      return gnuradio::make_block_sptr<vector_to_stream_64bit_impl>(
        itemsize, nitems_per_block);
    }


    /*
     * The private constructor
     */
    vector_to_stream_64bit_impl::vector_to_stream_64bit_impl(size_t itemsize, size_t nitems_per_block)
      : gr::sync_interpolator("vector_to_stream_64bit",
              gr::io_signature::make(1, 1, itemsize*nitems_per_block),
              gr::io_signature::make(1, 1, itemsize), nitems_per_block)
    {}

    /*
     * Our virtual destructor.
     */
    vector_to_stream_64bit_impl::~vector_to_stream_64bit_impl()
    {
    }

    int
    vector_to_stream_64bit_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {

      size_t block_size = output_signature()->sizeof_stream_item(0);

      const char* in = (const char*)input_items[0];
      char* out = (char*)output_items[0];

      memcpy(out, in, noutput_items * block_size);

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace mikes_oot */
} /* namespace gr */
