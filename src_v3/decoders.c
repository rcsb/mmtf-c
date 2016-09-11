#include "c_headers.h"

void* decode_apply_strategy( const char* input, uint32_t input_length, uint32_t* output_length, int strategy, int32_t parameter ) {
	switch( strategy ) {
		case 1:
			return float_from_bytes( input, input_length, output_length );
		case 2:
			return int8_from_bytes( input, input_length, output_length );
		case 3:
			return int16_from_bytes( input, input_length, output_length );
		case 4:
			return int32_from_bytes( input, input_length, output_length );
		case 5:
			return strings_from_bytes( input, input_length, parameter, output_length );
		case 6: {
			*output_length = input_length;
			char* result = malloc( sizeof( char ) * ( (*output_length) + 1 ) );
			strcpy( result, input );
			result[*output_length] = '\0';
			return result;
		}
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
		default: {
			fprintf( stderr, "Error in decode_apply_strategy: %i does not refer to any strategy.\n", strategy );
			return NULL;
		}
	}
}

