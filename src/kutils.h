#ifndef INCLUDE_KUTILS_H
#define INCLUDE_KUTILS_H

typedef unsigned char *va_list;
#define va_start(list, param) (list = (((va_list)&param) + sizeof(param)))
#define va_arg(list, type) (*(type *)((list += sizeof(type)) - sizeof(type)))


#endif
