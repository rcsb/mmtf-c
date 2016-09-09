//*******************************************************************************
//******	This code is published under the .... license
//******
//******
//******	This file is the code file for the MMTF parser for the C language.
//******
//******	The authors of this code are: Julien Ferté, ...
//******
//******
//******
//******
//******
//******
//******
//*******************************************************************************

#include "mmtf_parser.h"


//*** Initiate a MMTF_container
MMTF_container* MMTF_container_new( void ) {
	MMTF_container* result = malloc( sizeof( MMTF_container ) );

	result->mmtfVersion = NULL;
	result->mmtfProducer = NULL;
	result->spaceGroup = NULL;
	result->structureId = NULL;
	result->title = NULL;
	result->depositionDate = NULL;
	result->releaseDate = NULL;
	result->bioAssemblyList = NULL;
	result->entityList = NULL;
	result->experimentalMethods = NULL;
	result->groupList = NULL;
	result->bondAtomList = NULL;
	result->bondOrderList = NULL;
	result->xCoordList = NULL;
	result->yCoordList = NULL;
	result->zCoordList = NULL;
	result->bFactorList = NULL;
	result->atomIdList = NULL;
	result->altLocList = NULL;
	result->occupancyList = NULL;
	result->groupIdList = NULL;
	result->groupTypeList = NULL;
	result->secStructList = NULL;
	result->insCodeList = NULL;
	result->sequenceIndexList = NULL;
	result->chainIdList = NULL;
	result->chainNameList = NULL;
	result->groupsPerChain = NULL;
	result->chainsPerModel = NULL;

	return result;
}


//*** Destroy a MMTF_container
void MMTF_container_destroy( MMTF_container* thing ) {
	uint64_t i;

	free( thing->mmtfVersion );
	free( thing->mmtfProducer );
	free( thing->spaceGroup );
	free( thing->structureId );
	free( thing->title );
	free( thing->depositionDate );
	free( thing->releaseDate );

	for( i = 0; i < thing->bioAssemblyListCount; ++i ) {
		MMTF_BioAssembly_destroy( &(thing->bioAssemblyList[i]) );
	}
	free( thing->bioAssemblyList );

	for( i = 0; i < thing->entityListCount; ++i ) {
		MMTF_Entity_destroy( &(thing->entityList[i]) );
	}
	free( thing->entityList );

	for( i = 0; i < thing->experimentalMethodsCount; ++i ) {
		free( thing->experimentalMethods[i] );
	}
	free( thing->experimentalMethods );

	for( i = 0; i < thing->groupListCount; ++i ) {
		MMTF_GroupType_destroy( &(thing->groupList[i]) );
	}
	free( thing->groupList );

	free( thing->bondAtomList );
	free( thing->bondOrderList );
	free( thing->xCoordList );
	free( thing->yCoordList );
	free( thing->zCoordList );
	free( thing->bFactorList );
	free( thing->atomIdList );
	free( thing->altLocList );
	free( thing->occupancyList );
	free( thing->groupIdList );
	free( thing->groupTypeList );
	free( thing->secStructList );
	free( thing->insCodeList );
	free( thing->sequenceIndexList );

	for( i = 0; i < thing->chainIdListCount; ++i ) {
		free( thing->chainIdList[i] );
	}
	free( thing->chainIdList );

	for( i = 0; i < thing->chainNameListCount; ++i ) {
		free( thing->chainNameList[i] );
	}
	free( thing->chainNameList );

	free( thing->groupsPerChain );
	free( thing->chainsPerModel );

	free( thing );
}


void MMTF_BioAssembly_destroy( MMTF_BioAssembly* bio_assembly ) {
	free( bio_assembly->name );

	uint64_t i;
	for( i = 0; i < bio_assembly->transformListCount; ++i ) {
		MMTF_Transform_destroy( &(bio_assembly->transformList[i]) );
	}
	free( bio_assembly->transformList );

	free( bio_assembly );
}
void MMTF_Transform_destroy( MMTF_Transform* transform ) {
	free( transform->chainIndexList );

	free( transform );
}
void MMTF_Entity_destroy( MMTF_Entity* entity ) {
	free( entity->chainIndexList );
	free( entity->description );
	free( entity->type );
	free( entity->sequence );

	free( entity );
}
void MMTF_GroupType_destroy( MMTF_GroupType* group_type ) {
	uint64_t i;

	free( group_type->formalChargeList );

	for( i = 0; i < group_type->atomNameListCount; ++i ) {
		free( group_type->atomNameList[i] );
	}
	free( group_type->atomNameList );

	for( i = 0; i < group_type->elementListCount; ++i ) {
		free( group_type->elementList[i] );
	}
	free( group_type->elementList );

	free( group_type->bondAtomList );
	free( group_type->bondOrderList );
	free( group_type->groupName );
	free( group_type->chemCompType );

	free( group_type );
}

//*** Array converters
// From bytes[] to float32[], int8[], int16[], int32[] and string

