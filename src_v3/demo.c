#include "c_headers.h"

char* fetch_string( msgpack_object* object ) {
	if( object->type != MSGPACK_OBJECT_STR ) {
		fprintf( stderr, "%sError in fetch_string: the entry encoded in the MMTF is not a string.\n%s", KRED, KNRM );
		return NULL;
	}

	msgpack_object_str* value_string = &(object->via.str);

	char* result = malloc( sizeof( char ) * ( value_string->size + 1 ) );
	strncpy( result, value_string->ptr, value_string->size );
	result[value_string->size] = '\0';

	return result;
}

char fetch_char( msgpack_object* object ) {
	if( object->type != MSGPACK_OBJECT_STR) {
		fprintf( stderr, "%sError in fetch_string: the entry encoded in the MMTF is not a string.\n%s", KRED, KNRM );
		return '\0';
	}

	return *(object->via.str.ptr);
}

uint64_t fetch_int( msgpack_object* object ) {
	uint64_t result;

    if(object->type == MSGPACK_OBJECT_POSITIVE_INTEGER) {
        result = object->via.u64;
    }
    else if(object->type == MSGPACK_OBJECT_NEGATIVE_INTEGER) {
        result = object->via.i64;
	}
	else {
		fprintf( stderr, "%sError in fetch_int: the entry encoded in the MMTF is not an integer.\n%s", KRED, KNRM );
		return NAN;
	}

	return result;
}

float fetch_float( msgpack_object* object ) {
	if( object->type != MSGPACK_OBJECT_FLOAT ) {
		fprintf( stderr, "%sError in fetch_float: the entry encoded in the MMTF is not a float.\n%s", KRED, KNRM );
		return NAN;
	}
	
	return (float) object->via.f64;
}

void* fetch_array( msgpack_object* object, uint64_t* length ) {
	if( object->type != MSGPACK_OBJECT_BIN ) {
		fprintf( stderr, "%sError in fetch_array: the entry encoded in the MMTF is not binary data.\n%s", KRED, KNRM );
		return NULL;
	}

	const char* bytes = object->via.bin.ptr;

	int parameter = get_param(bytes);
	int strategy = get_strategy(bytes);
	(*length) = get_len(bytes);

	printf( "Applying the strategy %i with parameter %i for decoding a byte array of length %i into an int32 array of length %lu.\n", strategy, parameter, object->via.bin.size - 12, *length );

    uint32_t out_length;
    return decode_apply_strategy(bytes + 12, object->via.bin.size - 12, &out_length, strategy, parameter);
}

size_t* fetch_clear_lu_array( msgpack_object* object, uint64_t* length ) {
	if( object->type != MSGPACK_OBJECT_ARRAY ) {
		fprintf( stderr, "%sError in fetch_clear_array: the entry encoded in the MMTF is not an array.\n%s", KRED, KNRM );
		return NULL;
	}

	msgpack_object* current_value = object->via.array.ptr;
	(*length) = object->via.array.size;
	msgpack_object* last_value = current_value + (*length);

	printf( "Unpacking an unsigned long int array of length %lu.\n", *length );

	size_t* result = malloc( sizeof( size_t ) * (*length) );

	int i = 0;
	for( ; current_value < last_value; ++current_value ) {
		result[i] = current_value->via.u64;
		++i;
	}

    return result;
}

int* fetch_clear_int_array( msgpack_object* object, uint64_t* length ) {
	if( object->type != MSGPACK_OBJECT_ARRAY ) {
		fprintf( stderr, "%sError in fetch_clear_array: the entry encoded in the MMTF is not an array.\n%s", KRED, KNRM );
		return NULL;
	}

	msgpack_object* current_value = object->via.array.ptr;
	(*length) = object->via.array.size;
	msgpack_object* last_value = current_value + (*length);

	printf( "Unpacking an int array of length %lu.\n", *length );

	int* result = malloc( sizeof( int ) * (*length) );

	int i = 0;
	for( ; current_value < last_value; ++current_value ) {
		result[i] = (int) current_value->via.u64;
		++i;
	}

    return result;
}

