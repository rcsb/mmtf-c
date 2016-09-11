#include <stdlib.h>
#include <msgpack.h>
#include "array_decoders.c"
#include "decoders.c"
#include "mmtf_structure.c"

msgpack_object object_by_key( msgpack_object *o, const char *name, uint32_t* object_length )
{
  msgpack_object_kv* p = o->via.map.ptr;
  msgpack_object_kv* const pend = o->via.map.ptr + o->via.map.size;

  int l = strlen(name);
  for(; p < pend; ++p) {
    if (p->key.type == MSGPACK_OBJECT_STR) {   
      if (l==p->key.via.str.size && !strncmp(name,p->key.via.str.ptr,p->key.via.str.size) ) {
		*object_length = p->val.via.array.size;
        return p->val;
	  }
    }
  }
}

float* fetch_field_float_array(msgpack_object *object, char* name, uint32_t* length) {
	uint32_t object_length;

    msgpack_object t_object = object_by_key(object, name, &object_length);

    const char * bytes = t_object.via.bin.ptr;

	int parameter = get_param(bytes);
	int strategy = get_strategy(bytes);
	(*length) = get_len(bytes);

	printf( "%s: applying the strategy %i with parameter %i for decoding a byte array of length %u into a float array of length %u.\n", name, strategy, parameter, object_length - 12, *length );

    uint32_t out_len;
    float* result = decode_apply_strategy_into_float(bytes + 12, object_length - 12, &out_len, strategy, parameter);

	if( result != NULL ) {
	    printf( "%s: structure decoded into a float array of length %u.\n", name, out_len );
	}

	return result;
}

int8_t* fetch_field_int8_array(msgpack_object *object, char* name, uint32_t* length) {
	uint32_t object_length;

    msgpack_object t_object = object_by_key(object,name, &object_length);

    const char * bytes = t_object.via.bin.ptr;

	int strategy = get_strategy(bytes);
	(*length) = get_len(bytes);

	printf( "%s: applying the strategy %i for decoding a byte array of length %i into an int8 array of length %u.\n", name, strategy, object_length - 12, *length );

    uint32_t out_len;
    int8_t* result = decode_apply_strategy_into_int8(bytes + 12, object_length - 12, &out_len, strategy);

	if( result != NULL ) {
	    printf( "%s: structure decoded.\n", name );
	}

	return result;
}

int16_t* fetch_field_int16_array(msgpack_object *object, char* name, uint32_t* length) {
	uint32_t object_length;

    msgpack_object t_object = object_by_key(object,name, &object_length);

    const char * bytes = t_object.via.bin.ptr;

	int strategy = get_strategy(bytes);
	(*length) = get_len(bytes);

	printf( "%s: applying the strategy %i for decoding a byte array of length %i into an int16 array of length %u.\n", name, strategy, object_length - 12, *length );

    uint32_t out_len;
    int16_t* result = decode_apply_strategy_into_int16(bytes + 12, object_length - 12, &out_len, strategy);

	if( result != NULL ) {
	    printf( "%s: structure decoded.\n", name );
	}

	return result;
}

int32_t* fetch_field_int32_array(msgpack_object *object, char* name, uint32_t* length) {
	uint32_t object_length;

    msgpack_object t_object = object_by_key(object,name, &object_length);

    const char * bytes = t_object.via.bin.ptr;

	int strategy = get_strategy(bytes);
	(*length) = get_len(bytes);

	printf( "%s: applying the strategy %i for decoding a byte array of length %i into an int32 array of length %u.\n", name, strategy, object_length - 12, *length );

    uint32_t out_len;
    int32_t* result = decode_apply_strategy_into_int32(bytes + 12, object_length - 12, &out_len, strategy);

	if( result != NULL ) {
	    printf( "%s: structure decoded.\n", name );
	}

	return result;
}

char** fetch_field_string_array(msgpack_object *object, char* name, uint32_t* length) {
	uint32_t object_length;

    msgpack_object t_object = object_by_key(object,name, &object_length);

    const char * bytes = t_object.via.bin.ptr;

	int parameter = get_param(bytes);
	int strategy = get_strategy(bytes);
	(*length) = get_len(bytes);

	printf( "%s: applying the strategy %i with parameter %i for decoding a byte array of length %i into a string array of length %u.\n", name, strategy, parameter, object_length - 12, *length );

    uint32_t out_len;
    char** result = decode_apply_strategy_into_string_array(bytes + 12, object_length - 12, &out_len, strategy, parameter);

	if( result != NULL ) {
	    printf( "%s: structure decoded.\n", name );
	}

	return result;
}

char* fetch_field_string(msgpack_object *object, char* name, uint32_t* length) {
	uint32_t object_length;

    msgpack_object t_object = object_by_key(object,name, &object_length);

    const char * bytes = t_object.via.bin.ptr;

	int parameter = get_param(bytes);
	int strategy = get_strategy(bytes);
	(*length) = get_len(bytes);

	printf( "%s: decoding a byte array of length %i into a string of length %u.\n", name, object_length - 12, *length );

	char* result = malloc( sizeof(char) * (object_length - 12 + 1) );
	strcpy(result, bytes + 12);

	if( result != NULL ) {
	    printf( "%s: structure decoded.\n", name );
	}

	return result;
}


void decode_struct(msgpack_object *object, mmtf_struct* thing) {
	int length;

	(*thing).xCoordList = fetch_field_float_array( object, "xCoordList", &length );
	(*thing).yCoordList = fetch_field_float_array( object, "yCoordList", &length );
	(*thing).zCoordList = fetch_field_float_array( object, "zCoordList", &length );
	(*thing).bFactorList = fetch_field_float_array( object, "bFactorList", &length );

	(*thing).occupancyList = fetch_field_float_array( object, "occupancyList", &length );
}

void parse_msgpack(char *buffer,int msgsize, mmtf_struct* thing){
    msgpack_zone mempool;
    msgpack_zone_init(&mempool, 2048);
    msgpack_object deserialized;
	msgpack_unpack(buffer, msgsize, NULL, &mempool, &deserialized);

printf( "msgpack_object unpacked\n" );

    decode_struct(&deserialized, thing);

printf( "MMTF structure decoded\n" );

    msgpack_zone_destroy(&mempool);
}

void convert_MMTF_file_to_MMTF_struct(char *name, mmtf_struct* thing)
{
	FILE *file;
	char *buffer;
	unsigned long fileLen;
	//Open file
	file = fopen(name, "rb");
	if (!file)
	{
		fprintf(stderr, "Unable to open file %s.\n", name);
		return;
	}
	//Get file length
	fseek(file, 0, SEEK_END);
	fileLen=ftell(file);
	fseek(file, 0, SEEK_SET);

	fprintf(stdout, "file length = %lu\n", fileLen);

	//Allocate memory
	buffer=(char *)malloc(fileLen+1);
	if (!buffer)
	{
		fprintf(stderr, "Memory error!");
                                fclose(file);
		return;
	}
	//Read file contents into buffer
	fread(buffer, fileLen, 1, file);
	fclose(file);

	parse_msgpack(buffer, fileLen, thing);

	free(buffer);
}


int main(void)
{
	mmtf_struct thing;

	convert_MMTF_file_to_MMTF_struct("../data/4HHB.mmtf", &thing);
}

