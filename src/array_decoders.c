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
/*
typedef struct list_int32_t list_int32_t;

struct list_int32_t {
	list_int32_t* previous;
	uint32_t value;
	list_int32_t* next;
};

int32_t* recursive_indexing_decode_from_16( int16_t* input, unsigned long input_length, unsigned long* output_length ) {

printf( "%lu\n", input_length );

	list_int32_t* start = (list_int32_t*) malloc( sizeof(list_int32_t) );

printf( "vfdfg\n" );

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

printf( "%i.\n" , input[i] );

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
*/

int32_t* recursive_indexing_decode_from_16( int16_t* input, unsigned long input_length, unsigned long* output_length ) {
	(*output_length) = 0;
	int i;
	for( i = 0; i < input_length; ++i ) {
		if( input[i] != INT16_MAX && input[i] != INT16_MIN ) {
			++(*output_length);
		}
	}

	int32_t* output = (int32_t*) malloc(sizeof(int32_t)*(*output_length)); // The output needs to be freed by the calling process

	int j = 0;
	output[j] = 0;

	for( i = 0; i < input_length; ++i ) {
		output[j] += input[i];

		if( input[i] != INT16_MAX && input[i] != INT16_MIN && j+1 < *output_length ) {
			++j;
			output[j] = 0;
		}
	}

	return output;
}

int32_t* recursive_indexing_decode_from_8( int8_t* input, unsigned long input_length, unsigned long* output_length ) {
	(*output_length) = 0;
	int i;
	for( i = 0; i < input_length; ++i ) {
		if( input[i] != INT8_MAX && input[i] != INT8_MIN ) {
			++(*output_length);
		}
	}

	int32_t* output = malloc(sizeof(int32_t)*(*output_length)); // The output needs to be freed by the calling process

	int j = 0;
	output[j] = 0;

	for( i = 0; i < input_length; ++i ) {
		output[j] += input[i];

		if( input[i] != INT8_MAX && input[i] != INT8_MIN && j+1 < *output_length ) {
			++j;
			output[j] = 0;
		}
	}

	return output;
}

/*
int32_t* recursive_indexing_decode_from_8( int8_t* input, unsigned long input_length, unsigned long* output_length ) {
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

		adding_next = ( input[i] == INT8_MAX || input[i] == INT8_MIN );
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
*/

// Integer decoding

float* integer_decode_from_16( int16_t* input, unsigned long input_length, int32_t parameter, unsigned long* output_length ) {
	(*output_length) = input_length;
	float* output = (float*) malloc( sizeof(float) * (*output_length) );

	float parameter_float = (float) parameter;
	int i;
	for(i = 0; i < input_length; ++i ) {
		output[i] = ((float) input[i])/parameter_float;
	}

	return output;
}

float* integer_decode_from_32( int32_t* input, unsigned long input_length, int32_t parameter, unsigned long* output_length ) {
	(*output_length) = input_length;
	float* output = (float*) malloc( sizeof(float) * (*output_length) );

	float parameter_float = (float) parameter;
	int i;
	for(i = 0; i < input_length; ++i ) {
		output[i] = ((float) input[i])/parameter_float;
	}

	return output;
}


