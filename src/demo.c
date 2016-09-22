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

#include "demo.h"

int main(int argc, char** argv)
{
	MMTF_container* example = MMTF_parser_MMTF_container_new();
	MMTF_parser_MMTF_container_from_file(argv[1], example);
	MMTF_parser_MMTF_container_destroy( example );
}

