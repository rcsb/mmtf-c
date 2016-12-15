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
    /** @brief The fields in this section hold group-related data     */
typedef struct {
    int32_t *			formalChargeList;           /**< formalChargeList List of formal charges as Integers */
    char **				WITHCOUNT(atomNameList);    /**< List of atom names, 0 to 5 character Strings*/
    char **				WITHCOUNT(elementList);     /**< List of elements, 0 to 3 character Strings */
    int32_t *			WITHCOUNT(bondAtomList);    /**< List of bonded atom indices, Integers */
    int8_t *			WITHCOUNT(bondOrderList);   /**< List of bond orders as Integers between 1 and 4 */
    char *				groupName;           /**< The name of the group, 0 to 5 characters */
    char				singleLetterCode;    /**< The single letter code, 1 character */
    char *				chemCompType;        /**< The chemical component type */
} MMTF_GroupType;

    /** @brief The fields in this section hold chain-related data */
typedef struct {
    int32_t *			WITHCOUNT(chainIndexList);   /**< Indices of fields in chainIdList and chainNameList fields */
    char *				description;    /**< Description of the entity */
    char *				type;           /**< Name of the entity type */
    char *				sequence;       /**< Sequence of the full construct in one-letter-code */
} MMTF_Entity;

typedef struct {
    int32_t *			WITHCOUNT(chainIndexList);  /**< Indices of fields in chainIdList and chainNameList fields */
    float				matrix[16];                 /**< 4X4 Transformation matrix  */
} MMTF_Transform;

typedef struct {
    MMTF_Transform*		WITHCOUNT(transformList);   /**< List of MMTF_transform objects */
    char*				name;                       /**< Name of the biological assembly */
} MMTF_BioAssembly;

    /** @brief MMTF container containing all data */

typedef struct {
    char *				mmtfVersion;    /**< The version of MMTF */
    char *				mmtfProducer;   /**< The producer of MMTF */
    float				unitCell[6];    /**< Array of 6 defining the unit cell: a, b, c, alpha, beta, gamma */
    char *				spaceGroup;     /**< the space group name */
    char *				structureId;    /**< An ID for the structure, for eg. the 4 letter PDB ID */
    char *				title;          /**< description of the structural data included in the file */
    char *				depositionDate; /**< Date of the deposition of the structure in a database in format YYY-MM-DD */
    char *				releaseDate;    /**< Date of release of the structure in the database in format YYY-MM-DD */
    MMTF_BioAssembly*	WITHCOUNT(bioAssemblyList); /**< List of MMTF_BioAssembly objects */
    MMTF_Entity *		WITHCOUNT(entityList);      /**< List of MMTF_Entity objects */
    char **				WITHCOUNT(experimentalMethods);
                            /**< experimentalMethods is a character array of unknown length so we
                             * need to specify the number of elements. String must be one of the
                             * following: \n
                             *  ELECTRON CRYSTALLOGRAPHY \n
                             *  ELECTRON MICROSCOPY \n
                             *  EPR \n
                             *  FIBER DIFFRACTION \n
                             *  FLUORESCENCE TRANSFER \n
                             *  INFRARED SPECTROSCOPY \n
                             *  NEUTRON DIFFRACTION \n
                             *  POWDER DIFFRACTION \n
                             *  SOLID-STATE NMR \n
                             *  SOLUTION NMR \n
                             *  SOLUTION SCATTERING \n
                             *  THEORETICAL MODEL \n
                             *  X-RAY DIFFRACTION
                             */
    size_t				numberOfExperimentalMethods; /**< The number of experimental methods for the structure */
    float				resolution; /**< Resolution of the structure */
    float				rFree;      /**< Rfree of the structure */
    float				rWork;      /**< Rwork of the structure */
    int32_t				numBonds;   /**< Total number of bonds in the structure */
    int32_t				numAtoms;   /**< Total number of atoms in the structure */
    int32_t				numGroups;  /**< Total number of groups in the structure */
    int32_t				numChains;  /**< Total number of chains in the structure */
    int32_t				numModels;  /**< Total number of models in the structrue */
    MMTF_GroupType *	WITHCOUNT(groupList);     /**< List of MMTF_GroupType objects */
    int32_t *			WITHCOUNT(bondAtomList);  /**< Pairs of values represent indices of covalently bonded atoms */
    int8_t *			WITHCOUNT(bondOrderList); /**< 8 bit signed interger */
    float *				xCoordList;     /**< List of x coordinates */
    float *				yCoordList;     /**< List of y coordinates */
    float *				zCoordList;     /**< List of z coordinates */
    float *				bFactorList;    /**< List of Bfactors */
    int32_t *           atomIdList;     /**< List of atom serial numbers */
    char *				altLocList;     /**< List of alternate locations */
    float *				occupancyList;  /**< List of occupancies */
    int32_t *			groupIdList;    /**< List of groups/residues */
    int32_t *			groupTypeList;
    int8_t *			secStructList;  /**< 8 bit signed interger */
    char *				insCodeList;    /**< List of insertion codes */
    int32_t *			sequenceIndexList;
    char **				WITHCOUNT(chainIdList);     /**< Array of 4-char strings */
    char **				WITHCOUNT(chainNameList);   /**< Array of 4-char strings */
    int32_t *			groupsPerChain; /**< List of total number of groups in each chain */
    int32_t *			chainsPerModel; /**< List of total number of chains in each model */
} MMTF_container;

/**
 * @brief Macros for declaration generic initialization and destroying functions
 */

#define CODEGEN_MMTF_parser_TYPE(type) \
    type * type ## _new(void); \
    void   type ## _init(type *); \
    void   type ## _destroy(type *); \
    void   type ## _clear(type *); \
    void   type ## _free(type *);
/**
 * @fn MMTF_container_free(MMTF_container* thing)
 * @param thing The MMTF_container to be freed
 * @return void
 */
CODEGEN_MMTF_parser_TYPE(MMTF_container)
CODEGEN_MMTF_parser_TYPE(MMTF_BioAssembly)
CODEGEN_MMTF_parser_TYPE(MMTF_Transform)
CODEGEN_MMTF_parser_TYPE(MMTF_Entity)
CODEGEN_MMTF_parser_TYPE(MMTF_GroupType)

#undef CODEGEN_MMTF_parser_TYPE


    /* MMTF and MsgPack */

    /**
     * @fn void MMTF_unpack_from_string(const char* buffer, size_t msgsize, MMTF_container* thing )
     * @brief Creates MMTF_container from string
     * @param[in] buffer
     * @param[in] msgsize
     * @param[out] thing
     * @return void
     */
void MMTF_unpack_from_string(const char* buffer, size_t msgsize, MMTF_container* thing );


    /**
     * @fn void MMTF_unpack_from_file(const char* name, MMTF_container* thing)
     * @brief Decode a MMTF_container from a file
     * @param[in] name Filename of the mmtf file which you want to read
     * @param[out] thing new/empty MMTF_container in  which the contents will be loaded by this function
     * @return void
     */
void MMTF_unpack_from_file(const char* , MMTF_container* );

#undef WITHCOUNT

#ifdef __cplusplus
}
#endif
#endif
