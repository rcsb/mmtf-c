[![Build Status](https://travis-ci.org/rcsb/mmtf-c.svg?branch=master)](https://travis-ci.org/rcsb/mmtf-c)
[![License](https://img.shields.io/badge/license-Apache%202.0-blue.svg?style=flat)](https://github.com/rcsb/mmtf-c/blob/master/LICENSE)
[![Status](http://img.shields.io/badge/status-beta-red.svg?style=flat)](https://github.com/rcsb/mmtf-python/)
[![Version](http://img.shields.io/badge/version-0.0.1-blue.svg?style=flat)](https://github.com/rcsb/mmtf-c/)

# MMTF C API and decoder

The [**m**acro**m**olecular **t**ransmission **f**ormat (MMTF)](https://mmtf.rcsb.org/) is a binary encoding of biological structures.

This repository holds the C compatible API and decoding library.

For a C++ API and encoding/decoding library, see https://github.com/rcsb/mmtf-cpp

## Dependencies

* [msgpack-c](https://github.com/msgpack/msgpack-c) >=1.0 (e.g. `libmsgpack-dev` on Debian)

## Examples

* [demo.c](src/demo.c): Load a MMTF file
* [traverse.c](src/traverse.c): Iterate over atoms and bonds

## Usage

The library is a single C file, [mmtf_parser.c](src/mmtf_parser.c), and an accompanying header file, [mmtf_parser.h](src/mmtf_parser.h).
Copy both to your project and compile them with your other C files.

For example, here are the command lines to compile a demo using the MMTF parser:

	gcc -c mmtf_parser.c
	gcc -c demo.c
	gcc -o demo mmtf_parser.o demo.o -lmsgpackc

## Projects using mmtf-c

* [PyMOL](https://pymol.org) uses `mmtf-c` for loading MMTF files (and `mmtf-cpp` for exporting them)
