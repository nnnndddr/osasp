#!/bin/bash

outfile=$1
mydir=$2
exp=$3

if [ $# -eq 3 ]
then
  if [ -d $mydir ]
  then
    find $mydir -type f -name "*.$exp" -printf '%f\n' | sort > $outfile
  else
    echo "Directory not found">&2
  fi
else
  echo "Unexpected number of parameters">&2
fi
