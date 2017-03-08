//*******************************************************************************
//******	This code is published under the .... license
//******
//******
//******	This file is the code file for the sample code of the MMTF parser for the C language.
//******
//******	The authors of this code is Julien Ferté
//******
//******
//******
//******
//******
//******
//******
//*******************************************************************************

#include "mmtf_parser.h"

#include <stdio.h>
#include <string.h>

#define GET_ITEM_OPTIONAL(L, i, d) ((L) ? (L)[i] : (d))

/**
 * @brief If any value from \link MMTF_container::insCodeList insCodeList \endlink or
 * \link MMTF_container::altLocList altLocList \endlink is empty,
 * it would would replace the character to a dot
 * @param c character which needs to be checked
 * @return The c character if it is not empty otherwise a dot
 */
static
char safechar(char c) {
	return (c < ' ') ? '.' : c;
}

/**
 * @brief This is the main traversal function to read out all the contents of MMTF_container
 * @param example Any existing MMTF_container which you want to read
 * @return void
 * @bug
 */
static
void MMTF_traverse_main(MMTF_container* example) {
	//	# initialize index counters
	int modelIndex = 0;
	int chainIndex = 0;
	int groupIndex = 0;
	int atomIndex = 0;
	//# traverse models
	int i;
	for (i = 0; i < example->numModels; i++) {
		int modelChainCount = example->chainsPerModel[i];
		printf("modelIndex: %d\n", modelIndex);
		//    # traverse chains
		int j;
		for (j = 0; j < modelChainCount; j++) {
			printf("chainIndex : %d\n", chainIndex);
			printf("Chain id: %s\n", example->chainIdList[chainIndex]);
			printf("Chain name: %s\n", GET_ITEM_OPTIONAL(example->chainNameList, chainIndex, ""));
			int chainGroupCount = example->groupsPerChain[chainIndex];
			//        # traverse groups
			int k;
			for (k = 0; k < chainGroupCount; k++) {
				printf("groupIndex: %d\n", groupIndex);
				printf("groupId: %d\n", example->groupIdList[groupIndex]);
				printf("insCodeList: %c\n",
						safechar(GET_ITEM_OPTIONAL(example->insCodeList, groupIndex, 0)));
				printf("secStruc: %d\n", GET_ITEM_OPTIONAL(example->secStructList, groupIndex, -1));
				printf("seqIndex: %i\n", GET_ITEM_OPTIONAL(example->sequenceIndexList, groupIndex, -1));
				printf("groupType: %d\n", example->groupTypeList[groupIndex]);
				MMTF_GroupType group =
						example->groupList[example->groupTypeList[groupIndex]];
				printf("Group name: %s\n", group.groupName);
				printf("Single letter code: %c\n", group.singleLetterCode);
				printf("Chem comp type: %s\n", group.chemCompType);
				int atomOffset = atomIndex;

				int l;
				for (l = 0; l < group.bondOrderListCount; l++) {
					// ****** Issue here - > I get print outs of the same each time
					printf("Atom id One: %d\n",
							(atomOffset + group.bondAtomList[l * 2])); //  # atomIndex1
					printf("Atom id Two: %d\n",
							(atomOffset + group.bondAtomList[l * 2 + 1])); //  # atomIndex2
					printf("Bond order: %d\n", group.bondOrderList[l]);
				}
				int groupAtomCount = group.atomNameListCount;
				for (l = 0; l < groupAtomCount; l++) {
					printf("atomIndex: %d\n", atomIndex);
					printf("x coord: %f\n", example->xCoordList[atomIndex]);
					printf("y coord: %f\n", example->yCoordList[atomIndex]);
					printf("z coord: %f\n", example->zCoordList[atomIndex]);
					printf("b factor: %f\n", GET_ITEM_OPTIONAL(example->bFactorList, atomIndex, 0.0f));
					printf("atom id: %d\n", GET_ITEM_OPTIONAL(example->atomIdList, atomIndex, -1));
					printf("altLocList: %c\n",
							safechar(GET_ITEM_OPTIONAL(example->altLocList, atomIndex, 0)));
					printf("occupancy: %f\n", GET_ITEM_OPTIONAL(example->occupancyList, atomIndex, 1.0f));
					printf("charge: %d\n", group.formalChargeList[l]);
					printf("atom name: %s\n", group.atomNameList[l]);
					printf("element: %s\n", group.elementList[l]);
					atomIndex++;
				}
				groupIndex++;
			}
			chainIndex++;
		}
		modelIndex++;
	}
	printf("Number of inter group bonds: %d\n",
			(int) example->bondOrderListCount);
	for (i = 0; i < example->bondOrderListCount; i++) {
		//*** Issue here - seems too few (two entries for 4HHB).
		printf("Atom One: %d\n", example->bondAtomList[i * 2]);
		printf("Atom Two: %d\n", example->bondAtomList[i * 2 + 1]);
		printf("Bond order: %d\n", example->bondOrderList[i]);
	}
}


