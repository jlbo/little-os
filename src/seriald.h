#ifndef INCLUDE_SERIALD_H
#define INCLUDE_SERIALD_H

#include "io.h"
#include "in.h"

#define SERIAL_COM1_BASE	0x3F8

int serial_write(unsigned short com, char *buf, int len);
void serial_initw(unsigned short com);

#endif
