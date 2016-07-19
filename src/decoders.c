#include "num_converters.c"
#include "array_converters.c"

int get_strategy(const char * bytes) {
	return get_int(bytes,0);
}

int get_len(const char * bytes){
	return get_int(bytes,4);
}

int  get_param(const char * bytes) {
	return get_int(bytes,8);
}

float* decode_apply_strategy_into_float( const char* input, unsigned long input_length, unsigned long* output_length, int strategy, int32_t parameter ) {
	switch( strategy ) {
		case 1:
			return float_from_bytes( input, input_length, output_length );
		case 2:
			fprintf( stderr, "Error in the function decode_apply_strategy_into_float: the demanded strategy is %i and the demanded output is a float array.\n", strategy );
			return NULL;
		case 3:
			fprintf( stderr, "Error in the function decode_apply_strategy_into_float: the demanded strategy is %i and the demanded output is a float array.\n", strategy );
			return NULL;
		case 4:
			fprintf( stderr, "Error in the function decode_apply_strategy_into_float: the demanded strategy is %i and the demanded output is a float array.\n", strategy );
			return NULL;
		case 5:
			fprintf( stderr, "Error in the function decode_apply_strategy_into_float: the demanded strategy is %i and the demanded output is a float array.\n", strategy );
			return NULL;
		case 6:
			fprintf( stderr, "Error in the function decode_apply_strategy_into_float: the demanded strategy is %i and the demanded output is a float array.\n", strategy );
			return NULL;
		case 7:
			fprintf( stderr, "Error in the function decode_apply_strategy_into_float: the demanded strategy is %i and the demanded output is a float array.\n", strategy );
			return NULL;
		case 8:
			fprintf( stderr, "Error in the function decode_apply_strategy_into_float: the demanded strategy is %i and the demanded output is a float array.\n", strategy );
			return NULL;
		case 9: {
			unsigned long step1_length;
			int32_t* step1 = int32_from_bytes( input, input_length, &step1_length );

			unsigned long step2_length;
			int32_t* step2 = run_length_decode( step1, step1_length, &step2_length );
			free(step1);

			float* output = integer_decode_from_32( step2, step2_length, parameter, output_length );
			free(step2);

			return output;
		}
		case 10: {
			unsigned long step1_length;
			int16_t* step1 = int16_from_bytes( input, input_length, &step1_length );

			unsigned long step2_length;
			int32_t* step2 = recursive_indexing_decode_from_16( step1, step1_length, &step2_length );
			free(step1);

			unsigned long step3_length;
			int32_t* step3 = delta_decode( step2, step2_length, &step3_length );
			free(step2);

			float* output = integer_decode_from_32( step3, step3_length, parameter, output_length );
			free(step3);

			return output;
		}
		case 11: {
			unsigned long step1_length;
			int16_t* step1 = int16_from_bytes( input, input_length, &step1_length );

			float* output = integer_decode_from_16( step1, step1_length, parameter, output_length );
			free(step1);

			return output;
		}
		case 12: {
			unsigned long step1_length;
			int16_t* step1 = int16_from_bytes( input, input_length, &step1_length );

			unsigned long step2_length;
			int32_t* step2 = recursive_indexing_decode_from_16( step1, step1_length, &step2_length );
			free(step1);

			float* output = integer_decode_from_32( step2, step2_length, parameter, output_length );
			free(step2);

			return output;
		}
		case 13: {
			unsigned long step1_length;
			int8_t* step1 = int8_from_bytes( input, input_length, &step1_length );

			unsigned long step2_length;
			int32_t* step2 = recursive_indexing_decode_from_8( step1, step1_length, &step2_length );
			free(step1);

			float* output = integer_decode_from_32( step2, step2_length, parameter, output_length );
			free(step2);

			return output;
		}
		case 14:
			fprintf( stderr, "Error in the function decode_apply_strategy_into_float: the demanded strategy is %i and the demanded output is a float array.\n", strategy );
			return NULL;
		case 15:
			fprintf( stderr, "Error in the function decode_apply_strategy_into_float: the demanded strategy is %i and the demanded output is a float array.\n", strategy );
			return NULL;
	}
}

