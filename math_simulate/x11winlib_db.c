#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "winlib_db.h"

Display *disp;
int scrn;
Window win;
Colormap cmap;
GC    gc;
XImage *image;
XColor color;
Pixmap pixmap; //for double buffering

FILE *fp;

int width= DEFAULT_WIDTH;
int height= DEFAULT_HEIGHT;


void FatalError(char *str)
{
  if (fp != NULL) {
    fclose(fp);
  }
  fprintf(stderr,"Error:%s\n",str);
  
  fflush(stderr);
  exit(1);
}



void initwin(){
  XSetWindowAttributes wa;
  XVisualInfo vi;
  XSizeHints hints;
  XEvent event;
  if((disp =XOpenDisplay(NULL))==NULL){
    FatalError("Can't open display.");
  }
  scrn = DefaultScreen(disp);

  /*  if(! XMatchVisualInfo(disp,scrn,DISPLAY_DEPTH,DirectColor, &vi)){
    FatalError("Can't open color display.");
    }
  wa.background_pixel = 0;
  wa.border_pixel =0;
  wa.colormap =XCreateColormap(disp,RootWindow(disp,scrn),
			      vi.visual,AllocNone);
  wa.event_mask = KeyPressMask |StructureNotifyMask |ExposureMask
    |ButtonPressMask;
  */
  win =XCreateSimpleWindow(disp, RootWindow(disp,scrn),0,0,width,height,2,
			   2, WhitePixel(disp,scrn) );
  pixmap = XCreatePixmap(disp, win, width, height, DefaultDepth(disp, 0));
  
  /*  image =XCreateImage(disp, qvi.visual, vi.depth,
		      ZPixmap, 0, 0, width, height, 32, 0);
  if ((image->data = (char *)malloc(image->bytes_per_line*height)) ==NULL){
    FatalError("No memory..");
  }
  memset(image->data,0, image->bytes_per_line*height);
  */
  gc= XCreateGC(disp, win, 0,0);
  XSetForeground(disp, gc, BlackPixel(disp,scrn));
  XSetBackground(disp, gc, WhitePixel(disp,scrn));
  XSetFunction(disp, gc, GXcopy);
  XMapRaised(disp,win);
  g_clear();
  XFlush(disp);
}

void dot(int x, int y){
  XDrawLine(disp,pixmap,gc,x,y,x,y);
}

void text(int x, int y,char *str){
  XDrawString(disp,pixmap,gc,x,y,str,strlen(str));
}

void g_line(int x0, int y0, int x1, int y1){
  XDrawLine(disp,pixmap,gc,x0,y0,x1,y1);
}

void g_rgb(unsigned short r, unsigned short g, unsigned short b){
  XColor xc;
  xc.red = r; xc.green = g; xc.blue = b;
  XAllocColor(disp,DefaultColormap(disp,scrn),&xc);
  XSetForeground(disp,gc,xc.pixel);
}

void g_fill(int x0,int y0, int wid, int hei){
  XFillRectangle(disp,pixmap,gc,x0,y0,wid, hei);
}

void g_box(int x0, int y0, int wid, int hei){
  XDrawRectangle(disp,pixmap,gc,x0,y0,wid, hei);
}

void g_clear(){
  g_rgb(65535,65535,65535);
  XFillRectangle(disp,pixmap,gc,0,0,width, height);  
  g_rgb(0,0,0);
}

void display_update(){ //for double buffering
  XCopyArea(disp,pixmap,win,gc,0,0,width,height,0,0);
  XFlush(disp);
}
