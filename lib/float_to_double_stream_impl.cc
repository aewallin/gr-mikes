/* -*- c++ -*- */
/*
 * Copyright 2024 KH.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "float_to_double_stream_impl.h"
#include <gnuradio/io_signature.h>

namespace gr {
namespace mikes_oot {

using input_type = float;
using output_type = double;
float_to_double_stream::sptr float_to_double_stream::make()
{
    return gnuradio::make_block_sptr<float_to_double_stream_impl>();
}


/*
 * The private constructor
 */
float_to_double_stream_impl::float_to_double_stream_impl()
    : gr::sync_block("float_to_double_stream",
                     gr::io_signature::make(
                         1 /* min inputs */, 1 /* max inputs */, sizeof(input_type)),
                     gr::io_signature::make(
                         1 /* min outputs */, 1 /*max outputs */, sizeof(output_type)))
{
}

/*
 * Our virtual destructor.
 */
float_to_double_stream_impl::~float_to_double_stream_impl() {}

int float_to_double_stream_impl::work(int noutput_items,
                                      gr_vector_const_void_star& input_items,
                                      gr_vector_void_star& output_items)
{
    auto in = static_cast<const input_type*>(input_items[0]);
    auto out = static_cast<output_type*>(output_items[0]);

    for (int index = 0; index < noutput_items; index++) {
      out[index] = in[index];
    }

    // Tell runtime system how many output items we produced.
    return noutput_items;
}

} /* namespace mikes_oot */
} /* namespace gr */
