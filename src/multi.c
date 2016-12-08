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

void read_file(const char* input_path){
        MMTF_container* example = MMTF_container_new();
        MMTF_unpack_from_file(input_path, example);
        MMTF_container_free(example);
        printf("%s read\n", input_path);
}

int main(int argc, char** argv)
{
    read_file("data/173D.mmtf");
    read_file("data/1AA6.mmtf");
    read_file("data/1BNA.mmtf");
    read_file("data/1CAG.mmtf");
    read_file("data/1HTQ.mmtf");
    read_file("data/1IGT.mmtf");
    read_file("data/1L2Q.mmtf");
    read_file("data/1LPV.mmtf");
    read_file("data/1MSH.mmtf");
    read_file("data/1O2F.mmtf");
    read_file("data/1R9V.mmtf");
    read_file("data/1SKM.mmtf");
    read_file("data/3NJW.mmtf");
    read_file("data/3ZYB.mmtf");
    read_file("data/4CK4.mmtf");
    read_file("data/4CUP.mmtf");
    read_file("data/4HHB.mmtf");
    read_file("data/4OPJ.mmtf");
    read_file("data/4P3R.mmtf");
    read_file("data/4V5A.mmtf");
    read_file("data/4Y60.mmtf");
    read_file("data/5EMG.mmtf");
    read_file("data/5ESW.mmtf");
    return 0;
}

