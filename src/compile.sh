#!/bin/bash

if [ $# -ne 2 ]; then 
    echo "Usage : $0 <boost_include_dir> <boost_link_dir>" 
    exit -1  
fi

BOOST_INC=$1
BOOST_LIB=$2

[ ! -z build ] && rm -rf build

mkdir build 
cd build 
echo "Calling cmake"
if cmake .. -DBOOST_INCLUDEDIR=$BOOST_INC -DBOOST_LIBRARYDIR=$BOOST_LIB ; then
    cd -
else
    cd -
    rm -rf build
fi
