
int get_int(const char * bytes, int i){
	return (bytes[i] << 24) + (bytes[i+1] << 16) + (bytes[i+2] << 8) + bytes[i+3];
}

int get_short(const char * bytes, int i){
	return (bytes[i] << 8) + bytes[i+1];
}

int get_byte(const char * bytes, int i){
	return bytes[i];
}

