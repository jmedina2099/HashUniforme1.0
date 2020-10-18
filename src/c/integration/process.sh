#!/bin/bash

OUT_PATH=$1
TIMEFORMAT='TIME (USER)=%3lU'

echo '===> PROCESANDO.. FILE=[['$2']]-BITS=[['$5']] OUT_DIR=[['$OUT_PATH']]'
echo -ne '\e[?7l'

echo 'COUNTING BITS..'
time nice -0 ../random/random $2 

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
echo 'COUNTING BITS..'
nice -0 ../random/random $7 
echo '----------|/'

if [ $# -gt 6 ]
then
  echo 'compressing (bz2) the hashes..'
  time nice -0 tar cJf $OUT_PATH/$7.xz $7
  echo 'COUNTING BITS..'
  time nice -0 ../random/random $7.xz
fi

ls $7 $OUT_PATH/$7.xz -lh
ls $7 $OUT_PATH/$7.xz -l

BITS_HASH=`stat -c %s $7`
BITS_HASH_XZ=`stat -c %s $OUT_PATH/$7.xz`

echo $BITS_HASH
echo $BITS_HASH_XZ

BITS_DIFF=$(( BITS_HASH_XZ - BITS_HASH ))
DIV=`echo "if($BITS_DIFF<0) print 0; $BITS_HASH_XZ/$BITS_HASH" | bc -l`

echo 'DIFFER='$BITS_DIFF' bits'
echo 'COMPRESS_RATIO='$DIV%''
echo '----------------|/'

echo -ne '\e[?7h'
echo '===> DONE!'