int32_t* fetch_clear_int32_array( msgpack_object* object, uint64_t* length ) {
	if( object->type != MSGPACK_OBJECT_ARRAY ) {
		fprintf( stderr, "%sError in fetch_clear_array: the entry encoded in the MMTF is not an array.\n%s", KRED, KNRM );
		return NULL;
	}

	msgpack_object* current_value = object->via.array.ptr;
	(*length) = object->via.array.size;
	msgpack_object* last_value = current_value + (*length);

	printf( "Unpacking an int32 array of length %lu.\n", *length );

	int32_t* result = malloc( sizeof( int32_t ) * (*length) );

	int i = 0;
	for( ; current_value < last_value; ++current_value ) {
		result[i] = (int32_t) current_value->via.u64;
		++i;
	}

    return result;
}

char* fetch_clear_int8_array( msgpack_object* object, uint64_t* length ) {
	if( object->type != MSGPACK_OBJECT_ARRAY ) {
		fprintf( stderr, "%sError in fetch_clear_array: the entry encoded in the MMTF is not an array.\n%s", KRED, KNRM );
		return NULL;
	}

	msgpack_object* current_value = object->via.array.ptr;
	(*length) = object->via.array.size;
	msgpack_object* last_value = current_value + (*length);

	printf( "Unpacking an int8 array of length %lu.\n", *length );

	char* result = malloc( sizeof( char ) * (*length) );

	int i = 0;
	for( ; current_value < last_value; ++current_value ) {
		result[i] = (char) current_value->via.u64;
		++i;
	}

    return result;
}

float* fetch_clear_float_array( msgpack_object* object, uint64_t* length ) {
	if( object->type != MSGPACK_OBJECT_ARRAY ) {
		fprintf( stderr, "%sError in fetch_clear_array: the entry encoded in the MMTF is not an array.\n%s", KRED, KNRM );
		return NULL;
	}

	msgpack_object* current_value = object->via.array.ptr;
	(*length) = object->via.array.size;
	msgpack_object* last_value = current_value + (*length);

	printf( "Unpacking a float array of length %lu.\n", *length );

	float* result = malloc( sizeof( float ) * (*length) );

	int i = 0;
	for( ; current_value < last_value; ++current_value ) {
		result[i] = (float) current_value->via.f64;
		++i;
	}

    return result;
}

char** fetch_clear_string_array( msgpack_object* object, uint64_t* length ) {
	if( object->type != MSGPACK_OBJECT_ARRAY ) {
		fprintf( stderr, "%sError in fetch_clear_array: the entry encoded in the MMTF is not an array.\n%s", KRED, KNRM );
		return NULL;
	}

	msgpack_object* current_value = object->via.array.ptr;
	(*length) = object->via.array.size;
	msgpack_object* last_value = current_value + (*length);

	printf( "Unpacking a string array of length %lu.\n", *length );

	char** result = malloc( sizeof( char* ) * (*length) );

	int i = 0;
	uint32_t string_size;
	for( ; current_value < last_value; ++current_value ) {
		string_size = current_value->via.str.size;
		result[i] = malloc( sizeof( char ) * ( string_size + 1 ) );
		strncpy( result[i], current_value->via.str.ptr, string_size );
		result[i][string_size] = '\0';
		++i;
	}

    return result;
}

Entity* fetch_entityList( msgpack_object* object, uint64_t* length ) {
	if( object->type != MSGPACK_OBJECT_ARRAY ) {
		fprintf( stderr, "%sError in fetch_entityList: the entry encoded in the MMTF is not an array.\n%s", KRED, KNRM );
		return NULL;
	}

	msgpack_object* current_entity = object->via.array.ptr;
	uint32_t entityList_count = object->via.array.size;
	msgpack_object* stop_entity = object->via.array.ptr + entityList_count;

	printf( "Decoding an array of %u Entity.\n", entityList_count );

	Entity* result = malloc( sizeof( Entity ) * entityList_count );

	int i = 0;
	for( ; current_entity < stop_entity; ++current_entity ) {
		put_entity( current_entity, &(result[i]) );
		++i;
	}

    return result;
}

