#!/bin/bash

for file in ./*.graph
do
    gnuplot -p -e "filename='$file'"  graph.plg &
done