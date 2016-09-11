#ifndef DEMO_H
#define DEMO_H

#include "h_headers.h"

#include "mmtf_structure.h"

char* fetch_string( msgpack_object* );
uint64_t fetch_int( msgpack_object* );
float fetch_float( msgpack_object* );
void* fetch_array( msgpack_object*, uint64_t* );

size_t* fetch_clear_lu_array( msgpack_object*, uint64_t* );
int* fetch_clear_int_array( msgpack_object*, uint64_t* );
int32_t* fetch_clear_int32_array( msgpack_object*, uint64_t* );
char* fetch_clear_int8_array( msgpack_object*, uint64_t* );
float* fetch_clear_float_array( msgpack_object*, uint64_t* );
char** fetch_clear_string_array( msgpack_object*, uint64_t* );

Entity* fetch_entityList( msgpack_object*, uint64_t* );
void put_entity( msgpack_object*, Entity* );

GroupType* fetch_groupList( msgpack_object*, uint64_t* );
void put_groupType( msgpack_object*, GroupType* );

BioAssembly* fetch_bioAssemblyList( msgpack_object*, uint64_t* );
void put_bioAssembly( msgpack_object*, BioAssembly* );
Transform* fetch_transformList( msgpack_object*, uint64_t* );
void put_transform( msgpack_object*, Transform* );

void decode_struct( msgpack_object*, mmtf_struct* );

void decode_struct( msgpack_object*, mmtf_struct* );
void parse_msgpack( char*, int, mmtf_struct* );
void convert_MMTF_file_to_MMTF_struct( char*, mmtf_struct* );

int main( int, char** );

#endif
