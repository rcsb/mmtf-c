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


//*** Initiate a struct
MMTF_container* MMTF_parser_MMTF_container_new( void ) {
	MMTF_container* result = malloc( sizeof( MMTF_container ) );

	if( result == NULL ) {
		fprintf( stderr, "Error in %s: couldn't allocate memory.\n", __FUNCTION__ );
		return NULL;
	}

	return MMTF_parser_MMTF_container_initialize( result );
}
MMTF_BioAssembly* MMTF_parser_MMTF_BioAssembly_new( void ) {
	MMTF_BioAssembly* result = malloc( sizeof( MMTF_BioAssembly ) );

	if( result == NULL ) {
		fprintf( stderr, "Error in %s: couldn't allocate memory.\n", __FUNCTION__ );
		return NULL;
	}

	return MMTF_parser_MMTF_BioAssembly_initialize( result );
}
MMTF_Transform* MMTF_parser_MMTF_Transform_new( void ) {
	MMTF_Transform* result = malloc( sizeof( MMTF_Transform ) );

	if( result == NULL ) {
		fprintf( stderr, "Error in %s: couldn't allocate memory.\n", __FUNCTION__ );
		return NULL;
	}

	return MMTF_parser_MMTF_Transform_initialize( result );
}
MMTF_Entity* MMTF_parser_MMTF_Entity_new( void ) {
	MMTF_Entity* result = malloc( sizeof( MMTF_Entity ) );

	if( result == NULL ) {
		fprintf( stderr, "Error in %s: couldn't allocate memory.\n", __FUNCTION__ );
		return NULL;
	}

	return MMTF_parser_MMTF_Entity_initialize( result );
}
MMTF_GroupType* MMTF_parser_MMTF_GroupType_new( void ) {
	MMTF_GroupType* result = malloc( sizeof( MMTF_GroupType ) );

	if( result == NULL ) {
		fprintf( stderr, "Error in %s: couldn't allocate memory.\n", __FUNCTION__ );
		return NULL;
	}

	return MMTF_parser_MMTF_GroupType_initialize( result );
}

//*** Initialize a struct
MMTF_container* MMTF_parser_MMTF_container_initialize( MMTF_container* result ) {
	result->mmtfVersion = NULL;
	result->mmtfProducer = NULL;
	int i;
	for( i = 0; i < 6; ++i ) {
		result->unitCell[i] = 0.f;
	}
	result->spaceGroup = NULL;
	result->structureId = NULL;
	result->title = NULL;
	result->depositionDate = NULL;
	result->releaseDate = NULL;
	result->bioAssemblyListCount = 0;
	result->bioAssemblyList = NULL;
	result->entityListCount = 0;
	result->entityList = NULL;
	result->experimentalMethodsCount = 0;
	result->experimentalMethods = NULL;
	result->numberOfExperimentalMethods = 0;
    result->resolution = 0.f;
    result->rFree = 0.f;
    result->rWork = 0.f;
    result->numBonds = 0;
    result->numAtoms = 0;
    result->numGroups = 0;
    result->numChains = 0;
    result->numModels = 0;
	result->groupListCount = 0;
	result->groupList = NULL;
	result->bondAtomListCount = 0;
	result->bondAtomList = NULL;
	result->bondOrderListCount = 0;
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
	result->chainIdListCount = 0;
	result->chainIdList = NULL;
	result->chainNameListCount = 0;
	result->chainNameList = NULL;
	result->groupsPerChain = NULL;
	result->chainsPerModel = NULL;

	return result;
}
MMTF_BioAssembly* MMTF_parser_MMTF_BioAssembly_initialize( MMTF_BioAssembly* result ) {
	result->transformListCount = 0;
	result->transformList = NULL;
	result->name = NULL;

	return result;
}
MMTF_Transform* MMTF_parser_MMTF_Transform_initialize( MMTF_Transform* result ) {
	result->chainIndexList= NULL;
	int i;
	for( i = 0; i < 16; ++i ) {
		result->matrix[i] = 0.;
	}

	return result;
}
MMTF_Entity* MMTF_parser_MMTF_Entity_initialize( MMTF_Entity* result ) {
	result->chainIndexList = NULL;
	result->description = NULL;
	result->type = NULL;
	result->sequence = NULL;

	return result;
}
MMTF_GroupType* MMTF_parser_MMTF_GroupType_initialize( MMTF_GroupType* result ) {
	result->formalChargeList = NULL;
	result->atomNameListCount = 0;
    result->atomNameList = NULL;
	result->elementListCount = 0;
    result->elementList = NULL;
	result->bondAtomListCount = 0;
    result->bondAtomList = NULL;
	result->bondOrderListCount = 0;
    result->bondOrderList = NULL;
    result->groupName = NULL;
    result->singleLetterCode = '\0';
    result->chemCompType = NULL;

	return result;
}

