#!/usr/bin/env bash

BAZEL_VERSION="0.23.0"

if [[ "$TRAVIS_OS_NAME" == "linux" ]]; then
  # update itself
  # sudo apt-get update

  BAZEL_DEB="bazel_${BAZEL_VERSION}-linux-x86_64.deb"
  # install bazel via deb pack
  wget https://github.com/bazelbuild/bazel/releases/download/${BAZEL_VERSION}/${BAZEL_DEB}
  sudo dpkg -i ${BAZEL_DEB}
  rm -f ${BAZEL_DEB}

  # Error: corrupt installation: file '/home/travis/.cache/bazel/_bazel_travis/install/xxx/_embedded_binaries/client' missing. 
  # Please remove '/home/travis/.cache/bazel/_bazel_travis/install/xx' and try again.
  rm -rf $HOME/.cache/bazel/_bazel_travis/install/

  # download miniconda if not exists
  # [ ! -d $HOME/mc ] && wget https://repo.continuum.io/miniconda/Miniconda3-latest-Linux-x86_64.sh -O miniconda.sh
  [ ! -f $HOME/mc/bin/conda ] && wget https://repo.continuum.io/miniconda/Miniconda3-latest-Linux-x86_64.sh -O miniconda.sh
  # wget https://repo.continuum.io/miniconda/Miniconda3-latest-Linux-x86_64.sh -O miniconda.sh
fi

if [[ "$TRAVIS_OS_NAME" == "osx" ]]; then
  # update itself
  # brew update && brew upgrade

  # install bazel using brew
  # md5sha1sum is for mc
  brew install bazel md5sha1sum
  
  # c++ driver
  brew install mongo-cxx-driver

  # Error: corrupt installation: file '/home/travis/.cache/bazel/_bazel_travis/install/xxx/_embedded_binaries/client' missing. 
  # Please remove '/home/travis/.cache/bazel/_bazel_travis/install/xx' and try again.
  rm -rf $HOME/.cache/bazel/_bazel_travis/install/

  # download if not exists
  [ ! -f $HOME/mc/bin/conda ] && wget https://repo.continuum.io/miniconda/Miniconda3-latest-MacOSX-x86_64.sh -O miniconda.sh
  # wget https://repo.continuum.io/miniconda/Miniconda3-latest-MacOSX-x86_64.sh -O miniconda.sh
fi

# install miniconda
if [ -f miniconda.sh ]; then
  echo "install miniconda"
  chmod +x miniconda.sh
  rm -rf $HOME/mc # ERROR: File or directory already exists: /home/travis/mc
  ./miniconda.sh -b -p $HOME/mc
  rm -rf miniconda.sh
fi

conda install --yes numpy



