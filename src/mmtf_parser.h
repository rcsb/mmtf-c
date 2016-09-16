//*******************************************************************************
//******	This code is published under the .... license
//******
//******
//******	This file is the header file for the MMTF parser for the C language.
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


#ifndef MMTF_PARSER_H
#define MMTF_PARSER_H

//*** For the constant NAN
#include <math.h>

//*** MsgPack lib
#include <msgpack.h>

//*** Standard libs
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>


//*** The MMTF structure
typedef struct {
    int *				formalChargeList;    // List of formal charges as Integers
	uint64_t			atomNameListCount;
    char **				atomNameList;        // List of atom names, 0 to 5 character Strings
	uint64_t			elementListCount;
    char **				elementList;         // List of elements, 0 to 3 character Strings
	uint64_t			bondAtomListCount;
    int *				bondAtomList;        // List of bonded atom indices, Integers
	uint64_t			bondOrderListCount;
    char *				bondOrderList;       // List of bond orders as Integers between 1 and 4
    char *				groupName;           // The name of the group, 0 to 5 characters
    char				singleLetterCode;    // The single letter code, 1 character
    char *				chemCompType;         // The chemical component type
} MMTF_GroupType;

typedef struct {
    size_t *			chainIndexList;   // Indices of fields in chainIdList and chainNameList fields
    char *				description;      // Description of the entity
    char *				type;             // Name of the entity type
    char *				sequence;          // Sequence of the full construct in one-letter-code
} MMTF_Entity;

typedef struct {
	size_t*				chainIndexList;
	float				matrix[16];
} MMTF_Transform;

typedef struct {
	uint64_t			transformListCount;
	MMTF_Transform*		transformList;
	char*				name;
} MMTF_BioAssembly;

typedef struct {
    char *				mmtfVersion;
    char *				mmtfProducer;
    float				unitCell[6];
    char *				spaceGroup;
    char *				structureId;
    char *				title;
    char *				depositionDate;
    char *				releaseDate;
	uint64_t			bioAssemblyListCount;
	MMTF_BioAssembly*	bioAssemblyList;
	uint64_t			entityListCount;
    MMTF_Entity *		entityList;
    /* experimentalMethods this is a character array of unknown length so we
     * need to specify the number of elements. String must be one of the
     * following
        ELECTRON CRYSTALLOGRAPHY
        ELECTRON MICROSCOPY
        EPR
        FIBER DIFFRACTION
        FLUORESCENCE TRANSFER
        INFRARED SPECTROSCOPY
        NEUTRON DIFFRACTION
        POWDER DIFFRACTION
        SOLID-STATE NMR
        SOLUTION NMR
        SOLUTION SCATTERING
        THEORETICAL MODEL
        X-RAY DIFFRACTION
     */
	uint64_t			experimentalMethodsCount;
    char **				experimentalMethods;
    size_t				numberOfExperimentalMethods;
    float				resolution;
    float				rFree;
    float				rWork;
    int32_t				numBonds;
    int32_t				numAtoms;
    int32_t				numGroups;
    int32_t				numChains;
    int32_t				numModels;
	uint64_t			groupListCount;
    MMTF_GroupType *	groupList;
	uint64_t			bondAtomListCount;
    int32_t *			bondAtomList;
	uint64_t			bondOrderListCount;
    int8_t *			bondOrderList; // 8 bit signed interger
    float *				xCoordList;
    float *				yCoordList;
    float *				zCoordList;
    float *				bFactorList;
    signed int *		atomIdList;
    char *				altLocList;
    float *				occupancyList;
    int32_t *			groupIdList;
    int32_t *			groupTypeList;
    int8_t *			secStructList; // 8 bit signed interger
    char *				insCodeList;
    int32_t *			sequenceIndexList;
	uint64_t			chainIdListCount;
    char **				chainIdList;    // Array of 4-char strings
	uint64_t			chainNameListCount;
    char **				chainNameList; // Array of 4-char strings
    int32_t *			groupsPerChain;
    int32_t *			chainsPerModel;
} MMTF_container;


//*** Create a struct
MMTF_container* MMTF_parser_MMTF_container_new( void );
MMTF_BioAssembly* MMTF_parser_MMTF_BioAssembly_new( void );
MMTF_Transform* MMTF_parser_MMTF_Transform_new( void );
MMTF_Entity* MMTF_parser_MMTF_Entity_new( void );
MMTF_GroupType* MMTF_parser_MMTF_GroupType_new( void );

//*** Initialize a struct
MMTF_container* MMTF_parser_MMTF_container_initialize( MMTF_container* );
MMTF_BioAssembly* MMTF_parser_MMTF_BioAssembly_initialize( MMTF_BioAssembly* );
MMTF_Transform* MMTF_parser_MMTF_Transform_initialize( MMTF_Transform* );
MMTF_Entity* MMTF_parser_MMTF_Entity_initialize( MMTF_Entity* );
MMTF_GroupType* MMTF_parser_MMTF_GroupType_initialize( MMTF_GroupType* );

//*** Empty a struct
MMTF_container* MMTF_parser_MMTF_container_empty( MMTF_container* );
MMTF_BioAssembly* MMTF_parser_MMTF_BioAssembly_empty( MMTF_BioAssembly* );
MMTF_Transform* MMTF_parser_MMTF_Transform_empty( MMTF_Transform* );
MMTF_Entity* MMTF_parser_MMTF_Entity_empty( MMTF_Entity* );
MMTF_GroupType* MMTF_parser_MMTF_GroupType_empty( MMTF_GroupType* );