//*** Empty a struct
MMTF_container* MMTF_parser_MMTF_container_empty( MMTF_container* result ) {
	if( result != NULL ) {
//*** Destroy what is inside
		MMTF_parser_MMTF_container_destroy_inside( result );
//*** Initialize with the default values
		MMTF_parser_MMTF_container_initialize( result );
	}
	else {
		fprintf( stderr, "Error in %s: the argument you put is NULL, it is not a valid pointer.\n", __FUNCTION__ );
	}

	return result;
}
MMTF_BioAssembly* MMTF_parser_MMTF_BioAssembly_empty( MMTF_BioAssembly* result ) {
	if( result != NULL ) {
//*** Destroy what is inside
		MMTF_parser_MMTF_BioAssembly_destroy_inside( result );
//*** Initialize with the default values
		MMTF_parser_MMTF_BioAssembly_initialize( result );
	}
	else {
		fprintf( stderr, "Error in %s: the argument you put is NULL, it is not a valid pointer.\n", __FUNCTION__ );
	}

	return result;
}
MMTF_Transform* MMTF_parser_MMTF_Transform_empty( MMTF_Transform* result ) {
	if( result != NULL ) {
//*** Destroy what is inside
		MMTF_parser_MMTF_Transform_destroy_inside( result );
//*** Initialize with the default values
		MMTF_parser_MMTF_Transform_initialize( result );
	}
	else {
		fprintf( stderr, "Error in %s: the argument you put is NULL, it is not a valid pointer.\n", __FUNCTION__ );
	}

	return result;
}
MMTF_Entity* MMTF_parser_MMTF_Entity_empty( MMTF_Entity* result ) {
	if( result != NULL ) {
//*** Destroy what is inside
		MMTF_parser_MMTF_Entity_destroy_inside( result );
//*** Initialize with the default values
		MMTF_parser_MMTF_Entity_initialize( result );
	}
	else {
		fprintf( stderr, "Error in %s: the argument you put is NULL, it is not a valid pointer.\n", __FUNCTION__ );
	}

	return result;
}
MMTF_GroupType* MMTF_parser_MMTF_GroupType_empty( MMTF_GroupType* result ) {
	if( result != NULL ) {
//*** Destroy what is inside
		MMTF_parser_MMTF_GroupType_destroy_inside( result );
//*** Initialize with the default values
		MMTF_parser_MMTF_GroupType_initialize( result );
	}
	else {
		fprintf( stderr, "Error in %s: the argument you put is NULL, it is not a valid pointer.\n", __FUNCTION__ );
	}

	return result;
}

