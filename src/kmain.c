#include "framebufferd.h"
#include "seriald.h"

unsigned short lcom = SERIAL_COM1_BASE;

int main (void)
{
	return 0;
}

void init (void)
{
	serial_initw(lcom);	
}
