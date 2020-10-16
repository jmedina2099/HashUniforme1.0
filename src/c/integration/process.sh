#!/bin/bash


echo '===> PROCESANDO..[' $1 ']'..
echo -ne '\e[?7l'

if [ $# -lt 5 ] 
then
  echo 'cat is parsing file..'
  cat $1 | while read line 
  do
    nice -0 ./unit_test "$line" $2 $3 $4
  done
else
  echo 'c is parsing file..'
  nice -0 ./unit_test "" $2 $3 $4 $1 $6
fi

echo -ne '\e[?7h'
echo '===> DONE!'
