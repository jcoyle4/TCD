#!/bin/bash

Temp=0
J=100
h=0

gcc ex11.c -o ex11 -lm
 
for((T=300; T>Temp; T--))
do
	./ex11 $T $h $J >> temp_and_netmag_for_J=$J.dat
done
