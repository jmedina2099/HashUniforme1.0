#!/bin/bash


echo '===> PROCESANDO..[' $1 ']'..

cat $1 | while read line 
do
   # do something with $line here
   nice -0 ./unit_test "$line" $2 $3 $4 
done
