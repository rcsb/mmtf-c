#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// Run length decode

int32_t* run_length_decode( int32_t* input, unsigned long input_length, unsigned long* output_length ) {
	(*output_length) = 0;

	if( input_length % 2 != 0 ) {
		fprintf( stderr, "Error in calling run-length decode: your input length %lu is not an even number.\n", input_length );
		return NULL;
	}

	int i;
	int32_t value, number;
	for(i = 0; i < input_length; i += 2) {
		value = input[i];
		number = input[i+1];

		(*output_length) += number;
	}

	int32_t* output = malloc(sizeof(int32_t)*(*output_length)); // The output needs to be freed by the calling process

	int j = 0;
	int k;
	for(i = 0; i < input_length; i += 2) {
		value = input[i];
		number = input[i+1];

		for(k = 0; k < number; ++k) {
			output[j] = value;
			++j;
		}
	}

	return output;
}

// Delta decode

int32_t* delta_decode( int32_t* input, unsigned long input_length, unsigned long* output_length ) {
	(*output_length) = input_length;
	int32_t* output = malloc(sizeof(int32_t)*(*output_length)); // The output needs to be freed by the calling process

	output[0] = input[0];
	int i;
	for(i = 1; i < input_length; ++i) {
		output[i] = output[i-1] + input[i];
	}

	return output;
}

// Recursive indexing decode

typedef struct list_int32_t list_int32_t;

struct list_int32_t {
	list_int32_t* previous;
	uint32_t value;
	list_int32_t* next;
};

int32_t* recursive_indexing_decode( int16_t* input, unsigned long input_length, unsigned long* output_length ) {
	list_int32_t* start = malloc( sizeof(list_int32_t) );

	list_int32_t* current = start;
	(*current).previous = NULL;
	(*current).value = 0;
	(*current).next = NULL;

	(*output_length) = 1;

	int i;
	bool adding_next = true;
	for(i = 0; i < input_length; ++i) {
		if( ! adding_next ) {
			list_int32_t* next = malloc( sizeof(list_int32_t) );
			(*current).next = next;
			(*next).previous = current;
			(*next).value = 0;
			(*next).next = NULL;
			current = next;
			++(*output_length);
		}

		(*current).value += input[i];

		adding_next = ( input[i] == INT16_MAX || input[i] == INT16_MIN );
	}

	int32_t* output = malloc(sizeof(int32_t)*(*output_length)); // The output needs to be freed by the calling process

	int j = 0;

	current = start;
	while( current != NULL ) {
		output[j] = (*current).value;
		list_int32_t* next = (*current).next;
		free(current);
		current = next;
		++j;
	}

	return output;
}

// Integer decoding

float* integer_decode( int32_t* input, unsigned long input_length, int32_t parameter, unsigned long* output_length ) {
	(*output_length) = input_length;
	float* output = malloc( sizeof(float) * (*output_length) );

	float parameter_float = (float) parameter;
	int i;
	for(i = 0; i < input_length; ++i ) {
		output[i] = ((float) input[i])/parameter_float;
	}

	return output;
}

// From bytes[] to float32[], int8[], int16[], int32[] and string

typedef union {
	char c[4];
	float f;
} four_bytes_as_float;

float* float_from_bytes( char* input, unsigned long input_length, unsigned long* output_length ) {
	if( input_length % 4 != 0 ) {
		fprintf( stderr, "Error in the function float_from_bytes : the input length %lu is not a multiple of 4.\n", input_length );
		return NULL;
	}

	(*output_length) = input_length/4;

	float* output = malloc( sizeof(float)*(*output_length) );

	four_bytes_as_float u;
	int i;
	for( i = 0; i < input_length; i = i + 4 ) {
		u.c[0] = input[i];
		u.c[1] = input[i+1];
		u.c[2] = input[i+2];
		u.c[3] = input[i+3];

		output[i/4] = u.f;
	}

	return output;
}

typedef union {
	char c;
	int8_t i;
} one_byte_as_int8;

int8_t* int8_from_bytes( char* input, unsigned long input_length, unsigned long* output_length ) {
	(*output_length) = input_length;

	int8_t* output = malloc( sizeof(int8_t)*(*output_length) );

	one_byte_as_int8 u;
	int i;
	for( i = 0; i < input_length; ++i ) {
		u.c = input[i];

		output[i] = u.i;
	}

	return output;
}

typedef union {
	char c[2];
	int16_t i;
} two_bytes_as_int16;

int16_t* int16_from_bytes( char* input, unsigned long input_length, unsigned long* output_length ) {
	if( input_length % 2 != 0 ) {
		fprintf( stderr, "Error in the function int16_from_bytes : the input length %lu is not a multiple of 2.\n", input_length );
		return NULL;
	}

	(*output_length) = input_length/2;

	int16_t* output = malloc( sizeof(int16_t)*(*output_length) );

	two_bytes_as_int16 u;
	int i;
	for( i = 0; i < input_length; i = i + 2 ) {
		u.c[0] = input[i];
		u.c[1] = input[i+1];

		output[i/2] = u.i;
	}

	return output;
}

typedef union {
	char c[4];
	int32_t i;
} four_bytes_as_int32;

int32_t* int32_from_bytes( char* input, unsigned long input_length, unsigned long* output_length ) {
	if( input_length % 4 != 0 ) {
		fprintf( stderr, "Error in the function float_from_bytes : the input length %lu is not a multiple of 4.\n", input_length );
		return NULL;
	}

	(*output_length) = input_length/4;

	int32_t* output = malloc( sizeof(int32_t)*(*output_length) );

	four_bytes_as_int32 u;
	int i;
	for( i = 0; i < input_length; i = i + 4 ) {
		u.c[0] = input[i];
		u.c[1] = input[i+1];
		u.c[2] = input[i+2];
		u.c[3] = input[i+3];

		output[i/4] = u.i;
	}

	return output;
}

char** strings_from_bytes( char* input, unsigned long input_length, unsigned long parameter, unsigned long* output_length ) {
	if( input_length % parameter != 0 ) {
		fprintf( stderr, "Error in the function float_from_bytes : the input length %lu is not a multiple of your parameter %lu.\n", input_length, parameter );
		return NULL;
	}

	(*output_length) = input_length / parameter;

	char** output = malloc( sizeof(char*)*(*output_length) );
	int i,j;
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