int8_t* decode_apply_strategy_into_int8( const char* input, unsigned long input_length, unsigned long* output_length, int strategy ) {
	switch( strategy ) {
		case 1:
			fprintf( stderr, "Error in the function decode_apply_strategy_into_int8: the demanded strategy is %i and the demanded output is an int8 array.\n", strategy );
			return NULL;
		case 2:
			return int8_from_bytes( input, input_length, output_length );
		case 3:
			fprintf( stderr, "Error in the function decode_apply_strategy_into_int8: the demanded strategy is %i and the demanded output is an int8 array.\n", strategy );
			return NULL;
		case 4:
			fprintf( stderr, "Error in the function decode_apply_strategy_into_int8: the demanded strategy is %i and the demanded output is an int8 array.\n", strategy );
			return NULL;
		case 5:
			fprintf( stderr, "Error in the function decode_apply_strategy_into_int8: the demanded strategy is %i and the demanded output is an int8 array.\n", strategy );
			return NULL;
		case 6:
			fprintf( stderr, "Error in the function decode_apply_strategy_into_int8: the demanded strategy is %i and the demanded output is an int8 array.\n", strategy );
			return NULL;
		case 7:
			fprintf( stderr, "Error in the function decode_apply_strategy_into_int8: the demanded strategy is %i and the demanded output is an int8 array.\n", strategy );
			return NULL;
		case 8:
			fprintf( stderr, "Error in the function decode_apply_strategy_into_int8: the demanded strategy is %i and the demanded output is an int8 array.\n", strategy );
			return NULL;
		case 9:
			fprintf( stderr, "Error in the function decode_apply_strategy_into_int8: the demanded strategy is %i and the demanded output is an int8 array.\n", strategy );
			return NULL;
		case 10:
			fprintf( stderr, "Error in the function decode_apply_strategy_into_int8: the demanded strategy is %i and the demanded output is an int8 array.\n", strategy );
			return NULL;
		case 11:
			fprintf( stderr, "Error in the function decode_apply_strategy_into_int8: the demanded strategy is %i and the demanded output is an int8 array.\n", strategy );
			return NULL;
		case 12:
			fprintf( stderr, "Error in the function decode_apply_strategy_into_int8: the demanded strategy is %i and the demanded output is an int8 array.\n", strategy );
			return NULL;
		case 13:
			fprintf( stderr, "Error in the function decode_apply_strategy_into_int8: the demanded strategy is %i and the demanded output is an int8 array.\n", strategy );
			return NULL;
		case 14:
			fprintf( stderr, "Error in the function decode_apply_strategy_into_int8: the demanded strategy is %i and the demanded output is an int8 array.\n", strategy );
			return NULL;
		case 15:
			fprintf( stderr, "Error in the function decode_apply_strategy_into_int8: the demanded strategy is %i and the demanded output is an int8 array.\n", strategy );
			return NULL;
	}
}

