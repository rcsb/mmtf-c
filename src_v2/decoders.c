#include "c_headers.h"

float* decode_apply_strategy_into_float( const char* input, uint32_t input_length, uint32_t* output_length, int strategy, int32_t parameter ) {
	switch( strategy ) {
		case 1:
			return float_from_bytes( input, input_length, output_length );
		case 9: {
			uint32_t step1_length;
			int32_t* step1 = int32_from_bytes( input, input_length, &step1_length );

			uint32_t step2_length;
			int32_t* step2 = run_length_decode( step1, step1_length, &step2_length );
			free(step1);

			float* output = integer_decode_from_32( step2, step2_length, parameter, output_length );
			free(step2);

			return output;
		}
		case 10: {
			uint32_t step1_length;
			int16_t* step1 = int16_from_bytes( input, input_length, &step1_length );

			uint32_t step2_length;
			int32_t* step2 = recursive_indexing_decode_from_16( step1, step1_length, &step2_length );
			free(step1);

			uint32_t step3_length;
			int32_t* step3 = delta_decode( step2, step2_length, &step3_length );
			free(step2);

			float* output = integer_decode_from_32( step3, step3_length, parameter, output_length );
			free(step3);

			return output;
		}
		case 11: {
			uint32_t step1_length;
			int16_t* step1 = int16_from_bytes( input, input_length, &step1_length );

			float* output = integer_decode_from_16( step1, step1_length, parameter, output_length );
			free(step1);

			return output;
		}
		case 12: {
			uint32_t step1_length;
			int16_t* step1 = int16_from_bytes( input, input_length, &step1_length );

			uint32_t step2_length;
			int32_t* step2 = recursive_indexing_decode_from_16( step1, step1_length, &step2_length );
			free(step1);

			float* output = integer_decode_from_32( step2, step2_length, parameter, output_length );
			free(step2);

			return output;
		}
		case 13: {
			uint32_t step1_length;
			int8_t* step1 = int8_from_bytes( input, input_length, &step1_length );

			uint32_t step2_length;
			int32_t* step2 = recursive_indexing_decode_from_8( step1, step1_length, &step2_length );
			free(step1);

			float* output = integer_decode_from_32( step2, step2_length, parameter, output_length );
			free(step2);

			return output;
		}
		default:
			break;
	}
	fprintf( stderr, "Error in the function decode_apply_strategy_into_float: the demanded strategy is %i and the demanded output is a float array.\n", strategy );
	return NULL;
}

int8_t* decode_apply_strategy_into_int8( const char* input, uint32_t input_length, uint32_t* output_length, int strategy ) {
	switch( strategy ) {
		case 2:
			return int8_from_bytes( input, input_length, output_length );
		default:
			break;
	}
	fprintf( stderr, "Error in the function decode_apply_strategy_into_int8: the demanded strategy is %i and the demanded output is an int8 array.\n", strategy );
	return NULL;
}

int16_t* decode_apply_strategy_into_int16( const char* input, uint32_t input_length, uint32_t* output_length, int strategy ) {
	switch( strategy ) {
		case 3:
			return int16_from_bytes( input, input_length, output_length );
		default:
			break;
	}
	fprintf( stderr, "Error in the function decode_apply_strategy_into_int16: the demanded strategy is %i and the demanded output is an int16 array.\n", strategy );
	return NULL;
}

int32_t* decode_apply_strategy_into_int32( const char* input, uint32_t input_length, uint32_t* output_length, int strategy ) {
	switch( strategy ) {
		case 4:
			return int32_from_bytes( input, input_length, output_length );
		case 7: {
			uint32_t step1_length;
			int32_t* step1 = int32_from_bytes( input, input_length, &step1_length );

			int32_t* output = run_length_decode( step1, step1_length, output_length );
			free(step1);

			return output;
		}
		case 8: {
			uint32_t step1_length;
			int32_t* step1 = int32_from_bytes( input, input_length, &step1_length );

			uint32_t step2_length;
			int32_t* step2 = run_length_decode( step1, step1_length, &step2_length );
			free(step1);

			int32_t* output = delta_decode( step2, step2_length, output_length );
			free(step2);

			return output;
		}
		case 14: {
			uint32_t step1_length;
			int16_t* step1 = int16_from_bytes( input, input_length, &step1_length );

			int32_t* output = recursive_indexing_decode_from_16( step1, step1_length, output_length );
			free(step1);

			return output;
		}
		case 15: {
			uint32_t step1_length;
			int8_t* step1 = int8_from_bytes( input, input_length, &step1_length );

			int32_t* output = recursive_indexing_decode_from_8( step1, step1_length, output_length );
			free(step1);

			return output;
		}
		default:
			break;
	}
	fprintf( stderr, "Error in the function decode_apply_strategy_into_int32: the demanded strategy is %i and the demanded output is an int32 array.\n", strategy );
	return NULL;
}

char** decode_apply_strategy_into_string_array( const char* input, uint32_t input_length, uint32_t* output_length, int strategy, int32_t parameter ) {
	switch( strategy ) {
		case 5:
			return strings_from_bytes( input, input_length, parameter, output_length );
		default:
			break;
	}
	fprintf( stderr, "Error in the function decode_apply_strategy_into_int32: the demanded strategy is %i and the demanded output is a string array.\n", strategy );
	return NULL;
}

char* decode_apply_strategy_into_string( const char* input, uint32_t input_length, uint32_t* output_length, int strategy, int32_t parameter ) {
	switch( strategy ) {
		case 6:
			*output_length = input_length;
			char* result = malloc( sizeof( char ) * ( (*output_length) + 1 ) );
			strcpy( result, input );
			result[*output_length] = '\0';
			return result;
		default:
			break;
	}
	fprintf( stderr, "Error in the function decode_apply_strategy_into_string: the demanded strategy is %i and the demanded output is a string.\n", strategy );
	return NULL;
}

