#include "c_headers.h"

msgpack_object array_by_key( msgpack_object *o, const char *name, uint32_t* object_length )
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

msgpack_object string_by_key( msgpack_object *o, const char *name, uint32_t* object_length )
{
  msgpack_object_kv* p = o->via.map.ptr;
  msgpack_object_kv* const pend = o->via.map.ptr + o->via.map.size;

  int l = strlen(name);
  for(; p < pend; ++p) {
    if (p->key.type == MSGPACK_OBJECT_STR) {   
      if (l==p->key.via.str.size && !strncmp(name,p->key.via.str.ptr,p->key.via.str.size) ) {
		*object_length = p->val.via.str.size;
        return p->val;
	  }
    }
  }
}

msgpack_object int_by_key( msgpack_object *o, const char *name )
{
  msgpack_object_kv* p = o->via.map.ptr;
  msgpack_object_kv* const pend = o->via.map.ptr + o->via.map.size;

  int l = strlen(name);
  for(; p < pend; ++p) {
    if (p->key.type == MSGPACK_OBJECT_STR) {   
      if (l==p->key.via.str.size && !strncmp(name,p->key.via.str.ptr,p->key.via.str.size) ) {
        return p->val;
	  }
    }
  }
}

msgpack_object float_by_key( msgpack_object *o, const char *name )
{
  msgpack_object_kv* p = o->via.map.ptr;
  msgpack_object_kv* const pend = o->via.map.ptr + o->via.map.size;

  int l = strlen(name);
  for(; p < pend; ++p) {
    if (p->key.type == MSGPACK_OBJECT_STR) {   
      if (l==p->key.via.str.size && !strncmp(name,p->key.via.str.ptr,p->key.via.str.size) ) {
        return p->val;
	  }
    }
  }
}

float* fetch_field_float_array(msgpack_object *object, char* name, uint32_t* length) {
	uint32_t object_length;

    msgpack_object t_object = array_by_key(object, name, &object_length);

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

    msgpack_object t_object = array_by_key(object,name, &object_length);

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

    msgpack_object t_object = array_by_key(object,name, &object_length);

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

    msgpack_object t_object = array_by_key(object,name, &object_length);

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

    msgpack_object t_object = array_by_key(object,name, &object_length);

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

    msgpack_object t_object = string_by_key(object, name, &object_length);

    char * result = malloc( sizeof( char ) * (object_length + 1) );
	strncpy( result, t_object.via.str.ptr, object_length );
	result[object_length] = '\0';
	(*length) = object_length;

	printf( "%s: decoding a byte array of length %i into a string of length %u.\n", name, object_length, object_length );

	if( result != NULL ) {
	    printf( "%s: structure decoded.\n", name );
	}

	return result;
}

uint64_t fetch_field_int(msgpack_object *object, char* name) {
    msgpack_object t_object = int_by_key(object, name);

	uint64_t result;

    if(t_object.type == MSGPACK_OBJECT_POSITIVE_INTEGER) {
        result = t_object.via.u64;
    }
    if(t_object.type == MSGPACK_OBJECT_NEGATIVE_INTEGER) {
        result = t_object.via.i64;
	}

	printf( "%s: decoding into an unsigned int 64 : %lu.\n", name, result );

	printf( "%s: structure decoded.\n", name );

	return result;
}

uint64_t fetch_field_float(msgpack_object *object, char* name) {
    msgpack_object t_object = float_by_key(object, name);

	float result = t_object.via.f64;

	printf( "%s: decoding into a float 64 : %f.\n", name, result );

	printf( "%s: structure decoded.\n", name );

	return result;
}


void decode_struct(msgpack_object *object, mmtf_struct* thing) {
	int length;

	(*thing).mmtfVersion = fetch_field_string( object, "mmtfVersion", &length );
	(*thing).mmtfProducer = fetch_field_string( object, "mmtfProducer", &length );
	(*thing).title = fetch_field_string( object, "title", &length );
	(*thing).structureId = fetch_field_string( object, "structureId", &length );
	(*thing).depositionDate = fetch_field_string( object, "depositionDate", &length );
	(*thing).releaseDate = fetch_field_string( object, "releaseDate", &length );

	(*thing).numBonds = fetch_field_int( object, "numBonds" );
	(*thing).numAtoms = fetch_field_int( object, "numAtoms" );
	(*thing).numGroups = fetch_field_int( object, "numGroups" );
	(*thing).numChains = fetch_field_int( object, "numChains" );
	(*thing).numModels = fetch_field_int( object, "numModels" );

	(*thing).spaceGroup = fetch_field_string( object, "spaceGroup", &length );

/*
	float* unitCell = fetch_field_float_array( object, "unitCell", &length );
	int i;
	for( i = 0; i < 6; ++i ) {
		(*thing).unitCell[i] = unitCell[i];
	}

	(*thing).unitCell = fetch_field_float_array( object, "unitCell", &length );
	(*thing).bioAssemblyList = fetch_field_float_array( object, "bioAssemblyList", &length );

	(*thing).entityList = ...( object, "entityList", &length );
*/

	(*thing).resolution = fetch_field_float( object, "resolution" );
	(*thing).rFree = fetch_field_float( object, "rFree" );
	(*thing).rWork = fetch_field_float( object, "rWork" );

/*
	(*thing).experimentalMethods = fetch_field_string_array( object, "experimentalMethods", &length );
*/

	(*thing).bondAtomList = fetch_field_int32_array( object, "bondAtomList", &length );
	(*thing).bondOrderList = fetch_field_int8_array( object, "bondOrderList", &length );

	(*thing).chainIdList = fetch_field_string_array( object, "chainIdList", &length );
	(*thing).chainNameList = fetch_field_string_array( object, "chainNameList", &length );

/*
	(*thing).chainsPerModel = fetch_field_int32_array( object, "chainsPerModel", &length );
	(*thing).groupsPerChain = fetch_field_int32_array( object, "groupsPerChain", &length );

	(*thing).groupList = ...( object, "groupList", &length );
*/

	(*thing).groupTypeList = fetch_field_int32_array( object, "groupTypeList", &length );
	(*thing).groupIdList = fetch_field_int32_array( object, "groupIdList", &length );

	(*thing).secStructList = fetch_field_int8_array( object, "secStructList", &length );

	(*thing).insCodeList = fetch_field_string( object, "insCodeList", &length );

	(*thing).sequenceIndexList = fetch_field_int32_array( object, "sequenceIndexList", &length );
	(*thing).atomIdList = fetch_field_int32_array( object, "atomIdList", &length );

	(*thing).altLocList = fetch_field_string( object, "altLocList", &length );

	(*thing).bFactorList = fetch_field_float_array( object, "bFactorList", &length );

	(*thing).xCoordList = fetch_field_float_array( object, "xCoordList", &length );
	(*thing).yCoordList = fetch_field_float_array( object, "yCoordList", &length );
	(*thing).zCoordList = fetch_field_float_array( object, "zCoordList", &length );

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


int main(int argc, char** argv)
{
	mmtf_struct thing;

	convert_MMTF_file_to_MMTF_struct("./4HHB.mmtf", &thing);

// One must free everything from the MMTF structure thing
}

