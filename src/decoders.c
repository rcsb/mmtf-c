#include "num_converters.c"
#include "array_converters.c"

int get_strategy(const char * bytes) {
	return get_int(bytes,0);
}

int get_len(const char * bytes){
	return get_int(bytes,4);
}

int  get_param(const char * bytes) {
	return get_int(bytes,8);
}