//*** Destroy the innner of a struct
MMTF_container* MMTF_parser_MMTF_container_destroy_inside( MMTF_container* thing ) {
	if( thing != NULL ) {
		uint64_t i;

		if( thing->mmtfVersion != NULL )
			free( thing->mmtfVersion );
		if( thing->mmtfProducer != NULL )
			free( thing->mmtfProducer );
		if( thing->spaceGroup != NULL )
			free( thing->spaceGroup );
		if( thing->structureId != NULL )
			free( thing->structureId );
		if( thing->title != NULL )
			free( thing->title );
		if( thing->depositionDate != NULL )
			free( thing->depositionDate );
		if( thing->releaseDate != NULL )
			free( thing->releaseDate );

		if( thing->bioAssemblyList != NULL ) {
			for( i = 0; i < thing->bioAssemblyListCount; ++i ) {
				MMTF_parser_MMTF_BioAssembly_destroy_inside( &(thing->bioAssemblyList[i]) );
			}
			free( thing->bioAssemblyList );
		}

		if( thing->entityList != NULL ) {
			for( i = 0; i < thing->entityListCount; ++i ) {
				MMTF_parser_MMTF_Entity_destroy_inside( &(thing->entityList[i]) );
			}
			free( thing->entityList );
		}

		if( thing->experimentalMethods != NULL ) {
			for( i = 0; i < thing->experimentalMethodsCount; ++i ) {
				free( thing->experimentalMethods[i] );
			}
			free( thing->experimentalMethods );
		}

		if( thing->groupList != NULL ) {
			for( i = 0; i < thing->groupListCount; ++i ) {
				MMTF_parser_MMTF_GroupType_destroy_inside( &(thing->groupList[i]) );
			}
			free( thing->groupList );
		}

		if( thing->bondAtomList != NULL )
			free( thing->bondAtomList );
		if( thing->bondOrderList != NULL )
			free( thing->bondOrderList );
		if( thing->xCoordList != NULL )
			free( thing->xCoordList );
		if( thing->yCoordList != NULL )
			free( thing->yCoordList );
		if( thing->zCoordList != NULL )
			free( thing->zCoordList );
		if( thing->bFactorList != NULL )
			free( thing->bFactorList );
		if( thing->atomIdList != NULL )
			free( thing->atomIdList );
		if( thing->altLocList != NULL )
			free( thing->altLocList );
		if( thing->occupancyList != NULL )
			free( thing->occupancyList );
		if( thing->groupIdList != NULL )
			free( thing->groupIdList );
		if( thing->groupTypeList != NULL )
			free( thing->groupTypeList );
		if( thing->secStructList != NULL )
			free( thing->secStructList );
		if( thing->insCodeList != NULL )
			free( thing->insCodeList );
		if( thing->sequenceIndexList != NULL )
			free( thing->sequenceIndexList );

		if( thing->chainIdList != NULL ) {
			for( i = 0; i < thing->chainIdListCount; ++i ) {
				free( thing->chainIdList[i] );
			}
			free( thing->chainIdList );
		}

		if( thing->chainNameList != NULL ) {
			for( i = 0; i < thing->chainNameListCount; ++i ) {
				free( thing->chainNameList[i] );
			}
			free( thing->chainNameList );
		}

		if( thing->groupsPerChain != NULL )
			free( thing->groupsPerChain );
		if( thing->chainsPerModel != NULL )
			free( thing->chainsPerModel );
	}
	else {
		fprintf( stderr, "Error in %s: the argument you put is NULL, it is not a valid pointer.\n", __FUNCTION__ );
	}
	return thing;
}
MMTF_BioAssembly* MMTF_parser_MMTF_BioAssembly_destroy_inside( MMTF_BioAssembly* bio_assembly ) {
	if( bio_assembly != NULL ) {
		if( bio_assembly->name != NULL )
			free( bio_assembly->name );

		uint64_t i;
		if( bio_assembly->transformList != NULL ) {
			for( i = 0; i < bio_assembly->transformListCount; ++i ) {
				MMTF_parser_MMTF_Transform_destroy_inside( &(bio_assembly->transformList[i]) );
			}
			free( bio_assembly->transformList );
		}
	}
	else {
		fprintf( stderr, "Error in %s: the argument you put is NULL, it is not a valid pointer.\n", __FUNCTION__ );
	}
	return bio_assembly;
}
MMTF_Transform* MMTF_parser_MMTF_Transform_destroy_inside( MMTF_Transform* transform ) {
	if( transform != NULL ) {
		if( transform->chainIndexList != NULL )
			free( transform->chainIndexList );
	}
	else {
		fprintf( stderr, "Error in %s: the argument you put is NULL, it is not a valid pointer.\n", __FUNCTION__ );
	}
	return transform;
}
MMTF_Entity* MMTF_parser_MMTF_Entity_destroy_inside( MMTF_Entity* entity ) {
	if( entity != NULL ) {
		if( entity->chainIndexList != NULL )
			free( entity->chainIndexList );
		if( entity->description != NULL )
			free( entity->description );
		if( entity->type != NULL )
			free( entity->type );
		if( entity->sequence != NULL )
			free( entity->sequence );
	}
	else {
		fprintf( stderr, "Error in %s: the argument you put is NULL, it is not a valid pointer.\n", __FUNCTION__ );
	}
	return entity;
}
MMTF_GroupType* MMTF_parser_MMTF_GroupType_destroy_inside( MMTF_GroupType* group_type ) {
	if( group_type != NULL ) {
		uint64_t i;

		if( group_type->formalChargeList != NULL )
			free( group_type->formalChargeList );

		if( group_type->atomNameList != NULL ) {
			for( i = 0; i < group_type->atomNameListCount; ++i ) {
				free( group_type->atomNameList[i] );
			}
			free( group_type->atomNameList );
		}

		if( group_type->elementList != NULL ) {
			for( i = 0; i < group_type->elementListCount; ++i ) {
				free( group_type->elementList[i] );
			}
			free( group_type->elementList );
		}

		if( group_type->bondAtomList != NULL )
			free( group_type->bondAtomList );
		if( group_type->bondOrderList != NULL )
			free( group_type->bondOrderList );
		if( group_type->groupName != NULL )
			free( group_type->groupName );
		if( group_type->chemCompType != NULL )
			free( group_type->chemCompType );
	}
	else {
		fprintf( stderr, "Error in %s: the argument you put is NULL, it is not a valid pointer.\n", __FUNCTION__ );
	}
	return group_type;
}


