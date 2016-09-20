#!/usr/bin/env python

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
