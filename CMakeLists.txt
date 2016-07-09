cmake_minimum_required (VERSION 2.8.11)
project (MMTF)
add_executable (helloDemo src/demo.c)
target_link_libraries (helloDemo LINK_PUBLIC msgpackc)
