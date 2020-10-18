#!/bin/bash

OUT_DIR_HASH=$1
OUT_DIR_HASH_XZ=$2
TIMEFORMAT='TIME (USER)=%3lU'

echo '===> PROCESANDO.. FILE=[['$3']]-BITS=[['$6']] OUT_DIR_HASH=[['$OUT_DIR_HASH']] OUT_DIR_HASH_XZ=[['$OUT_DIR_HASH_XZ']]'

echo 'COUNTING BITS..'
time nice -0 ../random/random $3 

echo 'HASHING...'
if [ $5 -eq 1 ]
then
  echo -ne '\e[?7l'
fi
if [ $# -lt 5 ] 
then
  echo 'cat is parsing file..'
  cat $3 | while read line 
  do
    time nice -0 ./unit_test "$line" $4 $5 $6
  done
else
  echo 'c is parsing file..'
  time nice -0 ./unit_test "" $4 $5 $6 $3 $8
fi
if [ $5 -eq 1 ]
then
  echo -ne '\e[?7h'
fi
echo 'HASHING IS DONE!'

echo 'COUNTING BITS..'
nice -0 ../random/random $8 
echo '----------|/'

if [ $# -gt 6 ]
then
  echo 'COMPRESSING (bz2)..'
  time nice -0 tar cJf $OUT_PATH/$8.xz $8
  echo 'COUNTING BITS..'
  time nice -0 ../random/random $8.xz
fi

ls $8 $OUT_PATH/$8.xz -lh
ls $8 $OUT_PATH/$8.xz -l

BITS_HASH=`stat -c %s $8`
BITS_HASH_XZ=`stat -c %s $OUT_PATH/$8.xz`

echo $BITS_HASH
echo $BITS_HASH_XZ

BITS_DIFF=$(( BITS_HASH_XZ - BITS_HASH ))
DIV=`echo "if($BITS_DIFF<0) print 0; $BITS_HASH_XZ/$BITS_HASH" | bc -l`

echo 'DIFFER='$BITS_DIFF' bits'
echo 'COMPRESS_RATIO='$DIV%''
echo '----------------|/'
echo '===> DONE!'