//*** Destroy a struct
void MMTF_parser_MMTF_container_destroy( MMTF_container* thing ) {
	if( thing != NULL ) {
		MMTF_parser_MMTF_container_destroy_inside( thing );
		free( thing );
	}
	else {
		fprintf( stderr, "Error in %s: the argument you put is NULL, it is not a valid pointer.\n", __FUNCTION__ );
	}
}
void MMTF_parser_MMTF_BioAssembly_destroy( MMTF_BioAssembly* bio_assembly ) {
	if( bio_assembly != NULL ) {
		MMTF_parser_MMTF_BioAssembly_destroy_inside( bio_assembly );
		free( bio_assembly );
	}
	else {
		fprintf( stderr, "Error in %s: the argument you put is NULL, it is not a valid pointer.\n", __FUNCTION__ );
	}
}
void MMTF_parser_MMTF_Transform_destroy( MMTF_Transform* transform ) {
	if( transform != NULL ) {
		MMTF_parser_MMTF_Transform_destroy_inside( transform );
		free( transform );
	}
	else {
		fprintf( stderr, "Error in %s: the argument you put is NULL, it is not a valid pointer.\n", __FUNCTION__ );
	}
}
void MMTF_parser_MMTF_Entity_destroy( MMTF_Entity* entity ) {
	if( entity != NULL ) {
		MMTF_parser_MMTF_Entity_destroy_inside( entity );
		free( entity );
	}
	else {
		fprintf( stderr, "Error in %s: the argument you put is NULL, it is not a valid pointer.\n", __FUNCTION__ );
	}
}
void MMTF_parser_MMTF_GroupType_destroy( MMTF_GroupType* group_type ) {
	if( group_type != NULL ) {
		MMTF_parser_MMTF_GroupType_destroy_inside( group_type );
		free( group_type );
	}
	else {
		fprintf( stderr, "Error in %s: the argument you put is NULL, it is not a valid pointer.\n", __FUNCTION__ );
	}
}

