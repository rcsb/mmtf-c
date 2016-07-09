#include <stdlib.h>
#include <msgpack.h>

static const int BUFFERSIZE = 2048;

int main(void)
{
    char *inbuffer = (char *) malloc(BUFFERSIZE);
    if(NULL == inbuffer) {
            fprintf(stderr, "Out of memory!");
            return 1;
    }
    FILE *fp = fopen("../data/4HHB.mmtf", "rb");
    size_t off = 0;
    size_t read = 0;
    msgpack_unpacked unpacked;
    msgpack_unpacked_init(&unpacked);
    do {
            read = fread(inbuffer, sizeof(char), BUFFERSIZE - off, fp);
            off = 0;
            while(msgpack_unpack_next(&unpacked, inbuffer, read, &off)) {
                    msgpack_object_print(stdout, unpacked.data);
                    puts("");
            }
            memcpy(inbuffer, &(inbuffer[off]), read-off);
            off = read - off;
    } while(read != 0);
    msgpack_object_print(stdout, unpacked.data);
    free(inbuffer);
    fclose(fp);
    msgpack_unpacked_destroy(&unpacked);
    return 0;
}
