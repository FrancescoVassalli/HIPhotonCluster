#!/bin/csh
cd ../clustering/build
make install
cd ../../subtraction/build
make install
cd ../../burners/build
make install
exit 0
