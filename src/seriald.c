#include "io.h"
#include "in.h"

#define SERIAL_DATA_PORT(base) 		(base)
#define SERIAL_FIFO_COMMAND_PORT(base)	(base + 2)
#define SERIAL_LINE_COMMAND_PORT(base) 	(base + 3)
#define SERIAL_MODEM_COMMAND_PORT(base)	(base + 4)
#define SERIAL_LINE_STATUS_PORT(base)	(base + 5)

#define SERIAL_LINE_ENABLE_DLAB		0X80

#define BAUD_DIVISOR			2
/* serial_configure_baud_rate:
*  Sets the speed of the data being sent by giving it a divisor 
*/

static void 
serial_configure_baud_rate(unsigned short com, unsigned short divisor)
{
	outb(SERIAL_LINE_COMMAND_PORT(com),
		SERIAL_LINE_ENABLE_DLAB);
	outb(SERIAL_DATA_PORT(com),
		(divisor >> 8) & 0x00FF);
	outb(SERIAL_DATA_PORT(com),
		divisor & 0x00FF);
}

static void 
serial_configure_line(unsigned short com)
{
	outb(SERIAL_LINE_COMMAND_PORT(com), 0x03);
}

static void 
serial_configure_buffer(unsigned short com)
{
	outb(SERIAL_FIFO_COMMAND_PORT(com), 0xC7);
}

static void 
serial_configure_modem(unsigned short com)
{
	outb(SERIAL_MODEM_COMMAND_PORT(com), 0x03);
}

static int 
serial_is_transmit_fifo_empty(unsigned int com)
{
	return inb(SERIAL_LINE_STATUS_PORT(com)) & 0X20;
}

void serial_initw(unsigned short com)
{
	serial_configure_baud_rate(com, BAUD_DIVISOR);
	serial_configure_line(com);
	serial_configure_buffer(com);
	serial_configure_modem(com);
}

static void 
serial_write_ch(unsigned short com, char c)
{	
	while (serial_is_transmit_fifo_empty(com) == 0);

	outb(SERIAL_DATA_PORT(com), c);
}

int serial_write_com(unsigned short com, char *buf, int len)
{
	char *p;
	int n;

	
	for (n = 0, p = buf; n < len; n++, p++) {
		serial_write_ch(com, *p);
	}
	
	return n;
}


