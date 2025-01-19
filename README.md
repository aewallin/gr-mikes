# gr-mikes

GnuRadio C++ oot module implementing:

* Frequency counting using Pi-, Lambda-, Omega-counter implementations. float64 output.
* float64 number type conversion to float32 
* ZeroMQ float64 source block, for downstream processing of frequency counter data (e.g. plotting, InfluxDB, etc.)

## References

* Sherman, Jordens, "Oscillator metrology with software defined radio", https://doi.org/10.1063/1.4950898
* Rubiola et al., "The Omega Counter, a Frequency Counter Based on the Linear Regression", 2015, https://arxiv.org/abs/1506.05009
* Dawkins, et al., “Considerations on the 
measurement of the stability of oscillators with frequency counters,” in 
IEEE Transactions on Ultrasonics, Ferroelectrics, and Frequency Control, 
vol. 54, no. 5, pp. 918-925, May 2007. https://doi.org/10.1109/TUFFC.2007.337
