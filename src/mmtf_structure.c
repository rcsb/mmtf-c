struct GroupType {
    int *   formalChargeList    // List of formal charges as Integers
    char ** atomNameList        // List of atom names, 0 to 5 character Strings
    char ** elementList Array   // List of elements, 0 to 3 character Strings
    int *   bondAtomList Array  // List of bonded atom indices, Integers
    char *  bondOrderList Array // List of bond orders as Integers between 1 and 4
    char *  groupName           // The name of the group, 0 to 5 characters
    char    singleLetterCode    // The single letter code, 1 character
    char *  chemCompType String // The chemical component type
}

struct Entity {
    size_t * chainIndexList;   // Indices of fields in chainIdList and chainNameList fields
    char *   description;      // Description of the entity
    char *   type;             // Name of the entity type
    char *   sequence          // Sequence of the full construct in one-letter-code
}

struct mmtf_struct {
    char *    mmtfVersion;
    char *    mmtfProducer;
    float[6]  unitCell;
    char *    spaceGroup;
    char *    structureId;
    char *    title;
    char *    depositionDate;
    char *    releaseDate;
    float[16] bioAssemblyList;
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
    int             numBonds;
    int             numAtoms;
    int             numGroups;
    int             numChains;
    int             numModels;
    GroupType *     groupList;
    signed int *    bondAtomList;
    unsigned char * bondOrderList; // 8 bit unsigned interger
    float *         xCoordList;
    float *         yCoordList;
    float *         zCoordList;
    float *         bFactorList;
    signed int *    atomIdList;
    char *          altLocList;
    float *         occupancyList;
    signed int *    groupIdList;
    signed int *    groupTypeList;
    signed char *   secStructList; // 8 bit unsigned interger
    char *          insCodeList;
    signed int *    sequenceIndexList;
    char **         chainIdList;    // Array of 4-char strings
    char **         chainNameList; // Array of 4-char strings
    unsigned int    groupsPerChain;
    unsigned int    chainsPerModel
};
