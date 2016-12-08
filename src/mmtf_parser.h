//*******************************************************************************
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
//******	This file is the header file for the MMTF parser for the C language.
//******
//******	The authors of this code are: Julien Ferté (http://www.julienferte.com/),
//******	Anthony Bradley, Thomas Holder.
//******
//******
//******	Other contributors: Yana Valasatava, Alexander Rose.
//******
//*******************************************************************************


#ifndef MMTF_PARSER_H
#define MMTF_PARSER_H

//*** Standard libs
#include <stdlib.h>
#include <stdint.h>

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

#define CODEGEN_MMTF_parser_TYPE(type) \
    type * type ## _new(void); \
    void   type ## _init(type *); \
    void   type ## _destroy(type *); \
    void   type ## _clear(type *); \
    void   type ## _free(type *);

CODEGEN_MMTF_parser_TYPE(MMTF_container)
CODEGEN_MMTF_parser_TYPE(MMTF_BioAssembly)
CODEGEN_MMTF_parser_TYPE(MMTF_Transform)
CODEGEN_MMTF_parser_TYPE(MMTF_Entity)
CODEGEN_MMTF_parser_TYPE(MMTF_GroupType)

#undef CODEGEN_MMTF_parser_TYPE

//*** MMTF and MsgPack
void MMTF_unpack_from_string(const char*, size_t, MMTF_container* );


//*** Decode a MMTF container from a file
void MMTF_unpack_from_file(const char*, MMTF_container* );

#undef WITHCOUNT

#ifdef __cplusplus
}
#endif
#endif
