#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed Oct 19 14:13:29 2022

@author: mikes
"""

import numpy as np
from scipy import signal

import matplotlib.pyplot as plt

# N = 4
# b, a = signal.butter(N, 0.1, 'low', analog=False)
# w, h = signal.freqz(b, a)

# #plt.semilogx(w, 20 * np.log10(np.abs(h)), color='silver', ls='dashed')

# b, a = signal.bessel(N, 0.1, 'low', analog=False, norm='phase')

# w, h = signal.freqz(b, a)

# #plt.semilogx(w, 20 * np.log10(np.abs(h)))

khi = np.sqrt(2)/2
bw = 2*np.pi/200

phi0 = bw#*(1/fs)/2

denom = 1 + 2*khi*phi0 + phi0**2

alpha = (4*phi0*khi)/denom # prop
beta = (4*phi0**2)/denom # int

a = [1, -1]
b = [alpha+beta, -alpha]

w, h = signal.freqz(b,a, 4096)

plt.semilogx(w, 20 * np.log10(np.abs(h)))

#plt.title('Bessel filter magnitude response (with Butterworth)')
plt.xlabel('Frequency [radians / second]')
plt.ylabel('Amplitude [dB]')
plt.margins(0, 0.1)
plt.grid(which='both', axis='both')
plt.axvline(100, color='green')  # cutoff frequency

