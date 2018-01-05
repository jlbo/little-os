#include "framebufferd.h"
#include "seriald.h"
#include "klog.h"
unsigned short lcom = SERIAL_COM1_BASE;


void init (void)
{
	serial_initw(lcom);	
}

void kmain (void)
{
	init();
	klog(KLOG_INFO, "OS is in kmain\n");	
}

