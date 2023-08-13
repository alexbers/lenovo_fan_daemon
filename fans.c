#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#include "ec.h"

#define IS_LENOVO_SLIM9 1

#define FAN1_ADDR (0xFE0B0000 + 0x5A4 + 0x4)
#define FAN2_ADDR (FAN1_ADDR + 1)

#define FAN1_MODE_REG 147
#define FAN1_SPEED_W_REG 148
#define FAN1_SPEED_R_REG 149

#define FAN2_MODE_REG 150
#define FAN2_SPEED_W_REG 151
#define FAN2_SPEED_R_REG 152

#define MIN_FAN_SPEED 255
#define FAN_MANUAL_MODE 20
#define FAN_AUTO_MODE 4

int mem_fd = -1;


int write_phys_mem(off_t offset, unsigned char c) {
    if (lseek(mem_fd, offset, SEEK_SET) != offset) {
        return 0;
    }
    return write(mem_fd, &c, sizeof(unsigned char)) == sizeof(unsigned char);
}

int read_phys_mem(off_t offset, unsigned char *c) {
    if (lseek(mem_fd, offset, SEEK_SET) != offset) {
        return 0;
    }
    return read(mem_fd, c, sizeof(unsigned char)) == sizeof(unsigned char);
}


int turn_off_fans() {
    if (IS_LENOVO_SLIM9) {
        return write_phys_mem(FAN1_ADDR, 1) && write_phys_mem(FAN2_ADDR, 1);
    } else {
        int ret = write_ec(FAN1_MODE_REG, FAN_MANUAL_MODE);
        ret &= write_ec(FAN2_MODE_REG, FAN_MANUAL_MODE);

        ret &= write_ec(FAN1_SPEED_W_REG, MIN_FAN_SPEED);
        ret &= write_ec(FAN2_SPEED_W_REG, MIN_FAN_SPEED);
        return ret;
    }
}

int turn_on_fans() {
    if (IS_LENOVO_SLIM9) {
        return write_phys_mem(FAN1_ADDR, 0) && write_phys_mem(FAN2_ADDR, 0);
    } else {
        unsigned char val;
        int ret = write_ec(FAN1_MODE_REG, FAN_AUTO_MODE);
        ret &= write_ec(FAN2_MODE_REG, FAN_AUTO_MODE);

        ret &= read_ec(FAN1_MODE_REG, &val);
        ret &= (val == FAN_AUTO_MODE);
        ret &= read_ec(FAN2_MODE_REG, &val);
        ret &= (val == FAN_AUTO_MODE);

        return ret;
    }
}

int both_fans_running() {
    if (IS_LENOVO_SLIM9) {
        unsigned char fan1, fan2;
        if (
            !read_phys_mem(FAN1_ADDR, &fan1) ||
            !read_phys_mem(FAN2_ADDR, &fan2)
        ) {
            return 0;
        }
        return fan1 == 0 && fan2 == 0;
    } else {
        unsigned char val;
        int ret = read_ec(FAN1_MODE_REG, &val);
        ret &= (val == FAN_AUTO_MODE);
        ret &= read_ec(FAN2_MODE_REG, &val);
        ret &= (val == FAN_AUTO_MODE);
        return ret;
    }
}

int init_fans() {
    if (IS_LENOVO_SLIM9) {
        mem_fd = open("/dev/mem", O_RDWR, 0);
        return mem_fd >= 0;
    } else {
        return ec_init();
    }
}

int deinit_fans() {
    if (IS_LENOVO_SLIM9) {
        return close(mem_fd);
    } else {
        return 1;
    }
}