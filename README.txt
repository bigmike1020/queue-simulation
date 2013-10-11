Mike Senn <mpsenn@ncsu.edu>
ECE 579 Assignment 2

Compile/build by running make. Tested in GCC 4.4.7. Results with standard
options are in results.txt.

Usage: ./simulate [options]
Options:
  --help        Display this help message
  -p 
  --packets     Number of packets that departed from the client queue. 
                (default N=30) 
  -h 
  --arrHigh     Time to transmit a packet from the server at high rate. 
                (default D_H=1.0) 
  -l 
  --arrLow      Time to transmit a packet from the server at low rate. 
                (default D_L=2.0) 
  -s 
  --serverMean  Mean service time in the infinite server queue. 
                (default 1/μ_d=10.0) 
  -c 
  --clientMean  Mean service time in the client queue. 
                (default 1/μ_q=1.5) 
  --thresLow    Low threshold level in the server queue. 
                (default T_L=3) 
  --thresHigh   High threshold level in the server queue. 
                (default T_H=3) 
  --seed        Pseudorandom number generator seed. (default R=45647) 
  --boring      Print in a boring format that prints the entire state 
                on one line. 

