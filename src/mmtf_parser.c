//*******************************************************************************
//******	This code is published under the .... license
//******
//******
//******	This file is the code file for the MMTF parser for the C language.
//******
//******	The authors of this code are: Julien Ferté, Thomas Holder, ...
//******
//******
//******
//******
//******
//******
//******
//*******************************************************************************

#include "mmtf_parser.h"

#define MALLOC_ARRAY(type, size) (type*) malloc((size) * sizeof(type))

/*
 * Type aliases for code generation
 */

#define TYPEALIAS_char      char
#define TYPEALIAS_int8      int8_t
#define TYPEALIAS_int32     int32_t
#define TYPEALIAS_lu        size_t
#define TYPEALIAS_float     float
#define TYPEALIAS_string    char*
#define TYPEALIAS_int       int

enum {
    MMTF_TYPE_ANY = 0,
    MMTF_TYPE_char,
    MMTF_TYPE_int8 = MMTF_TYPE_char,
    MMTF_TYPE_int16,
    MMTF_TYPE_int32,
    MMTF_TYPE_lu,
    MMTF_TYPE_float,
    MMTF_TYPE_string
};

/*
 * Macros for null-pointer checking
 */

#define IF_NULL_PTRERROR_RETURN(ptr, returnvalue) \
    if (!ptr) { \
        fprintf(stderr, "Error in %s: NULL pointer.\n", __FUNCTION__); \
        return returnvalue; \
    }

#define IF_NULL_ALLOCERROR_RETURN(ptr, returnvalue) \
    if (!ptr) { \
        fprintf(stderr, "Error in %s: couldn't allocate memory.\n", __FUNCTION__ ); \
        return returnvalue; \
    }

#define IF_NOT_MULTIPLE_ERROR_RETURN(length, size, returnvalue) \
	if((length) % (size) != 0) { \
		fprintf(stderr, "Error in %s: length %u is not a multiple of %u.\n", __FUNCTION__, length, size ); \
		return returnvalue; \
	}

#define IF_NULL_PTRERROR_RETURN_NULL(ptr) \
    IF_NULL_PTRERROR_RETURN(ptr, NULL)

#define IF_NULL_ALLOCERROR_RETURN_NULL(ptr) \
    IF_NULL_ALLOCERROR_RETURN(ptr, NULL)

/*
 * Macros for iterating over a msgpack map
 */

