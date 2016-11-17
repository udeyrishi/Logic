#!/usr/bin/env bash

# Copyright 2016 Udey Rishi

# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at

# http://www.apache.org/licenses/LICENSE-2.0

# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

####################### Logic installation script #########################

####################### Configurable installation variables ###############
BUILD_FLAVOUR="release" # choices: release/debug
INSTALLATION_DIR="/usr/local/bin/"
###########################################################################

####################### Don't touch below this !! #########################

# Validate all dependencies are met
if ! which "cmake" > /dev/null 2>&1 ; then
    echo "Need cmake to be present in the PATH env var for building"
    exit -1
fi

if ! which "make" > /dev/null 2>&1 ; then
    echo "Need make to be present in the PATH env var for building"
    exit -1
fi

TEMP_BUILD_DIR=".__1122temp__logic_dir__1122"
BINARY_NAME="logic"
BUILD_PATH="./$TEMP_BUILD_DIR/bin/$BINARY_NAME"

# Build
if mkdir -p $TEMP_BUILD_DIR ; then
    cd $TEMP_BUILD_DIR
else
    echo "Cannot create the temporary build directory $TEMP_BUILD_DIR. Please ensure you have the permissions."
    exit -1
fi

if ! (cmake .. -DCMAKE_BUILD_TYPE=$BUILD_FLAVOUR && make $BINARY_NAME) ; then
    echo "Build failed. Quitting installation."
    rm -rf $TEMP_BUILD_DIR
    exit -1
fi

cd ..

# Move the binary to destination
if ! mkdir -p $INSTALLATION_DIR ; then
    echo "Cannot create the installation directory $INSTALLATION_DIR. Please ensure you have the permissions."
    rm -rf $TEMP_BUILD_DIR
    exit -1
fi

if ! mv $BUILD_PATH $INSTALLATION_DIR ; then
    echo "Cannot move the compiled binary to the installation directory $INSTALLATION_DIR. Please ensure you have the permissions."
    rm -rf $TEMP_BUILD_DIR
    exit -1
fi

# Cleanup
rm -rf $TEMP_BUILD_DIR
