#!/bin/bash

gcc -o executable.run $1 -lmsgpackc `pkg-config --cflags --libs glib-2.0`