//*** Array converters
// From bytes[] to float32[], int8[], int16[], int32[] and string

float* MMTF_parser_float_from_bytes( const char* input, uint32_t input_length, uint32_t* output_length ) {
	if( input_length % 4 != 0 ) {
		fprintf( stderr, "Error in %s : the input length %u is not a multiple of 4.\n", __FUNCTION__, input_length );
		return NULL;
	}

	(*output_length) = input_length/4;

	float* output = malloc( sizeof(float)*(*output_length) );

	if( output == NULL ) {
		fprintf( stderr, "Error in %s: couldn't allocate memory for its output.\n", __FUNCTION__ );
		return NULL;
	}

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

	if( output == NULL ) {
		fprintf( stderr, "Error in %s: couldn't allocate memory for its output.\n", __FUNCTION__ );
		return NULL;
	}

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
		fprintf( stderr, "Error in %s: the input length %u is not a multiple of 2.\n", __FUNCTION__, input_length );
		return NULL;
	}

	(*output_length) = input_length/2;

	int16_t* output = malloc( sizeof(int16_t)*(*output_length) );

	if( output == NULL ) {
		fprintf( stderr, "Error in %s: couldn't allocate memory for its output.\n", __FUNCTION__ );
		return NULL;
	}

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
		fprintf( stderr, "Error in %s: the input length %u is not a multiple of 4.\n", __FUNCTION__, input_length );
		return NULL;
	}

	(*output_length) = input_length/4;

	int32_t* output = malloc( sizeof(int32_t) * (*output_length) );

	if( output == NULL ) {
		fprintf( stderr, "Error in %s: couldn't allocate memory for its output.\n", __FUNCTION__ );
		return NULL;
	}

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

	if( output == NULL ) {
		fprintf( stderr, "Error in %s: couldn't allocate memory for its output.\n", __FUNCTION__ );
		return NULL;
	}

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
		fprintf( stderr, "Error in %s: the input length %u is not a multiple of your parameter %u.\n", __FUNCTION__, input_length, parameter );
		return NULL;
	}

	(*output_length) = input_length / parameter;

	char** output = malloc( sizeof(char*)*(*output_length) );

	if( output == NULL ) {
		fprintf( stderr, "Error in %s: couldn't allocate memory for its output.\n", __FUNCTION__ );
		return NULL;
	}

	uint32_t i,j;
	for( i = 0; i < *output_length; ++i ) {
		output[i] = malloc( sizeof(char)*parameter );

		if( output[i] == NULL ) {
			fprintf( stderr, "Error in %s: couldn't allocate memory for one of the strings.\n", __FUNCTION__ );
			return NULL;
		}
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
		fprintf( stderr, "Error in %s: your input length %u is not an even number.\n", __FUNCTION__, input_length );
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

	if( output == NULL ) {
		fprintf( stderr, "Error in %s: couldn't allocate memory for its output.\n", __FUNCTION__ );
		return NULL;
	}

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

	if( output == NULL ) {
		fprintf( stderr, "Error in %s: couldn't allocate memory for its output.\n", __FUNCTION__ );
		return NULL;
	}

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

	if( output == NULL ) {
		fprintf( stderr, "Error in %s: couldn't allocate memory for its output.\n", __FUNCTION__ );
		return NULL;
	}

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

	if( output == NULL ) {
		fprintf( stderr, "Error in %s: couldn't allocate memory for its output.\n", __FUNCTION__ );
		return NULL;
	}

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

	if( output == NULL ) {
		fprintf( stderr, "Error in %s: couldn't allocate memory for its output.\n", __FUNCTION__ );
		return NULL;
	}

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

	if( output == NULL ) {
		fprintf( stderr, "Error in %s: couldn't allocate memory for its output.\n", __FUNCTION__ );
		return NULL;
	}

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

			if( result == NULL ) {
				fprintf( stderr, "Error in %s: couldn't allocate memory for its output.\n", __FUNCTION__ );
				return NULL;
			}

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
			fprintf( stderr, "Error in %s: %i does not refer to any strategy.\n", __FUNCTION__, strategy );
			return NULL;
		}
	}
}


