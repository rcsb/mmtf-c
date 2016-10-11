//***************************************************************************************************
//******	Copyright [2016] [RCSB]
//******	
//******	Licensed under the Apache License, Version 2.0 (the "License");
//******	you may not use this file except in compliance with the License.
//******	You may obtain a copy of the License at
//******	
//******	    http://www.apache.org/licenses/LICENSE-2.0
//******	
//******	Unless required by applicable law or agreed to in writing, software
//******	distributed under the License is distributed on an "AS IS" BASIS,
//******	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//******	See the License for the specific language governing permissions and
//******	limitations under the License.
//******	
//******	
//******	This file is mmtf_parser.h, the header file of the MMTF parser
//******	for the C language.
//******	
//******	The authors of this code are Julien Ferté (http://www.julienferte.com/),
//******	Anthony Bradie, Alexandr and Yana Valasatava.
//******	
//************************************************************************************************

#include "mmtf_parser.h"

// byteorder functions ("ntohl" etc.)
#ifdef WIN32
#include <Winsock2.h>
#else
#include <arpa/inet.h>
#endif

// typed array memory allocation
#define MALLOC_ARRAY(type, size) (type*) malloc((size) * sizeof(type))

/*
 * Type aliases for code generation
 */

#define TYPEALIAS_char      char
#define TYPEALIAS_int8      int8_t
#define TYPEALIAS_int32     int32_t
#define TYPEALIAS_float     float
#define TYPEALIAS_string    char*
#define TYPEALIAS_int       int

enum {
    MMTF_TYPE_char,
    MMTF_TYPE_int8 = MMTF_TYPE_char,
    MMTF_TYPE_int16,
    MMTF_TYPE_int32,
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
        const msgpack_object* key = &(current_key_value->key); \
        const msgpack_object* value = &(current_key_value->val); \
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
        size_t _length; \
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
        size_t _length; \
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
        const msgpack_object* iter = object->via.array.ptr; \
        (*length) = object->via.array.size; \
        const msgpack_object* iter_end = iter + (*length); \
        type_ * result = MALLOC_ARRAY(type_, *length); \
        IF_NULL_ALLOCERROR_RETURN_NULL(result); \
        int i = 0; \
        for (; iter != iter_end ; ++iter, ++i) { \
            RESULT_I_ASSIGN; \
        } \
        return result; \
    }