float* MMTF_parser_float_from_bytes( const char* input, uint32_t input_length, uint32_t* output_length ) {
	if( input_length % 4 != 0 ) {
		fprintf( stderr, "Error in the function float_from_bytes : the input length %u is not a multiple of 4.\n", input_length );
		return NULL;
	}

	(*output_length) = input_length/4;

	float* output = malloc( sizeof(float)*(*output_length) );

	MMTF_parser_four_bytes_as_float u;
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

int8_t* MMTF_parser_int8_from_bytes( const char* input, uint32_t input_length, uint32_t* output_length ) {
	(*output_length) = input_length;

	int8_t* output = malloc( sizeof(int8_t)*(*output_length) );

	MMTF_parser_one_byte_as_int8 u;
	uint32_t i;
	for( i = 0; i < input_length; ++i ) {
		u.c = input[i];

		output[i] = u.i;
	}

	return output;
}

int16_t* MMTF_parser_int16_from_bytes( const char* input, uint32_t input_length, uint32_t* output_length ) {
	if( input_length % 2 != 0 ) {
		fprintf( stderr, "Error in the function int16_from_bytes : the input length %u is not a multiple of 2.\n", input_length );
		return NULL;
	}

	(*output_length) = input_length/2;

	int16_t* output = malloc( sizeof(int16_t)*(*output_length) );

	MMTF_parser_two_bytes_as_int16 u;
	uint32_t i;
	for( i = 0; i < input_length; i = i + 2 ) {
		u.c[0] = input[i+1];
		u.c[1] = input[i];

		output[i/2] = u.i;
	}

	return output;
}

int MMTF_parser_get_strategy(const char * bytes) {
	MMTF_parser_four_bytes_as_int32 ct;

	ct.c[0] = bytes[3];
	ct.c[1] = bytes[2];
	ct.c[2] = bytes[1];
	ct.c[3] = bytes[0];

	return ct.i;
}

int MMTF_parser_get_len(const char * bytes){
	MMTF_parser_four_bytes_as_int32 ct;

	ct.c[0] = bytes[7];
	ct.c[1] = bytes[6];
	ct.c[2] = bytes[5];
	ct.c[3] = bytes[4];

	return ct.i;
}

int  MMTF_parser_get_param(const char * bytes) {
	MMTF_parser_four_bytes_as_int32 ct;

	ct.c[0] = bytes[11];
	ct.c[1] = bytes[10];
	ct.c[2] = bytes[9];
	ct.c[3] = bytes[8];

	return ct.i;
}

int32_t* MMTF_parser_int32_from_bytes( const char* input, const uint32_t input_length, uint32_t* output_length ) {
	if( input_length % 4 != 0 ) {
		fprintf( stderr, "Error in the function int32_from_bytes : the input length %u is not a multiple of 4.\n", input_length );
		return NULL;
	}

	(*output_length) = input_length/4;

	int32_t* output = malloc( sizeof(int32_t) * (*output_length) );

	MMTF_parser_four_bytes_as_int32 u;
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

char* MMTF_parser_string_from_int32( const int32_t* input, const uint32_t input_length, uint32_t* output_length ) {
	(*output_length) = input_length * 4;

	char* output = malloc( sizeof( char ) * (*output_length) );

	MMTF_parser_four_bytes_as_int32 u;
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

char** MMTF_parser_strings_from_bytes( const char* input, uint32_t input_length, uint32_t parameter, uint32_t* output_length ) {
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


//*** Array decoders
// Run-length decode
int32_t* MMTF_parser_run_length_decode( int32_t* input, uint32_t input_length, uint32_t* output_length ) {
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
int32_t* MMTF_parser_delta_decode( int32_t* input, uint32_t input_length, uint32_t* output_length ) {
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
int32_t* MMTF_parser_recursive_indexing_decode_from_16( int16_t* input, uint32_t input_length, uint32_t* output_length ) {
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

int32_t* MMTF_parser_recursive_indexing_decode_from_8( int8_t* input, uint32_t input_length, uint32_t* output_length ) {
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
float* MMTF_parser_integer_decode_from_16( int16_t* input, uint32_t input_length, int32_t parameter, uint32_t* output_length ) {
	(*output_length) = input_length;
	float* output = (float*) malloc( sizeof(float) * (*output_length) );

	float parameter_float = (float) parameter;
	uint32_t i;
	for(i = 0; i < input_length; ++i ) {
		output[i] = ((float) input[i])/parameter_float;
	}

	return output;
}

float* MMTF_parser_integer_decode_from_32( int32_t* input, uint32_t input_length, int32_t parameter, uint32_t* output_length ) {
	(*output_length) = input_length;
	float* output = (float*) malloc( sizeof(float) * (*output_length) );

	float parameter_float = (float) parameter;
	uint32_t i;
	for(i = 0; i < input_length; ++i ) {
		output[i] = ((float) input[i])/parameter_float;
	}

	return output;
}


//*** Applying a decoding strategy for getting an array
void* MMTF_parser_decode_apply_strategy( const char* input, uint32_t input_length, uint32_t* output_length, int strategy, int32_t parameter ) {
	switch( strategy ) {
		case 1:
			return MMTF_parser_float_from_bytes( input, input_length, output_length );
		case 2:
			return MMTF_parser_int8_from_bytes( input, input_length, output_length );
		case 3:
			return MMTF_parser_int16_from_bytes( input, input_length, output_length );
		case 4:
			return MMTF_parser_int32_from_bytes( input, input_length, output_length );
		case 5:
			return MMTF_parser_strings_from_bytes( input, input_length, parameter, output_length );
		case 6: {
			*output_length = input_length;
			char* result = malloc( sizeof( char ) * ( (*output_length) + 1 ) );
			strcpy( result, input );
			result[*output_length] = '\0';
			return result;
		}
		case 7: {
			uint32_t step1_length;
			int32_t* step1 = MMTF_parser_int32_from_bytes( input, input_length, &step1_length );

			int32_t* output = MMTF_parser_run_length_decode( step1, step1_length, output_length );
			free(step1);

			return output;
		}
		case 8: {
			uint32_t step1_length;
			int32_t* step1 = MMTF_parser_int32_from_bytes( input, input_length, &step1_length );

			uint32_t step2_length;
			int32_t* step2 = MMTF_parser_run_length_decode( step1, step1_length, &step2_length );
			free(step1);

			int32_t* output = MMTF_parser_delta_decode( step2, step2_length, output_length );
			free(step2);

			return output;
		}
		case 9: {
			uint32_t step1_length;
			int32_t* step1 = MMTF_parser_int32_from_bytes( input, input_length, &step1_length );

			uint32_t step2_length;
			int32_t* step2 = MMTF_parser_run_length_decode( step1, step1_length, &step2_length );
			free(step1);

			float* output = MMTF_parser_integer_decode_from_32( step2, step2_length, parameter, output_length );
			free(step2);

			return output;
		}
		case 10: {
			uint32_t step1_length;
			int16_t* step1 = MMTF_parser_int16_from_bytes( input, input_length, &step1_length );

			uint32_t step2_length;
			int32_t* step2 = MMTF_parser_recursive_indexing_decode_from_16( step1, step1_length, &step2_length );
			free(step1);

			uint32_t step3_length;
			int32_t* step3 = MMTF_parser_delta_decode( step2, step2_length, &step3_length );
			free(step2);

			float* output = MMTF_parser_integer_decode_from_32( step3, step3_length, parameter, output_length );
			free(step3);

			return output;
		}
		case 11: {
			uint32_t step1_length;
			int16_t* step1 = MMTF_parser_int16_from_bytes( input, input_length, &step1_length );

			float* output = MMTF_parser_integer_decode_from_16( step1, step1_length, parameter, output_length );
			free(step1);

			return output;
		}
		case 12: {
			uint32_t step1_length;
			int16_t* step1 = MMTF_parser_int16_from_bytes( input, input_length, &step1_length );

			uint32_t step2_length;
			int32_t* step2 = MMTF_parser_recursive_indexing_decode_from_16( step1, step1_length, &step2_length );
			free(step1);

			float* output = MMTF_parser_integer_decode_from_32( step2, step2_length, parameter, output_length );
			free(step2);

			return output;
		}
		case 13: {
			uint32_t step1_length;
			int8_t* step1 = MMTF_parser_int8_from_bytes( input, input_length, &step1_length );

			uint32_t step2_length;
			int32_t* step2 = MMTF_parser_recursive_indexing_decode_from_8( step1, step1_length, &step2_length );
			free(step1);

			float* output = MMTF_parser_integer_decode_from_32( step2, step2_length, parameter, output_length );
			free(step2);

			return output;
		}
		case 14: {
			uint32_t step1_length;
			int16_t* step1 = MMTF_parser_int16_from_bytes( input, input_length, &step1_length );

			int32_t* output = MMTF_parser_recursive_indexing_decode_from_16( step1, step1_length, output_length );
			free(step1);

			return output;
		}
		case 15: {
			uint32_t step1_length;
			int8_t* step1 = MMTF_parser_int8_from_bytes( input, input_length, &step1_length );

			int32_t* output = MMTF_parser_recursive_indexing_decode_from_8( step1, step1_length, output_length );
			free(step1);

			return output;
		}
		default: {
			fprintf( stderr, "Error in decode_apply_strategy: %i does not refer to any strategy.\n", strategy );
			return NULL;
		}
	}
}


//*** Unpacking from MsgPack and applying strategy
char* MMTF_parser_fetch_string( msgpack_object* object ) {
	if( object->type != MSGPACK_OBJECT_STR ) {
		fprintf( stderr, "Error in fetch_string: the entry encoded in the MMTF is not a string.\n" );
		return NULL;
	}

	msgpack_object_str* value_string = &(object->via.str);

	char* result = malloc( sizeof( char ) * ( value_string->size + 1 ) );
	strncpy( result, value_string->ptr, value_string->size );
	result[value_string->size] = '\0';

	return result;
}

char MMTF_parser_fetch_char( msgpack_object* object ) {
	if( object->type != MSGPACK_OBJECT_STR) {
		fprintf( stderr, "Error in fetch_string: the entry encoded in the MMTF is not a string.\n" );
		return '\0';
	}

	return *(object->via.str.ptr);
}

uint64_t MMTF_parser_fetch_int( msgpack_object* object ) {
	uint64_t result;

    if(object->type == MSGPACK_OBJECT_POSITIVE_INTEGER) {
        result = object->via.u64;
    }
    else if(object->type == MSGPACK_OBJECT_NEGATIVE_INTEGER) {
        result = object->via.i64;
	}
	else {
		fprintf( stderr, "Error in fetch_int: the entry encoded in the MMTF is not an integer.\n" );
		return NAN;
	}

	return result;
}

float MMTF_parser_fetch_float( msgpack_object* object ) {
	if( object->type != MSGPACK_OBJECT_FLOAT ) {
		fprintf( stderr, "Error in fetch_float: the entry encoded in the MMTF is not a float.\n" );
		return NAN;
	}
	
	return (float) object->via.f64;
}

void* MMTF_parser_fetch_array( msgpack_object* object, uint64_t* length ) {
	if( object->type != MSGPACK_OBJECT_BIN ) {
		fprintf( stderr, "Error in fetch_array: the entry encoded in the MMTF is not binary data.\n" );
		return NULL;
	}

	const char* bytes = object->via.bin.ptr;

	int parameter = MMTF_parser_get_param(bytes);
	int strategy = MMTF_parser_get_strategy(bytes);
	(*length) = MMTF_parser_get_len(bytes);

//printf( "Applying the strategy %i with parameter %i for decoding a byte array of length %i into an int32 array of length %lu.\n", strategy, parameter, object->via.bin.size - 12, *length );

    uint32_t out_length;
    return MMTF_parser_decode_apply_strategy(bytes + 12, object->via.bin.size - 12, &out_length, strategy, parameter);
}

size_t* MMTF_parser_fetch_clear_lu_array( msgpack_object* object, uint64_t* length ) {
	if( object->type != MSGPACK_OBJECT_ARRAY ) {
		fprintf( stderr, "Error in fetch_clear_array: the entry encoded in the MMTF is not an array.\n" );
		return NULL;
	}

	msgpack_object* current_value = object->via.array.ptr;
	(*length) = object->via.array.size;
	msgpack_object* last_value = current_value + (*length);

//printf( "Unpacking an unsigned long int array of length %lu.\n", *length );

	size_t* result = malloc( sizeof( size_t ) * (*length) );

	int i = 0;
	for( ; current_value < last_value; ++current_value ) {
		result[i] = current_value->via.u64;
		++i;
	}

    return result;
}

int* MMTF_parser_fetch_clear_int_array( msgpack_object* object, uint64_t* length ) {
	if( object->type != MSGPACK_OBJECT_ARRAY ) {
		fprintf( stderr, "Error in fetch_clear_array: the entry encoded in the MMTF is not an array.\n" );
		return NULL;
	}

	msgpack_object* current_value = object->via.array.ptr;
	(*length) = object->via.array.size;
	msgpack_object* last_value = current_value + (*length);

//printf( "Unpacking an int array of length %lu.\n", *length );

	int* result = malloc( sizeof( int ) * (*length) );

	int i = 0;
	for( ; current_value < last_value; ++current_value ) {
		result[i] = (int) current_value->via.u64;
		++i;
	}

    return result;
}

int32_t* MMTF_parser_fetch_clear_int32_array( msgpack_object* object, uint64_t* length ) {
	if( object->type != MSGPACK_OBJECT_ARRAY ) {
		fprintf( stderr, "Error in fetch_clear_array: the entry encoded in the MMTF is not an array.\n" );
		return NULL;
	}

	msgpack_object* current_value = object->via.array.ptr;
	(*length) = object->via.array.size;
	msgpack_object* last_value = current_value + (*length);

//printf( "Unpacking an int32 array of length %lu.\n", *length );

	int32_t* result = malloc( sizeof( int32_t ) * (*length) );

	int i = 0;
	for( ; current_value < last_value; ++current_value ) {
		result[i] = (int32_t) current_value->via.u64;
		++i;
	}

    return result;
}

char* MMTF_parser_fetch_clear_int8_array( msgpack_object* object, uint64_t* length ) {
	if( object->type != MSGPACK_OBJECT_ARRAY ) {
		fprintf( stderr, "Error in fetch_clear_array: the entry encoded in the MMTF is not an array.\n" );
		return NULL;
	}

	msgpack_object* current_value = object->via.array.ptr;
	(*length) = object->via.array.size;
	msgpack_object* last_value = current_value + (*length);

//printf( "Unpacking an int8 array of length %lu.\n", *length );

	char* result = malloc( sizeof( char ) * (*length) );

	int i = 0;
	for( ; current_value < last_value; ++current_value ) {
		result[i] = (char) current_value->via.u64;
		++i;
	}

    return result;
}

float* MMTF_parser_fetch_clear_float_array( msgpack_object* object, uint64_t* length ) {
	if( object->type != MSGPACK_OBJECT_ARRAY ) {
		fprintf( stderr, "Error in fetch_clear_array: the entry encoded in the MMTF is not an array.\n" );
		return NULL;
	}

	msgpack_object* current_value = object->via.array.ptr;
	(*length) = object->via.array.size;
	msgpack_object* last_value = current_value + (*length);

//printf( "Unpacking a float array of length %lu.\n", *length );

	float* result = malloc( sizeof( float ) * (*length) );

	int i = 0;
	for( ; current_value < last_value; ++current_value ) {
		result[i] = (float) current_value->via.f64;
		++i;
	}

    return result;
}

char** MMTF_parser_fetch_clear_string_array( msgpack_object* object, uint64_t* length ) {
	if( object->type != MSGPACK_OBJECT_ARRAY ) {
		fprintf( stderr, "Error in fetch_clear_array: the entry encoded in the MMTF is not an array.\n" );
		return NULL;
	}

	msgpack_object* current_value = object->via.array.ptr;
	(*length) = object->via.array.size;
	msgpack_object* last_value = current_value + (*length);

//printf( "Unpacking a string array of length %lu.\n", *length );

	char** result = malloc( sizeof( char* ) * (*length) );

	int i = 0;
	uint32_t string_size;
	for( ; current_value < last_value; ++current_value ) {
		string_size = current_value->via.str.size;
		result[i] = malloc( sizeof( char ) * ( string_size + 1 ) );
		strncpy( result[i], current_value->via.str.ptr, string_size );
		result[i][string_size] = '\0';
		++i;
	}

    return result;
}

MMTF_Entity* MMTF_parser_fetch_entityList( msgpack_object* object, uint64_t* length ) {
	if( object->type != MSGPACK_OBJECT_ARRAY ) {
		fprintf( stderr, "Error in fetch_entityList: the entry encoded in the MMTF is not an array.\n" );
		return NULL;
	}

	msgpack_object* current_entity = object->via.array.ptr;
	uint32_t entityList_count = object->via.array.size;
	msgpack_object* stop_entity = object->via.array.ptr + entityList_count;

//printf( "Decoding an array of %u Entity.\n", entityList_count );

	MMTF_Entity* result = malloc( sizeof( MMTF_Entity ) * entityList_count );

	int i = 0;
	for( ; current_entity < stop_entity; ++current_entity ) {
		MMTF_parser_put_entity( current_entity, &(result[i]) );
		++i;
	}

    return result;
}

void MMTF_parser_put_entity( msgpack_object* object, MMTF_Entity* entity ) {
	if( object->type != MSGPACK_OBJECT_MAP ) {
		fprintf( stderr, "Error in put_entity: the entry encoded in the MMTF is not a map.\n" );
		return;
	}

	msgpack_object_kv* current_key_value = object->via.map.ptr;
	msgpack_object_kv* last_key_value = current_key_value + object->via.map.size;

	for( ; current_key_value <= last_key_value; ++current_key_value ) {
		msgpack_object* key = &(current_key_value->key);
		msgpack_object* value = &(current_key_value->val);

		if( key->type == MSGPACK_OBJECT_STR ) {

//printf( "Key %.*s:\n", key->via.str.size, key->via.str.ptr );

			uint32_t key_size = key->via.str.size;
			const char* key_string = key->via.str.ptr;

			if( key_size == strlen( "description" ) && strncmp( key_string, "description", key_size ) == 0 ) {
				(*entity).description = MMTF_parser_fetch_string( value );

//printf( "description : %s\n\n", (*entity).description );
			}

			if( key_size == strlen( "type" ) && strncmp( key_string, "type", key_size ) == 0 ) {
				(*entity).type = MMTF_parser_fetch_string( value );

//printf( "type : %s\n\n", (*entity).type );
			}

			if( key_size == strlen( "chainIndexList" ) && strncmp( key_string, "chainIndexList", key_size ) == 0 ) {
				uint64_t length;
				(*entity).chainIndexList = MMTF_parser_fetch_clear_lu_array( value, &length );
			}

			if( key_size == strlen( "sequence" ) && strncmp( key_string, "sequence", key_size ) == 0 ) {
				(*entity).sequence = MMTF_parser_fetch_string( value );

//printf( "sequence : %s\n\n", (*entity).sequence );
			}
		}
	}
}

MMTF_GroupType* MMTF_parser_fetch_groupList( msgpack_object* object, uint64_t* length ) {
	if( object->type != MSGPACK_OBJECT_ARRAY ) {
		fprintf( stderr, "Error in fetch_groupList: the entry encoded in the MMTF is not an array.\n" );
		return NULL;
	}

	msgpack_object* current_groupType = object->via.array.ptr;
	uint32_t groupList_count = object->via.array.size;
	msgpack_object* stop_groupType = object->via.array.ptr + groupList_count;

//printf( "Decoding an array of %u GroupType.\n", groupList_count );

	MMTF_GroupType* result = malloc( sizeof( MMTF_GroupType ) * groupList_count );

	int i = 0;
	for( ; current_groupType < stop_groupType; ++current_groupType ) {
		MMTF_parser_put_groupType( current_groupType, &(result[i]) );
		++i;
	}

    return result;
}

void MMTF_parser_put_groupType( msgpack_object* object, MMTF_GroupType* group_type ) {
	if( object->type != MSGPACK_OBJECT_MAP ) {
		fprintf( stderr, "Error in put_groupType: the entry encoded in the MMTF is not a map.\n" );
		return;
	}

	msgpack_object_kv* current_key_value = object->via.map.ptr;
	msgpack_object_kv* last_key_value = current_key_value + object->via.map.size;

	for( ; current_key_value <= last_key_value; ++current_key_value ) {
		msgpack_object* key = &(current_key_value->key);
		msgpack_object* value = &(current_key_value->val);

		if( key->type == MSGPACK_OBJECT_STR ) {

//printf( "Key %.*s:\n", key->via.str.size, key->via.str.ptr );

			uint32_t key_size = key->via.str.size;
			const char* key_string = key->via.str.ptr;

			if( key_size == strlen( "formalChargeList" ) && strncmp( key_string, "formalChargeList", key_size ) == 0 ) {
				uint64_t length;
				(*group_type).formalChargeList = MMTF_parser_fetch_clear_int_array( value, &length );
			}

			if( key_size == strlen( "atomNameList" ) && strncmp( key_string, "atomNameList", key_size ) == 0 ) {
				(*group_type).atomNameList = MMTF_parser_fetch_clear_string_array( value, &(group_type->atomNameListCount) );
			}

			if( key_size == strlen( "elementList" ) && strncmp( key_string, "elementList", key_size ) == 0 ) {
				(*group_type).elementList = MMTF_parser_fetch_clear_string_array( value, &(group_type->elementListCount) );
			}

			if( key_size == strlen( "bondAtomList" ) && strncmp( key_string, "bondAtomList", key_size ) == 0 ) {
				uint64_t length;
				(*group_type).bondAtomList = MMTF_parser_fetch_clear_int_array( value, &length );
			}

			if( key_size == strlen( "bondOrderList" ) && strncmp( key_string, "bondOrderList", key_size ) == 0 ) {
				uint64_t length;
				(*group_type).bondOrderList = MMTF_parser_fetch_clear_int8_array( value, &length );
			}

			if( key_size == strlen( "groupName" ) && strncmp( key_string, "groupName", key_size ) == 0 ) {
				(*group_type).groupName = MMTF_parser_fetch_string( value );

//printf( "groupName : %s\n\n", (*group_type).groupName );
			}

			if( key_size == strlen( "singleLetterCode" ) && strncmp( key_string, "singleLetterCode", key_size ) == 0 ) {
				(*group_type).singleLetterCode = MMTF_parser_fetch_char( value );

//printf( "singleLetterCode : %c\n\n", (*group_type).singleLetterCode );
			}

			if( key_size == strlen( "chemCompType" ) && strncmp( key_string, "chemCompType", key_size ) == 0 ) {
				(*group_type).chemCompType = MMTF_parser_fetch_string( value );

//printf( "chemCompType : %s\n\n", (*group_type).chemCompType );
			}
		}
	}
}

MMTF_BioAssembly* MMTF_parser_fetch_bioAssemblyList( msgpack_object* object, uint64_t* length ) {
	if( object->type != MSGPACK_OBJECT_ARRAY ) {
		fprintf( stderr, "Error in fetch_bioAssemblyList: the entry encoded in the MMTF is not an array.\n" );
		return NULL;
	}

	msgpack_object* current_bioAssembly = object->via.array.ptr;
	uint32_t bioAssembly_count = object->via.array.size;
	msgpack_object* stop_bioAssembly = object->via.array.ptr + bioAssembly_count;

//printf( "Decoding an array of %u BioAssembly.\n", bioAssembly_count );

	MMTF_BioAssembly* result = malloc( sizeof( MMTF_BioAssembly ) * bioAssembly_count );

	int i = 0;
	for( ; current_bioAssembly < stop_bioAssembly; ++current_bioAssembly ) {
		MMTF_parser_put_bioAssembly( current_bioAssembly, &(result[i]) );
		++i;
	}

    return result;
}

void MMTF_parser_put_bioAssembly( msgpack_object* object, MMTF_BioAssembly* bio_assembly ) {
	if( object->type != MSGPACK_OBJECT_MAP ) {
		fprintf( stderr, "Error in put_bioAssembly: the entry encoded in the MMTF is not a map.\n" );
		return;
	}

	msgpack_object_kv* current_key_value = object->via.map.ptr;
	msgpack_object_kv* last_key_value = current_key_value + object->via.map.size;

	for( ; current_key_value <= last_key_value; ++current_key_value ) {
		msgpack_object* key = &(current_key_value->key);
		msgpack_object* value = &(current_key_value->val);

		if( key->type == MSGPACK_OBJECT_STR ) {

//printf( "Key %.*s:\n", key->via.str.size, key->via.str.ptr );

			uint32_t key_size = key->via.str.size;
			const char* key_string = key->via.str.ptr;

			if( key_size == strlen( "name" ) && strncmp( key_string, "name", key_size ) == 0 ) {
				(*bio_assembly).name = MMTF_parser_fetch_string( value );

//printf( "name : %s\n", (*bio_assembly).name );
			}

			if( key_size == strlen( "transformList" ) && strncmp( key_string, "transformList", key_size ) == 0 ) {
				(*bio_assembly).transformList = MMTF_parser_fetch_transformList( value, &((*bio_assembly).transformListCount) );
			}
		}
	}
}

MMTF_Transform* MMTF_parser_fetch_transformList( msgpack_object* object, uint64_t* length ) {
	if( object->type != MSGPACK_OBJECT_ARRAY ) {
		fprintf( stderr, "Error in fetch_transformList: the entry encoded in the MMTF is not an array.\n" );
		return NULL;
	}

	msgpack_object* current_transform = object->via.array.ptr;
	uint32_t transform_count = object->via.array.size;
	msgpack_object* stop_transform = object->via.array.ptr + transform_count;

//printf( "Decoding an array of %u Transform.\n", transform_count );

	MMTF_Transform* result = malloc( sizeof( MMTF_Transform ) * transform_count );

	int i = 0;
	for( ; current_transform < stop_transform; ++current_transform ) {
		MMTF_parser_put_transform( current_transform, &(result[i]) );
		++i;
	}

    return result;
}

void MMTF_parser_put_transform( msgpack_object* object, MMTF_Transform* transform ) {
	if( object->type != MSGPACK_OBJECT_MAP ) {
		fprintf( stderr, "Error in put_transform: the entry encoded in the MMTF is not a map.\n" );
		return;
	}

	msgpack_object_kv* current_key_value = object->via.map.ptr;
	msgpack_object_kv* last_key_value = current_key_value + object->via.map.size;

	for( ; current_key_value <= last_key_value; ++current_key_value ) {
		msgpack_object* key = &(current_key_value->key);
		msgpack_object* value = &(current_key_value->val);

		if( key->type == MSGPACK_OBJECT_STR ) {

//printf( "Key %.*s:\n", key->via.str.size, key->via.str.ptr );

			uint32_t key_size = key->via.str.size;
			const char* key_string = key->via.str.ptr;

			if( key_size == strlen( "chainIndexList" ) && strncmp( key_string, "chainIndexList", key_size ) == 0 ) {
				uint64_t length;
				(*transform).chainIndexList = MMTF_parser_fetch_clear_lu_array( value, &length );
			}

			if( key_size == strlen( "matrix" ) && strncmp( key_string, "matrix", key_size ) == 0 ) {
				uint64_t length;
				float* matrix = MMTF_parser_fetch_clear_float_array( value, &length );

				if( length != 16 ) {
					fprintf( stderr, "Error in put_transform: the length of the matrix entry is %lu, it should have a length of 16.\n", length );
					return;
				}

				int i;
				for( i = 0; i < 16; ++i ) {
					(*transform).matrix[i] = matrix[i];

//printf( "%i = %f\n", i, matrix[i] );

				}
			}
		}
	}
}

void MMTF_parser_msgpack_object_to_MMTF_container(msgpack_object* object, MMTF_container* thing) {
	msgpack_object_kv* current_key_value = object->via.map.ptr;
	msgpack_object_kv* last_key_value = current_key_value + object->via.map.size;

	for( ; current_key_value <= last_key_value; ++current_key_value ) {
		msgpack_object* key = &(current_key_value->key);
		msgpack_object* value = &(current_key_value->val);

		if( key->type == MSGPACK_OBJECT_STR ) {

//printf( "Key %.*s:\n", key->via.str.size, key->via.str.ptr );

			uint32_t key_size = key->via.str.size;
			const char* key_string = key->via.str.ptr;

			if( key_size == strlen( "mmtfVersion" ) && strncmp( key_string, "mmtfVersion", key_size ) == 0 ) {
				(*thing).mmtfVersion = MMTF_parser_fetch_string( value );

//printf( "mmtfVersion : %s\n\n", (*thing).mmtfVersion );
			}

			if( key_size == strlen( "mmtfProducer" ) && strncmp( key_string, "mmtfProducer", key_size ) == 0 ) {
				(*thing).mmtfProducer = MMTF_parser_fetch_string( value );

//printf( "mmtfProducer : %s\n\n", (*thing).mmtfProducer );
			}

			if( key_size == strlen( "spaceGroup" ) && strncmp( key_string, "spaceGroup", key_size ) == 0 ) {
				(*thing).spaceGroup = MMTF_parser_fetch_string( value );

//printf( "spaceGroup : %s\n\n", (*thing).spaceGroup );
			}

			if( key_size == strlen( "structureId" ) && strncmp( key_string, "structureId", key_size ) == 0 ) {
				(*thing).structureId = MMTF_parser_fetch_string( value );

//printf( "structureId : %s\n\n", (*thing).structureId );
			}

			if( key_size == strlen( "title" ) && strncmp( key_string, "title", key_size ) == 0 ) {
				(*thing).title = MMTF_parser_fetch_string( value );

//printf( "title : %s\n\n", (*thing).title );
			}

			if( key_size == strlen( "depositionDate" ) && strncmp( key_string, "depositionDate", key_size ) == 0 ) {
				(*thing).depositionDate = MMTF_parser_fetch_string( value );

//printf( "depositionDate : %s\n\n", (*thing).depositionDate );
			}

			if( key_size == strlen( "releaseDate" ) && strncmp( key_string, "releaseDate", key_size ) == 0 ) {
				(*thing).releaseDate = MMTF_parser_fetch_string( value );

//printf( "releaseDate : %s\n\n", (*thing).releaseDate );
			}

			if( key_size == strlen( "numBonds" ) && strncmp( key_string, "numBonds", key_size ) == 0 ) {
				(*thing).numBonds = (int32_t) MMTF_parser_fetch_int( value );

//printf( "numBonds : %i\n\n", (*thing).numBonds );
			}

			if( key_size == strlen( "numAtoms" ) && strncmp( key_string, "numAtoms", key_size ) == 0 ) {
				(*thing).numAtoms = (int32_t) MMTF_parser_fetch_int( value );

//printf( "numAtoms : %i\n\n", (*thing).numAtoms );
			}

			if( key_size == strlen( "numGroups" ) && strncmp( key_string, "numGroups", key_size ) == 0 ) {
				(*thing).numGroups = (int32_t) MMTF_parser_fetch_int( value );

//printf( "numGroups : %i\n\n", (*thing).numGroups );
			}

			if( key_size == strlen( "numChains" ) && strncmp( key_string, "numChains", key_size ) == 0 ) {
				(*thing).numChains = (int32_t) MMTF_parser_fetch_int( value );

//printf( "numChains : %i\n\n", (*thing).numChains );
			}

			if( key_size == strlen( "numModels" ) && strncmp( key_string, "numModels", key_size ) == 0 ) {
				(*thing).numModels = (int32_t) MMTF_parser_fetch_int( value );

//printf( "numModels : %i\n\n", (*thing).numModels );
			}

			if( key_size == strlen( "entityList" ) && strncmp( key_string, "entityList", key_size ) == 0 ) {
				(*thing).entityList = MMTF_parser_fetch_entityList( value, &(thing->entityListCount) );

//printf( "entityList : %p\n\n", (*thing).entityList );
			}

			if( key_size == strlen( "unitCell" ) && strncmp( key_string, "unitCell", key_size ) == 0 ) {
				uint64_t length;
				float* unitCell = MMTF_parser_fetch_clear_float_array( value, &length );

				if( unitCell != NULL ) {
//printf( "unitCell : ");
					int i;
					for( i = 0; i < 6; ++i ) {
						(*thing).unitCell[i] = unitCell[i];
//printf( "%f ", unitCell[i] );
					}
				}
			}

			if( key_size == strlen( "bioAssemblyList" ) && strncmp( key_string, "bioAssemblyList", key_size ) == 0 ) {
				(*thing).bioAssemblyList = MMTF_parser_fetch_bioAssemblyList( value, &((*thing).bioAssemblyListCount) );
			}

			if( key_size == strlen( "resolution" ) && strncmp( key_string, "resolution", key_size ) == 0 ) {
				(*thing).resolution = MMTF_parser_fetch_float( value );

//printf( "resolution : %f\n\n", (*thing).resolution );
			}

			if( key_size == strlen( "rFree" ) && strncmp( key_string, "rFree", key_size ) == 0 ) {
				(*thing).rFree = MMTF_parser_fetch_float( value );

//printf( "rFree : %f\n\n", (*thing).rFree );
			}

			if( key_size == strlen( "rWork" ) && strncmp( key_string, "rWork", key_size ) == 0 ) {
				(*thing).rWork = MMTF_parser_fetch_float( value );

//printf( "rWork : %f\n\n", (*thing).rWork );
			}

			if( key_size == strlen( "experimentalMethods" ) && strncmp( key_string, "experimentalMethods", key_size ) == 0 ) {
				(*thing).experimentalMethods = MMTF_parser_fetch_clear_string_array( value, &(thing->experimentalMethodsCount) );

//int i;
//for( i = 0; i < thing->experimentalMethodsCount; ++i ) {
//printf( "experimentalMethods : %s\n\n", (*thing).experimentalMethods[i] );
//}

			}

			if( key_size == strlen( "bondAtomList" ) && strncmp( key_string, "bondAtomList", key_size ) == 0 ) {
				uint64_t length;
				(*thing).bondAtomList = (int32_t*) MMTF_parser_fetch_array( value, &length );

//printf( "bondAtomList : %p\n\n", (*thing).bondAtomList );
			}

			if( key_size == strlen( "chainsPerModel" ) && strncmp( key_string, "chainsPerModel", key_size ) == 0 ) {
				uint64_t length;
				(*thing).chainsPerModel = MMTF_parser_fetch_clear_int32_array( value, &length );

//printf( "chainsPerModel : %p\n\n", (*thing).chainsPerModel );
			}

			if( key_size == strlen( "groupsPerChain" ) && strncmp( key_string, "groupsPerChain", key_size ) == 0 ) {
				uint64_t length;
				(*thing).groupsPerChain = MMTF_parser_fetch_clear_int32_array( value, &length );

//printf( "groupsPerChain : %p\n\n", (*thing).groupsPerChain );
			}

			if( key_size == strlen( "groupList" ) && strncmp( key_string, "groupList", key_size ) == 0 ) {
				(*thing).groupList = MMTF_parser_fetch_groupList( value, &(thing->groupListCount) );

//printf( "groupList : %p\n\n", (*thing).groupList );
			}

			if( key_size == strlen( "groupTypeList" ) && strncmp( key_string, "groupTypeList", key_size ) == 0 ) {
				uint64_t length;
				(*thing).groupTypeList = (int32_t*) MMTF_parser_fetch_array( value, &length );

//printf( "groupTypeList : %p\n\n", (*thing).groupTypeList );
			}

			if( key_size == strlen( "groupIdList" ) && strncmp( key_string, "groupIdList", key_size ) == 0 ) {
				uint64_t length;
				(*thing).groupIdList = (int32_t*) MMTF_parser_fetch_array( value, &length );

//printf( "groupIdList : %p\n\n", (*thing).groupIdList );
			}

			if( key_size == strlen( "sequenceIndexList" ) && strncmp( key_string, "sequenceIndexList", key_size ) == 0 ) {
				uint64_t length;
				(*thing).sequenceIndexList = (int32_t*) MMTF_parser_fetch_array( value, &length );

//printf( "sequenceIndexList : %p\n\n", (*thing).sequenceIndexList );
			}

			if( key_size == strlen( "atomIdList" ) && strncmp( key_string, "atomIdList", key_size ) == 0 ) {
				uint64_t length;
				(*thing).atomIdList = (int32_t*) MMTF_parser_fetch_array( value, &length );

//printf( "atomIdList : %p\n\n", (*thing).atomIdList );
			}

			if( key_size == strlen( "insCodeList" ) && strncmp( key_string, "insCodeList", key_size ) == 0 ) {
				uint64_t length;
				(*thing).insCodeList = (char*) MMTF_parser_fetch_array( value, &length );

//printf( "insCodeList : %p\n\n", (*thing).insCodeList );
			}

			if( key_size == strlen( "altLocList" ) && strncmp( key_string, "altLocList", key_size ) == 0 ) {
				uint64_t length;
				(*thing).altLocList = (char*) MMTF_parser_fetch_array( value, &length );

//printf( "altLocList : %p\n\n", (*thing).altLocList );
			}

			if( key_size == strlen( "bondOrderList" ) && strncmp( key_string, "bondOrderList", key_size ) == 0 ) {
				uint64_t length;
				(*thing).bondOrderList = (int8_t*) MMTF_parser_fetch_array( value, &length );

//printf( "bondOrderList : %p\n\n", (*thing).bondOrderList );
			}

			if( key_size == strlen( "secStructList" ) && strncmp( key_string, "secStructList", key_size ) == 0 ) {
				uint64_t length;
				(*thing).secStructList = (int8_t*) MMTF_parser_fetch_array( value, &length );

//printf( "secStructList : %p\n\n", (*thing).secStructList );
			}

			if( key_size == strlen( "bFactorList" ) && strncmp( key_string, "bFactorList", key_size ) == 0 ) {
				uint64_t length;
				(*thing).bFactorList = (float*) MMTF_parser_fetch_array( value, &length );

//printf( "bFactorList : %p\n\n", (*thing).bFactorList );
			}

			if( key_size == strlen( "xCoordList" ) && strncmp( key_string, "xCoordList", key_size ) == 0 ) {
				uint64_t length;
				(*thing).xCoordList = (float*) MMTF_parser_fetch_array( value, &length );

//printf( "xCoordList : %p\n\n", (*thing).xCoordList );
			}

			if( key_size == strlen( "yCoordList" ) && strncmp( key_string, "yCoordList", key_size ) == 0 ) {
				uint64_t length;
				(*thing).yCoordList = (float*) MMTF_parser_fetch_array( value, &length );

//printf( "yCoordList : %p\n\n", (*thing).yCoordList );
			}

			if( key_size == strlen( "zCoordList" ) && strncmp( key_string, "zCoordList", key_size ) == 0 ) {
				uint64_t length;
				(*thing).zCoordList = (float*) MMTF_parser_fetch_array( value, &length );

//printf( "zCoordList : %p\n\n", (*thing).zCoordList );
			}

			if( key_size == strlen( "occupancyList" ) && strncmp( key_string, "occupancyList", key_size ) == 0 ) {
				uint64_t length;
				(*thing).occupancyList = (float*) MMTF_parser_fetch_array( value, &length );

//printf( "occupancyList : %p\n\n", (*thing).occupancyList );
			}

			if( key_size == strlen( "chainIdList" ) && strncmp( key_string, "chainIdList", key_size ) == 0 ) {
				(*thing).chainIdList = (char**) MMTF_parser_fetch_array( value, &(thing->chainIdListCount) );

//printf( "chainIdList : %p\n\n", (*thing).chainIdList );
			}

			if( key_size == strlen( "chainNameList" ) && strncmp( key_string, "chainNameList", key_size ) == 0 ) {
				(*thing).chainNameList = (char**) MMTF_parser_fetch_array( value, &(thing->chainNameListCount) );

//printf( "chainNameList : %p\n\n", (*thing).chainNameList );
			}
		}
	}
}

void MMTF_parser_parse_msgpack(char *buffer,int msgsize, MMTF_container* thing){
    msgpack_zone mempool;
    msgpack_zone_init(&mempool, 2048);
    msgpack_object deserialized;
	msgpack_unpack(buffer, msgsize, NULL, &mempool, &deserialized);

//printf( "msgpack_object unpacked\n" );

    MMTF_parser_msgpack_object_to_MMTF_container(&deserialized, thing);

//printf( "MMTF structure decoded\n" );

    msgpack_zone_destroy(&mempool);
}


//*** Decode a MMTF container from a file
void MMTF_parser_MMTF_container_from_file(char *name, MMTF_container* thing)
{
	FILE *file;
	char *buffer;
	unsigned long fileLen;

//*** Open file
	file = fopen(name, "rb");
	if (!file)
	{
		fprintf(stderr, "Unable to open file %s.\n", name);
		return;
	}

//*** Get file length
	fseek(file, 0, SEEK_END);
	fileLen=ftell(file);
	fseek(file, 0, SEEK_SET);

//printf("File length = %lu\n", fileLen);

//*** Allocate memory
	buffer=(char *)malloc(fileLen+1);
	if (!buffer)
	{
		fprintf(stderr, "Memory error!");
        fclose(file);
		return;
	}

//*** Read file contents into buffer
	fread(buffer, fileLen, 1, file);
	fclose(file);

	MMTF_parser_parse_msgpack(buffer, fileLen, thing);

	free(buffer);
}

