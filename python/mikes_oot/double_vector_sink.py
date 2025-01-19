#!/usr/bin/env python
# -*- coding: utf-8 -*-

import numpy as np
from gnuradio import gr

class double_vector_sink(gr.sync_block):
    """
    docstring for block double_vector_sink
    """
    def __init__(self, inp=0.0):
        gr.sync_block.__init__(self,
            name="double_vector_sink",
            in_sig=[np.float64],
            out_sig=None)
        self.d = []
        
    def work(self, input_items, output_items):
        in0 = input_items[0]
        for x in in0:
            self.d.append(x.item())
        return len(input_items[0])

    def data(self):
        return self.d