#define CODEGEN_MMTF_parser_fetch_array(type_, RESULT_I_ASSIGN) \
    static TYPEALIAS_ ## type_ * MMTF_parser_fetch_ ## type_ ## _array( \
            const msgpack_object* object, size_t* length) { \
        if (object->type == MSGPACK_OBJECT_BIN) { \
            return (TYPEALIAS_ ## type_*) \
            MMTF_parser_fetch_typed_array(object, length, MMTF_TYPE_ ## type_); \
        } \
        CODEGEN_BODY_fetch_OBJECT_ARRAY(TYPEALIAS_ ## type_, RESULT_I_ASSIGN); \
    }

#define CODEGEN_MMTF_parser_fetch_List(type_, suffix) \
    type_ * MMTF_parser_fetch_ ## suffix ## List( \
            const msgpack_object* object, size_t* length) { \
        CODEGEN_BODY_fetch_OBJECT_ARRAY(type_, { \
            MMTF_parser_put_ ## suffix(iter, result + i); \
        }) \
    }

/*
 * Generate "initialize", "new", "empty" and "destroy" functions for MMTF struct types.
 */
CODEGEN_MMTF_parser_TYPE(MMTF_container)
CODEGEN_MMTF_parser_TYPE(MMTF_BioAssembly)
CODEGEN_MMTF_parser_TYPE(MMTF_Transform)
CODEGEN_MMTF_parser_TYPE(MMTF_Entity)
CODEGEN_MMTF_parser_TYPE(MMTF_GroupType)

//*** Destroy the innner of a struct
MMTF_container* MMTF_parser_MMTF_container_destroy_inside( MMTF_container* thing ) {
    size_t i;
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
    size_t i;
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
    size_t i;
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

static inline
void assign_bigendian_4(void * dst, const char * src) {
    *((uint32_t*)dst) = ntohl(*((uint32_t*)src));
}

static inline
void assign_bigendian_2(void * dst, const char * src) {
    *((uint16_t*)dst) = ntohs(*((uint16_t*)src));
}

static
void array_copy_bigendian_4(void * dst, const char * src, size_t n) {
    size_t i;
    for (i = 0; i < n; i += 4) {
        assign_bigendian_4(((char*)dst) + i, src + i);
    }
}

static
void array_copy_bigendian_2(void * dst, const char * src, size_t n) {
    size_t i;
    for (i = 0; i < n; i += 2) {
        assign_bigendian_2(((char*)dst) + i, src + i);
    }
}

float* MMTF_parser_float_from_bytes( const char* input, uint32_t input_length, uint32_t* output_length ) {
	IF_NOT_MULTIPLE_ERROR_RETURN(input_length, 4, NULL);

	(*output_length) = input_length/4;

	float* output = MALLOC_ARRAY(float, *output_length);
    IF_NULL_ALLOCERROR_RETURN_NULL(output);

	array_copy_bigendian_4(output, input, input_length);

	return output;
}

int8_t* MMTF_parser_int8_from_bytes( const char* input, uint32_t input_length, uint32_t* output_length ) {
	(*output_length) = input_length;

	int8_t* output = MALLOC_ARRAY(int8_t, *output_length);
    IF_NULL_ALLOCERROR_RETURN_NULL(output);

	memcpy(output, input, input_length);

	return output;
}

int16_t* MMTF_parser_int16_from_bytes( const char* input, uint32_t input_length, uint32_t* output_length ) {
	IF_NOT_MULTIPLE_ERROR_RETURN(input_length, 2, NULL);

	(*output_length) = input_length/2;

	int16_t* output = MALLOC_ARRAY(int16_t, (*output_length) );
    IF_NULL_ALLOCERROR_RETURN_NULL(output);

	array_copy_bigendian_2(output, input, input_length);

	return output;
}

int MMTF_parser_get_strategy(const char * bytes) {
	MMTF_parser_four_bytes_as_int32 ct;
	assign_bigendian_4(&ct.i, bytes);
	return ct.i;
}

int MMTF_parser_get_len(const char * bytes){
	MMTF_parser_four_bytes_as_int32 ct;
	assign_bigendian_4(&ct.i, bytes + 4);
	return ct.i;
}

int  MMTF_parser_get_param(const char * bytes) {
	MMTF_parser_four_bytes_as_int32 ct;
	assign_bigendian_4(&ct.i, bytes + 8);
	return ct.i;
}

int32_t* MMTF_parser_int32_from_bytes( const char* input, const uint32_t input_length, uint32_t* output_length ) {
	IF_NOT_MULTIPLE_ERROR_RETURN(input_length, 4, NULL);

	(*output_length) = input_length/4;

	int32_t* output = MALLOC_ARRAY(int32_t, (*output_length) );
    IF_NULL_ALLOCERROR_RETURN_NULL(output);

	array_copy_bigendian_4(output, input, input_length);

	return output;
}

char** MMTF_parser_strings_from_bytes( const char* input, uint32_t input_length, uint32_t parameter, uint32_t* output_length ) {
	IF_NOT_MULTIPLE_ERROR_RETURN(input_length, parameter, NULL);

	(*output_length) = input_length / parameter;

	char** output = MALLOC_ARRAY(char*, (*output_length) );
    IF_NULL_ALLOCERROR_RETURN_NULL(output);

	uint32_t i;
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
int32_t* MMTF_parser_run_length_decode(const int32_t* input, uint32_t input_length, uint32_t* output_length ) {
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
int32_t* MMTF_parser_delta_decode(const int32_t* input, uint32_t input_length, uint32_t* output_length ) {
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
int32_t* MMTF_parser_recursive_indexing_decode_from_16(const int16_t* input, uint32_t input_length, uint32_t* output_length ) {
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

int32_t* MMTF_parser_recursive_indexing_decode_from_8(const int8_t* input, uint32_t input_length, uint32_t* output_length ) {
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
float* MMTF_parser_integer_decode_from_16(const int16_t* input, uint32_t input_length, int32_t parameter, uint32_t* output_length ) {
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

float* MMTF_parser_integer_decode_from_32(const int32_t* input, uint32_t input_length, int32_t parameter, uint32_t* output_length ) {
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
void MMTF_parser_put_string(const msgpack_object* object, char** out) {
    size_t string_size = object->via.str.size;
    char * result = (*out) = MALLOC_ARRAY(char, (string_size + 1));
    IF_NULL_ALLOCERROR_RETURN(result,);
    memcpy(result, object->via.str.ptr, string_size);
    result[string_size] = '\0';
}

//*** Unpacking from MsgPack and applying strategy
char* MMTF_parser_fetch_string( const msgpack_object* object ) {
	if( object->type != MSGPACK_OBJECT_STR ) {
		fprintf( stderr, "Error in %s: the entry encoded in the MMTF is not a string.\n", __FUNCTION__ );
		return NULL;
	}

    char * result = NULL;
    MMTF_parser_put_string(object, &result);
	return result;
}

char MMTF_parser_fetch_char( const msgpack_object* object ) {
	if( object->type != MSGPACK_OBJECT_STR) {
		fprintf( stderr, "Error in %s: the entry encoded in the MMTF is not a string.\n", __FUNCTION__ );
		return '\0';
	}

	return *(object->via.str.ptr);
}

int64_t MMTF_parser_fetch_int( const msgpack_object* object ) {
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

float MMTF_parser_fetch_float( const msgpack_object* object ) {
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
void* MMTF_parser_fetch_typed_array( const msgpack_object* object, size_t* length, int typecode) {
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

    if (typecode != typecheck) {
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
CODEGEN_MMTF_parser_fetch_array(char,   result[i] = iter->via.u64)
CODEGEN_MMTF_parser_fetch_array(int8,   result[i] = iter->via.u64)
CODEGEN_MMTF_parser_fetch_array(int32,  result[i] = iter->via.u64)
CODEGEN_MMTF_parser_fetch_array(float,  result[i] = iter->via.f64)
CODEGEN_MMTF_parser_fetch_array(string, MMTF_parser_put_string(iter, result + i))

bool MMTF_parser_compare_msgpack_string_char_array( const msgpack_object_str* m_string, const char* string ) {
	return (m_string->size == strlen( string ) && strncmp( m_string->ptr, string, m_string->size ) == 0);
}

<<<<<<< HEAD
MMTF_Entity* MMTF_parser_fetch_entityList( msgpack_object* object, uint64_t* length ) {
	if( object->type != MSGPACK_OBJECT_ARRAY ) {
		fprintf( stderr, "Error in %s: the entry encoded in the MMTF is not an array.\n", __FUNCTION__ );
		return NULL;
	}

	msgpack_object* current_entity = object->via.array.ptr;
	uint32_t entityList_count = object->via.array.size;
	*length = (uint64_t) entityList_count;
	msgpack_object* stop_entity = object->via.array.ptr + entityList_count;

//printf( "Decoding an array of %u Entity.\n", entityList_count );

	MMTF_Entity* result = malloc( sizeof( MMTF_Entity ) * entityList_count );

	if( result == NULL ) {
		fprintf( stderr, "Error in %s: couldn't allocate memory for the EntityList.\n", __FUNCTION__ );
		return NULL;
	}

	int i = 0;
	for( ; current_entity < stop_entity; ++current_entity ) {
		MMTF_parser_put_entity( current_entity, &(result[i]) );
		++i;
	}

    return result;
}

void MMTF_parser_put_entity( msgpack_object* object, MMTF_Entity* entity ) {
	if( object->type != MSGPACK_OBJECT_MAP ) {
		fprintf( stderr, "Error in %s: the entry encoded in the MMTF is not a map.\n", __FUNCTION__ );
		return;
	}

	msgpack_object_kv* current_key_value = object->via.map.ptr;
	msgpack_object_kv* last_key_value = current_key_value + object->via.map.size;

	for( ; current_key_value <= last_key_value; ++current_key_value ) {
		msgpack_object* key = &(current_key_value->key);
		msgpack_object* value = &(current_key_value->val);

		if( key->type == MSGPACK_OBJECT_STR ) {

//printf( "Key %.*s:\n", key->via.str.size, key->via.str.ptr );

//			uint32_t key_size = key->via.str.size;
//			const char* key_string = key->via.str.ptr;

			if( MMTF_parser_compare_msgpack_string_char_array( &(key->via.str), "description" ) ) {
				(*entity).description = MMTF_parser_fetch_string( value );

//printf( "description : %s\n\n", (*entity).description );
			}

			if( MMTF_parser_compare_msgpack_string_char_array( &(key->via.str), "type" ) ) {
				(*entity).type = MMTF_parser_fetch_string( value );

//printf( "type : %s\n\n", (*entity).type );
			}

			if( MMTF_parser_compare_msgpack_string_char_array( &(key->via.str), "chainIndexList" ) ) {
				uint64_t length;
				(*entity).chainIndexList = MMTF_parser_fetch_clear_lu_array( value, &length );
			}

			if( MMTF_parser_compare_msgpack_string_char_array( &(key->via.str), "sequence" ) ) {
				(*entity).sequence = MMTF_parser_fetch_string( value );

//printf( "sequence : %s\n\n", (*entity).sequence );
			}
		}
	}
}

MMTF_GroupType* MMTF_parser_fetch_groupList( msgpack_object* object, uint64_t* length ) {
	if( object->type != MSGPACK_OBJECT_ARRAY ) {
		fprintf( stderr, "Error in %s: the entry encoded in the MMTF is not an array.\n", __FUNCTION__ );
		return NULL;
	}

	msgpack_object* current_groupType = object->via.array.ptr;
	uint32_t groupList_count = object->via.array.size;
	*length = (uint64_t) groupList_count;
	msgpack_object* stop_groupType = object->via.array.ptr + groupList_count;

//printf( "Decoding an array of %u GroupType.\n", groupList_count );

	MMTF_GroupType* result = malloc( sizeof( MMTF_GroupType ) * groupList_count );

	if( result == NULL ) {
		fprintf( stderr, "Error in %s: couldn't allocate memory for the GroupType.\n", __FUNCTION__ );
		return NULL;
	}

	int i = 0;
	for( ; current_groupType < stop_groupType; ++current_groupType ) {
		MMTF_parser_put_groupType( current_groupType, &(result[i]) );
		++i;
	}

    return result;
}

void MMTF_parser_put_groupType( msgpack_object* object, MMTF_GroupType* group_type ) {
	if( object->type != MSGPACK_OBJECT_MAP ) {
		fprintf( stderr, "Error in %s: the entry encoded in the MMTF is not a map.\n", __FUNCTION__ );
		return;
	}

	msgpack_object_kv* current_key_value = object->via.map.ptr;
	msgpack_object_kv* last_key_value = current_key_value + object->via.map.size;

	for( ; current_key_value <= last_key_value; ++current_key_value ) {
		msgpack_object* key = &(current_key_value->key);
		msgpack_object* value = &(current_key_value->val);

		if( key->type == MSGPACK_OBJECT_STR ) {

//printf( "Key %.*s:\n", key->via.str.size, key->via.str.ptr );

//			uint32_t key_size = key->via.str.size;
//			const char* key_string = key->via.str.ptr;

			if( MMTF_parser_compare_msgpack_string_char_array( &(key->via.str), "formalChargeList" ) ) {
				uint64_t length;
				(*group_type).formalChargeList = MMTF_parser_fetch_clear_int_array( value, &length );
			}

			if( MMTF_parser_compare_msgpack_string_char_array( &(key->via.str), "atomNameList" ) ) {
				(*group_type).atomNameList = MMTF_parser_fetch_clear_string_array( value, &(group_type->atomNameListCount) );
			}

			if( MMTF_parser_compare_msgpack_string_char_array( &(key->via.str), "elementList" ) ) {
				(*group_type).elementList = MMTF_parser_fetch_clear_string_array( value, &(group_type->elementListCount) );
			}

			if( MMTF_parser_compare_msgpack_string_char_array( &(key->via.str), "bondAtomList" ) ) {
				(*group_type).bondAtomList = MMTF_parser_fetch_clear_int_array( value, &(group_type->bondAtomListCount) );
			}

			if( MMTF_parser_compare_msgpack_string_char_array( &(key->via.str), "bondOrderList" ) ) {
				(*group_type).bondOrderList = MMTF_parser_fetch_clear_int8_array( value, &(group_type->bondOrderListCount) );
			}

			if( MMTF_parser_compare_msgpack_string_char_array( &(key->via.str), "groupName" ) ) {
				(*group_type).groupName = MMTF_parser_fetch_string( value );

//printf( "groupName : %s\n\n", (*group_type).groupName );
			}

			if( MMTF_parser_compare_msgpack_string_char_array( &(key->via.str), "singleLetterCode" ) ) {
				(*group_type).singleLetterCode = MMTF_parser_fetch_char( value );

//printf( "singleLetterCode : %c\n\n", (*group_type).singleLetterCode );
			}

			if( MMTF_parser_compare_msgpack_string_char_array( &(key->via.str), "chemCompType" ) ) {
				(*group_type).chemCompType = MMTF_parser_fetch_string( value );

//printf( "chemCompType : %s\n\n", (*group_type).chemCompType );
			}
		}
	}
}

MMTF_BioAssembly* MMTF_parser_fetch_bioAssemblyList( msgpack_object* object, uint64_t* length ) {
	if( object->type != MSGPACK_OBJECT_ARRAY ) {
		fprintf( stderr, "Error in %s: the entry encoded in the MMTF is not an array.\n", __FUNCTION__ );
		return NULL;
	}

	msgpack_object* current_bioAssembly = object->via.array.ptr;
	uint32_t bioAssembly_count = object->via.array.size;
	*length = (uint64_t) bioAssembly_count;
	msgpack_object* stop_bioAssembly = object->via.array.ptr + bioAssembly_count;

//printf( "Decoding an array of %u BioAssembly.\n", bioAssembly_count );

	MMTF_BioAssembly* result = malloc( sizeof( MMTF_BioAssembly ) * bioAssembly_count );

	if( result == NULL ) {
		fprintf( stderr, "Error in %s: couldn't allocate memory for the bioAssemblyList.\n", __FUNCTION__ );
		return NULL;
	}

	int i = 0;
	for( ; current_bioAssembly < stop_bioAssembly; ++current_bioAssembly ) {
		MMTF_parser_put_bioAssembly( current_bioAssembly, &(result[i]) );
		++i;
	}

    return result;
}

void MMTF_parser_put_bioAssembly( msgpack_object* object, MMTF_BioAssembly* bio_assembly ) {
	if( object->type != MSGPACK_OBJECT_MAP ) {
		fprintf( stderr, "Error in %s: the entry encoded in the MMTF is not a map.\n", __FUNCTION__ );
		return;
	}

	msgpack_object_kv* current_key_value = object->via.map.ptr;
	msgpack_object_kv* last_key_value = current_key_value + object->via.map.size;

	for( ; current_key_value <= last_key_value; ++current_key_value ) {
		msgpack_object* key = &(current_key_value->key);
		msgpack_object* value = &(current_key_value->val);

		if( key->type == MSGPACK_OBJECT_STR ) {

//printf( "Key %.*s:\n", key->via.str.size, key->via.str.ptr );

//			uint32_t key_size = key->via.str.size;
//			const char* key_string = key->via.str.ptr;

			if( MMTF_parser_compare_msgpack_string_char_array( &(key->via.str), "name" ) ) {
				(*bio_assembly).name = MMTF_parser_fetch_string( value );

//printf( "name : %s\n", (*bio_assembly).name );
			}

			if( MMTF_parser_compare_msgpack_string_char_array( &(key->via.str), "transformList" ) ) {
				(*bio_assembly).transformList = MMTF_parser_fetch_transformList( value, &(bio_assembly->transformListCount) );
			}
		}
	}
}

MMTF_Transform* MMTF_parser_fetch_transformList( msgpack_object* object, uint64_t* length ) {
	if( object->type != MSGPACK_OBJECT_ARRAY ) {
		fprintf( stderr, "Error in %s: the entry encoded in the MMTF is not an array.\n", __FUNCTION__ );
		return NULL;
	}

	msgpack_object* current_transform = object->via.array.ptr;
	uint32_t transform_count = object->via.array.size;
	*length = (uint64_t) transform_count;
	msgpack_object* stop_transform = object->via.array.ptr + transform_count;

//printf( "Decoding an array of %u Transform.\n", transform_count );

	MMTF_Transform* result = malloc( sizeof( MMTF_Transform ) * transform_count );

	if( result == NULL ) {
		fprintf( stderr, "Error in %s: couldn't allocate memory for the transformList.\n", __FUNCTION__ );
		return NULL;
	}

	int i = 0;
	for( ; current_transform < stop_transform; ++current_transform ) {
		MMTF_parser_put_transform( current_transform, &(result[i]) );
		++i;
	}

    return result;
}

void MMTF_parser_put_transform( msgpack_object* object, MMTF_Transform* transform ) {
	if( object->type != MSGPACK_OBJECT_MAP ) {
		fprintf( stderr, "Error in %s: the entry encoded in the MMTF is not a map.\n", __FUNCTION__ );
		return;
	}

	msgpack_object_kv* current_key_value = object->via.map.ptr;
	msgpack_object_kv* last_key_value = current_key_value + object->via.map.size;

	for( ; current_key_value <= last_key_value; ++current_key_value ) {
		msgpack_object* key = &(current_key_value->key);
		msgpack_object* value = &(current_key_value->val);

		if( key->type == MSGPACK_OBJECT_STR ) {

//printf( "Key %.*s:\n", key->via.str.size, key->via.str.ptr );

//			uint32_t key_size = key->via.str.size;
//			const char* key_string = key->via.str.ptr;

			if( MMTF_parser_compare_msgpack_string_char_array( &(key->via.str), "chainIndexList" ) ) {
				uint64_t length;
				(*transform).chainIndexList = MMTF_parser_fetch_clear_lu_array( value, &length );
			}

			if( MMTF_parser_compare_msgpack_string_char_array( &(key->via.str), "matrix" ) ) {
				uint64_t length;
				float* matrix = MMTF_parser_fetch_clear_float_array( value, &length );

				if( matrix == NULL ) {
					fprintf( stderr, "Error in %s: there is no matrix to fetch.\n", __FUNCTION__ );
					return;
				}

				if( length != 16 ) {
					fprintf( stderr, "Error in %s: the length of the matrix entry is %lu, it should have a length of 16.\n", __FUNCTION__, length );
					return;
				}

				int i;
				for( i = 0; i < 16; ++i ) {
					(*transform).matrix[i] = matrix[i];

//printf( "%i = %f\n", i, matrix[i] );

				}

				free( matrix );
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

//			uint32_t key_size = key->via.str.size;
//			const char* key_string = key->via.str.ptr;

			if( MMTF_parser_compare_msgpack_string_char_array( &(key->via.str), "mmtfVersion" ) ) {
//			if( key_size == strlen( "mmtfVersion" ) && strncmp( key_string, "mmtfVersion", key_size ) == 0 ) {
				(*thing).mmtfVersion = MMTF_parser_fetch_string( value );

//printf( "mmtfVersion : %s\n\n", (*thing).mmtfVersion );
			}

			if( MMTF_parser_compare_msgpack_string_char_array( &(key->via.str), "mmtfProducer" ) ) {
				(*thing).mmtfProducer = MMTF_parser_fetch_string( value );

//printf( "mmtfProducer : %s\n\n", (*thing).mmtfProducer );
			}

			if( MMTF_parser_compare_msgpack_string_char_array( &(key->via.str), "spaceGroup" ) ) {
				(*thing).spaceGroup = MMTF_parser_fetch_string( value );

//printf( "spaceGroup : %s\n\n", (*thing).spaceGroup );
			}

			if( MMTF_parser_compare_msgpack_string_char_array( &(key->via.str), "structureId" ) ) {
				(*thing).structureId = MMTF_parser_fetch_string( value );

//printf( "structureId : %s\n\n", (*thing).structureId );
			}

			if( MMTF_parser_compare_msgpack_string_char_array( &(key->via.str), "title" ) ) {
				(*thing).title = MMTF_parser_fetch_string( value );

//printf( "title : %s\n\n", (*thing).title );
			}

			if( MMTF_parser_compare_msgpack_string_char_array( &(key->via.str), "depositionDate" ) ) {
				(*thing).depositionDate = MMTF_parser_fetch_string( value );

//printf( "depositionDate : %s\n\n", (*thing).depositionDate );
			}

			if( MMTF_parser_compare_msgpack_string_char_array( &(key->via.str), "releaseDate" ) ) {
				(*thing).releaseDate = MMTF_parser_fetch_string( value );

//printf( "releaseDate : %s\n\n", (*thing).releaseDate );
			}

			if( MMTF_parser_compare_msgpack_string_char_array( &(key->via.str), "numBonds" ) ) {
				(*thing).numBonds = (int32_t) MMTF_parser_fetch_int( value );

//printf( "numBonds : %i\n\n", (*thing).numBonds );
			}

			if( MMTF_parser_compare_msgpack_string_char_array( &(key->via.str), "numAtoms" ) ) {
				(*thing).numAtoms = (int32_t) MMTF_parser_fetch_int( value );

//printf( "numAtoms : %i\n\n", (*thing).numAtoms );
			}

			if( MMTF_parser_compare_msgpack_string_char_array( &(key->via.str), "numGroups" ) ) {
				(*thing).numGroups = (int32_t) MMTF_parser_fetch_int( value );

//printf( "numGroups : %i\n\n", (*thing).numGroups );
			}

			if( MMTF_parser_compare_msgpack_string_char_array( &(key->via.str), "numChains" ) ) {
				(*thing).numChains = (int32_t) MMTF_parser_fetch_int( value );

//printf( "numChains : %i\n\n", (*thing).numChains );
			}

			if( MMTF_parser_compare_msgpack_string_char_array( &(key->via.str), "numModels" ) ) {
				(*thing).numModels = (int32_t) MMTF_parser_fetch_int( value );

//printf( "numModels : %i\n\n", (*thing).numModels );
			}

			if( MMTF_parser_compare_msgpack_string_char_array( &(key->via.str), "entityList" ) ) {
				(*thing).entityList = MMTF_parser_fetch_entityList( value, &(thing->entityListCount) );

//printf( "entityList : %p\n\n", (*thing).entityList );
			}

			if( MMTF_parser_compare_msgpack_string_char_array( &(key->via.str), "unitCell" ) ) {
				uint64_t length;
				float* unitCell = MMTF_parser_fetch_clear_float_array( value, &length );

				if( unitCell != NULL ) {
//printf( "unitCell : ");
					int i;
					for( i = 0; i < 6; ++i ) {
						(*thing).unitCell[i] = unitCell[i];
//printf( "%f ", unitCell[i] );
					}

					free( unitCell );
				}
			}

			if( MMTF_parser_compare_msgpack_string_char_array( &(key->via.str), "bioAssemblyList" ) ) {
				(*thing).bioAssemblyList = MMTF_parser_fetch_bioAssemblyList( value, &((*thing).bioAssemblyListCount) );
			}

			if( MMTF_parser_compare_msgpack_string_char_array( &(key->via.str), "resolution" ) ) {
				(*thing).resolution = MMTF_parser_fetch_float( value );

//printf( "resolution : %f\n\n", (*thing).resolution );
			}

			if( MMTF_parser_compare_msgpack_string_char_array( &(key->via.str), "rFree" ) ) {
				(*thing).rFree = MMTF_parser_fetch_float( value );

//printf( "rFree : %f\n\n", (*thing).rFree );
			}

			if( MMTF_parser_compare_msgpack_string_char_array( &(key->via.str), "rWork" ) ) {
				(*thing).rWork = MMTF_parser_fetch_float( value );

//printf( "rWork : %f\n\n", (*thing).rWork );
			}

			if( MMTF_parser_compare_msgpack_string_char_array( &(key->via.str), "experimentalMethods" ) ) {
				(*thing).experimentalMethods = MMTF_parser_fetch_clear_string_array( value, &(thing->experimentalMethodsCount) );

//int i;
//for( i = 0; i < thing->experimentalMethodsCount; ++i ) {
//printf( "experimentalMethods : %s\n\n", (*thing).experimentalMethods[i] );
//}

			}

			if( MMTF_parser_compare_msgpack_string_char_array( &(key->via.str), "bondAtomList" ) ) {
				(*thing).bondAtomList = (int32_t*) MMTF_parser_fetch_array( value, &(thing->bondAtomListCount) );

//printf( "bondAtomList : %p\n\n", (*thing).bondAtomList );
			}

			if( MMTF_parser_compare_msgpack_string_char_array( &(key->via.str), "chainsPerModel" ) ) {
				uint64_t length;
				(*thing).chainsPerModel = MMTF_parser_fetch_clear_int32_array( value, &length );

//printf( "chainsPerModel : %p\n\n", (*thing).chainsPerModel );
			}

			if( MMTF_parser_compare_msgpack_string_char_array( &(key->via.str), "groupsPerChain" ) ) {
				uint64_t length;
				(*thing).groupsPerChain = MMTF_parser_fetch_clear_int32_array( value, &length );

//printf( "groupsPerChain : %p\n\n", (*thing).groupsPerChain );
			}

			if( MMTF_parser_compare_msgpack_string_char_array( &(key->via.str), "groupList" ) ) {
				(*thing).groupList = MMTF_parser_fetch_groupList( value, &(thing->groupListCount) );

//printf( "groupList : %p\n\n", (*thing).groupList );
//printf( "groupListCount : %lu\n\n", (*thing).groupListCount );
			}

			if( MMTF_parser_compare_msgpack_string_char_array( &(key->via.str), "groupTypeList" ) ) {
				uint64_t length;
				(*thing).groupTypeList = (int32_t*) MMTF_parser_fetch_array( value, &length );

//printf( "groupTypeList : %p\n\n", (*thing).groupTypeList );
			}

			if( MMTF_parser_compare_msgpack_string_char_array( &(key->via.str), "groupIdList" ) ) {
				uint64_t length;
				(*thing).groupIdList = (int32_t*) MMTF_parser_fetch_array( value, &length );

//printf( "groupIdList : %p\n\n", (*thing).groupIdList );
			}

			if( MMTF_parser_compare_msgpack_string_char_array( &(key->via.str), "sequenceIndexList" ) ) {
				uint64_t length;
				(*thing).sequenceIndexList = (int32_t*) MMTF_parser_fetch_array( value, &length );

//printf( "sequenceIndexList : %p\n\n", (*thing).sequenceIndexList );
			}

			if( MMTF_parser_compare_msgpack_string_char_array( &(key->via.str), "atomIdList" ) ) {
				uint64_t length;
				(*thing).atomIdList = (int32_t*) MMTF_parser_fetch_array( value, &length );

//printf( "atomIdList : %p\n\n", (*thing).atomIdList );
			}

			if( MMTF_parser_compare_msgpack_string_char_array( &(key->via.str), "insCodeList" ) ) {
				uint64_t length;
				(*thing).insCodeList = (char*) MMTF_parser_fetch_array( value, &length );

//printf( "insCodeList : %p\n\n", (*thing).insCodeList );
			}

			if( MMTF_parser_compare_msgpack_string_char_array( &(key->via.str), "altLocList" ) ) {
				uint64_t length;
				(*thing).altLocList = (char*) MMTF_parser_fetch_array( value, &length );

//printf( "altLocList : %p\n\n", (*thing).altLocList );
			}

			if( MMTF_parser_compare_msgpack_string_char_array( &(key->via.str), "bondOrderList" ) ) {
				(*thing).bondOrderList = (int8_t*) MMTF_parser_fetch_array( value, &(thing->bondOrderListCount) );

//printf( "bondOrderList : %p\n\n", (*thing).bondOrderList );
			}

			if( MMTF_parser_compare_msgpack_string_char_array( &(key->via.str), "secStructList" ) ) {
				uint64_t length;
				(*thing).secStructList = (int8_t*) MMTF_parser_fetch_array( value, &length );

//printf( "secStructList : %p\n\n", (*thing).secStructList );
			}

			if( MMTF_parser_compare_msgpack_string_char_array( &(key->via.str), "bFactorList" ) ) {
				uint64_t length;
				(*thing).bFactorList = (float*) MMTF_parser_fetch_array( value, &length );

//printf( "bFactorList : %p\n\n", (*thing).bFactorList );
			}

			if( MMTF_parser_compare_msgpack_string_char_array( &(key->via.str), "xCoordList" ) ) {
				uint64_t length;
				(*thing).xCoordList = (float*) MMTF_parser_fetch_array( value, &length );

//printf( "xCoordList : %p\n\n", (*thing).xCoordList );
			}

			if( MMTF_parser_compare_msgpack_string_char_array( &(key->via.str), "yCoordList" ) ) {
				uint64_t length;
				(*thing).yCoordList = (float*) MMTF_parser_fetch_array( value, &length );

//printf( "yCoordList : %p\n\n", (*thing).yCoordList );
			}

			if( MMTF_parser_compare_msgpack_string_char_array( &(key->via.str), "zCoordList" ) ) {
				uint64_t length;
				(*thing).zCoordList = (float*) MMTF_parser_fetch_array( value, &length );

//printf( "zCoordList : %p\n\n", (*thing).zCoordList );
			}

			if( MMTF_parser_compare_msgpack_string_char_array( &(key->via.str), "occupancyList" ) ) {
				uint64_t length;
				(*thing).occupancyList = (float*) MMTF_parser_fetch_array( value, &length );

//printf( "occupancyList : %p\n\n", (*thing).occupancyList );
			}

			if( MMTF_parser_compare_msgpack_string_char_array( &(key->via.str), "chainIdList" ) ) {
				(*thing).chainIdList = (char**) MMTF_parser_fetch_array( value, &(thing->chainIdListCount) );

//printf( "chainIdList : %p\n\n", (*thing).chainIdList );
			}

			if( MMTF_parser_compare_msgpack_string_char_array( &(key->via.str), "chainNameList" ) ) {
				(*thing).chainNameList = (char**) MMTF_parser_fetch_array( value, &(thing->chainNameListCount) );

//printf( "chainNameList : %p\n\n", (*thing).chainNameList );
			}
		}
	}
}

void MMTF_parser_parse_msgpack(char *buffer,int msgsize, MMTF_container* thing){
=======
static
void MMTF_parser_put_entity( const msgpack_object* object, MMTF_Entity* entity ) {
    MAP_ITERATE_BEGIN(object);
    FETCH_AND_ASSIGN(entity, string, description);
    FETCH_AND_ASSIGN(entity, string, type);
    FETCH_AND_ASSIGN_WITHCOUNT(entity, int32_array, chainIndexList);
    FETCH_AND_ASSIGN(entity, string, sequence);
    MAP_ITERATE_END();
}

static
void MMTF_parser_put_group( const msgpack_object* object, MMTF_GroupType* group_type ) {
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
void MMTF_parser_put_bioAssembly( const msgpack_object* object, MMTF_BioAssembly* bio_assembly ) {
    MAP_ITERATE_BEGIN(object);
    FETCH_AND_ASSIGN(bio_assembly, string, name);
    FETCH_AND_ASSIGN_WITHCOUNT(bio_assembly, transformList, transformList);
    MAP_ITERATE_END();
}

static
void MMTF_parser_put_transform( const msgpack_object* object, MMTF_Transform* transform ) {
    MAP_ITERATE_BEGIN(object);
    FETCH_AND_ASSIGN_WITHCOUNT(transform, int32_array, chainIndexList);
    FETCH_AND_ASSIGN_ARRAY(transform, float, matrix);
    MAP_ITERATE_END();
}

CODEGEN_MMTF_parser_fetch_List(MMTF_Entity, entity)
CODEGEN_MMTF_parser_fetch_List(MMTF_GroupType, group)
CODEGEN_MMTF_parser_fetch_List(MMTF_BioAssembly, bioAssembly)
CODEGEN_MMTF_parser_fetch_List(MMTF_Transform, transform)

void MMTF_parser_msgpack_object_to_MMTF_container(const msgpack_object* object, MMTF_container* thing) {
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
>>>>>>> e4bc1516f09d24a225734a2c1c4d6243a62b77b5
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
