#include "urImage.h"

urImage::urImage(const char* file_name)
{
	unsigned int sig_read=0;
	buffer=NULL;

	FILE *fp;

	if ((fp = fopen(file_name, "rb")) == NULL) {
		return;
	}

	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

	if (png_ptr == NULL) {
		fclose(fp);
		return;
	}

	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL) {
		fclose(fp);
		png_destroy_write_struct(&png_ptr,  NULL);
		return;
	}

	if (setjmp(png_jmpbuf(png_ptr))) {
		fclose(fp);
		png_destroy_write_struct(&png_ptr, &info_ptr);
		return;
	}

	png_init_io(png_ptr, fp);
	png_set_sig_bytes(png_ptr, sig_read);
	png_read_png(png_ptr, info_ptr, NULL, NULL);

	png_bytep *row_pointers = png_get_rows(png_ptr, info_ptr);

	png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, &interlace_type, NULL, NULL);

	if (bit_depth != 8) {
		fprintf(stderr, "expected 8 bit depth from %s.\n", file_name);
		return;
	}
	
	channels=png_ptr->channels;

	buffer = new png_byte[width * height * channels];
	for (unsigned int i = 0; i < height; i++) {
		memcpy(buffer + channels * i * width, row_pointers[i], width*channels);
	}

	png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
	fclose(fp);
}


urImage::~urImage(void)
{
	if(buffer) 
		delete [] buffer;

}


void urImage::resize(png_uint_32 newwidth, png_uint_32 newheight) {
	if(!buffer) return;
	if(newwidth<width || newheight<height) return;
	
	png_byte *newbuf = new png_byte[newwidth*newheight*channels];
	memset(newbuf, 0, newwidth*newheight*channels);
	for(int i=0;i<height;i++) 
		memcpy(newbuf + channels * i * newwidth, buffer + channels * i * width , width*channels);
	
	width=newwidth;
	height=newheight;
	
	delete [] buffer;
	buffer=newbuf;
}

void urImage::flipPixels() {
	for(int i=0;i<height;i++) {
		png_bytep pixel=buffer + channels * (i * width);
		for(int j=0;j<width;j++) {
			png_byte temp=pixel[0];
			pixel[0]=pixel[2];
			pixel[2]=temp;
			pixel += channels;
		}
	}
}