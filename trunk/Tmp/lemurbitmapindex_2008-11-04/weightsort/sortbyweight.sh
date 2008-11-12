#!/usr/bin/env bash
PATH=..:.:../..:$PATH
if [ -z $1 ]
then
  echo " This is Daniel's implementation of Owen Kaser's sort-by-weight idea."
  echo "usage: sortbyweight.sh somefile somedelimiter"
  echo "the default delimiter is ,"
  echo "make sure you do 'make weightsort' first"
  exit 
fi
weightinserter -d ${2:-,} $1  | sort  -t${2:-,} | pruneoddcolumns -d ${2:-,}
