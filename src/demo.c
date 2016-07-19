#include <stdlib.h>
#include <msgpack.h>
#include "array_decoders.c"
#include "decoders.c"
#include "mmtf_structure.c"

msgpack_object object_by_key( msgpack_object *o, const char *name )
{
  msgpack_object_kv* p = o->via.map.ptr;
  msgpack_object_kv* const pend = o->via.map.ptr + o->via.map.size;
  int l = strlen(name);
  for(; p < pend; ++p) {
    if (p->key.type == MSGPACK_OBJECT_STR) {   
      if (l==p->key.via.str.size && !strncmp(name,p->key.via.str.ptr,p->key.via.str.size) )
        return p->val;
    }
  }
}


float* fetch_field_float_array(msgpack_object *object, char* name) {
    msgpack_object t_object = object_by_key(object,name);

    const char * bytes = t_object.via.bin.ptr;

	int parameter = get_param(bytes);
	int strategy = get_strategy(bytes);
	int length = get_len(bytes);

	printf( "%s: applying the strategy %i with parameter %i for decoding a byte array of length %i into a float array.\n", name, strategy, parameter, length );

    unsigned long out_len;
    float* result = decode_apply_strategy_into_float(bytes + 12, length - 1, &out_len, strategy, parameter);

    printf( "%s: structure decoded.\n", name );

	return result;
}

int8_t* fetch_field_int8_array(msgpack_object *object, char* name) {
    msgpack_object t_object = object_by_key(object,name);

    const char * bytes = t_object.via.bin.ptr;

	int strategy = get_strategy(bytes);
	int length = get_len(bytes);

	printf( "%s: applying the strategy %i for decoding a byte array of length %i into an int8 array.\n", name, strategy, length );

    unsigned long out_len;
    int8_t* result = decode_apply_strategy_into_int8(bytes + 12, length - 1, &out_len, strategy);

    printf( "%s: structure decoded.\n", name );

	return result;
}

int16_t* fetch_field_int16_array(msgpack_object *object, char* name) {
    msgpack_object t_object = object_by_key(object,name);

    const char * bytes = t_object.via.bin.ptr;

	int strategy = get_strategy(bytes);
	int length = get_len(bytes);

	printf( "%s: applying the strategy %i for decoding a byte array of length %i into an int16 array.\n", name, strategy, length );

    unsigned long out_len;
    int16_t* result = decode_apply_strategy_into_int16(bytes + 12, length - 1, &out_len, strategy);

    printf( "%s: structure decoded.\n", name );

	return result;
}

int32_t* fetch_field_int32_array(msgpack_object *object, char* name) {
    msgpack_object t_object = object_by_key(object,name);

    const char * bytes = t_object.via.bin.ptr;

	int strategy = get_strategy(bytes);
	int length = get_len(bytes);

	printf( "%s: applying the strategy %i for decoding a byte array of length %i into an int32 array.\n", name, strategy, length );

    unsigned long out_len;
    int32_t* result = decode_apply_strategy_into_int32(bytes + 12, length - 1, &out_len, strategy);

    printf( "%s: structure decoded.\n", name );

	return result;
}

char** fetch_field_string_array(msgpack_object *object, char* name) {
    msgpack_object t_object = object_by_key(object,name);

    const char * bytes = t_object.via.bin.ptr;

	int parameter = get_param(bytes);
	int strategy = get_strategy(bytes);
	int length = get_len(bytes);

	printf( "%s: applying the strategy %i with parameter %i for decoding a byte array of length %i into a string array.\n", name, strategy, parameter, length );

    unsigned long out_len;
    char** result = decode_apply_strategy_into_string_array(bytes + 12, length - 1, &out_len, strategy, parameter);

    printf( "%s: structure decoded.\n", name );

	return result;
}

char* fetch_field_string(msgpack_object *object, char* name) {
    msgpack_object t_object = object_by_key(object,name);

    const char * bytes = t_object.via.bin.ptr;

	int parameter = get_param(bytes);
	int strategy = get_strategy(bytes);
	int length = get_len(bytes);

	printf( "%s: decoding a byte array of length %i into a string.\n", name, length );

	char* result = malloc( sizeof(char) * length );
	strcpy(result, bytes + 12);

    printf( "%s: structure decoded.\n", name );

	return result;
}


void decode_struct(msgpack_object *object, mmtf_struct* thing) {
	(*thing).bFactorList = fetch_field_float_array( object, "bFactorList" );
}

void parse_msgpack(char *buffer,int msgsize, mmtf_struct* thing){
    msgpack_zone mempool;
    msgpack_zone_init(&mempool, 2048);
    msgpack_object *deserialized;
    msgpack_unpack(buffer, msgsize, NULL, &mempool, deserialized);
    decode_struct(deserialized, thing);
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

