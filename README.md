To compile program cmake is necessary. Wolrd file is hardcoded in program and it should be put in main project directory and named `map.markov`
Compiling process (enter main project directory):
* mkdir build
* cd build
* cmake ..
* make
* ./DirectMarkov (in no arguments given)
* ./DirectMarkov 0.99 0.22 (example with discountign factor and learning parameter)


Graphs in gnuplot are generated by gnuplot.sh
* ./gnuplot.sh
