#!/bin/bash
# Update Doxygen documentation after push to 'master'.
# Author: @pah

set -e

PLATFORM="$(uname -s | tr 'A-Z' 'a-z')"

DOXYGEN_VER=doxygen-1.8.15
DOXYGEN_TAR=${DOXYGEN_VER}.linux.bin.tar.gz
DOXYGEN_URL="http://doxygen.nl/files/${DOXYGEN_TAR}"

DOXYFILE_PATH=tools/docs/Doxyfile

skip() {
	echo "$@" 1>&2
	echo "Exiting..." 1>&2
	exit 0
}

abort() {
	echo "Error: $@" 1>&2
	echo "Exiting..." 1>&2
	exit 1
}

if [ ! -f $DOXYFILE_PATH ]; then
	abort "Doxyfile not found in $DOXYFILE_PATH"
fi

rm -rf docs/html
doxygen $DOXYFILE_PATH

