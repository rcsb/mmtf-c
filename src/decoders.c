

int get_strategy(const char * bytes) {
    int i = 0;
    return (bytes[i] << 24) + (bytes[i+1] << 16) + (bytes[i+2] << 8) + bytes[i+3];
}

int get_len(const char * bytes){
    int i = 4;
    return (bytes[i] << 24) + (bytes[i+1] << 16) + (bytes[i+2] << 8) + bytes[i+3];
}

int  get_param(const char * bytes) {
    int i = 8;
    return (bytes[i] << 24) + (bytes[i+1] << 16) + (bytes[i+2] << 8) + bytes[i+3];
}





