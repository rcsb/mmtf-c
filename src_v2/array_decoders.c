#include "c_headers.h"

// Run-length decode

int32_t* run_length_decode( int32_t* input, uint32_t input_length, uint32_t* output_length ) {
	(*output_length) = 0;

	if( input_length % 2 != 0 ) {
		fprintf( stderr, "Error in calling run-length decode: your input length %u is not an even number.\n", input_length );
		return NULL;
	}

	uint32_t i;
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

int32_t* delta_decode( int32_t* input, uint32_t input_length, uint32_t* output_length ) {
	(*output_length) = input_length;
	int32_t* output = malloc(sizeof(int32_t)*(*output_length)); // The output needs to be freed by the calling process

	output[0] = input[0];
	uint32_t i;
	for(i = 1; i < input_length; ++i) {
		output[i] = output[i-1] + input[i];
	}

	return output;
}

// Recursive indexing decode

int32_t* recursive_indexing_decode_from_16( int16_t* input, uint32_t input_length, uint32_t* output_length ) {
	(*output_length) = 0;
	uint32_t i;
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

int32_t* recursive_indexing_decode_from_8( int8_t* input, uint32_t input_length, uint32_t* output_length ) {
	(*output_length) = 0;
	uint32_t i;
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

// Integer decoding

float* integer_decode_from_16( int16_t* input, uint32_t input_length, int32_t parameter, uint32_t* output_length ) {
	(*output_length) = input_length;
	float* output = (float*) malloc( sizeof(float) * (*output_length) );

	float parameter_float = (float) parameter;
	uint32_t i;
	for(i = 0; i < input_length; ++i ) {
		output[i] = ((float) input[i])/parameter_float;
	}

	return output;
}

float* integer_decode_from_32( int32_t* input, uint32_t input_length, int32_t parameter, uint32_t* output_length ) {
	(*output_length) = input_length;
	float* output = (float*) malloc( sizeof(float) * (*output_length) );

	float parameter_float = (float) parameter;
	uint32_t i;
	for(i = 0; i < input_length; ++i ) {
		output[i] = ((float) input[i])/parameter_float;
	}

	return output;
}


