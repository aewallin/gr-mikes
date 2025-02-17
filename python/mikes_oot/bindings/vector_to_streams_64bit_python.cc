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
/* BINDTOOL_HEADER_FILE(vector_to_streams_64bit.h) */
/* BINDTOOL_HEADER_FILE_HASH(b6c521f5d35081b8fc692a541f09fc9f)                     */
/***********************************************************************************/

#include <pybind11/complex.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

#include <gnuradio/mikes_oot/vector_to_streams_64bit.h>
// pydoc.h is automatically generated in the build directory
#include <vector_to_streams_64bit_pydoc.h>

void bind_vector_to_streams_64bit(py::module& m)
{

    using vector_to_streams_64bit = ::gr::mikes_oot::vector_to_streams_64bit;


    py::class_<vector_to_streams_64bit,
               gr::sync_block,
               gr::block,
               gr::basic_block,
               std::shared_ptr<vector_to_streams_64bit>>(
        m, "vector_to_streams_64bit", D(vector_to_streams_64bit))

        .def(py::init(&vector_to_streams_64bit::make),
             py::arg("itemsize"),
             py::arg("nstreams"),
             D(vector_to_streams_64bit, make))


        ;
}
