/* -*- c++ -*- */
/*
 * Copyright 2024 KH.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_MIKES_OOT_FLOAT_TO_DOUBLE_STREAM_IMPL_H
#define INCLUDED_MIKES_OOT_FLOAT_TO_DOUBLE_STREAM_IMPL_H

#include <gnuradio/mikes_oot/float_to_double_stream.h>

namespace gr {
namespace mikes_oot {

class float_to_double_stream_impl : public float_to_double_stream
{
private:
    // Nothing to declare in this block.

public:
    float_to_double_stream_impl();
    ~float_to_double_stream_impl();

    // Where all the action really happens
    int work(int noutput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items);
};

} // namespace mikes_oot
} // namespace gr

#endif /* INCLUDED_MIKES_OOT_FLOAT_TO_DOUBLE_STREAM_IMPL_H */
