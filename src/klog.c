#include "klog.h"

#define FRAME_BUFFER_MODE 0
#define SERIAL_MODE 1

#define MAX_INT_CHARS 33
static const unsigned short lcom = SERIAL_COM1_BASE;

static void 
sreverse(char *s, char *t, unsigned int len)
{
	unsigned int n;
	char *tp = t + len - 1;
	char *sp = s;

	for (n = 0; n < len; n++) {
		*sp = *tp;
		sp++;
		tp--;
	}
}

static void
kprint_int(int (*pwrite)(char *buf, unsigned int len), int ival)
{
	
	char sval[MAX_INT_CHARS];
	char rval[MAX_INT_CHARS];
	unsigned int slen = 0;
	unsigned int rlen = 0;

	if (ival == 0) {
		pwrite("0", 1);
		return;
	}
	
	if (ival < 0) {
		sval[0] = '-';
		slen++;
		ival *= -1;
	}

	while (ival > 0) {
		rval[rlen++] = ival % 10;
		ival /= 10;
	}
	
	sreverse(&sval[slen], rval, rlen);
	slen += rlen;
	pwrite(sval, slen);
}

static int
serial_writed(char *buf, unsigned int len)
{
	return serial_write(lcom, buf, len);
}

// floats are a pain, so they are not supported yet
void kprintf(char *fmt, int mode, ...)
{
	va_list ap;
	char *p;
	char *sval;
	int ival;
	int (*pwrite)(char *buf, unsigned int len);

	if (mode) {
		pwrite = &serial_writed;
	}else{
		pwrite = &fb_write;
	}
		
	va_start(ap, mode);
	for (p = fmt; *p; p++) {
		if (*p != '%') {
			pwrite(p, 1);
			continue;
		}
		switch(*++p) {
		case 'd':
			ival = va_arg(ap, int);
			kprint_int(pwrite, ival);
			break;
		case 's':
			for (sval = va_arg(ap, char *); *sval; sval++) {
				pwrite(sval, 1);
			}
			break;
		default:
			pwrite(p, 1);
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
