/* -*- c++ -*- */
/*
 * Copyright 2022 KH.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef LIB_PUB_SINK_64BIT_IMPL_H_
#define LIB_PUB_SINK_64BIT_IMPL_H_

#include <gnuradio/mikes_oot/pub_sink_64bit.h>
#include <string>
#include "base_impl.h"

namespace gr {
namespace mikes_oot {

class pub_sink_64bit_impl : public pub_sink_64bit, public base_sink_impl {
 private:
  // Nothing to declare in this block.

 public:
  pub_sink_64bit_impl(size_t itemsize,
                      size_t vlen,
                      char* address,
                      int timeout,
                      bool pass_tags,
                      int hwm,
                      const std::string& key,
                      bool drop_on_hwm);
  //  ~pub_sink_64bit_impl();

  //  Where all the action really happens
  int work(
          int noutput_items,
          gr_vector_const_void_star &input_items,
          gr_vector_void_star &output_items) override;
  std::string last_endpoint() override {
      return base_sink_impl::last_endpoint();
  }
};

}  // namespace mikes_oot
}  // namespace gr

#endif  // LIB_PUB_SINK_64BIT_IMPL_H_
