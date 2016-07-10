#include <stdlib.h>
#include <msgpack.h>

#include "array_decoders.c"
#include "decoders.c"



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


void decode_struct(msgpack_object *object) {
    msgpack_object t_object = object_by_key(object,"bFactorList");
    const char * bytes = t_object.via.bin.ptr;
    printf("Param: %d\n",get_param(bytes));
    printf("Strategy: %d\n",get_strategy(bytes));
    printf("Len: %d\n",get_len(bytes));
}

void parse_msgpack(char *buffer,int msgsize){
    msgpack_zone mempool;
    msgpack_zone_init(&mempool, 2048);
    msgpack_object *deserialized;
    msgpack_unpack(buffer, msgsize, NULL, &mempool, deserialized);
    decode_struct(deserialized);

printf("test 1\n");

    msgpack_zone_destroy(&mempool);

printf("test 2\n");
}

void read_file(char *name)
{
	FILE *file;
	char *buffer;
	unsigned long fileLen;
	//Open file
	file = fopen(name, "rb");
	if (!file)
	{
		fprintf(stderr, "Unable to open file %s", name);
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
	parse_msgpack(buffer, fileLen);

printf("test 3\n");

	free(buffer);

printf("test 4\n");
}


int main(void)
{
//	read_file("../data/4HHB.mmtf");

	int32_t input[] = { 100, 100, 100, 100, 50, 50 };

	unsigned long output_length;
	float* output = integer_decode( input, 6, 100, &output_length );
	int i;
	for(i = 0; i < 6; ++i ) {
		printf( "%f\n", output[i] );
	}
	free( output );
}

