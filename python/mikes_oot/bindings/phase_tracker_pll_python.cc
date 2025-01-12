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
/* BINDTOOL_HEADER_FILE(phase_tracker_pll.h)                                        */
/* BINDTOOL_HEADER_FILE_HASH(4bc9c14bee07ae0b4e9ee550c2b91128)                     */
/***********************************************************************************/

#include <pybind11/complex.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

#include <gnuradio/mikes_oot/phase_tracker_pll.h>
// pydoc.h is automatically generated in the build directory
#include <phase_tracker_pll_pydoc.h>

void bind_phase_tracker_pll(py::module& m)
{

    using phase_tracker_pll = ::gr::mikes_oot::phase_tracker_pll;


    py::class_<phase_tracker_pll,
               gr::sync_block,
               gr::block,
               gr::basic_block,
               std::shared_ptr<phase_tracker_pll>>(
        m, "phase_tracker_pll", D(phase_tracker_pll))

        .def(py::init(&phase_tracker_pll::make),
             py::arg("w"),
             py::arg("max_freq"),
             py::arg("min_freq"),
             py::arg("damp_c"),
             py::arg("gain_set"),
             py::arg("alpha"),
             py::arg("beta"),
             py::arg("kp"),
             py::arg("ki"),
             py::arg("kii"),
             py::arg("kd"),
             py::arg("lock_thr"),
             py::arg("filter_b"),
             py::arg("filter_a"),
             D(phase_tracker_pll, make))


        .def("lock_detector",
             &phase_tracker_pll::lock_detector,
             D(phase_tracker_pll, lock_detector))


        .def("set_lock_threshold",
             &phase_tracker_pll::set_lock_threshold,
             py::arg("arg0"),
             D(phase_tracker_pll, set_lock_threshold))


        .def("set_loop_bandwidth",
             &phase_tracker_pll::set_loop_bandwidth,
             py::arg("bw"),
             D(phase_tracker_pll, set_loop_bandwidth))


        .def("set_damping_factor",
             &phase_tracker_pll::set_damping_factor,
             py::arg("df"),
             D(phase_tracker_pll, set_damping_factor))


        .def("set_alpha",
             &phase_tracker_pll::set_alpha,
             py::arg("alpha"),
             D(phase_tracker_pll, set_alpha))


        .def("set_beta",
             &phase_tracker_pll::set_beta,
             py::arg("beta"),
             D(phase_tracker_pll, set_beta))


        .def("set_frequency",
             &phase_tracker_pll::set_frequency,
             py::arg("freq"),
             D(phase_tracker_pll, set_frequency))


        .def("set_phase",
             &phase_tracker_pll::set_phase,
             py::arg("phase"),
             D(phase_tracker_pll, set_phase))


        .def("set_min_freq",
             &phase_tracker_pll::set_min_freq,
             py::arg("freq"),
             D(phase_tracker_pll, set_min_freq))


        .def("set_max_freq",
             &phase_tracker_pll::set_max_freq,
             py::arg("freq"),
             D(phase_tracker_pll, set_max_freq))


        .def("set_kp",
             &phase_tracker_pll::set_kp,
             py::arg("kp"),
             D(phase_tracker_pll, set_kp))


        .def("set_ki",
             &phase_tracker_pll::set_ki,
             py::arg("ki"),
             D(phase_tracker_pll, set_ki))


        .def("set_kii",
             &phase_tracker_pll::set_kii,
             py::arg("kii"),
             D(phase_tracker_pll, set_kii))


        .def("set_kd",
             &phase_tracker_pll::set_kd,
             py::arg("kd"),
             D(phase_tracker_pll, set_kd))


        .def("set_taps",
             &phase_tracker_pll::set_taps,
             py::arg("filter_b"),
             py::arg("filter_a"),
             D(phase_tracker_pll, set_taps))


        .def("get_loop_bandwidth",
             &phase_tracker_pll::get_loop_bandwidth,
             D(phase_tracker_pll, get_loop_bandwidth))


        .def("get_damping_factor",
             &phase_tracker_pll::get_damping_factor,
             D(phase_tracker_pll, get_damping_factor))


        .def("get_alpha", &phase_tracker_pll::get_alpha, D(phase_tracker_pll, get_alpha))


        .def("get_beta", &phase_tracker_pll::get_beta, D(phase_tracker_pll, get_beta))


        .def("get_frequency",
             &phase_tracker_pll::get_frequency,
             D(phase_tracker_pll, get_frequency))


        .def("get_phase", &phase_tracker_pll::get_phase, D(phase_tracker_pll, get_phase))


        .def("get_min_freq",
             &phase_tracker_pll::get_min_freq,
             D(phase_tracker_pll, get_min_freq))


        .def("get_max_freq",
             &phase_tracker_pll::get_max_freq,
             D(phase_tracker_pll, get_max_freq))

        ;
}