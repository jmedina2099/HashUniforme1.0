#!/bin/bash


echo '===> PROCESANDO..[' $1 ']-[' $4 ']'..
echo -ne '\e[?7l'

nice -0 ../random/random $1 

if [ $# -lt 5 ] 
then
  echo 'cat is parsing file..'
  cat $1 | while read line 
  do
    time nice -0 ./unit_test "$line" $2 $3 $4
  done
else
  echo 'c is parsing file..'
  time nice -0 ./unit_test "" $2 $3 $4 $1 $6
fi
nice -0 ../random/random $6 

if [ $# -gt 6 ]
then
  echo 'compressing the hashes..'
  time nice -0 tar cJPf $6.xz $6
  nice -0 ../random/random /spacio/azrael/$6.xz 
fi

echo -ne '\e[?7h'
echo '===> DONE!'
