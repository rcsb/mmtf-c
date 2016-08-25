#include "c_headers.h"

// From bytes[] to float32[], int8[], int16[], int32[] and string

float* float_from_bytes( const char* input, uint32_t input_length, uint32_t* output_length ) {
	if( input_length % 4 != 0 ) {
		fprintf( stderr, "Error in the function float_from_bytes : the input length %u is not a multiple of 4.\n", input_length );
		return NULL;
	}

	(*output_length) = input_length/4;

	float* output = malloc( sizeof(float)*(*output_length) );

	four_bytes_as_float u;
	uint32_t i;
	for( i = 0; i < input_length; i = i + 4 ) {
		u.c[0] = input[i+3];
		u.c[1] = input[i+2];
		u.c[2] = input[i+1];
		u.c[3] = input[i];

		output[i/4] = u.f;
	}

	return output;
}

int8_t* int8_from_bytes( const char* input, uint32_t input_length, uint32_t* output_length ) {
	(*output_length) = input_length;

	int8_t* output = malloc( sizeof(int8_t)*(*output_length) );

	one_byte_as_int8 u;
	uint32_t i;
	for( i = 0; i < input_length; ++i ) {
		u.c = input[i];

		output[i] = u.i;
	}

	return output;
}

int16_t* int16_from_bytes( const char* input, uint32_t input_length, uint32_t* output_length ) {
	if( input_length % 2 != 0 ) {
		fprintf( stderr, "Error in the function int16_from_bytes : the input length %u is not a multiple of 2.\n", input_length );
		return NULL;
	}

	(*output_length) = input_length/2;

	int16_t* output = malloc( sizeof(int16_t)*(*output_length) );

	two_bytes_as_int16 u;
	uint32_t i;
	for( i = 0; i < input_length; i = i + 2 ) {
		u.c[0] = input[i+1];
		u.c[1] = input[i];

		output[i/2] = u.i;
	}

	return output;
}

int get_strategy(const char * bytes) {
	four_bytes_as_int32 ct;

	ct.c[0] = bytes[3];
	ct.c[1] = bytes[2];
	ct.c[2] = bytes[1];
	ct.c[3] = bytes[0];

	return ct.i;
}

int get_len(const char * bytes){
	four_bytes_as_int32 ct;

	ct.c[0] = bytes[7];
	ct.c[1] = bytes[6];
	ct.c[2] = bytes[5];
	ct.c[3] = bytes[4];

	return ct.i;
}

int  get_param(const char * bytes) {
	four_bytes_as_int32 ct;

	ct.c[0] = bytes[11];
	ct.c[1] = bytes[10];
	ct.c[2] = bytes[9];
	ct.c[3] = bytes[8];

	return ct.i;
}

int32_t* int32_from_bytes( const char* input, const uint32_t input_length, uint32_t* output_length ) {
	if( input_length % 4 != 0 ) {
		fprintf( stderr, "Error in the function int32_from_bytes : the input length %u is not a multiple of 4.\n", input_length );
		return NULL;
	}

	(*output_length) = input_length/4;

	int32_t* output = malloc( sizeof(int32_t) * (*output_length) );

	four_bytes_as_int32 u;
	uint32_t i;
	for( i = 0; i < input_length; i = i + 4 ) {
		u.c[0] = input[i+3];
		u.c[1] = input[i+2];
		u.c[2] = input[i+1];
		u.c[3] = input[i];

		output[i/4] = u.i;
	}

	return output;
}

char* string_from_int32( const int32_t* input, const uint32_t input_length, uint32_t* output_length ) {
	(*output_length) = input_length * 4;

	char* output = malloc( sizeof( char ) * (*output_length) );

	four_bytes_as_int32 u;
	uint32_t i;
	for( i = 0; i < input_length; ++i ) {
		u.i = input[i];

		output[4*i+0] = u.c[0];
		output[4*i+1] = u.c[1];
		output[4*i+2] = u.c[2];
		output[4*i+3] = u.c[3];
	}

	return output;
}

char** strings_from_bytes( const char* input, uint32_t input_length, uint32_t parameter, uint32_t* output_length ) {
	if( input_length % parameter != 0 ) {
		fprintf( stderr, "Error in the function float_from_bytes : the input length %u is not a multiple of your parameter %u.\n", input_length, parameter );
		return NULL;
	}

	(*output_length) = input_length / parameter;

	char** output = malloc( sizeof(char*)*(*output_length) );
	uint32_t i,j;
	for( i = 0; i < *output_length; ++i ) {
		output[i] = malloc( sizeof(char)*parameter );
	}

	for( i = 0; i < *output_length; ++i ) {
		for( j = 0; j < parameter; ++j ) {
			output[i][j] = input[i * parameter + j];
		}
	}

	return output;
}