//*** Unpacking from MsgPack and applying strategy
char* MMTF_parser_fetch_string( msgpack_object* object ) {
	if( object->type != MSGPACK_OBJECT_STR ) {
		fprintf( stderr, "Error in %s: the entry encoded in the MMTF is not a string.\n", __FUNCTION__ );
		return NULL;
	}

	msgpack_object_str* value_string = &(object->via.str);

	char* result = malloc( sizeof( char ) * ( value_string->size + 1 ) );

	if( result == NULL ) {
		fprintf( stderr, "Error in %s: couldn't allocate memory for its output.\n", __FUNCTION__ );
		return NULL;
	}

	strncpy( result, value_string->ptr, value_string->size );
	result[value_string->size] = '\0';

	return result;
}

char MMTF_parser_fetch_char( msgpack_object* object ) {
	if( object->type != MSGPACK_OBJECT_STR) {
		fprintf( stderr, "Error in %s: the entry encoded in the MMTF is not a string.\n", __FUNCTION__ );
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

void* MMTF_parser_fetch_array( msgpack_object* object, uint64_t* length ) {
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
    return MMTF_parser_decode_apply_strategy(bytes + 12, object->via.bin.size - 12, &out_length, strategy, parameter);
}

size_t* MMTF_parser_fetch_clear_lu_array( msgpack_object* object, uint64_t* length ) {
	if( object->type != MSGPACK_OBJECT_ARRAY ) {
		fprintf( stderr, "Error in %s: the entry encoded in the MMTF is not an array.\n", __FUNCTION__ );
		return NULL;
	}

	msgpack_object* current_value = object->via.array.ptr;
	(*length) = (uint64_t) object->via.array.size;
	msgpack_object* last_value = current_value + (*length);

//printf( "Unpacking an unsigned long int array of length %lu.\n", *length );

	size_t* result = malloc( sizeof( size_t ) * (*length) );

	if( result == NULL ) {
		fprintf( stderr, "Error in %s: couldn't allocate memory for its output.\n", __FUNCTION__ );
		return NULL;
	}

	int i = 0;
	for( ; current_value < last_value; ++current_value ) {
		result[i] = current_value->via.u64;
		++i;
	}

    return result;
}

int* MMTF_parser_fetch_clear_int_array( msgpack_object* object, uint64_t* length ) {
	if( object->type != MSGPACK_OBJECT_ARRAY ) {
		fprintf( stderr, "Error in %s: the entry encoded in the MMTF is not an array.\n", __FUNCTION__ );
		return NULL;
	}

	msgpack_object* current_value = object->via.array.ptr;
	(*length) = (uint64_t) object->via.array.size;
	msgpack_object* last_value = current_value + (*length);

//printf( "Unpacking an int array of length %lu.\n", *length );

	int* result = malloc( sizeof( int ) * (*length) );

	if( result == NULL ) {
		fprintf( stderr, "Error in %s: couldn't allocate memory for its output.\n", __FUNCTION__ );
		return NULL;
	}

	int i = 0;
	for( ; current_value < last_value; ++current_value ) {
		result[i] = (int) current_value->via.u64;
		++i;
	}

    return result;
}

int32_t* MMTF_parser_fetch_clear_int32_array( msgpack_object* object, uint64_t* length ) {
	if( object->type != MSGPACK_OBJECT_ARRAY ) {
		fprintf( stderr, "Error in %s: the entry encoded in the MMTF is not an array.\n", __FUNCTION__ );
		return NULL;
	}

	msgpack_object* current_value = object->via.array.ptr;
	(*length) = (uint64_t) object->via.array.size;
	msgpack_object* last_value = current_value + (*length);

//printf( "Unpacking an int32 array of length %lu.\n", *length );

	int32_t* result = malloc( sizeof( int32_t ) * (*length) );

	if( result == NULL ) {
		fprintf( stderr, "Error in %s: couldn't allocate memory for its output.\n", __FUNCTION__ );
		return NULL;
	}

	int i = 0;
	for( ; current_value < last_value; ++current_value ) {
		result[i] = (int32_t) current_value->via.u64;
		++i;
	}

    return result;
}

char* MMTF_parser_fetch_clear_int8_array( msgpack_object* object, uint64_t* length ) {
	if( object->type != MSGPACK_OBJECT_ARRAY ) {
		fprintf( stderr, "Error in %s: the entry encoded in the MMTF is not an array.\n", __FUNCTION__ );
		return NULL;
	}

	msgpack_object* current_value = object->via.array.ptr;
	(*length) = (uint64_t) object->via.array.size;
	msgpack_object* last_value = current_value + (*length);

//printf( "Unpacking an int8 array of length %lu.\n", *length );

	char* result = malloc( sizeof( char ) * (*length) );

	if( result == NULL ) {
		fprintf( stderr, "Error in %s: couldn't allocate memory for its output.\n", __FUNCTION__ );
		return NULL;
	}

	int i = 0;
	for( ; current_value < last_value; ++current_value ) {
		result[i] = (char) current_value->via.u64;
		++i;
	}

    return result;
}

float* MMTF_parser_fetch_clear_float_array( msgpack_object* object, uint64_t* length ) {
	if( object->type != MSGPACK_OBJECT_ARRAY ) {
		fprintf( stderr, "Error in %s: the entry encoded in the MMTF is not an array.\n", __FUNCTION__ );
		return NULL;
	}

	msgpack_object* current_value = object->via.array.ptr;
	(*length) = (uint64_t) object->via.array.size;
	msgpack_object* last_value = current_value + (*length);

//printf( "Unpacking a float array of length %lu.\n", *length );

	float* result = malloc( sizeof( float ) * (*length) );

	if( result == NULL ) {
		fprintf( stderr, "Error in %s: couldn't allocate memory for its output.\n", __FUNCTION__ );
		return NULL;
	}

	int i = 0;
	for( ; current_value < last_value; ++current_value ) {
		result[i] = (float) current_value->via.f64;
		++i;
	}

    return result;
}

char** MMTF_parser_fetch_clear_string_array( msgpack_object* object, uint64_t* length ) {
	if( object->type != MSGPACK_OBJECT_ARRAY ) {
		fprintf( stderr, "Error in %s: the entry encoded in the MMTF is not an array.\n", __FUNCTION__ );
		return NULL;
	}

	msgpack_object* current_value = object->via.array.ptr;
	(*length) = (uint64_t) object->via.array.size;
	msgpack_object* last_value = current_value + (*length);

//printf( "Unpacking a string array of length %lu.\n", *length );

	char** result = malloc( sizeof( char* ) * (*length) );

	if( result == NULL ) {
		fprintf( stderr, "Error in %s: couldn't allocate memory for its output.\n", __FUNCTION__ );
		return NULL;
	}

	int i = 0;
	uint32_t string_size;
	for( ; current_value < last_value; ++current_value ) {
		string_size = current_value->via.str.size;
		result[i] = malloc( sizeof( char ) * ( string_size + 1 ) );

		if( result[i] == NULL ) {
			fprintf( stderr, "Error in %s: couldn't allocate memory for one of its strings.\n", __FUNCTION__ );
			return NULL;
		}

		strncpy( result[i], current_value->via.str.ptr, string_size );
		result[i][string_size] = '\0';
		++i;
	}

    return result;
}

bool MMTF_parser_compare_msgpack_string_char_array( const msgpack_object_str* m_string, const char* string ) {
	return (m_string->size == strlen( string ) && strncmp( m_string->ptr, string, m_string->size ) == 0);
}

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
					fprintf( stderr, "Error in %s: the length of the matrix entry is %llu, it should have a length of 16.\n", __FUNCTION__, length );
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

