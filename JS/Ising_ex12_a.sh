#!/bin/bash

Temp=0
J=-10
h=0

gcc ex12_a.c -o ex12_a -lm
 
for((T=150; T>Temp; T--))
do
	./ex12_a $T $h $J >> mag_vs_temp_a.dat
done
