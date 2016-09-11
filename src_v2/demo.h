#ifndef DEMO_H
#define DEMO_H

#include "h_headers.h"

#include "mmtf_structure.h"

msgpack_object object_by_key(msgpack_object*, const char*, uint32_t*);

float* fetch_field_float_array(msgpack_object*, char*, uint32_t*);
int8_t* fetch_field_int8_array(msgpack_object*, char*, uint32_t*);
int16_t* fetch_field_int16_array(msgpack_object*, char*, uint32_t*);
int32_t* fetch_field_int32_array(msgpack_object*, char*, uint32_t*);
char** fetch_field_string_array(msgpack_object*, char*, uint32_t*);
char* fetch_field_string(msgpack_object*, char*, uint32_t*);

void decode_struct(msgpack_object*, mmtf_struct*);
void parse_msgpack(char*, int, mmtf_struct*);
void convert_MMTF_file_to_MMTF_struct(char*, mmtf_struct*);

int main( int, char** );

#endif
