#ifndef EC_H
#define EC_H 1

int read_ec(unsigned char reg, unsigned char* val);
int write_ec(unsigned char reg, unsigned char val);
int ec_init();
void ec_dump();

#endif