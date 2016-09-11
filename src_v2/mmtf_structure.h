#ifndef MMTF_STRUCTURE_H
#define MMTF_STRUCTURE_H

typedef struct {
    int *   formalChargeList;    // List of formal charges as Integers
    char ** atomNameList;        // List of atom names, 0 to 5 character Strings
    char ** elementList;         // List of elements, 0 to 3 character Strings
    int *   bondAtomList;        // List of bonded atom indices, Integers
    char *  bondOrderList;       // List of bond orders as Integers between 1 and 4
    char *  groupName;           // The name of the group, 0 to 5 characters
    char    singleLetterCode;    // The single letter code, 1 character
    char *  chemCompType;         // The chemical component type
} GroupType;

typedef struct {
    size_t * chainIndexList;   // Indices of fields in chainIdList and chainNameList fields
    char *   description;      // Description of the entity
    char *   type;             // Name of the entity type
    char *   sequence;          // Sequence of the full construct in one-letter-code
} Entity;

typedef struct {
    char *    mmtfVersion;
    char *    mmtfProducer;
    float     unitCell[6];
    char *    spaceGroup;
    char *    structureId;
    char *    title;
    char *    depositionDate;
    char *    releaseDate;
    float     bioAssemblyList[16];
    Entity *  entityList;
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
    char **         experimentalMethods;
    size_t          numberOfExperimentalMethods;
    float           resolution;
    float           rFree;
    float           rWork;
    uint64_t        numBonds;
    uint64_t        numAtoms;
    uint64_t        numGroups;
    uint64_t        numChains;
    uint64_t        numModels;
    GroupType *     groupList;
    int32_t *       bondAtomList;
    int8_t *        bondOrderList; // 8 bit unsigned interger
    float *         xCoordList;
    float *         yCoordList;
    float *         zCoordList;
    float *         bFactorList;
    signed int *    atomIdList;
    char *          altLocList;
    float *         occupancyList;
    int32_t *       groupIdList;
    int32_t *       groupTypeList;
    int8_t *        secStructList; // 8 bit unsigned interger
    char *          insCodeList;
    int32_t *       sequenceIndexList;
    char **         chainIdList;    // Array of 4-char strings
    char **         chainNameList; // Array of 4-char strings
    int32_t *       groupsPerChain;
    int32_t *       chainsPerModel;
} mmtf_struct;

#endif
