/* From VGAlib, changed for svgalib */
/* partially copyrighted (C) 1993 by Hartmut Schirmer */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>		/* for usleep( long ) */
#include <string.h>
#include "vga.h"

static unsigned char line[2048 * 3];

static void
drawSquares(int const xmax, int const ymax) 
{
    unsigned int const center_x = xmax/2;
        /* The column just to the left of the center (assuming
           even number of columns)
        */
    unsigned int const center_y = ymax/2;
        /* The line just above the center (assuming
           even number of lines)
        */
            
    int x;

    for (x = 0; x < 64; x++) {
        int y;
        for (y = 0; y < 64; y++) {
            /* Top 3 squares */
            vga_setrgbcolor(x * 4 + 3, y * 4 + 3, 0);
            vga_drawpixel(center_x - 32 - 32 - 64 + x, center_y - 80 + y);
            vga_setrgbcolor(x * 4 + 3, 0, y * 4 + 3);
            vga_drawpixel(center_x -  32 + x,          center_y - 80 + y);
            vga_setrgbcolor(0, x * 4 + 3, y * 4 + 3);
            vga_drawpixel(center_x + 32 + 32 + x,      center_y - 80 + y);
            
            /* Bottom 3 squares */
            vga_setrgbcolor(x * 4 + 3, y * 4 + 3, 255);
            vga_drawpixel(center_x - 32 - 32 - 64 + x, center_y + 16 + y);
            vga_setrgbcolor(x * 4 + 3, 255, y * 4 + 3);
            vga_drawpixel(center_x -  32 + x,          center_y + 16 + y);
            vga_setrgbcolor(255, x * 4 + 3, y * 4 + 3);
            vga_drawpixel(center_x + 32 + 32 + x,      center_y + 16 + y);
        }
    }
}



static void testmode(int mode)
{
    int xmax, ymax, i, x, y, yw, ys, c;
    vga_modeinfo *modeinfo;

    vga_setmode(mode);

    modeinfo = vga_getmodeinfo(mode);

    printf("Width: %d  Height: %d  Colors: %d\n",
	   modeinfo->width,
	   modeinfo->height,
	   modeinfo->colors);
    printf("DisplayStartRange: %xh  Maxpixels: %d  Blit: %s\n",
	   modeinfo->startaddressrange,
	   modeinfo->maxpixels,
	   modeinfo->haveblit ? "YES" : "NO");

#ifdef TEST_MODEX
    if (modeinfo->colors == 256)
	printf("Switching to ModeX ... %s\n",
	       (vga_setmodeX()? "done" : "failed"));
#endif

    vga_screenoff();

    xmax = vga_getxdim() - 1;
    ymax = vga_getydim() - 1;

    vga_setcolor(vga_white());
    vga_drawline(0, 0, xmax, 0);
    vga_drawline(xmax, 0, xmax, ymax);
    vga_drawline(xmax, ymax, 0, ymax);
    vga_drawline(0, ymax, 0, 0);

    /* Draw crosses */
    for (i = 0; i <= 15; i++) {
	vga_setegacolor(i);
	vga_drawline(10 + i * 5, 10, 89 + i * 5, 89);
    }
    for (i = 0; i <= 15; i++) {
	vga_setegacolor(i);
	vga_drawline(89 + i * 5, 10, 10 + i * 5, 89);
    }

    vga_screenon();

    ys = 100;
    yw = (ymax - 100) / 4;
    switch (vga_getcolors()) {
    case 256:
        /* Draw horizontal color bands using palette */
	for (i = 0; i < 60; ++i) {
	    c = (i * 64) / 60;
	    vga_setpalette(i + 16, c, c, c);
	    vga_setpalette(i + 16 + 60, c, 0, 0);
	    vga_setpalette(i + 16 + (2 * 60), 0, c, 0);
	    vga_setpalette(i + 16 + (3 * 60), 0, 0, c);
	}
	line[0] = line[xmax] = 15;
	line[1] = line[xmax - 1] = 0;
	for (x = 2; x < xmax - 1; ++x)
	    line[x] = (((x - 2) * 60) / (xmax - 3)) + 16;
	for (y = ys; y < ys + yw; ++y)	/* gray */
	    vga_drawscanline(y, line);
	for (x = 2; x < xmax - 1; ++x)
	    line[x] += 60;
	ys += yw;
	for (y = ys; y < ys + yw; ++y)	/* red */
	    vga_drawscanline(y, line);
	for (x = 2; x < xmax - 1; ++x)
	    line[x] += 60;
	ys += yw;
	for (y = ys; y < ys + yw; ++y)	/* green */
	    vga_drawscanline(y, line);
	for (x = 2; x < xmax - 1; ++x)
	    line[x] += 60;
	ys += yw;
	for (y = ys; y < ys + yw; ++y)	/* blue */
	    vga_drawscanline(y, line);
	break;

    case 1 << 15:
    case 1 << 16:
    case 1 << 24:
        /* Draw horizontal color bands in RGB */
	for (x = 2; x < xmax - 1; ++x) {
	    c = ((x - 2) * 255) / (xmax - 4);
	    y = ys;
	    vga_setrgbcolor(c, c, c);
	    vga_drawline(x, y, x, y + yw - 1);
	    y += yw;
	    vga_setrgbcolor(c, 0, 0);
	    vga_drawline(x, y, x, y + yw - 1);
	    y += yw;
	    vga_setrgbcolor(0, c, 0);
	    vga_drawline(x, y, x, y + yw - 1);
	    y += yw;
	    vga_setrgbcolor(0, 0, c);
	    vga_drawline(x, y, x, y + yw - 1);
	}
        drawSquares(xmax, ymax);
	break;
    default:
        /* Draw vertical color bars */
	if (vga_getcolors() == 16) {
	    for (i = 0; i < xmax - 1; i++)
		line[i] = (i + 2) % 16;
	    line[0] = line[xmax] = 15;
	    line[1] = line[xmax - 1] = 0;
	}
	if (vga_getcolors() == 2) {
	    for (i = 0; i <= xmax; i++)
		line[i] = 0x11;
	    line[0] = 0x91;
	}
	for (i = 100; i < ymax - 1; i++)
	    vga_drawscanline(i, line);
	break;

    }

    if (getchar() == 'd')
	vga_dumpregs();

}