int16_t* decode_apply_strategy_into_int16( const char* input, unsigned long input_length, unsigned long* output_length, int strategy ) {
	switch( strategy ) {
		case 1:
			fprintf( stderr, "Error in the function decode_apply_strategy_into_int16: the demanded strategy is %i and the demanded output is an int16 array.\n", strategy );
			return NULL;
		case 2:
			fprintf( stderr, "Error in the function decode_apply_strategy_into_int16: the demanded strategy is %i and the demanded output is an int16 array.\n", strategy );
			return NULL;
		case 3:
			return int16_from_bytes( input, input_length, output_length );
		case 4:
			fprintf( stderr, "Error in the function decode_apply_strategy_into_int16: the demanded strategy is %i and the demanded output is an int16 array.\n", strategy );
			return NULL;
		case 5:
			fprintf( stderr, "Error in the function decode_apply_strategy_into_int16: the demanded strategy is %i and the demanded output is an int16 array.\n", strategy );
			return NULL;
		case 6:
			fprintf( stderr, "Error in the function decode_apply_strategy_into_int16: the demanded strategy is %i and the demanded output is an int16 array.\n", strategy );
			return NULL;
		case 7:
			fprintf( stderr, "Error in the function decode_apply_strategy_into_int16: the demanded strategy is %i and the demanded output is an int16 array.\n", strategy );
			return NULL;
		case 8:
			fprintf( stderr, "Error in the function decode_apply_strategy_into_int16: the demanded strategy is %i and the demanded output is an int16 array.\n", strategy );
			return NULL;
		case 9:
			fprintf( stderr, "Error in the function decode_apply_strategy_into_int16: the demanded strategy is %i and the demanded output is an int16 array.\n", strategy );
			return NULL;
		case 10:
			fprintf( stderr, "Error in the function decode_apply_strategy_into_int16: the demanded strategy is %i and the demanded output is an int16 array.\n", strategy );
			return NULL;
		case 11:
			fprintf( stderr, "Error in the function decode_apply_strategy_into_int16: the demanded strategy is %i and the demanded output is an int16 array.\n", strategy );
			return NULL;
		case 12:
			fprintf( stderr, "Error in the function decode_apply_strategy_into_int16: the demanded strategy is %i and the demanded output is an int16 array.\n", strategy );
			return NULL;
		case 13:
			fprintf( stderr, "Error in the function decode_apply_strategy_into_int16: the demanded strategy is %i and the demanded output is an int16 array.\n", strategy );
			return NULL;
		case 14:
			fprintf( stderr, "Error in the function decode_apply_strategy_into_int16: the demanded strategy is %i and the demanded output is an int16 array.\n", strategy );
			return NULL;
		case 15:
			fprintf( stderr, "Error in the function decode_apply_strategy_into_int16: the demanded strategy is %i and the demanded output is an int16 array.\n", strategy );
			return NULL;
	}
}

int32_t* decode_apply_strategy_into_int32( const char* input, unsigned long input_length, unsigned long* output_length, int strategy ) {
	switch( strategy ) {
		case 1:
			fprintf( stderr, "Error in the function decode_apply_strategy_into_int32: the demanded strategy is %i and the demanded output is an int32 array.\n", strategy );
			return NULL;
		case 2:
			fprintf( stderr, "Error in the function decode_apply_strategy_into_int32: the demanded strategy is %i and the demanded output is an int32 array.\n", strategy );
			return NULL;
		case 3:
			fprintf( stderr, "Error in the function decode_apply_strategy_into_int32: the demanded strategy is %i and the demanded output is an int32 array.\n", strategy );
			return NULL;
		case 4:
			return int32_from_bytes( input, input_length, output_length );
		case 5:
			fprintf( stderr, "Error in the function decode_apply_strategy_into_int32: the demanded strategy is %i and the demanded output is an int32 array.\n", strategy );
			return NULL;
		case 6:
			fprintf( stderr, "Error in the function decode_apply_strategy_into_int32: the demanded strategy is %i and the demanded output is an int32 array.\n", strategy );
			return NULL;
		case 7: {
			unsigned long step1_length;
			int32_t* step1 = int32_from_bytes( input, input_length, &step1_length );

			int32_t* output = run_length_decode( step1, step1_length, output_length );
			free(step1);

			return output;
		}
		case 8: {
			unsigned long step1_length;
			int32_t* step1 = int32_from_bytes( input, input_length, &step1_length );

			unsigned long step2_length;
			int32_t* step2 = run_length_decode( step1, step1_length, &step2_length );
			free(step1);

			int32_t* output = delta_decode( step2, step2_length, output_length );
			free(step2);

			return output;
		}
		case 9:
			fprintf( stderr, "Error in the function decode_apply_strategy_into_int32: the demanded strategy is %i and the demanded output is an int32 array.\n", strategy );
			return NULL;
		case 10:
			fprintf( stderr, "Error in the function decode_apply_strategy_into_int32: the demanded strategy is %i and the demanded output is an int32 array.\n", strategy );
			return NULL;
		case 11:
			fprintf( stderr, "Error in the function decode_apply_strategy_into_int32: the demanded strategy is %i and the demanded output is an int32 array.\n", strategy );
			return NULL;
		case 12:
			fprintf( stderr, "Error in the function decode_apply_strategy_into_int32: the demanded strategy is %i and the demanded output is an int32 array.\n", strategy );
			return NULL;
		case 13:
			fprintf( stderr, "Error in the function decode_apply_strategy_into_int32: the demanded strategy is %i and the demanded output is an int32 array.\n", strategy );
			return NULL;
		case 14: {
			unsigned long step1_length;
			int16_t* step1 = int16_from_bytes( input, input_length, &step1_length );

			int32_t* output = recursive_indexing_decode_from_16( step1, step1_length, output_length );
			free(step1);

			return output;
		}
		case 15: {
			unsigned long step1_length;
			int8_t* step1 = int8_from_bytes( input, input_length, &step1_length );

			int32_t* output = recursive_indexing_decode_from_8( step1, step1_length, output_length );
			free(step1);

			return output;
		}
	}
}

