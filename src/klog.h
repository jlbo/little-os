#ifndef INCLUDE_KLOG_H
#define INCLUDE_KLOG_H

#include "seriald.h"

#define KLOG_DEBUG	0
#define KLOG_INFO	1
#define KLOG_ERROR	2

void kprintf(char *fmt, int mode, ...);
void klog(int level, char *buf);
#endif
