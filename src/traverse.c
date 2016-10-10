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

#include "traverse.h"

static
char safechar(char c) {
  return (c < ' ') ? '.' : c;
}

int main(int argc, char** argv)
{
	MMTF_container* example = MMTF_parser_MMTF_container_new();
	MMTF_parser_MMTF_container_from_file(argv[1], example);
    //*** The following two lines are an example of re-using a MMTF_container and can be removed.
	MMTF_parser_MMTF_container_empty( example );
	MMTF_parser_MMTF_container_from_file(argv[1], example);
	// Now iterate over this data structure
    //	# initialize index counters
    int modelIndex = 0;
    int chainIndex = 0;
    int groupIndex = 0;
    int atomIndex = 0;
    //# traverse models
    int i;
for(i=0;  i<example->numModels; i++){
    int modelChainCount = example->chainsPerModel[i];
    printf("modelIndex: %d\n",modelIndex);
//    # traverse chains
	int j;
    for(j=0; j< modelChainCount; j++){
        printf("chainIndex : %d\n",chainIndex);
        printf("Chain id: %s\n",example->chainIdList[chainIndex]);
        printf("Chain name: %s\n",example->chainNameList[chainIndex]);
        int chainGroupCount = example->groupsPerChain[ chainIndex ];
//        # traverse groups
		int k;
        for(k=0; k<chainGroupCount;k++){
            printf("groupIndex: %d\n",groupIndex);
            printf("groupId: %d\n", example->groupIdList[ groupIndex ]);
            printf("insCodeList: %c\n", safechar(example->insCodeList[ groupIndex ]));
            printf("secStruc: %d\n",example->secStructList[ groupIndex ]);
            printf("seqIndex: %i\n",example->sequenceIndexList[ groupIndex ]);
            printf("groupType: %d\n",example->groupTypeList[ groupIndex ]);
            MMTF_GroupType group = example->groupList[ example->groupTypeList[ groupIndex ] ];
            printf("Group name: %s\n",group.groupName);
            printf("Single letter code: %c\n",group.singleLetterCode);
            printf("Chem comp type: %s\n",group.chemCompType);
            int atomOffset = atomIndex;

			int l;
            for(l=0; l<group.bondOrderListCount;l++){
            // ****** Issue here - > I get print outs of the same each time
                printf("Atom id One: %d\n",(atomOffset + group.bondAtomList[ l * 2 ])); //  # atomIndex1
                printf("Atom id Two: %d\n",(atomOffset + group.bondAtomList[ l * 2 + 1 ])); //  # atomIndex2
                printf("Bond order: %d\n", group.bondOrderList[ l ]);
            }
           int groupAtomCount = group.atomNameListCount;
           for(l=0; l<groupAtomCount;l++){
                printf("atomIndex: %d\n", atomIndex);
                printf("x coord: %f\n", example->xCoordList[ atomIndex ]);
                printf("y coord: %f\n", example->yCoordList[ atomIndex ]);
                printf("z coord: %f\n", example->zCoordList[ atomIndex ]);
                printf("b factor: %f\n", example->bFactorList[ atomIndex ]);
                printf("atom id: %d\n", example->atomIdList[ atomIndex ]);
                printf("altLocList: %c\n", safechar(example->altLocList[ atomIndex ]));
                printf("occupancy: %f\n", example->occupancyList[ atomIndex ]);
                printf("charge: %d\n", group.formalChargeList[ l ]);
                printf("atom name: %s\n", group.atomNameList[ l ]);
                printf("element: %s\n", group.elementList[ l ]);
                atomIndex++;
           }
            groupIndex++;
            }
        chainIndex++;
        }
   modelIndex++;
   }
printf("Number of inter group bonds: %d\n", (int)example->bondOrderListCount);
for (i=0; i<example->bondOrderListCount;i++){
//*** Issue here - seems too few (two entries for 4HHB).
    printf("Atom One: %d\n",example->bondAtomList[i*2]);
    printf("Atom Two: %d\n",example->bondAtomList[i*2+1]);
    printf("Bond order: %d\n",example->bondOrderList[i]);
}
	
	MMTF_parser_MMTF_container_destroy( example );
	return 0;
}

