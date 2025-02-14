/* -*- c++ -*- */
/*
 * Copyright 2022 KH.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <gnuradio/io_signature.h>
#include "double_to_float_stream_impl.h"

namespace gr {
  namespace mikes_oot {

    using input_type = double;
    using output_type = float;
    double_to_float_stream::sptr
    double_to_float_stream::make() {
      return gnuradio::make_block_sptr<double_to_float_stream_impl>();
    }


    /*
     * The private constructor
     */
    double_to_float_stream_impl::double_to_float_stream_impl()
      : gr::sync_block("double_to_float_stream",
              gr::io_signature::make(1, 1, sizeof(input_type)),
              gr::io_signature::make(1, 1, sizeof(output_type)))
    {}

    /*
     * Our virtual destructor.
     */
    double_to_float_stream_impl::~double_to_float_stream_impl() {
    }

    int
    double_to_float_stream_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items) {
      auto in = static_cast<const input_type*>(input_items[0]);
      auto out = static_cast<output_type*>(output_items[0]);

      for (int index = 0; index < noutput_items; index++) {
         out[index] = static_cast<output_type>(in[index]);
      }

      return noutput_items;
    }

  } /* namespace mikes_oot */
} /* namespace gr */
