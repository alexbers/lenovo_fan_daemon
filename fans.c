#include <stdio.h>

#include "ec.h"

#define FAN1_MODE_REG 147
#define FAN1_SPEED_W_REG 148
#define FAN1_SPEED_R_REG 149

#define FAN2_MODE_REG 150
#define FAN2_SPEED_W_REG 151
#define FAN2_SPEED_R_REG 152

#define MIN_FAN_SPEED 255
#define FAN_MANUAL_MODE 20
#define FAN_AUTO_MODE 4

int turn_off_fans() {
    int ret = write_ec(FAN1_MODE_REG, FAN_MANUAL_MODE);
    ret &= write_ec(FAN2_MODE_REG, FAN_MANUAL_MODE);
    
    ret &= write_ec(FAN1_SPEED_W_REG, MIN_FAN_SPEED);
    ret &= write_ec(FAN2_SPEED_W_REG, MIN_FAN_SPEED);
    return ret;
}

int turn_on_fans() {
    unsigned char val;
    int ret = write_ec(FAN1_MODE_REG, FAN_AUTO_MODE);
    ret &= write_ec(FAN2_MODE_REG, FAN_AUTO_MODE);
    
    ret &= read_ec(FAN1_MODE_REG, &val);
    ret &= (val == FAN_AUTO_MODE);
    ret &= read_ec(FAN2_MODE_REG, &val);
    ret &= (val == FAN_AUTO_MODE);

    return ret;
}

int both_fans_running() {
    unsigned char val;
    int ret = read_ec(FAN1_MODE_REG, &val);
    ret &= (val == FAN_AUTO_MODE);
    ret &= read_ec(FAN2_MODE_REG, &val);
    ret &= (val == FAN_AUTO_MODE);
    return ret;
}

int init_fans() {
    ec_init();
}
