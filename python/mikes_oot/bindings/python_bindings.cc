/*
 * Copyright 2020 Free Software Foundation, Inc.
 *
 * This file is part of GNU Radio
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include <pybind11/pybind11.h>

#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <numpy/arrayobject.h>

namespace py = pybind11;

// Headers for binding functions
/**************************************/
// The following comment block is used for
// gr_modtool to insert function prototypes
// Please do not delete
/**************************************/
// BINDING_FUNCTION_PROTOTYPES(
    void bind_phase_tracker_pll(py::module& m);
    void bind_freq_counter(py::module& m);
    void bind_vector_to_stream_64bit(py::module& m);
    void bind_stream_to_vector_64bit(py::module& m);
    void bind_streams_to_vector_64bit(py::module& m);
    void bind_vector_to_streams_64bit(py::module& m);
    void bind_pub_sink_64bit(py::module& m);
    void bind_double_to_float_stream(py::module& m);
    void bind_float_to_double_stream(py::module& m);
// ) END BINDING_FUNCTION_PROTOTYPES


// We need this hack because import_array() returns NULL
// for newer Python versions.
// This function is also necessary because it ensures access to the C API
// and removes a warning.
void* init_numpy()
{
    import_array();
    return NULL;
}

PYBIND11_MODULE(mikes_oot_python, m)
{
    // Initialize the numpy C API
    // (otherwise we will see segmentation faults)
    init_numpy();

    // Allow access to base block methods
    py::module::import("gnuradio.gr");

    /**************************************/
    // The following comment block is used for
    // gr_modtool to insert binding function calls
    // Please do not delete
    /**************************************/
    // BINDING_FUNCTION_CALLS(
    bind_phase_tracker_pll(m);
    bind_freq_counter(m);
    bind_vector_to_stream_64bit(m);
    bind_stream_to_vector_64bit(m);
    bind_streams_to_vector_64bit(m);
    bind_vector_to_streams_64bit(m);
    bind_pub_sink_64bit(m);
    bind_double_to_float_stream(m);
    bind_float_to_double_stream(m);
    // ) END BINDING_FUNCTION_CALLS
}
