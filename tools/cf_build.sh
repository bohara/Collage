#!/bin/sh

# Automatic build script for Equalizer. Used on the sf compile farm and other
# machines. Note the hardcoded build directory below.
# Usage : <name> <email>

cd $HOME/Software/build/equalizer/trunk/src
exec 1>`hostname`.make
exec 2>&1
svn up || exit
make clean || mail -s "Equalizer build failure on `hostname`" $1 < `hostname`.make
make || mail -s "Equalizer build failure on `hostname`" $1 < `hostname`.make
