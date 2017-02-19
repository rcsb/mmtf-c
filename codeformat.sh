#!/bin/bash

set -e

CLANG_FORMAT="clang-format"

GSED=$(which gsed 2>/dev/null || echo sed)

if ! $GSED --version >/dev/null 2>/dev/null; then
    echo "error: GNU sed required"
    exit 2
fi

SOURCEFILES="src/mmtf_parser*.h src/mmtf_parser.c"

# formatting based on .clang-format
$CLANG_FORMAT -i $SOURCEFILES

# don't align backslashes (line continuation)
$GSED -i 's!  *\\$! \\!' $SOURCEFILES

# break after "static"
$GSED -i 's!^\(static\( inline\)\?\) !\1\n!' $SOURCEFILES
