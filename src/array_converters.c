#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

// From bytes[] to float32[], int8[], int16[], int32[] and string

typedef union {
	char c[4];
	float f;
} four_bytes_as_float;

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

typedef union {
	char c;
	int8_t i;
} one_byte_as_int8;

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

typedef union {
	char c[2];
	int16_t i;
} two_bytes_as_int16;

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

typedef union {
	char c[4];
	int32_t i;
} four_bytes_as_int32;

/*
int get_strategy(const char * bytes) {
	return get_int(bytes,0);
}

int get_len(const char * bytes){
	return get_int(bytes,4);
}

int  get_param(const char * bytes) {
	return get_int(bytes,8);
}
*/

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

	int32_t* output = malloc( (*output_length)*sizeof(int32_t) );

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


// int 32
int32_t* convert_to_int_32(const char* bytes){
printf("in funct");
long tot_len = (sizeof(bytes)-12)/4;
printf("gotlen");
int32_t* output = malloc(sizeof(int32_t)*(tot_len)); // The output needs to be freed by the calling process
printf("alloc");
int ind = 0;
int i;
for(i=12; i<sizeof(bytes); i+=4){
	output[ind] = get_int(bytes,i);
	ind++;
}
return output;
}


// int 16
int32_t* convert_to_int_16(const char* bytes){
printf("in funct");
long tot_len = (sizeof(bytes)-12)/2;
int ind = 0;
int32_t* output = malloc(sizeof(int32_t)*(tot_len)); // The output needs to be freed by the calling process
int i;
for(i=12; i<sizeof(bytes); i+=2){
         output[ind] = get_short(bytes,i);
	ind++;
}

return output;
}


// int 8
int32_t* convert_to_int_8(const char* bytes){

long tot_len = (sizeof(bytes)-12);

int32_t* output = malloc(sizeof(int32_t)*(tot_len)); // The output needs to be freed by the calling process

int ind = 0;
int i;
for(i=12; i<sizeof(bytes); i+=1){
         output[ind] = get_byte(bytes,i);
	ind++;
}

return output;
}
