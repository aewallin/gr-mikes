# Copyright 2020 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

########################################################################
# Check if there is C++ code at all
########################################################################
if(NOT mikes_oot_sources)
    MESSAGE(STATUS "No C++ sources... skipping python bindings")
    return()
endif(NOT mikes_oot_sources)

########################################################################
# Check for pygccxml
########################################################################
GR_PYTHON_CHECK_MODULE_RAW(
    "pygccxml"
    "import pygccxml"
    PYGCCXML_FOUND
    )

include(GrPybind)

########################################################################
# Python Bindings
########################################################################
list(APPEND mikes_oot_python_files
    phase_tracker_pll_python.cc
    vector_to_stream_64bit_python.cc
    stream_to_vector_64bit_python.cc
    streams_to_vector_64bit_python.cc
    vector_to_streams_64bit_python.cc
    pub_sink_64bit_python.cc
    double_to_float_stream_python.cc
    freq_counter_python.cc
    float_to_double_stream_python.cc python_bindings.cc)

GR_PYBIND_MAKE_OOT(mikes_oot
   ../../..
   gr::mikes_oot
   "${mikes_oot_python_files}")

# copy bindings extension for use in QA test module
add_custom_command(TARGET mikes_oot_python POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:mikes_oot_python>
    ${CMAKE_BINARY_DIR}/test_modules/gnuradio/mikes_oot/
)

install(TARGETS mikes_oot_python DESTINATION ${GR_PYTHON_DIR}/gnuradio/mikes_oot COMPONENT pythonapi)
