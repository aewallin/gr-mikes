/* -*- c++ -*- */
/*
 * Copyright 2024 KH.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_MIKES_OOT_FLOAT_TO_DOUBLE_STREAM_H
#define INCLUDED_MIKES_OOT_FLOAT_TO_DOUBLE_STREAM_H

#include <gnuradio/mikes_oot/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
namespace mikes_oot {

/*!
 * \brief <+description of block+>
 * \ingroup mikes_oot
 *
 */
class MIKES_OOT_API float_to_double_stream : virtual public gr::sync_block
{
public:
    typedef std::shared_ptr<float_to_double_stream> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of mikes_oot::float_to_double_stream.
     *
     * To avoid accidental use of raw pointers, mikes_oot::float_to_double_stream's
     * constructor is in a private implementation
     * class. mikes_oot::float_to_double_stream::make is the public interface for
     * creating new instances.
     */
    static sptr make();
};

} // namespace mikes_oot
} // namespace gr

#endif /* INCLUDED_MIKES_OOT_FLOAT_TO_DOUBLE_STREAM_H */
