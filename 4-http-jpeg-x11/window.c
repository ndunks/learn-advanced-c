#include <stdio.h>
#include <unistd.h>
#include <jpeglib.h>
#include <setjmp.h>
#include <X11/Xlib.h>
#include <X11/extensions/shape.h>

#include "main.h"
#include "window.h"

typedef struct BGR
{
	unsigned char blue, green, red, alpha;
};

struct my_error_mgr
{
	struct jpeg_error_mgr pub; /* "public" fields */

	jmp_buf setjmp_buffer; /* for return to caller */
};

typedef struct my_error_mgr *my_error_ptr;
/*
 * Here's the routine that will replace the standard error_exit method:
 */

METHODDEF(void)
my_error_exit(j_common_ptr cinfo)
{
	/* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
	my_error_ptr myerr = (my_error_ptr)cinfo->err;

	/* Always display the message. */
	/* We could postpone this until after returning, if we chose. */
	(*cinfo->err->output_message)(cinfo);

	/* Return control to the setjmp point */
	longjmp(myerr->setjmp_buffer, 1);
}

int display_image(char *input_buffer, size_t size)
{
	Display *display;
	Window window;
	XEvent e;
	XImage *ximage;
	Screen *screen;
	JDIMENSION w, h;
	char *xdata;
	int screen_num, bgr_pad = 4; // 24 bit/32 bit use 4 bytes
	struct jpeg_decompress_struct cinfo;
	struct my_error_mgr jerr;
	JSAMPARRAY row_buffer;
	int row_stride, offset_w;
	struct BGR *px;
	
	display = XOpenDisplay(NULL);
	if (display == NULL)
	{
		fprintf(stderr, "Cannot open display\n");
		return 1;
	}

	screen_num = DefaultScreen(display);
	screen = ScreenOfDisplay(display, screen_num);
	if (screen->root_depth < 24)
	{
		printf("Unsupported screen color depth: %d\n", screen->root_depth);
		return 1;
	}

	/* Step 1: allocate and initialize JPEG decompression object */
	/* We set up the normal JPEG error routines, then override error_exit. */
	cinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = my_error_exit;
	/* Establish the setjmp return context for my_error_exit to use. */
	if (setjmp(jerr.setjmp_buffer))
	{
		jpeg_destroy_decompress(&cinfo);
		return 1;
	}

	jpeg_create_decompress(&cinfo);

	/* Step 2: specify data source (eg, a file) */
	jpeg_mem_src(&cinfo, input_buffer, size);

	/* Step 3: read file parameters with jpeg_read_header() */
	jpeg_read_header(&cinfo, TRUE);
	w = cinfo.image_width;
	h = cinfo.image_height;

	printf("JPEG: %ux%ux%d\n", w, h, cinfo.num_components);

	log_malloc(&xdata, bgr_pad * w * h, "xdata");

	/* Step 4: set parameters for decompression */
	/* In this example, we don't need to change any of the defaults set by
	* jpeg_read_header(), so we do nothing here.
	*/

	/* Step 5: Start decompressor */
	if (!jpeg_start_decompress(&cinfo))
	{
		printf("JPEG decompress failed\n");
		jpeg_destroy_decompress(&cinfo);
		return 1;
	}

	row_stride = cinfo.output_width * cinfo.output_components;
	printf("row: %ux%u\n", cinfo.output_width, cinfo.output_components);
	/* Make a one-row-high sample array that will go away when done with image */
	row_buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr)&cinfo, JPOOL_IMAGE, row_stride, 1);

	/* Step 6: while (scan lines remain to be read) */
	while (cinfo.output_scanline < cinfo.output_height)
	{
		/* jpeg_read_scanlines expects an array of pointers to scanlines.
		* Here the array is only one element long, but you could ask for
		* more than one scanline at a time if that's more convenient.
		*/
		jpeg_read_scanlines(&cinfo, row_buffer, 1);
		px = xdata + (cinfo.output_scanline * w * 4);
		for (offset_w = 0; offset_w < w; offset_w++)
		{
			px->red = row_buffer[0][offset_w * cinfo.output_components];
			px->green = row_buffer[0][offset_w * cinfo.output_components + 1];
			px->blue = row_buffer[0][offset_w * cinfo.output_components + 2];
			px++;
		}
	}

	/* Step 7: Finish decompression */
	jpeg_finish_decompress(&cinfo);

	// /* Step 8: Release JPEG decompression object */
	jpeg_destroy_decompress(&cinfo);

	ximage = XCreateImage(screen->display, screen->root_visual, screen->root_depth, ZPixmap, 0,
						  xdata, w, h, 32, 0);

	window = XCreateSimpleWindow(display, screen->root, 100, 100, w, h, 0,
								 screen->white_pixel, screen->white_pixel);

	XSelectInput(display, window, ExposureMask | KeyPressMask | ButtonPressMask | ButtonReleaseMask | Button1MotionMask);
	XMapWindow(display, window);

	while (1)
	{
		XNextEvent(display, &e);
		switch (e.type)
		{
		// Resize
		case Expose:
			XPutImage(display, window, DefaultGC(display, 0), ximage, 0, 0, 0, 0, w, h);
			break;
		// Keyboard key
		case KeyPress:
			printf("KEY: %d\n", e.xkey.keycode);
			break;
		case ButtonPress:
			printf("BTN DOWN: %d\n", e.xbutton.button);
			break;
		case ButtonRelease:
			printf("BTN UP: %d\n", e.xbutton.button);
			break;
		case MotionNotify:
			printf("MV: %03d %03d\n", e.xbutton.x, e.xbutton.y);
			break;
			break;
		default:
			printf("E.type: %d\n", e.type);
			break;
		}
	}

	XCloseDisplay(display);
	return 0;
}