/*
 * Copyright 2023 Free Software Foundation, Inc.
 *
 * This file is part of GNU Radio
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

/***********************************************************************************/
/* This file is automatically generated using bindtool and can be manually edited  */
/* The following lines can be configured to regenerate this file during cmake      */
/* If manual edits are made, the following tags should be modified accordingly.    */
/* BINDTOOL_GEN_AUTOMATIC(0)                                                       */
/* BINDTOOL_USE_PYGCCXML(0)                                                        */
/* BINDTOOL_HEADER_FILE(streams_to_vector_64bit.h) */
/* BINDTOOL_HEADER_FILE_HASH(544c5e940e8541d408cf30b5e8be08c1)                     */
/***********************************************************************************/

#include <pybind11/complex.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

#include <gnuradio/mikes_oot/streams_to_vector_64bit.h>
// pydoc.h is automatically generated in the build directory
#include <streams_to_vector_64bit_pydoc.h>

void bind_streams_to_vector_64bit(py::module& m)
{

    using streams_to_vector_64bit = ::gr::mikes_oot::streams_to_vector_64bit;


    py::class_<streams_to_vector_64bit,
               gr::sync_block,
               gr::block,
               gr::basic_block,
               std::shared_ptr<streams_to_vector_64bit>>(
        m, "streams_to_vector_64bit", D(streams_to_vector_64bit))

        .def(py::init(&streams_to_vector_64bit::make),
             py::arg("itemsize"),
             py::arg("nstreams"),
             D(streams_to_vector_64bit, make))


        ;
}
