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
//******	Anthony Bradley, Alexandr and Yana Valasatava.
//******	
//************************************************************************************************


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

#ifdef __cplusplus
extern "C" {
#endif

#define WITHCOUNT(name) name; size_t name ## Count

//*** The MMTF structure
typedef struct {
    int32_t *				formalChargeList;    // List of formal charges as Integers
    char **				WITHCOUNT(atomNameList);        // List of atom names, 0 to 5 character Strings
    char **				WITHCOUNT(elementList);         // List of elements, 0 to 3 character Strings
    int32_t *			WITHCOUNT(bondAtomList);        // List of bonded atom indices, Integers
    int8_t *			WITHCOUNT(bondOrderList);       // List of bond orders as Integers between 1 and 4
    char *				groupName;           // The name of the group, 0 to 5 characters
    char				singleLetterCode;    // The single letter code, 1 character
    char *				chemCompType;         // The chemical component type
} MMTF_GroupType;

typedef struct {
    int32_t *			WITHCOUNT(chainIndexList);   // Indices of fields in chainIdList and chainNameList fields
    char *				description;      // Description of the entity
    char *				type;             // Name of the entity type
    char *				sequence;          // Sequence of the full construct in one-letter-code
} MMTF_Entity;

typedef struct {
	int32_t *			WITHCOUNT(chainIndexList);
	float				matrix[16];
} MMTF_Transform;

typedef struct {
	MMTF_Transform*		WITHCOUNT(transformList);
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
	MMTF_BioAssembly*	WITHCOUNT(bioAssemblyList);
    MMTF_Entity *		WITHCOUNT(entityList);
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
    char **				WITHCOUNT(experimentalMethods);
    size_t				numberOfExperimentalMethods;
    float				resolution;
    float				rFree;
    float				rWork;
    int32_t				numBonds;
    int32_t				numAtoms;
    int32_t				numGroups;
    int32_t				numChains;
    int32_t				numModels;
    MMTF_GroupType *	WITHCOUNT(groupList);
    int32_t *			WITHCOUNT(bondAtomList);
    int8_t *			WITHCOUNT(bondOrderList); // 8 bit signed interger
    float *				xCoordList;
    float *				yCoordList;
    float *				zCoordList;
    float *				bFactorList;
    int32_t *		atomIdList;
    char *				altLocList;
    float *				occupancyList;
    int32_t *			groupIdList;
    int32_t *			groupTypeList;
    int8_t *			secStructList; // 8 bit signed interger
    char *				insCodeList;
    int32_t *			sequenceIndexList;
    char **				WITHCOUNT(chainIdList);    // Array of 4-char strings
    char **				WITHCOUNT(chainNameList); // Array of 4-char strings
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
int32_t* MMTF_parser_run_length_decode(const int32_t*, uint32_t, uint32_t*);

// Delta decode
int32_t* MMTF_parser_delta_decode(const int32_t*, uint32_t, uint32_t* );

// Recursive indexing decode
int32_t* MMTF_parser_recursive_indexing_decode_from_16(const int16_t*, uint32_t, uint32_t*);
int32_t* MMTF_parser_recursive_indexing_decode_from_8(const int8_t*, uint32_t, uint32_t* );

// Integer decoding
float* MMTF_parser_integer_decode_from_16(const int16_t*, uint32_t, int32_t, uint32_t* );
float* MMTF_parser_integer_decode_from_32(const int32_t*, uint32_t, int32_t, uint32_t* );

//*** Unpacking from MsgPack and applying strategy
char* MMTF_parser_fetch_string( const msgpack_object* );
int64_t MMTF_parser_fetch_int( const msgpack_object* );
float MMTF_parser_fetch_float( const msgpack_object* );

bool MMTF_parser_compare_msgpack_string_char_array( const msgpack_object_str*, const char* );

MMTF_Entity* MMTF_parser_fetch_entityList( const msgpack_object*, size_t* );

MMTF_GroupType* MMTF_parser_fetch_groupTypeList( const msgpack_object*, size_t* );

MMTF_BioAssembly* MMTF_parser_fetch_bioAssemblyList( const msgpack_object*, size_t* );
MMTF_Transform* MMTF_parser_fetch_transformList( const msgpack_object*, size_t* );


//*** MMTF and MsgPack
void MMTF_parser_msgpack_object_to_MMTF_container( const msgpack_object*, MMTF_container* );
void MMTF_parser_parse_msgpack(const char*, int, MMTF_container* );


//*** Decode a MMTF container from a file
void MMTF_parser_MMTF_container_from_file(const char*, MMTF_container* );

#undef WITHCOUNT

#ifdef __cplusplus
}
#endif
#endif