int main(int argc, char *argv[])
{
    int mode, mode2;
    int i, high;

    vga_init();			/* Initialize. */

    mode = -1;
	mode2= -1;
	
	if(argc==2) {
		mode = atoi(argv[1]);
	} else if(argc==3) {
		mode = atoi(argv[1]);
		mode2= atoi(argv[2]);
	}
		

    if (mode == -1) {
	printf("Choose one of the following video modes: \n");

	high = 0;
	for (i = 1; i <= vga_lastmodenumber(); i++)
	    if (vga_hasmode(i)) {
		vga_modeinfo *info;
		char expl[100];
		const char *cols = NULL;

		*expl = '\0';
		info = vga_getmodeinfo(i);
		switch (info->colors) {
		case 2:
		    cols = "2";
		    strcpy(expl, "1 bitplane, monochrome");
		    break;
		case 16:
		    cols = "16";
		    strcpy(expl, "4 bitplanes");
		    break;
		case 256:
		    if (i == G320x200x256)
			strcpy(expl, "packed-pixel");
		    else if (i == G320x240x256
			     || i == G320x400x256
			     || i == G360x480x256)
			strcpy(expl, "Mode X");
		    else
			strcpy(expl,
			       "packed-pixel, banked");
		    break;
		case 1 << 15:
		    cols = "32K";
		    strcpy(expl, "5-5-5 RGB, blue at LSB, banked");
		    break;
		case 1 << 16:
		    cols = "64K";
		    strcpy(expl, "5-6-5 RGB, blue at LSB, banked");
		    break;
		case 1 << 24:
		    cols = "16M";
		    if (info->bytesperpixel == 3) {
			if (info->flags & RGB_MISORDERED)
			    strcpy(expl, "8-8-8 BGR, red byte first, banked");
			else
			    strcpy(expl, "8-8-8 RGB, blue byte first, banked");
		    } else if (info->flags & RGB_MISORDERED)
			strcpy(expl, "8-8-8 RGBX, 32-bit pixels, X byte first, banked");
		    else
			strcpy(expl, "8-8-8 XRGB, 32-bit pixels, blue byte first, banked");
		    break;
		}
		if (info->flags & IS_INTERLACED) {
		    if (*expl != '\0')
			strcat(expl, ", ");
		    strcat(expl, "interlaced");
		}
		if (info->flags & IS_DYNAMICMODE) {
		    if (*expl != '\0')
			strcat(expl, ", ");
		    strcat(expl, "dynamically loaded");
		}
		high = i;
		printf("%5d: %dx%d, ",
		       i, info->width, info->height);
		if (cols == NULL)
		    printf("%d", info->colors);
		else
		    printf("%s", cols);
		printf(" colors ");
		if (*expl != '\0')
		    printf("(%s)", expl);
		printf("\n");
	    }
	printf("Enter mode number (1-%d): ", high);
	scanf("%d", &mode);
	getchar();
	printf("\n");

	if (mode < 1 || mode > GLASTMODE) {
	    printf("Error: Mode number out of range \n");
	    exit(-1);
	}
    }
    if (vga_hasmode(mode)) {
		testmode(mode);
		if(mode2!=-1 && vga_hasmode(mode2)) {
			testmode(mode2);
		}
	} else {
		printf("Error: Video mode not supported by driver\n");
		exit(-1);
    }
    
	vga_setmode(TEXT);
    
    return 0;
}
