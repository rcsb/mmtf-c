#ifndef ARRAY_CONVERTERS_H
#define ARRAY_CONVERTERS_H

#include "h_headers.h"

typedef union {
	char c[4];
	float f;
} four_bytes_as_float;

float* float_from_bytes( const char*, uint32_t, uint32_t* );

typedef union {
	char c;
	int8_t i;
} one_byte_as_int8;

int8_t* int8_from_bytes( const char*, uint32_t, uint32_t* );

typedef union {
	char c[2];
	int16_t i;
} two_bytes_as_int16;

int16_t* int16_from_bytes( const char*, uint32_t, uint32_t* );

typedef union {
	char c[4];
	int32_t i;
} four_bytes_as_int32;

int get_strategy(const char *);
int get_len(const char *);
int  get_param(const char *);

int32_t* int32_from_bytes( const char*, const uint32_t, uint32_t* );
char** strings_from_bytes( const char*, uint32_t, uint32_t, uint32_t* );


// int 32
int32_t* convert_to_int_32(const char*);

// int 16
int32_t* convert_to_int_16(const char*);

// int 8
int32_t* convert_to_int_8(const char*);

#endif
