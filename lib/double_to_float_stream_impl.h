/* -*- c++ -*- */
/*
 * Copyright 2022 KH.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_MIKES_OOT_DOUBLE_TO_FLOAT_STREAM_IMPL_H
#define INCLUDED_MIKES_OOT_DOUBLE_TO_FLOAT_STREAM_IMPL_H

#include <gnuradio/mikes_oot/double_to_float_stream.h>

namespace gr {
  namespace mikes_oot {

    class double_to_float_stream_impl : public double_to_float_stream
    {
     private:
      // Nothing to declare in this block.

     public:
      double_to_float_stream_impl();
      ~double_to_float_stream_impl();

      // Where all the action really happens
      int work(
              int noutput_items,
              gr_vector_const_void_star &input_items,
              gr_vector_void_star &output_items
      );
    };

  } // namespace mikes_oot
} // namespace gr

#endif /* INCLUDED_MIKES_OOT_DOUBLE_TO_FLOAT_STREAM_IMPL_H */
