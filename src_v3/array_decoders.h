#ifndef ARRAY_DECODERS_H
#define ARRAY_DECODERS_H

#include "h_headers.h"

// Run-length decode
int32_t* run_length_decode( int32_t*, uint32_t, uint32_t*);

// Delta decode
int32_t* delta_decode( int32_t*, uint32_t, uint32_t* );

// Recursive indexing decode
int32_t* recursive_indexing_decode_from_16( int16_t*, uint32_t, uint32_t*);
int32_t* recursive_indexing_decode_from_8( int8_t*, uint32_t, uint32_t* );

// Integer decoding
float* integer_decode_from_16( int16_t*, uint32_t, int32_t, uint32_t* );
float* integer_decode_from_32( int32_t*, uint32_t, int32_t, uint32_t* );

#endif
