#!/bin/bash

OUT_PATH=$1

echo '===> PROCESANDO..['$2']-['$5'] OUT_DIR='$OUT_PATH
echo -ne '\e[?7l'

nice -0 ../random/random $2 

echo 'HASHING...'
if [ $# -lt 5 ] 
then
  echo 'cat is parsing file..'
  cat $2 | while read line 
  do
    time nice -0 ./unit_test "$line" $3 $4 $5
  done
else
  echo 'c is parsing file..'
  time nice -0 ./unit_test "" $3 $4 $5 $2 $7
fi
echo 'HASHING IS DONE!'

nice -0 ../random/random $7 

if [ $# -gt 6 ]
then
  echo 'compressing (bz2) the hashes..'
  time nice -0 tar cJPf $OUT_PATH/$7.xz $7
fi

ls $7 $OUT_PATH/$7.xz -lh
ls $7 $OUT_PATH/$7.xz -l

BITS_HASH=`stat -c %s $7`
BITS_HASH_XZ=`stat -c %s $OUT_PATH/$7.xz`

echo $BITS_HASH
echo $BITS_HASH_XZ

BITS_DIFF=$(( BITS_HASH_XZ - BITS_HASH ))
DIV=`echo $BITS_HASH_XZ*100/$BITS_HASH | bc -l`

echo 'DIFFER='$BITS_DIFF' bits'
echo 'COMPRESS_PERCENTAGE='$DIV%''

echo -ne '\e[?7h'
echo '===> DONE!'
