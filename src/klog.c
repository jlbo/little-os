#include "klog.h"

#define FRAME_BUFFER_MODE 0
#define SERIAL_MODE 1

static const unsigned short lcom = SERIAL_COM1_BASE;


static void
kprint_int(int (*pwrite)(char *buf, int len), int ival)
{

}

void kprintf(char *fmt, int mode, ...)
{
	va_list ap;
	char *p, char *sval;
	int ival;
	double dval;
	int (*pwrite)(char *buf, int len);

	if (mode) {
		pwrite = &serial_writed;
	}else{
		pwrite = &fb_write;
	}
		
	va_start(ap, mode);
	for (p = fmt; *p; p++) {
		if (*p != '%') {
			pwrite(*p, 1);
			continue;
		}
		switch(*++p) {
		case 'd':
			ival = va_arg(ap, int);
			break;
		case 'f':
			dval = va_arg(ap, double);
			break;
		case 's':
			for (sval = va_arg(ap, char *); *sval; sval++) {
				pwrite(*sval, 1);
			}
		default:
			pwrite(*p, 1);
			break;
		}
	}
			
				
}

void klog(int level, char *buf)
{
	switch (level) {
	case KLOG_DEBUG:
		kprintf("%s: %s", SERIAL_MODE, "DEBUG", buf);
		break;
	case KLOG_INFO:
		kprintf("%s: %s", SERIAL_MODE, "INFO", buf);
		break;
	case KLOG_ERROR:
		kprintf("%s: %s", SERIAL_MODE, "ERROR", buf);
		break;
	default:
		kprintf("%s: %s", SERIAL_MODE, "INFO", buf);
		break;
	}
}

static int
serial_writed(char *buf, int len)
{
	serial_write_com(lcom, buf, len);
}