void put_entity( msgpack_object* object, Entity* entity ) {
	if( object->type != MSGPACK_OBJECT_MAP ) {
		fprintf( stderr, "%sError in put_entity: the entry encoded in the MMTF is not a map.\n%s", KRED, KNRM );
		return;
	}

	msgpack_object_kv* current_key_value = object->via.map.ptr;
	msgpack_object_kv* last_key_value = current_key_value + object->via.map.size;

	for( ; current_key_value <= last_key_value; ++current_key_value ) {
		msgpack_object* key = &(current_key_value->key);
		msgpack_object* value = &(current_key_value->val);

		if( key->type == MSGPACK_OBJECT_STR ) {

printf( "Key %.*s:\n", key->via.str.size, key->via.str.ptr );

			uint32_t key_size = key->via.str.size;
			const char* key_string = key->via.str.ptr;

			if( key_size == strlen( "description" ) && strncmp( key_string, "description", key_size ) == 0 ) {
				(*entity).description = fetch_string( value );

				printf( "description : %s\n\n", (*entity).description );
			}

			if( key_size == strlen( "type" ) && strncmp( key_string, "type", key_size ) == 0 ) {
				(*entity).type = fetch_string( value );

				printf( "type : %s\n\n", (*entity).type );
			}

			if( key_size == strlen( "chainIndexList" ) && strncmp( key_string, "chainIndexList", key_size ) == 0 ) {
				uint64_t length;
				(*entity).chainIndexList = fetch_clear_lu_array( value, &length );
			}

			if( key_size == strlen( "sequence" ) && strncmp( key_string, "sequence", key_size ) == 0 ) {
				(*entity).sequence = fetch_string( value );

				printf( "sequence : %s\n\n", (*entity).sequence );
			}
		}
	}
}

GroupType* fetch_groupList( msgpack_object* object, uint64_t* length ) {
	if( object->type != MSGPACK_OBJECT_ARRAY ) {
		fprintf( stderr, "%sError in fetch_groupList: the entry encoded in the MMTF is not an array.\n%s", KRED, KNRM );
		return NULL;
	}

	msgpack_object* current_groupType = object->via.array.ptr;
	uint32_t groupList_count = object->via.array.size;
	msgpack_object* stop_groupType = object->via.array.ptr + groupList_count;

	printf( "Decoding an array of %u GroupType.\n", groupList_count );

	GroupType* result = malloc( sizeof( GroupType ) * groupList_count );

	int i = 0;
	for( ; current_groupType < stop_groupType; ++current_groupType ) {
		put_groupType( current_groupType, &(result[i]) );
		++i;
	}

    return result;
}

void put_groupType( msgpack_object* object, GroupType* group_type ) {
	if( object->type != MSGPACK_OBJECT_MAP ) {
		fprintf( stderr, "%sError in put_groupType: the entry encoded in the MMTF is not a map.\n%s", KRED, KNRM );
		return;
	}

	msgpack_object_kv* current_key_value = object->via.map.ptr;
	msgpack_object_kv* last_key_value = current_key_value + object->via.map.size;

	for( ; current_key_value <= last_key_value; ++current_key_value ) {
		msgpack_object* key = &(current_key_value->key);
		msgpack_object* value = &(current_key_value->val);

		if( key->type == MSGPACK_OBJECT_STR ) {

printf( "Key %.*s:\n", key->via.str.size, key->via.str.ptr );

			uint32_t key_size = key->via.str.size;
			const char* key_string = key->via.str.ptr;

			if( key_size == strlen( "formalChargeList" ) && strncmp( key_string, "formalChargeList", key_size ) == 0 ) {
				uint64_t length;
				(*group_type).formalChargeList = fetch_clear_int_array( value, &length );
			}

			if( key_size == strlen( "atomNameList" ) && strncmp( key_string, "atomNameList", key_size ) == 0 ) {
				uint64_t length;
				(*group_type).atomNameList = fetch_clear_string_array( value, &length );
			}

			if( key_size == strlen( "elementList" ) && strncmp( key_string, "elementList", key_size ) == 0 ) {
				uint64_t length;
				(*group_type).elementList = fetch_clear_string_array( value, &length );
			}

			if( key_size == strlen( "bondAtomList" ) && strncmp( key_string, "bondAtomList", key_size ) == 0 ) {
				uint64_t length;
				(*group_type).bondAtomList = fetch_clear_int_array( value, &length );
			}

			if( key_size == strlen( "bondOrderList" ) && strncmp( key_string, "bondOrderList", key_size ) == 0 ) {
				uint64_t length;
				(*group_type).bondOrderList = fetch_clear_int8_array( value, &length );
			}

			if( key_size == strlen( "groupName" ) && strncmp( key_string, "groupName", key_size ) == 0 ) {
				(*group_type).groupName = fetch_string( value );

				printf( "groupName : %s\n\n", (*group_type).groupName );
			}

			if( key_size == strlen( "singleLetterCode" ) && strncmp( key_string, "singleLetterCode", key_size ) == 0 ) {
				(*group_type).singleLetterCode = fetch_char( value );

				printf( "singleLetterCode : %c\n\n", (*group_type).singleLetterCode );
			}

			if( key_size == strlen( "chemCompType" ) && strncmp( key_string, "chemCompType", key_size ) == 0 ) {
				(*group_type).chemCompType = fetch_string( value );

				printf( "chemCompType : %s\n\n", (*group_type).chemCompType );
			}
		}
	}
}