/**
 * @brief This function tells if the group atoms belong to HETATM
 * @param type The group of type \link MMTF_GroupType::chemCompType chemCompType \endlink
 * that needs to be checked.
 * @return 0 or 1
 */
static
int is_hetatm(char* type) {
    const char* hetatm_type[] = {
        "D-SACCHARIDE",
        "D-SACCHARIDE 1,4 AND 1,4 LINKING",
        "D-SACCHARIDE 1,4 AND 1,6 LINKING",
        "L-SACCHARIDE",
        "L-SACCHARIDE 1,4 AND 1,4 LINKING",
        "L-SACCHARIDE 1,4 AND 1,6 LINKING",
        "SACCHARIDE",
        "OTHER",
        "NON-POLYMER",
        0 };
    for( int i=0; hetatm_type[i]; ++i) {
        if (strcmp(type,hetatm_type[i])==0)
            return 1;
    }
    return 0;
}

/**
 * @brief This function reads out the contents of MMTF_container in a PDB-like fashion
 * Columns are in order : ATOM/HETATM AtomId Element AtomName AltLoc GroupId GroupType
 * InsCode ChainName x y z B-factor Occupancy Charge
 * @param example Any existing MMTF_container which you want to read
 * @return void
 */
static
void MMTF_traverse_pdb_like(MMTF_container* example) {


	int modelIndex = 0;
	int chainIndex = 0;
	int groupIndex = 0;
	int atomIndex = 0;

	//# traverse models
	for (int i = 0; i < example->numModels; i++, modelIndex++) {
		//    # traverse chains
		for (int j = 0; j < example->chainsPerModel[modelIndex]; j++, chainIndex++) {
			//        # traverse groups
			for (int k = 0; k < example->groupsPerChain[chainIndex]; k++, groupIndex++) {
				MMTF_GroupType group =
						example->groupList[example->groupTypeList[groupIndex]];
				int groupAtomCount = group.atomNameListCount;

				for (int l = 0; l < groupAtomCount; l++, atomIndex++) {
                    // ATOM or HETATM
                    if (is_hetatm(group.chemCompType))
                        printf("HETATM ");
                    else
                        printf("ATOM ");
					// Atom serial
					printf("%d ", GET_ITEM_OPTIONAL(example->atomIdList, atomIndex, -1));
					// Atom name
					printf("%s ", group.atomNameList[l]);
					// Alternate location
					printf("%c ", safechar(GET_ITEM_OPTIONAL(example->altLocList, atomIndex, 0)));
					// Group name
					printf("%s ", group.groupName);
					// Chain
					printf("%s ", GET_ITEM_OPTIONAL(example->chainNameList, chainIndex, ""));
					// Group serial
					printf("%d ", example->groupIdList[groupIndex]);
					// Insertion code
					printf("%c ", safechar(GET_ITEM_OPTIONAL(example->insCodeList, groupIndex, 0)));
					// x, y, z
					printf("%f ", example->xCoordList[atomIndex]);
					printf("%f ", example->yCoordList[atomIndex]);
					printf("%f ", example->zCoordList[atomIndex]);
					// B-factor
					printf("%f ", GET_ITEM_OPTIONAL(example->bFactorList, atomIndex, 0.0f));
					// Occupancy
					printf("%f ", GET_ITEM_OPTIONAL(example->occupancyList, atomIndex, 1.0f));
					// Element
					printf("%s ", group.elementList[l]);
					// Charge
					printf("%d \n", group.formalChargeList[l]);
				}
			}
		}
	}
}

int main(int argc, char** argv) {
	if (argc < 2) {
	  puts("useage: traverse <mmtffile>");
	  return 1;
	}

	MMTF_container* example = MMTF_container_new();
	MMTF_unpack_from_file(argv[1], example);

	if (argc > 2) {
	  MMTF_traverse_main(example);
	} else {
	  MMTF_traverse_pdb_like(example);
	}

	MMTF_container_free(example);
	return 0;
}
