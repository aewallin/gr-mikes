/* -*- c++ -*- */
/*
 * Copyright 2022 KH.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_MIKES_OOT_VECTOR_TO_STREAM_64BIT_IMPL_H
#define INCLUDED_MIKES_OOT_VECTOR_TO_STREAM_64BIT_IMPL_H

#include <gnuradio/mikes_oot/vector_to_stream_64bit.h>

namespace gr {
  namespace mikes_oot {

    class vector_to_stream_64bit_impl : public vector_to_stream_64bit
    {
     private:
      // Nothing to declare in this block.

     public:
      vector_to_stream_64bit_impl(size_t itemsize, size_t nitems_per_block);
      ~vector_to_stream_64bit_impl();

      // Where all the action really happens
      int work(
              int noutput_items,
              gr_vector_const_void_star &input_items,
              gr_vector_void_star &output_items
      );
    };

  } // namespace mikes_oot
} // namespace gr

#endif /* INCLUDED_MIKES_OOT_VECTOR_TO_STREAM_64BIT_IMPL_H */
