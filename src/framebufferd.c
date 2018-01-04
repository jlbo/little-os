#include "framebufferd.h"

#define FB_GREEN 2
#define FB_DARK_GRAY 8

#define FB_COMMAND_PORT 0x3D4
#define FB_DATA_PORT 0x3D5

#define FB_HIGH_BYTE_COMMAND 14
#define FB_LOW_BYTE_COMMAND 15

#define FB_MAX_POS 1999
#define FB_LAST_ROW 1920

static char *fb = (char *) 0x000B8000;
static unsigned short currpos = 0;

static void
fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg)
{
	fb[i] = c;
	fb[i+1] = (fg & 0x0F) << 4 | (bg & 0x0F);
}

static void
fb_copy_cell(unsigned int x, unsigned int y)
{
	fb[x] = fb[y];
	fb[x+1] = fb[y+1];
}

static void
fb_move_cursor(unsigned short pos)
{
	outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
	outb(FB_DATA_PORT, ((pos >> 8) & 0x00FF));
	outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
	outb(FB_DATA_PORT, (pos & 0x00FF));
}	

static void
clear_last_row()
{
	unsigned short pos = FB_LAST_ROW;
	while (pos <= FB_MAX_POS) {
		fb_write_cell(pos*2, 0, 0, 0);
		pos++;
	}
}

static void
scrollup()
{
	unsigned short to, from;
	
	from = 80;
	to = 0;
	
	while (from < FB_MAX_POS+1) {
		fb_copy_cell(to*2, from*2);
		to++;
		from++;
	}
	clear_last_row();
}

int 
write(char *buf, unsigned int len)
{
	char *p;
	unsigned int n;

	p = buf;
	for (n = 0; n < len; n++, p++) {
		
		if (*p == '\n') {
			currpos += 80 - (currpos % 80);
		}else{

			fb_write_cell(currpos*2, *p, FB_GREEN, FB_DARK_GRAY);
			currpos++;
		}
		
		if (currpos > FB_MAX_POS) {
			currpos = FB_MAX_POS;
			scrollup();
			currpos = FB_LAST_ROW;
		}
					
		fb_move_cursor(currpos);
	}
	return n;
}