BioAssembly* fetch_bioAssemblyList( msgpack_object* object, uint64_t* length ) {
	if( object->type != MSGPACK_OBJECT_ARRAY ) {
		fprintf( stderr, "%sError in fetch_bioAssemblyList: the entry encoded in the MMTF is not an array.\n%s", KRED, KNRM );
		return NULL;
	}

	msgpack_object* current_bioAssembly = object->via.array.ptr;
	uint32_t bioAssembly_count = object->via.array.size;
	msgpack_object* stop_bioAssembly = object->via.array.ptr + bioAssembly_count;

	printf( "Decoding an array of %u BioAssembly.\n", bioAssembly_count );

	BioAssembly* result = malloc( sizeof( BioAssembly ) * bioAssembly_count );

	int i = 0;
	for( ; current_bioAssembly < stop_bioAssembly; ++current_bioAssembly ) {
		put_bioAssembly( current_bioAssembly, &(result[i]) );
		++i;
	}

    return result;
}

void put_bioAssembly( msgpack_object* object, BioAssembly* bio_assembly ) {
	if( object->type != MSGPACK_OBJECT_MAP ) {
		fprintf( stderr, "%sError in put_bioAssembly: the entry encoded in the MMTF is not a map.\n%s", KRED, KNRM );
		return;
	}

	msgpack_object_kv* current_key_value = object->via.map.ptr;
	msgpack_object_kv* last_key_value = current_key_value + object->via.map.size;

	for( ; current_key_value <= last_key_value; ++current_key_value ) {
		msgpack_object* key = &(current_key_value->key);
		msgpack_object* value = &(current_key_value->val);

		if( key->type == MSGPACK_OBJECT_STR ) {

printf( "Key %.*s:\n", key->via.str.size, key->via.str.ptr );

			uint32_t key_size = key->via.str.size;
			const char* key_string = key->via.str.ptr;

			if( key_size == strlen( "name" ) && strncmp( key_string, "name", key_size ) == 0 ) {
				(*bio_assembly).name = fetch_string( value );

				printf( "name : %s\n", (*bio_assembly).name );
			}

			if( key_size == strlen( "transformList" ) && strncmp( key_string, "transformList", key_size ) == 0 ) {
				uint64_t length;
				(*bio_assembly).transformList = fetch_transformList( value, &length );
			}
		}
	}
}

Transform* fetch_transformList( msgpack_object* object, uint64_t* length ) {
	if( object->type != MSGPACK_OBJECT_ARRAY ) {
		fprintf( stderr, "%sError in fetch_transformList: the entry encoded in the MMTF is not an array.\n%s", KRED, KNRM );
		return NULL;
	}

	msgpack_object* current_transform = object->via.array.ptr;
	uint32_t transform_count = object->via.array.size;
	msgpack_object* stop_transform = object->via.array.ptr + transform_count;

	printf( "Decoding an array of %u Transform.\n", transform_count );

	Transform* result = malloc( sizeof( Transform ) * transform_count );

	int i = 0;
	for( ; current_transform < stop_transform; ++current_transform ) {
		put_transform( current_transform, &(result[i]) );
		++i;
	}

    return result;
}

