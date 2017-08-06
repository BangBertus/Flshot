#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/fl_draw.H>
//#include <FL/x.H>
#include <X11/extensions/shape.h>
#include <png.h>

XRectangle rect;
unsigned char *p;
char fname[128];

void save_png(const char *dst,unsigned char* pixels,int w,int h){
  
  FILE *fp;
  fp = fl_fopen(dst, "wb");
  png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
  png_infop info = png_create_info_struct(png);
  png_bytep ptr = (png_bytep)pixels;
  png_init_io(png, fp);
  png_set_IHDR(png, info, w, h, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
               PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
  png_write_info(png, info);
  
  for (int i = h; i > 0; i--, ptr += w * 3) {
    png_write_row(png,ptr);
  }

  png_write_end(png,info);
  png_destroy_write_struct(&png, &info);
  
  fclose(fp);
}


class WIN : public Fl_Window {
 public:
   
   WIN(int w, int h, const char*l = 0):Fl_Window(w,h,l){}
  
   void draw(){
	   Fl::wait(10);
	   p = fl_read_image(0,0,0,w(),h());
	   if(p){
		  save_png(fname,p,w(),h());
		  delete[]p;
	   }
	   hide();
   }
   
};

int main(int argc, char *argv[]) {
	WIN *win = new WIN(30,30);
	win->border(0);
	win->fullscreen();
	win->show();
	XShapeCombineRectangles(fl_display,fl_xid(win),0,0,0,&rect,0,ShapeSet,0);
	strcpy(fname,argc>1 ? argv[1] : "screenshot.png");
	return Fl::run();
}

