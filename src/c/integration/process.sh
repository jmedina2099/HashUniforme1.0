#!/bin/bash

export OUT_DIR_HASH=$1
export OUT_DIR_HASH_XZ=$2
export TIMEFORMAT='TIME (USER)=%3lU'
export BZIP=-9

echo '===> PROCESANDO.. FILE=[['$3']]-BITS=[['$6']]-[['$4'x]] OUT_DIR_HASH=[['$OUT_DIR_HASH']] OUT_DIR_HASH_XZ=[['$OUT_DIR_HASH_XZ']]'

#echo 'COUNTING FILE BITS..'
#time nice -0 ../random/random $3 

#if [ $5 -eq 1 ]
#then
#  echo -ne '\e[?7l'
#fi

echo 'HASHING...'
if [ $# -lt 5 ]
then
  echo 'cat is parsing file..'
  cat $3 | while read line 
  do
    time nice -0 ./unit_test "$line" $4 $5 $6
  done
else
  echo 'c is parsing file..'
  time nice -0 ./unit_test "" $4 $5 $6 $3 $OUT_DIR_HASH/$8
fi

#if [ $5 -eq 1 ]
#then
#  echo -ne '\e[?7h'
#fi

echo '***=========|/'
echo 'HASHING IS DONE!'

echo 'COUNTING HASHED BITS..'
time nice -0 ../random/random $OUT_DIR_HASH/$8 
echo '============|/'
echo 'COUNTING IS DONE!'

if [ $# -gt 6 ] && [ $7 -eq 1 ]
then
  echo 'COMPRESSING (bz2)..'
  #time nice -0 tar cJPf $OUT_DIR_HASH_XZ/$8.xz $OUT_DIR_HASH/$8
  time nice -0 bzip2 -kz9vc $OUT_DIR_HASH/$8 > $OUT_DIR_HASH_XZ/$8.xz
  echo 'COMPRESSING IS DONE!'

  echo 'COUNTING COMPRESS BITS..'
  time nice -0 ../random/random $OUT_DIR_HASH_XZ/$8.xz
  echo '============|/'
  echo 'COUNTING IS DONE!'

  ls $OUT_DIR_HASH/$8 $OUT_DIR_HASH_XZ/$8.xz -lh
  ls $OUT_DIR_HASH/$8 $OUT_DIR_HASH_XZ/$8.xz -l

  BITS_HASH=`stat -c %s $OUT_DIR_HASH/$8`
  BITS_HASH_XZ=`stat -c %s $OUT_DIR_HASH_XZ/$8.xz`

  # DELETE HASH FILE
  rm $OUT_DIR_HASH/$8
  if [ $9 -eq 1 ]
  then
    # DELETE HASH XZ FILE - (CLEAN_RM)
    rm $OUT_DIR_HASH_XZ/$8.xz
  fi

  BITS_DIFF=$(( BITS_HASH_XZ - BITS_HASH ))
  DIV=`echo "if($BITS_DIFF<0) print 0; scale=27; $BITS_HASH_XZ/$BITS_HASH" | bc -l`
  
  if [ $BITS_DIFF -lt 0 ]
  then
    SUB=`echo "print 0; scale=27; 1-$BITS_HASH_XZ/$BITS_HASH" | bc -l`
  else
    SUB=`echo "print 0; scale=27; $BITS_HASH_XZ/$BITS_HASH-1" | bc -l`
  fi
  SUB_E=`echo $SUB | awk '{printf "%.27e\n", $1}'`

  echo 'DIFFER='$BITS_DIFF' bits'
  echo 'COMPRESS_RATIO='$DIV'%'
  echo '----------------|/'
  echo '         error=('$SUB')=('$SUB_E')'
  echo '-----------------|/'
fi
echo '===> DONE!'