void put_transform( msgpack_object* object, Transform* transform ) {
	if( object->type != MSGPACK_OBJECT_MAP ) {
		fprintf( stderr, "%sError in put_transform: the entry encoded in the MMTF is not a map.\n%s", KRED, KNRM );
		return;
	}

	msgpack_object_kv* current_key_value = object->via.map.ptr;
	msgpack_object_kv* last_key_value = current_key_value + object->via.map.size;

	for( ; current_key_value <= last_key_value; ++current_key_value ) {
		msgpack_object* key = &(current_key_value->key);
		msgpack_object* value = &(current_key_value->val);

		if( key->type == MSGPACK_OBJECT_STR ) {

printf( "Key %.*s:\n", key->via.str.size, key->via.str.ptr );

			uint32_t key_size = key->via.str.size;
			const char* key_string = key->via.str.ptr;

			if( key_size == strlen( "chainIndexList" ) && strncmp( key_string, "chainIndexList", key_size ) == 0 ) {
				uint64_t length;
				(*transform).chainIndexList = fetch_clear_lu_array( value, &length );
			}

			if( key_size == strlen( "matrix" ) && strncmp( key_string, "matrix", key_size ) == 0 ) {
				uint64_t length;
				float* matrix = fetch_clear_float_array( value, &length );

				if( length != 16 ) {
					fprintf( stderr, "%sError in put_transform: the length of the matrix entry is %lu, it should have a length of 16.\n%s", KRED, length, KNRM );
					return;
				}

				int i;
				for( i = 0; i < 16; ++i ) {
					(*transform).matrix[i] = matrix[i];

printf( "%i = %f\n", i, matrix[i] );

				}
			}
		}
	}
}

