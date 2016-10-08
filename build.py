#!/usr/bin/env python

# Copyright 2016 Udey Rishi
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import argparse
import shutil
import os
from subprocess import call
import sys


if __name__ == '__main__':
    parser = argparse.ArgumentParser('Build Logic and LogicTests')
    parser.add_argument('-d', '--debug', help='Build the debug version', action='store_true')
    parser.add_argument('-r', '--rebuild', help='Clean before building', action='store_true')
    args = parser.parse_args()

    if args.rebuild:
        try:
            shutil.rmtree('debug')
        except OSError:
            pass
        try:
            shutil.rmtree('release')
        except OSError:
            pass

    build_type = 'debug' if args.debug else 'release'
    if not os.path.exists(build_type):
        os.makedirs(build_type)
    os.chdir(build_type)
    cmake_args = ['cmake', '-DCMAKE_BUILD_TYPE=' + build_type]
    if 'COMPILER' in os.environ:
        cmake_args.append('-DCMAKE_CXX_COMPILER=' + os.environ['COMPILER'])
    cmake_args.append('..')
    call(cmake_args)
    call(['make'])