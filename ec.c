#include <stdio.h>
#include <sys/io.h>

#define COMMAND_PORT 0x66
#define DATA_PORT 0x62

#define RW_TIMEOUT 500

#define OUTPUT_BUFFER_FULL 0x01
#define INPUT_BUFFER_FULL 0x02

#define READ_CMD 0x80
#define WRITE_CMD 0x81

static int wait_for_ec_status(unsigned char status, int is_set) {
    int i;

    for(i = 0; i < RW_TIMEOUT; i+=1) {
        unsigned char value = inb(COMMAND_PORT);

        if (is_set) {
            value = ~value;
        }

        if((status & value) == 0) {
            return 1;
        }
    }
    return 0;
}

int read_ec(unsigned char reg, unsigned char* val) {
    int ret;

    outb(READ_CMD, COMMAND_PORT);
    if (!wait_for_ec_status(INPUT_BUFFER_FULL, 0)) {
        fprintf(stderr, "EC read timeout 1\n");
        return 0;
    }

    outb(reg, DATA_PORT);

    if (!wait_for_ec_status(OUTPUT_BUFFER_FULL, 1)) {
        fprintf(stderr, "EC read timeout 2\n");
        return 0;
    }

    *val = inb(DATA_PORT);
    return 1;
}

int write_ec(unsigned char reg, unsigned char val) {
    int ret;

    outb(WRITE_CMD, COMMAND_PORT);
    if (!wait_for_ec_status(INPUT_BUFFER_FULL, 0)) {
        fprintf(stderr, "EC write timeout 1\n");
        return 0;
    }

    outb(reg, DATA_PORT);
    if (!wait_for_ec_status(INPUT_BUFFER_FULL, 0)) {
        fprintf(stderr, "EC write timeout 2\n");
        return 0;
    }

    outb(val, DATA_PORT);
    if (!wait_for_ec_status(INPUT_BUFFER_FULL, 0)) {
        fprintf(stderr, "EC write timeout 3\n");
        return 0;
    }
    
    return 1;
}

int ec_init() {
    ioperm(COMMAND_PORT, 1, 1);
    ioperm(DATA_PORT, 1, 1);
}

void ec_dump() {
    unsigned char val;
    int res = 0;

    for(int i=0; i<256; i+=16) {
        printf("%02x: ", i);
        for(int j=0; j< 16; j+=1) {
            res += read_ec(i+j, &val);
            printf("%02x ", (unsigned int) val);
        }
        printf("\n");
    }
    printf("%d\n", res);
}