void decode_struct(msgpack_object* object, mmtf_struct* thing) {
	msgpack_object_kv* current_key_value = object->via.map.ptr;
	msgpack_object_kv* last_key_value = current_key_value + object->via.map.size;

	for( ; current_key_value <= last_key_value; ++current_key_value ) {
		msgpack_object* key = &(current_key_value->key);
		msgpack_object* value = &(current_key_value->val);

		if( key->type == MSGPACK_OBJECT_STR ) {

printf( "Key %.*s:\n", key->via.str.size, key->via.str.ptr );

			uint32_t key_size = key->via.str.size;
			const char* key_string = key->via.str.ptr;

			if( key_size == strlen( "mmtfVersion" ) && strncmp( key_string, "mmtfVersion", key_size ) == 0 ) {
				(*thing).mmtfVersion = fetch_string( value );

				printf( "mmtfVersion : %s\n\n", (*thing).mmtfVersion );
			}

			if( key_size == strlen( "mmtfProducer" ) && strncmp( key_string, "mmtfProducer", key_size ) == 0 ) {
				(*thing).mmtfProducer = fetch_string( value );

				printf( "mmtfProducer : %s\n\n", (*thing).mmtfProducer );
			}

			if( key_size == strlen( "spaceGroup" ) && strncmp( key_string, "spaceGroup", key_size ) == 0 ) {
				(*thing).spaceGroup = fetch_string( value );

				printf( "spaceGroup : %s\n\n", (*thing).spaceGroup );
			}

			if( key_size == strlen( "structureId" ) && strncmp( key_string, "structureId", key_size ) == 0 ) {
				(*thing).structureId = fetch_string( value );

				printf( "structureId : %s\n\n", (*thing).structureId );
			}

			if( key_size == strlen( "title" ) && strncmp( key_string, "title", key_size ) == 0 ) {
				(*thing).title = fetch_string( value );

				printf( "title : %s\n\n", (*thing).title );
			}

			if( key_size == strlen( "depositionDate" ) && strncmp( key_string, "depositionDate", key_size ) == 0 ) {
				(*thing).depositionDate = fetch_string( value );

				printf( "depositionDate : %s\n\n", (*thing).depositionDate );
			}

			if( key_size == strlen( "releaseDate" ) && strncmp( key_string, "releaseDate", key_size ) == 0 ) {
				(*thing).releaseDate = fetch_string( value );

				printf( "releaseDate : %s\n\n", (*thing).releaseDate );
			}

			if( key_size == strlen( "numBonds" ) && strncmp( key_string, "numBonds", key_size ) == 0 ) {
				(*thing).numBonds = fetch_int( value );

				printf( "numBonds : %lu\n\n", (*thing).numBonds );
			}

			if( key_size == strlen( "numAtoms" ) && strncmp( key_string, "numAtoms", key_size ) == 0 ) {
				(*thing).numAtoms = fetch_int( value );

				printf( "numAtoms : %lu\n\n", (*thing).numAtoms );
			}

			if( key_size == strlen( "numGroups" ) && strncmp( key_string, "numGroups", key_size ) == 0 ) {
				(*thing).numGroups = fetch_int( value );

				printf( "numGroups : %lu\n\n", (*thing).numGroups );
			}

			if( key_size == strlen( "numChains" ) && strncmp( key_string, "numChains", key_size ) == 0 ) {
				(*thing).numChains = fetch_int( value );

				printf( "numChains : %lu\n\n", (*thing).numChains );
			}

			if( key_size == strlen( "numModels" ) && strncmp( key_string, "numModels", key_size ) == 0 ) {
				(*thing).numModels = fetch_int( value );

				printf( "numModels : %lu\n\n", (*thing).numModels );
			}

			if( key_size == strlen( "entityList" ) && strncmp( key_string, "entityList", key_size ) == 0 ) {
				uint64_t length;
				(*thing).entityList = fetch_entityList( value, &length );

				printf( "entityList : %p\n\n", (*thing).entityList );
			}

			if( key_size == strlen( "unitCell" ) && strncmp( key_string, "unitCell", key_size ) == 0 ) {
				uint64_t length;
				float* unitCell = fetch_clear_float_array( value, &length );

				if( unitCell != NULL ) {
					printf( "unitCell : ");
					int i;
					for( i = 0; i < 6; ++i ) {
						(*thing).unitCell[i] = unitCell[i];
						printf( "%f ", unitCell[i] );
					}
				}
			}

			if( key_size == strlen( "bioAssemblyList" ) && strncmp( key_string, "bioAssemblyList", key_size ) == 0 ) {
				uint64_t length;
				(*thing).bioAssemblyList = fetch_bioAssemblyList( value, &length );
			}

			if( key_size == strlen( "resolution" ) && strncmp( key_string, "resolution", key_size ) == 0 ) {
				(*thing).resolution = fetch_float( value );

				printf( "resolution : %f\n\n", (*thing).resolution );
			}

			if( key_size == strlen( "rFree" ) && strncmp( key_string, "rFree", key_size ) == 0 ) {
				(*thing).rFree = fetch_float( value );

				printf( "rFree : %f\n\n", (*thing).rFree );
			}

			if( key_size == strlen( "rWork" ) && strncmp( key_string, "rWork", key_size ) == 0 ) {
				(*thing).rWork = fetch_float( value );

				printf( "rWork : %f\n\n", (*thing).rWork );
			}

			if( key_size == strlen( "experimentalMethods" ) && strncmp( key_string, "experimentalMethods", key_size ) == 0 ) {
				uint64_t length;
				(*thing).experimentalMethods = fetch_clear_string_array( value, &length );

int i;
for( i = 0; i < length; ++i ) {
printf( "experimentalMethods : %s\n\n", (*thing).experimentalMethods[i] );
}

			}

			if( key_size == strlen( "bondAtomList" ) && strncmp( key_string, "bondAtomList", key_size ) == 0 ) {
				uint64_t length;
				(*thing).bondAtomList = (int32_t*) fetch_array( value, &length );

				printf( "bondAtomList : %p\n\n", (*thing).bondAtomList );
			}

			if( key_size == strlen( "chainsPerModel" ) && strncmp( key_string, "chainsPerModel", key_size ) == 0 ) {
				uint64_t length;
				(*thing).chainsPerModel = fetch_clear_int32_array( value, &length );

				printf( "chainsPerModel : %p\n\n", (*thing).chainsPerModel );
			}

			if( key_size == strlen( "groupsPerChain" ) && strncmp( key_string, "groupsPerChain", key_size ) == 0 ) {
				uint64_t length;
				(*thing).groupsPerChain = fetch_clear_int32_array( value, &length );

				printf( "groupsPerChain : %p\n\n", (*thing).groupsPerChain );
			}

			if( key_size == strlen( "groupList" ) && strncmp( key_string, "groupList", key_size ) == 0 ) {
				uint64_t length;
				(*thing).groupList = fetch_groupList( value, &length );

				printf( "groupList : %p\n\n", (*thing).groupList );
			}

			if( key_size == strlen( "groupTypeList" ) && strncmp( key_string, "groupTypeList", key_size ) == 0 ) {
				uint64_t length;
				(*thing).groupTypeList = (int32_t*) fetch_array( value, &length );

				printf( "groupTypeList : %p\n\n", (*thing).groupTypeList );
			}

			if( key_size == strlen( "groupIdList" ) && strncmp( key_string, "groupIdList", key_size ) == 0 ) {
				uint64_t length;
				(*thing).groupIdList = (int32_t*) fetch_array( value, &length );

				printf( "groupIdList : %p\n\n", (*thing).groupIdList );
			}

			if( key_size == strlen( "sequenceIndexList" ) && strncmp( key_string, "sequenceIndexList", key_size ) == 0 ) {
				uint64_t length;
				(*thing).sequenceIndexList = (int32_t*) fetch_array( value, &length );

				printf( "sequenceIndexList : %p\n\n", (*thing).sequenceIndexList );
			}

			if( key_size == strlen( "atomIdList" ) && strncmp( key_string, "atomIdList", key_size ) == 0 ) {
				uint64_t length;
				(*thing).atomIdList = (int32_t*) fetch_array( value, &length );

				printf( "atomIdList : %p\n\n", (*thing).atomIdList );
			}

			if( key_size == strlen( "insCodeList" ) && strncmp( key_string, "insCodeList", key_size ) == 0 ) {
				uint64_t length;
				(*thing).insCodeList = (char*) fetch_array( value, &length );

				printf( "insCodeList : %p\n\n", (*thing).insCodeList );
			}

			if( key_size == strlen( "altLocList" ) && strncmp( key_string, "altLocList", key_size ) == 0 ) {
				uint64_t length;
				(*thing).altLocList = (char*) fetch_array( value, &length );

				printf( "altLocList : %p\n\n", (*thing).altLocList );
			}

			if( key_size == strlen( "bondOrderList" ) && strncmp( key_string, "bondOrderList", key_size ) == 0 ) {
				uint64_t length;
				(*thing).bondOrderList = (int8_t*) fetch_array( value, &length );

				printf( "bondOrderList : %p\n\n", (*thing).bondOrderList );
			}

			if( key_size == strlen( "secStructList" ) && strncmp( key_string, "secStructList", key_size ) == 0 ) {
				uint64_t length;
				(*thing).secStructList = (int8_t*) fetch_array( value, &length );

				printf( "secStructList : %p\n\n", (*thing).secStructList );
			}

			if( key_size == strlen( "bFactorList" ) && strncmp( key_string, "bFactorList", key_size ) == 0 ) {
				uint64_t length;
				(*thing).bFactorList = (float*) fetch_array( value, &length );

				printf( "bFactorList : %p\n\n", (*thing).bFactorList );
			}

			if( key_size == strlen( "xCoordList" ) && strncmp( key_string, "xCoordList", key_size ) == 0 ) {
				uint64_t length;
				(*thing).xCoordList = (float*) fetch_array( value, &length );

				printf( "xCoordList : %p\n\n", (*thing).xCoordList );
			}

			if( key_size == strlen( "yCoordList" ) && strncmp( key_string, "yCoordList", key_size ) == 0 ) {
				uint64_t length;
				(*thing).yCoordList = (float*) fetch_array( value, &length );

				printf( "yCoordList : %p\n\n", (*thing).yCoordList );
			}

			if( key_size == strlen( "zCoordList" ) && strncmp( key_string, "zCoordList", key_size ) == 0 ) {
				uint64_t length;
				(*thing).zCoordList = (float*) fetch_array( value, &length );

				printf( "zCoordList : %p\n\n", (*thing).zCoordList );
			}

			if( key_size == strlen( "occupancyList" ) && strncmp( key_string, "occupancyList", key_size ) == 0 ) {
				uint64_t length;
				(*thing).occupancyList = (float*) fetch_array( value, &length );

				printf( "occupancyList : %p\n\n", (*thing).occupancyList );
			}

			if( key_size == strlen( "chainIdList" ) && strncmp( key_string, "chainIdList", key_size ) == 0 ) {
				uint64_t length;
				(*thing).chainIdList = (char**) fetch_array( value, &length );

				printf( "chainIdList : %p\n\n", (*thing).chainIdList );
			}

			if( key_size == strlen( "chainNameList" ) && strncmp( key_string, "chainNameList", key_size ) == 0 ) {
				uint64_t length;
				(*thing).chainNameList = (char**) fetch_array( value, &length );

				printf( "chainNameList : %p\n\n", (*thing).chainNameList );
			}
		}
	}
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

	convert_MMTF_file_to_MMTF_struct("./data/5ESW.mmtf", &thing);

// One must free everything from the MMTF structure
}