char** decode_apply_strategy_into_string_array( const char* input, unsigned long input_length, unsigned long* output_length, int strategy, int32_t parameter ) {
	switch( strategy ) {
		case 1:
			fprintf( stderr, "Error in the function decode_apply_strategy_into_int32: the demanded strategy is %i and the demanded output is a string array.\n", strategy );
			return NULL;
		case 2:
			fprintf( stderr, "Error in the function decode_apply_strategy_into_int32: the demanded strategy is %i and the demanded output is a string array.\n", strategy );
			return NULL;
		case 3:
			fprintf( stderr, "Error in the function decode_apply_strategy_into_int32: the demanded strategy is %i and the demanded output is a string array.\n", strategy );
			return NULL;
		case 4:
			return strings_from_bytes( input, input_length, parameter, output_length );
		case 5:
			fprintf( stderr, "Error in the function decode_apply_strategy_into_int32: the demanded strategy is %i and the demanded output is a string array.\n", strategy );
			return NULL;
		case 6:
			fprintf( stderr, "Error in the function decode_apply_strategy_into_int16: the demanded strategy is %i and the demanded output is a string array.\n", strategy );
			return NULL;
		case 7:
			fprintf( stderr, "Error in the function decode_apply_strategy_into_int16: the demanded strategy is %i and the demanded output is a string array.\n", strategy );
			return NULL;
		case 8:
			fprintf( stderr, "Error in the function decode_apply_strategy_into_int16: the demanded strategy is %i and the demanded output is a string array.\n", strategy );
			return NULL;
		case 9:
			fprintf( stderr, "Error in the function decode_apply_strategy_into_int16: the demanded strategy is %i and the demanded output is a string array.\n", strategy );
			return NULL;
		case 10:
			fprintf( stderr, "Error in the function decode_apply_strategy_into_int16: the demanded strategy is %i and the demanded output is a string array.\n", strategy );
			return NULL;
		case 11:
			fprintf( stderr, "Error in the function decode_apply_strategy_into_int16: the demanded strategy is %i and the demanded output is a string array.\n", strategy );
			return NULL;
		case 12:
			fprintf( stderr, "Error in the function decode_apply_strategy_into_int16: the demanded strategy is %i and the demanded output is a string array.\n", strategy );
			return NULL;
		case 13:
			fprintf( stderr, "Error in the function decode_apply_strategy_into_int16: the demanded strategy is %i and the demanded output is a string array.\n", strategy );
			return NULL;
		case 14:
			fprintf( stderr, "Error in the function decode_apply_strategy_into_int16: the demanded strategy is %i and the demanded output is a string array.\n", strategy );
			return NULL;
		case 15:
			fprintf( stderr, "Error in the function decode_apply_strategy_into_int16: the demanded strategy is %i and the demanded output is a string array.\n", strategy );
			return NULL;
	}
}

