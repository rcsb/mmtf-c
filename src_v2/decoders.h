#ifndef DECODERS_H
#define DECODERS_H

#include "h_headers.h"

float* decode_apply_strategy_into_float( const char*, uint32_t, uint32_t*, int, int32_t );
int8_t* decode_apply_strategy_into_int8( const char*, uint32_t, uint32_t*, int );
int16_t* decode_apply_strategy_into_int16( const char*, uint32_t, uint32_t*, int );
int32_t* decode_apply_strategy_into_int32( const char*, uint32_t, uint32_t*, int );
char** decode_apply_strategy_into_string_array( const char*, uint32_t, uint32_t*, int, int32_t );

#endif