//*** Destroy the innner of a struct
MMTF_container* MMTF_parser_MMTF_container_destroy_inside( MMTF_container* );
MMTF_BioAssembly* MMTF_parser_MMTF_BioAssembly_destroy_inside( MMTF_BioAssembly* );
MMTF_Transform* MMTF_parser_MMTF_Transform_destroy_inside( MMTF_Transform* );
MMTF_Entity* MMTF_parser_MMTF_Entity_destroy_inside( MMTF_Entity* );
MMTF_GroupType* MMTF_parser_MMTF_GroupType_destroy_inside( MMTF_GroupType* );

//*** Destroy a struct
void MMTF_parser_MMTF_container_destroy( MMTF_container* );
void MMTF_parser_MMTF_BioAssembly_destroy( MMTF_BioAssembly* );
void MMTF_parser_MMTF_Transform_destroy( MMTF_Transform* );
void MMTF_parser_MMTF_Entity_destroy( MMTF_Entity* );
void MMTF_parser_MMTF_GroupType_destroy( MMTF_GroupType* );


//*** Array converters
typedef union {
	char c[4];
	float f;
} MMTF_parser_four_bytes_as_float;

float* MMTF_parser_float_from_bytes( const char*, uint32_t, uint32_t* );

typedef union {
	char c;
	int8_t i;
} MMTF_parser_one_byte_as_int8;

int8_t* MMTF_parser_int8_from_bytes( const char*, uint32_t, uint32_t* );

typedef union {
	char c[2];
	int16_t i;
} MMTF_parser_two_bytes_as_int16;

int16_t* MMTF_parser_int16_from_bytes( const char*, uint32_t, uint32_t* );

typedef union {
	char c[4];
	int32_t i;
} MMTF_parser_four_bytes_as_int32;

int MMTF_parser_get_strategy(const char *);
int MMTF_parser_get_len(const char *);
int MMTF_parser_get_param(const char *);

int32_t* MMTF_parser_int32_from_bytes( const char*, const uint32_t, uint32_t* );
char** MMTF_parser_strings_from_bytes( const char*, uint32_t, uint32_t, uint32_t* );


//*** Array decoders
// Run-length decode
int32_t* MMTF_parser_run_length_decode( int32_t*, uint32_t, uint32_t*);

// Delta decode
int32_t* MMTF_parser_delta_decode( int32_t*, uint32_t, uint32_t* );

// Recursive indexing decode
int32_t* MMTF_parser_recursive_indexing_decode_from_16( int16_t*, uint32_t, uint32_t*);
int32_t* MMTF_parser_recursive_indexing_decode_from_8( int8_t*, uint32_t, uint32_t* );

// Integer decoding
float* MMTF_parser_integer_decode_from_16( int16_t*, uint32_t, int32_t, uint32_t* );
float* MMTF_parser_integer_decode_from_32( int32_t*, uint32_t, int32_t, uint32_t* );


//*** Applying a decoding strategy for getting an array
void* MMTF_parser_decode_apply_strategy( const char*, uint32_t, uint32_t*, int, int32_t );


//*** Unpacking from MsgPack and applying strategy
char* MMTF_parser_fetch_string( msgpack_object* );
uint64_t MMTF_parser_fetch_int( msgpack_object* );
float MMTF_parser_fetch_float( msgpack_object* );
void* MMTF_parser_fetch_array( msgpack_object*, uint64_t* );

size_t* MMTF_parser_fetch_clear_lu_array( msgpack_object*, uint64_t* );
int* MMTF_parser_fetch_clear_int_array( msgpack_object*, uint64_t* );
int32_t* MMTF_parser_fetch_clear_int32_array( msgpack_object*, uint64_t* );
char* MMTF_parser_fetch_clear_int8_array( msgpack_object*, uint64_t* );
float* MMTF_parser_fetch_clear_float_array( msgpack_object*, uint64_t* );
char** MMTF_parser_fetch_clear_string_array( msgpack_object*, uint64_t* );

bool MMTF_parser_compare_msgpack_string_char_array( const msgpack_object_str*, const char* );

MMTF_Entity* MMTF_parser_fetch_entityList( msgpack_object*, uint64_t* );
void MMTF_parser_put_entity( msgpack_object*, MMTF_Entity* );

MMTF_GroupType* MMTF_parser_fetch_groupList( msgpack_object*, uint64_t* );
void MMTF_parser_put_groupType( msgpack_object*, MMTF_GroupType* );

MMTF_BioAssembly* MMTF_parser_fetch_bioAssemblyList( msgpack_object*, uint64_t* );
void MMTF_parser_put_bioAssembly( msgpack_object*, MMTF_BioAssembly* );
MMTF_Transform* MMTF_parser_fetch_transformList( msgpack_object*, uint64_t* );
void MMTF_parser_put_transform( msgpack_object*, MMTF_Transform* );


//*** MMTF and MsgPack
void MMTF_parser_msgpack_object_to_MMTF_container( msgpack_object*, MMTF_container* );
void MMTF_parser_parse_msgpack( char*, int, MMTF_container* );


//*** Decode a MMTF container from a file
void MMTF_parser_MMTF_container_from_file( char*, MMTF_container* );

#endif
