#include <stdlib.h>

// int 32
int32_t* convert_to_int_32(const char* bytes){
printf("in funct");
long tot_len = (sizeof(bytes)-12)/4;
printf("gotlen");
int32_t* output = malloc(sizeof(int32_t)*(tot_len)); // The output needs to be freed by the calling process
printf("alloc");
int ind = 0;
for(int i=12; i<sizeof(bytes); i+=4){
	output[ind] = get_int(bytes,i);
	ind++;
}
return output;
}


// int 16
int32_t* convert_to_int_16(const char* bytes){
printf("in funct");
long tot_len = (sizeof(bytes)-12)/2;
int ind = 0;
int32_t* output = malloc(sizeof(int32_t)*(tot_len)); // The output needs to be freed by the calling process
for(int i=12; i<sizeof(bytes); i+=2){
         output[ind] = get_short(bytes,i);
	ind++;
}

return output;
}


// int 8
int32_t* convert_to_int_8(const char* bytes){

long tot_len = (sizeof(bytes)-12);

int32_t* output = malloc(sizeof(int32_t)*(tot_len)); // The output needs to be freed by the calling process

int ind = 0;
for(int i=12; i<sizeof(bytes); i+=1){
         output[ind] = get_byte(bytes,i);
	ind++;
}

return output;
}
