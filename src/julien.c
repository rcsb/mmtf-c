#include <stdlib.h>
#include <stdio.h>
//#include <msgpack.h>

void read_file(char* file_name) {
	FILE* file_pointer = fopen(file_name, "rb");

	unsigned char* buffer;

//	for( long int offset = 0; offset < file_length; ++offset ) {
	while(1) {

printf( "test 1\n" );

		fread( buffer, sizeof(buffer), 1, file_pointer );
		printf( "%c", *buffer );

		if( feof( file_pointer ) ) {
			break;
		}
	}
}

int main(void) {

printf( "test 1\n" );

	read_file("../data/4HHB.mmtf");
}


