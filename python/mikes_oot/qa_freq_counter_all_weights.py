#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Copyright 2023 KH.
#
# SPDX-License-Identifier: GPL-3.0-or-later
#
import numpy as np
import time

from gnuradio import gr, gr_unittest, analog, blocks
# from gnuradio import blocks
try:
    from gnuradio.mikes_oot import freq_counter_all_weights, double_to_float_stream, double_vector_sink
except ImportError:
    import os
    import sys
    dirname, filename = os.path.split(os.path.abspath(__file__))
    sys.path.append(os.path.join(dirname, "bindings"))
    from gnuradio.mikes_oot import freq_counter_all_weights

class qa_freq_counter_all_weights(gr_unittest.TestCase):

    def setUp(self):
        self.tb = gr.top_block()
        
        self.samp_rate  = 32000
        self.gate_s =  1


    def tearDown(self):
        self.tb = None

    def run_fixed_freq_test(self, fset, N_gates=int(pow(2,10))):
        # FIXME: Test will fail until you pass sensible arguments to the constructor
        # instance = freq_counter_all_weights()
        self.counter = freq_counter_all_weights((int(self.samp_rate*self.gate_s)),  # gate samples = vector length
                                                          self.samp_rate,  # sample rate
                                                          100,  # retuning threshold
                                                          False, # auto tune
                                                          20, # auto tune averaging N
                                                          0, # autotuning UHD ch
                                                          True)  # False=LO+IF, True=IF
        self.source = analog.sig_source_c(self.samp_rate, analog.GR_COS_WAVE, 
                                          fset, # frequency 
                                          1, # amplitude
                                          0, # offsetvector 
                                          0) # initial phase
        seed = int(time.time())
        self.noise = analog.noise_source_c(analog.GR_GAUSSIAN, 0.02, seed) # amplitude, seed(int)
        self.adder = blocks.add_vcc(1)
        self.tb.connect((self.source, 0), (self.adder, 0))
        self.tb.connect((self.noise, 0), (self.adder, 1))
        
        
        N_samples=int(N_gates*self.samp_rate) # test runs for this amount of samples
        
        self.hd = blocks.head(gr.sizeof_gr_complex*1, N_samples)
        self.v2s0 = blocks.vector_to_stream(gr.sizeof_gr_complex*1, 1)
        self.s2v0 = blocks.stream_to_vector(gr.sizeof_gr_complex*1, (int(self.samp_rate*self.gate_s)))

        self.d2f,  self.vs = [], []   
        self.vs64 = []
        for n in range(6):
            self.d2f.append( double_to_float_stream() )
            #self.s2v.append( blocks.stream_to_vector(gr.sizeof_float*1, 1) )
            self.vs.append( blocks.vector_sink_f(1, N_gates) )
            self.vs64.append( double_vector_sink() )

        self.tb.connect((self.adder, 0), (self.hd, 0))
        self.tb.connect((self.hd, 0), (self.v2s0, 0))
        self.tb.connect((self.v2s0, 0), (self.s2v0, 0))
        self.tb.connect((self.s2v0, 0), (self.counter, 0))
        
        for n in range(6):
            self.tb.connect((self.counter, n), (self.d2f[n], 0))
            #self.tb.connect((self.d2f[n], 0), (self.s2v[n], 0))
            self.tb.connect((self.d2f[n], 0), (self.vs[n], 0))
            self.tb.connect((self.counter, n), (self.vs64[n], 0))
            
        print('sample_rate %d, N_gates %d, fset %.12f'%( self.samp_rate, N_gates, fset))
        self.tb.run()
        dpi = np.array( self.vs[0].data() )
        #print( self.vs64[0].data() )
        dpi64 = np.array( self.vs64[0].data() )
        
        dlam = np.array( self.vs[1].data() )
        dlam = dlam[1:] # NOTE: first output value discarded, lambda counter needs 2*tau input data
        dom = np.array( self.vs[2].data() )
        print('y_err = (f-f0)/f0')
        yerrpi = (np.mean(dpi)-fset)/fset
        print('Pi \t%.12g(%.12g)\tN=%d\t yerr=%.12g'%(  np.mean(dpi), np.std(dpi), len(dpi),yerrpi))
        print('Pi64 \t%.12g(%.12g)\tN=%d\t yerr=%.12g'%(  np.mean(dpi64), np.std(dpi64), len(dpi64),yerrpi))
        #for p in dpi:
        #    print(p)
        yerrlam = (np.mean(dlam)-fset)/fset
        print('Lam \t%.12g(%.12g)\tN=%d\t yerr=%.12g'%(  np.mean(dlam), np.std(dlam), len(dlam),yerrlam))
        #print(dlam[0],dlam[1],dlam[2],dlam[3],dlam[4])
        yerrom = (np.mean(dom)-fset)/fset
        print('Om \t%.12g(%.12g)\tN=%d\t yerr=%.12g'%(  np.mean(dom), np.std(dom), len(dom), yerrom))
        
        assert( np.isclose(np.mean(dpi), fset, rtol=1e-04) )
        assert( np.isclose(np.mean(dlam), fset, rtol=1e-04) )
        assert( np.isclose(np.mean(dom), fset, rtol=1e-04) )



    def test_001(self):
        self.run_fixed_freq_test(123.456)

    def test_002(self):
        self.run_fixed_freq_test(-654.321)

    
    def test_003(self):
        self.run_fixed_freq_test(123.456, N_gates=int(pow(2,11)))

    def test_004(self):
        self.run_fixed_freq_test(123.456, N_gates=int(pow(2,12)))
        
    def test_005(self):
        self.run_fixed_freq_test(123.456, N_gates=int(pow(2,13)))
        
    def test_006(self):
        self.run_fixed_freq_test(123.456, N_gates=int(pow(2,14)))
        
    def test_007(self):
        self.run_fixed_freq_test(123.456, N_gates=int(pow(2,15)))


if __name__ == '__main__':
    gr_unittest.run(qa_freq_counter_all_weights)