#define MAP_ITERATE_BEGIN(object) \
    if (object->type != MSGPACK_OBJECT_MAP) { \
        fprintf(stderr, "Error in %s: the entry encoded in the MMTF is not a map.\n", __FUNCTION__); \
        return; \
    } \
    msgpack_object_kv* current_key_value = object->via.map.ptr; \
    msgpack_object_kv* last_key_value = current_key_value + object->via.map.size; \
    for (; current_key_value != last_key_value; ++current_key_value ) { \
        msgpack_object* key = &(current_key_value->key); \
        msgpack_object* value = &(current_key_value->val); \
        if (key->type != MSGPACK_OBJECT_STR) \
                continue;

#define MAP_ITERATE_END() }

/*
 * Macros for inside the map iteration
 */

#define FETCH_AND_ASSIGN(this_, type, name) \
    if (MMTF_parser_compare_msgpack_string_char_array(&(key->via.str), #name)) { \
        this_->name = MMTF_parser_fetch_ ## type(value); \
        continue; \
    }

#define FETCH_AND_ASSIGN_DUMMYCOUNT(this_, type, name) \
    if (MMTF_parser_compare_msgpack_string_char_array(&(key->via.str), #name)) { \
        uint64_t _length; \
        this_->name = MMTF_parser_fetch_ ## type(value, &_length); \
        continue; \
    }

#define FETCH_AND_ASSIGN_WITHCOUNT(this_, type, name) \
    if (MMTF_parser_compare_msgpack_string_char_array(&(key->via.str), #name)) { \
        this_->name = MMTF_parser_fetch_ ## type(value, &(this_->name ## Count)); \
        continue; \
    }

#define FETCH_AND_ASSIGN_ARRAY(this_, type, name) \
    if (MMTF_parser_compare_msgpack_string_char_array( &(key->via.str), #name )) { \
        uint64_t _length; \
        type * array = MMTF_parser_fetch_ ## type ## _array(value, &_length); \
        if (array != NULL) { \
            int i; \
            for(i = 0; i < _length; ++i) { this_->name[i] = array[i]; } \
            free(array); \
        } \
    }

/*
 * Macros for generating generic initialization and destroying functions
 */

#define CODEGEN_MMTF_parser_TYPE_initialize(type) \
    type * MMTF_parser_ ## type ## _initialize(type * result) { \
        memset(result, 0, sizeof(type)); \
        return result; \
    }

#define CODEGEN_MMTF_parser_TYPE_new(type) \
    type * MMTF_parser_ ## type ## _new(void) { \
        type * result = (type*) malloc(sizeof(type)); \
        IF_NULL_ALLOCERROR_RETURN_NULL(result); \
        return MMTF_parser_ ## type ## _initialize(result); \
    }

#define CODEGEN_MMTF_parser_TYPE_empty(type) \
    type * MMTF_parser_ ## type ## _empty( type * result ) { \
        IF_NULL_PTRERROR_RETURN_NULL(result); \
        MMTF_parser_ ## type ## _destroy_inside( result ); \
        MMTF_parser_ ## type ## _initialize( result ); \
        return result; \
    }

#define CODEGEN_MMTF_parser_TYPE_destroy(type) \
    void MMTF_parser_ ## type ## _destroy(type * thing) { \
        IF_NULL_PTRERROR_RETURN(thing,); \
        MMTF_parser_ ## type ## _destroy_inside(thing); \
        free(thing); \
    }

#define CODEGEN_MMTF_parser_TYPE(type) \
    CODEGEN_MMTF_parser_TYPE_initialize(type) \
    CODEGEN_MMTF_parser_TYPE_new(type) \
    CODEGEN_MMTF_parser_TYPE_empty(type) \
    CODEGEN_MMTF_parser_TYPE_destroy(type)

#define MMTF_parser_generic_destroy_inside(ptr) \
    free(*(ptr))

#define FREE_LIST(type_, name) \
    if (name != NULL) { \
        for (i = 0; i < name ## Count; ++i) { \
            MMTF_parser_ ## type_ ## _destroy_inside(name + i); \
        } \
        free(name); \
    }

/*
 * Macros for fetching and converting msgpack array objects
 */

#define CODEGEN_BODY_fetch_OBJECT_ARRAY(type_, RESULT_I_ASSIGN) \
    { \
        if (object->type != MSGPACK_OBJECT_ARRAY) { \
            fprintf(stderr, "Error in %s: the entry encoded in the MMTF is not an array.\n", __FUNCTION__); \
            return NULL; \
        } \
        msgpack_object* iter = object->via.array.ptr; \
        (*length) = (uint64_t) object->via.array.size; \
        msgpack_object* iter_end = iter + (*length); \
        type_ * result = MALLOC_ARRAY(type_, *length); \
        IF_NULL_ALLOCERROR_RETURN_NULL(result); \
        int i = 0; \
        for (; iter != iter_end ; ++iter, ++i) { \
            RESULT_I_ASSIGN; \
        } \
        return result; \
    }

#define CODEGEN_MMTF_parser_fetch_array(type_, RESULT_I_ASSIGN) \
    static TYPEALIAS_ ## type_ * MMTF_parser_fetch_ ## type_ ## _array(msgpack_object* object, uint64_t* length) { \
        if (object->type == MSGPACK_OBJECT_BIN) { \
            return (TYPEALIAS_ ## type_*) \
            MMTF_parser_fetch_typed_array(object, length, MMTF_TYPE_ ## type_); \
        } \
        CODEGEN_BODY_fetch_OBJECT_ARRAY(TYPEALIAS_ ## type_, RESULT_I_ASSIGN); \
    }

#define CODEGEN_MMTF_parser_fetch_List(type_, suffix) \
    type_ * MMTF_parser_fetch_ ## suffix ## List(msgpack_object* object, uint64_t* length) { \
        CODEGEN_BODY_fetch_OBJECT_ARRAY(type_, { \
            MMTF_parser_put_ ## suffix(iter, result + i); \
        }) \
    }

/*
 * Generate "initialize", "new", "empty" and "destroy" functions for MMTF struct types.
 */
CODEGEN_MMTF_parser_TYPE(MMTF_container);
CODEGEN_MMTF_parser_TYPE(MMTF_BioAssembly);
CODEGEN_MMTF_parser_TYPE(MMTF_Transform);
CODEGEN_MMTF_parser_TYPE(MMTF_Entity);
CODEGEN_MMTF_parser_TYPE(MMTF_GroupType);

//*** Destroy the innner of a struct
MMTF_container* MMTF_parser_MMTF_container_destroy_inside( MMTF_container* thing ) {
    uint64_t i;
    IF_NULL_PTRERROR_RETURN_NULL(thing);

    FREE_LIST(MMTF_BioAssembly, thing->bioAssemblyList);
    FREE_LIST(MMTF_Entity, thing->entityList);
    FREE_LIST(generic, thing->experimentalMethods);
    FREE_LIST(MMTF_GroupType, thing->groupList);
    FREE_LIST(generic, thing->chainIdList);
    FREE_LIST(generic, thing->chainNameList);

    free( thing->mmtfVersion );
    free( thing->mmtfProducer );
    free( thing->spaceGroup );
    free( thing->structureId );
    free( thing->title );
    free( thing->depositionDate );
    free( thing->releaseDate );
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
    free( thing->groupsPerChain );
    free( thing->chainsPerModel );

    return thing;
}
MMTF_BioAssembly* MMTF_parser_MMTF_BioAssembly_destroy_inside( MMTF_BioAssembly* bio_assembly ) {
    uint64_t i;
    IF_NULL_PTRERROR_RETURN_NULL(bio_assembly);
    FREE_LIST(MMTF_Transform, bio_assembly->transformList);
    free( bio_assembly->name );
    return bio_assembly;
}
MMTF_Transform* MMTF_parser_MMTF_Transform_destroy_inside( MMTF_Transform* transform ) {
    IF_NULL_PTRERROR_RETURN_NULL(transform);
    free( transform->chainIndexList );
	return transform;
}
MMTF_Entity* MMTF_parser_MMTF_Entity_destroy_inside( MMTF_Entity* entity ) {
    IF_NULL_PTRERROR_RETURN_NULL(entity);
    free( entity->chainIndexList );
    free( entity->description );
    free( entity->type );
    free( entity->sequence );
    return entity;
}
MMTF_GroupType* MMTF_parser_MMTF_GroupType_destroy_inside( MMTF_GroupType* group_type ) {
    uint64_t i;
    IF_NULL_PTRERROR_RETURN_NULL(group_type);
    FREE_LIST(generic, group_type->atomNameList);
    FREE_LIST(generic, group_type->elementList);
    free( group_type->formalChargeList );
    free( group_type->bondAtomList );
    free( group_type->bondOrderList );
    free( group_type->groupName );
    free( group_type->chemCompType );
    return group_type;
}

//*** Array converters
// From bytes[] to float32[], int8[], int16[], int32[] and string

float* MMTF_parser_float_from_bytes( const char* input, uint32_t input_length, uint32_t* output_length ) {
	IF_NOT_MULTIPLE_ERROR_RETURN(input_length, 4, NULL);

	(*output_length) = input_length/4;

	float* output = MALLOC_ARRAY(float, *output_length);
    IF_NULL_ALLOCERROR_RETURN_NULL(output);

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

	int8_t* output = MALLOC_ARRAY(int8_t, *output_length);
    IF_NULL_ALLOCERROR_RETURN_NULL(output);

	MMTF_parser_one_byte_as_int8 u;
	uint32_t i;
	for( i = 0; i < input_length; ++i ) {
		u.c = input[i];

		output[i] = u.i;
	}

	return output;
}

int16_t* MMTF_parser_int16_from_bytes( const char* input, uint32_t input_length, uint32_t* output_length ) {
	IF_NOT_MULTIPLE_ERROR_RETURN(input_length, 2, NULL);

	(*output_length) = input_length/2;

	int16_t* output = MALLOC_ARRAY(int16_t, (*output_length) );
    IF_NULL_ALLOCERROR_RETURN_NULL(output);

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
	IF_NOT_MULTIPLE_ERROR_RETURN(input_length, 4, NULL);

	(*output_length) = input_length/4;

	int32_t* output = MALLOC_ARRAY(int32_t, (*output_length) );
    IF_NULL_ALLOCERROR_RETURN_NULL(output);

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

	char* output = MALLOC_ARRAY( char , (*output_length) );
    IF_NULL_ALLOCERROR_RETURN_NULL(output);

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
	IF_NOT_MULTIPLE_ERROR_RETURN(input_length, parameter, NULL);

	(*output_length) = input_length / parameter;

	char** output = MALLOC_ARRAY(char*, (*output_length) );
    IF_NULL_ALLOCERROR_RETURN_NULL(output);

	uint32_t i,j;
	for( i = 0; i < *output_length; ++i ) {
		output[i] = MALLOC_ARRAY(char, parameter + 1);
        IF_NULL_ALLOCERROR_RETURN_NULL(output[i]);
        memcpy(output[i], input + (i * parameter), parameter);
        output[i][parameter] = 0;
	}

	return output;
}


//*** Array decoders
// Run-length decode
int32_t* MMTF_parser_run_length_decode( int32_t* input, uint32_t input_length, uint32_t* output_length ) {
	(*output_length) = 0;

	IF_NOT_MULTIPLE_ERROR_RETURN(input_length, 2, NULL);

	uint32_t i;
	int32_t value, number;
	for(i = 0; i < input_length; i += 2) {
		number = input[i+1];

		(*output_length) += number;
	}

	int32_t* output = MALLOC_ARRAY(int32_t, (*output_length)); // The output needs to be freed by the calling process
    IF_NULL_ALLOCERROR_RETURN_NULL(output);

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
	int32_t* output = MALLOC_ARRAY(int32_t, (*output_length)); // The output needs to be freed by the calling process
    IF_NULL_ALLOCERROR_RETURN_NULL(output);

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

	int32_t* output = (int32_t*) MALLOC_ARRAY(int32_t, (*output_length)); // The output needs to be freed by the calling process
    IF_NULL_ALLOCERROR_RETURN_NULL(output);

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

	int32_t* output = MALLOC_ARRAY(int32_t, (*output_length)); // The output needs to be freed by the calling process
    IF_NULL_ALLOCERROR_RETURN_NULL(output);

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
	float* output = (float*) MALLOC_ARRAY(float, (*output_length) );
    IF_NULL_ALLOCERROR_RETURN_NULL(output);

	float parameter_float = (float) parameter;
	uint32_t i;
	for(i = 0; i < input_length; ++i ) {
		output[i] = ((float) input[i])/parameter_float;
	}

	return output;
}

float* MMTF_parser_integer_decode_from_32( int32_t* input, uint32_t input_length, int32_t parameter, uint32_t* output_length ) {
	(*output_length) = input_length;
	float* output = (float*) MALLOC_ARRAY(float, (*output_length) );
    IF_NULL_ALLOCERROR_RETURN_NULL(output);

	float parameter_float = (float) parameter;
	uint32_t i;
	for(i = 0; i < input_length; ++i ) {
		output[i] = ((float) input[i])/parameter_float;
	}

	return output;
}


//*** Applying a decoding strategy for getting an array
static
void* MMTF_parser_decode_apply_strategy( const char* input,
        uint32_t input_length, uint32_t* output_length, int strategy,
        int32_t parameter, int * typecode) {
	switch( strategy ) {
		case 1:
			*typecode = MMTF_TYPE_float;
			return MMTF_parser_float_from_bytes( input, input_length, output_length );
		case 2:
			*typecode = MMTF_TYPE_int8;
			return MMTF_parser_int8_from_bytes( input, input_length, output_length );
		case 3:
			*typecode = MMTF_TYPE_int16;
			return MMTF_parser_int16_from_bytes( input, input_length, output_length );
		case 4:
			*typecode = MMTF_TYPE_int32;
			return MMTF_parser_int32_from_bytes( input, input_length, output_length );
		case 5:
			*typecode = MMTF_TYPE_string;
			return MMTF_parser_strings_from_bytes( input, input_length, parameter, output_length );
		case 6: {
            // pass
		}
		case 7: {
			uint32_t step1_length;
			int32_t* step1 = MMTF_parser_int32_from_bytes( input, input_length, &step1_length );

			int32_t* output = MMTF_parser_run_length_decode( step1, step1_length, output_length );
			free(step1);

            if (strategy == 6) {
                int i = 0;
                char* char_output = MALLOC_ARRAY(char, (*output_length));
                IF_NULL_ALLOCERROR_RETURN_NULL(char_output);
                for (; i < *output_length; ++i) {
                    char_output[i] = output[i];
                }
                free(output);

				*typecode = MMTF_TYPE_int8;
                return char_output;
            }

			*typecode = MMTF_TYPE_int32;
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

			*typecode = MMTF_TYPE_int32;
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

			*typecode = MMTF_TYPE_float;
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

			*typecode = MMTF_TYPE_float;
			return output;
		}
		case 11: {
			uint32_t step1_length;
			int16_t* step1 = MMTF_parser_int16_from_bytes( input, input_length, &step1_length );

			float* output = MMTF_parser_integer_decode_from_16( step1, step1_length, parameter, output_length );
			free(step1);

			*typecode = MMTF_TYPE_float;
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

			*typecode = MMTF_TYPE_float;
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

			*typecode = MMTF_TYPE_float;
			return output;
		}
		case 14: {
			uint32_t step1_length;
			int16_t* step1 = MMTF_parser_int16_from_bytes( input, input_length, &step1_length );

			int32_t* output = MMTF_parser_recursive_indexing_decode_from_16( step1, step1_length, output_length );
			free(step1);

			*typecode = MMTF_TYPE_int32;
			return output;
		}
		case 15: {
			uint32_t step1_length;
			int8_t* step1 = MMTF_parser_int8_from_bytes( input, input_length, &step1_length );

			int32_t* output = MMTF_parser_recursive_indexing_decode_from_8( step1, step1_length, output_length );
			free(step1);

			*typecode = MMTF_TYPE_int32;
			return output;
		}
		default: {
			fprintf( stderr, "Error in %s: %i does not refer to any strategy.\n", __FUNCTION__, strategy );
			return NULL;
		}
	}
}

/*
 * Copy string from 'object' to 'out'
 */
static
void MMTF_parser_put_string(msgpack_object* object, char** out) {
    size_t string_size = object->via.str.size;
    char * result = (*out) = MALLOC_ARRAY(char, (string_size + 1));
    IF_NULL_ALLOCERROR_RETURN(result,);
    memcpy(result, object->via.str.ptr, string_size);
    result[string_size] = '\0';
}

//*** Unpacking from MsgPack and applying strategy
char* MMTF_parser_fetch_string( msgpack_object* object ) {
	if( object->type != MSGPACK_OBJECT_STR ) {
		fprintf( stderr, "Error in %s: the entry encoded in the MMTF is not a string.\n", __FUNCTION__ );
		return NULL;
	}

    char * result = NULL;
    MMTF_parser_put_string(object, &result);
	return result;
}

char MMTF_parser_fetch_char( msgpack_object* object ) {
	if( object->type != MSGPACK_OBJECT_STR) {
		fprintf( stderr, "Error in %s: the entry encoded in the MMTF is not a string.\n", __FUNCTION__ );
		return '\0';
	}

	return *(object->via.str.ptr);
}

int64_t MMTF_parser_fetch_int( msgpack_object* object ) {
	int64_t result;

    if(object->type == MSGPACK_OBJECT_POSITIVE_INTEGER) {
        result = object->via.u64;
    }
    else if(object->type == MSGPACK_OBJECT_NEGATIVE_INTEGER) {
        result = object->via.i64;
	}
	else {
		fprintf( stderr, "Error in %s: the entry encoded in the MMTF is not an integer.\n", __FUNCTION__ );
		return NAN;
	}

	return result;
}

float MMTF_parser_fetch_float( msgpack_object* object ) {
	if( object->type != MSGPACK_OBJECT_FLOAT ) {
		fprintf( stderr, "Error in %s: the entry encoded in the MMTF is not a float.\n", __FUNCTION__ );
		return NAN;
	}
	
	return (float) object->via.f64;
}

/*
 * Fetch a compressed typed array
 */
static
void* MMTF_parser_fetch_typed_array( msgpack_object* object, uint64_t* length, int typecode) {
	if( object->type != MSGPACK_OBJECT_BIN ) {
		fprintf( stderr, "Error in %s: the entry encoded in the MMTF is not binary data.\n", __FUNCTION__ );
		return NULL;
	}

	const char* bytes = object->via.bin.ptr;

	int parameter = MMTF_parser_get_param(bytes);
	int strategy = MMTF_parser_get_strategy(bytes);
	(*length) = MMTF_parser_get_len(bytes);

//printf( "Applying the strategy %i with parameter %i for decoding a byte array of length %i into an int32 array of length %lu.\n", strategy, parameter, object->via.bin.size - 12, *length );

    uint32_t out_length;
    int typecheck;
    void * result = MMTF_parser_decode_apply_strategy(bytes + 12,
            object->via.bin.size - 12, &out_length, strategy, parameter,
            &typecheck);

    if (typecode != MMTF_TYPE_ANY && typecode != typecheck) {
        fprintf(stderr, "Error in %s: typecode mismatch %d %d\n",
                __FUNCTION__, typecode, typecheck);
        return NULL;
    }

    if (out_length != *length) {
        fprintf(stderr, "Error in %s: length mismatch %u %u\n",
                __FUNCTION__, out_length, (unsigned) *length);
        return NULL;
    }

    return result;
}

/*
 * Fetch a typed array.
 */
CODEGEN_MMTF_parser_fetch_array(char,   result[i] = iter->via.u64);
CODEGEN_MMTF_parser_fetch_array(int8,   result[i] = iter->via.u64);
CODEGEN_MMTF_parser_fetch_array(int32,  result[i] = iter->via.u64);
CODEGEN_MMTF_parser_fetch_array(lu,     result[i] = iter->via.u64);
CODEGEN_MMTF_parser_fetch_array(float,  result[i] = iter->via.f64);
CODEGEN_MMTF_parser_fetch_array(string, MMTF_parser_put_string(iter, result + i));

bool MMTF_parser_compare_msgpack_string_char_array( const msgpack_object_str* m_string, const char* string ) {
	return (m_string->size == strlen( string ) && strncmp( m_string->ptr, string, m_string->size ) == 0);
}

static
void MMTF_parser_put_entity( msgpack_object* object, MMTF_Entity* entity ) {
    MAP_ITERATE_BEGIN(object);
    FETCH_AND_ASSIGN(entity, string, description);
    FETCH_AND_ASSIGN(entity, string, type);
    FETCH_AND_ASSIGN_DUMMYCOUNT(entity, lu_array, chainIndexList);
    FETCH_AND_ASSIGN(entity, string, sequence);
    MAP_ITERATE_END();
}

static
void MMTF_parser_put_group( msgpack_object* object, MMTF_GroupType* group_type ) {
    MAP_ITERATE_BEGIN(object);
    FETCH_AND_ASSIGN_DUMMYCOUNT(group_type, int32_array, formalChargeList);
    FETCH_AND_ASSIGN_WITHCOUNT(group_type, string_array, atomNameList);
    FETCH_AND_ASSIGN_WITHCOUNT(group_type, string_array, elementList);
    FETCH_AND_ASSIGN_WITHCOUNT(group_type, int32_array, bondAtomList);
    FETCH_AND_ASSIGN_WITHCOUNT(group_type, int8_array, bondOrderList);
    FETCH_AND_ASSIGN(group_type, string, groupName);
    FETCH_AND_ASSIGN(group_type, char, singleLetterCode);
    FETCH_AND_ASSIGN(group_type, string, chemCompType);
    MAP_ITERATE_END();
}

static
void MMTF_parser_put_bioAssembly( msgpack_object* object, MMTF_BioAssembly* bio_assembly ) {
    MAP_ITERATE_BEGIN(object);
    FETCH_AND_ASSIGN(bio_assembly, string, name);
    FETCH_AND_ASSIGN_WITHCOUNT(bio_assembly, transformList, transformList);
    MAP_ITERATE_END();
}

static
void MMTF_parser_put_transform( msgpack_object* object, MMTF_Transform* transform ) {
    MAP_ITERATE_BEGIN(object);
    FETCH_AND_ASSIGN_DUMMYCOUNT(transform, lu_array, chainIndexList);
    FETCH_AND_ASSIGN_ARRAY(transform, float, matrix);
    MAP_ITERATE_END();
}

CODEGEN_MMTF_parser_fetch_List(MMTF_Entity, entity);
CODEGEN_MMTF_parser_fetch_List(MMTF_GroupType, group);
CODEGEN_MMTF_parser_fetch_List(MMTF_BioAssembly, bioAssembly);
CODEGEN_MMTF_parser_fetch_List(MMTF_Transform, transform);

void MMTF_parser_msgpack_object_to_MMTF_container(msgpack_object* object, MMTF_container* thing) {
    MAP_ITERATE_BEGIN(object);
    FETCH_AND_ASSIGN(thing, string, mmtfVersion);
    FETCH_AND_ASSIGN(thing, string, mmtfProducer);
    FETCH_AND_ASSIGN(thing, string, spaceGroup);
    FETCH_AND_ASSIGN(thing, string, structureId);
    FETCH_AND_ASSIGN(thing, string, title);
    FETCH_AND_ASSIGN(thing, string, depositionDate);
    FETCH_AND_ASSIGN(thing, string, releaseDate);
    FETCH_AND_ASSIGN(thing, int, numBonds);
    FETCH_AND_ASSIGN(thing, int, numAtoms);
    FETCH_AND_ASSIGN(thing, int, numGroups);
    FETCH_AND_ASSIGN(thing, int, numChains);
    FETCH_AND_ASSIGN(thing, int, numModels);
    FETCH_AND_ASSIGN(thing, float, resolution);
    FETCH_AND_ASSIGN(thing, float, rFree);
    FETCH_AND_ASSIGN(thing, float, rWork);
    FETCH_AND_ASSIGN_WITHCOUNT(thing, entityList, entityList);
    FETCH_AND_ASSIGN_WITHCOUNT(thing, bioAssemblyList, bioAssemblyList);
    FETCH_AND_ASSIGN_WITHCOUNT(thing, groupList, groupList);
    FETCH_AND_ASSIGN_WITHCOUNT(thing, int32_array, bondAtomList);
    FETCH_AND_ASSIGN_WITHCOUNT(thing, int8_array, bondOrderList);
    FETCH_AND_ASSIGN_WITHCOUNT(thing, string_array, chainIdList);
    FETCH_AND_ASSIGN_WITHCOUNT(thing, string_array, chainNameList);
    FETCH_AND_ASSIGN_DUMMYCOUNT(thing, int32_array, groupTypeList);
    FETCH_AND_ASSIGN_DUMMYCOUNT(thing, int32_array, groupIdList);
    FETCH_AND_ASSIGN_DUMMYCOUNT(thing, int32_array, sequenceIndexList);
    FETCH_AND_ASSIGN_DUMMYCOUNT(thing, int32_array, atomIdList);
    FETCH_AND_ASSIGN_DUMMYCOUNT(thing, char_array, insCodeList);
    FETCH_AND_ASSIGN_DUMMYCOUNT(thing, char_array, altLocList);
    FETCH_AND_ASSIGN_DUMMYCOUNT(thing, int8_array, secStructList);
    FETCH_AND_ASSIGN_DUMMYCOUNT(thing, float_array, bFactorList);
    FETCH_AND_ASSIGN_DUMMYCOUNT(thing, float_array, xCoordList);
    FETCH_AND_ASSIGN_DUMMYCOUNT(thing, float_array, yCoordList);
    FETCH_AND_ASSIGN_DUMMYCOUNT(thing, float_array, zCoordList);
    FETCH_AND_ASSIGN_DUMMYCOUNT(thing, float_array, occupancyList);
    FETCH_AND_ASSIGN_DUMMYCOUNT(thing, int32_array, chainsPerModel);
    FETCH_AND_ASSIGN_DUMMYCOUNT(thing, int32_array, groupsPerChain);
    FETCH_AND_ASSIGN_WITHCOUNT(thing, string_array, experimentalMethods);
    FETCH_AND_ASSIGN_ARRAY(thing, float, unitCell);
    MAP_ITERATE_END();
}

void MMTF_parser_parse_msgpack(const char *buffer,int msgsize, MMTF_container* thing){
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
void MMTF_parser_MMTF_container_from_file(const char *name, MMTF_container* thing)
{
	FILE *file;
	char *buffer;
	unsigned long fileLen;

//*** Open file
	file = fopen(name, "rb");
	if (!file) {
		fprintf(stderr, "Error in %s: unable to open file %s.\n", __FUNCTION__, name);
		return;
	}

//*** Get file length
	fseek(file, 0, SEEK_END);
	fileLen = ftell(file);
	fseek(file, 0, SEEK_SET);

//printf("File length = %lu\n", fileLen);

//*** Allocate memory
	buffer=(char *)malloc(fileLen+1);
	if (!buffer)
	{
		fprintf(stderr, "Error in %s: couldn't allocate memory.\n", __FUNCTION__);
        fclose(file);
		return;
	}

//*** Read file contents into buffer
	fread(buffer, fileLen, 1, file);
	fclose(file);

	MMTF_parser_parse_msgpack(buffer, fileLen, thing);

	free(buffer);
}

// vi:sw=4:ts=4
