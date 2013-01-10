#include "CTexturePngLoader.h"
#include <stdlib.h>
#include <string.h>
#include <png.h>

bool OpenUtility::CTexturePngLoader::IsCapable(const char *ext)
{
	if (CTextureLoader::IsCapable(ext,"png")) return(true);
	return(false);
}

unsigned char* OpenUtility::CTexturePngLoader::Load(const char *file,unsigned long &w,unsigned long &h,CTexture::EPicMode &channel)
{
	FILE *fp;
	png_byte header[8];

	if ((fp=fopen(file,"rb"))==NULL) return 0;

	// read the header
	fread(header,1,8,fp);
	if (png_sig_cmp(header,0,8))
	{
		// Error: not a PNG file
		fclose(fp);
		return NULL;
	}

	png_structp png_ptr;
	png_infop info_ptr;
	png_infop end_info;

	if ((png_ptr=png_create_read_struct(PNG_LIBPNG_VER_STRING,NULL,NULL,NULL))==0)
	{
		fclose(fp);
		return NULL;
	}

	// create png info struct
	if ((info_ptr=png_create_info_struct(png_ptr))==0)
	{
		png_destroy_read_struct(&png_ptr,(png_infopp)NULL,(png_infopp)NULL);
		fclose(fp);
		return NULL;
	}

	// create png info struct
	if ((end_info=png_create_info_struct(png_ptr))==0)
	{
		png_destroy_read_struct(&png_ptr,&info_ptr,(png_infopp)NULL);
		fclose(fp);
		return NULL;
	}

	// the code in this if statement gets called if libpng encounters an error
	if (setjmp(png_jmpbuf(png_ptr)))
	{
		png_destroy_read_struct(&png_ptr,&info_ptr,&end_info);
		fclose(fp);
		return NULL;
	}

	// init png reading
	png_init_io(png_ptr,fp);

	// let libpng know you already read the first 8 bytes
	png_set_sig_bytes(png_ptr,8);

	// read all the info up to the image data
	png_read_info(png_ptr,info_ptr);

	// variables to pass to get info
	int bit_depth,color_type,bytePerPixel;

	// get info about png
	png_get_IHDR(png_ptr,info_ptr,&w,&h,&bit_depth,&color_type,NULL,NULL,NULL);
	if (bit_depth!=8)
	{
		png_destroy_read_struct(&png_ptr,&info_ptr,&end_info);
		fclose(fp);
		return NULL;
	}
	switch (color_type)
	{
	case PNG_COLOR_TYPE_RGB:
		bytePerPixel=3;
		channel=CTexture::EPModeRGB;
		break;

	case PNG_COLOR_TYPE_RGB_ALPHA:
		bytePerPixel=4;
		channel=CTexture::EPModeRGBA;
		break;

	default:
		png_destroy_read_struct(&png_ptr,&info_ptr,&end_info);
		fclose(fp);
		return NULL;
	}

	// Update the png info struct.
	png_read_update_info(png_ptr,info_ptr);

	// Row size in bytes.
	int rowbytes=png_get_rowbytes(png_ptr,info_ptr);

	// glTexImage2d requires rows to be 4-byte aligned
	rowbytes+=3-((rowbytes-1)%4);

	// Allocate the image_data as a big block, to be given to opengl
	png_byte *image_data;
	if ((image_data=(png_byte*)malloc(rowbytes*h*sizeof(png_byte)+15))==NULL)
	{
		png_destroy_read_struct(&png_ptr,&info_ptr,&end_info);
		fclose(fp);
		return NULL;
	}

	// row_pointers is for pointing to image_data for reading the png with libpng
	png_bytep *row_pointers;
	if ((row_pointers=(png_bytep*)malloc(h*sizeof(png_bytep)))==NULL)
	{
		png_destroy_read_struct(&png_ptr,&info_ptr,&end_info);
		free(image_data);
		fclose(fp);
		return NULL;
	}

	// set the individual row_pointers to point at the correct offsets of image_data
	for (unsigned long i=0;i<h;i++)
		row_pointers[h-1-i]=image_data+i*rowbytes;

	// read the png into image_data through row_pointers
	png_read_image(png_ptr,row_pointers);

	png_destroy_read_struct(&png_ptr,&info_ptr,&end_info);
	free(row_pointers);
	fclose(fp);

	return(image_data);
}

void OpenUtility::CTexturePngLoader::DestroyData(unsigned char *data)
{
	free(data);
}
