#include <GL/glew.h>

#include "texutils.h"

#include <cstdio>
#include <cstdlib>
#include <GL/gl.h>
#include <png.h>

struct struct_tgaHeader {
    unsigned char  identsize;          // size of ID field that follows 18 byte header (0 usually)
    unsigned char  colourmaptype;      // type of colour map 0=none, 1=has palette
    unsigned char  imagetype;          // type of image 0=none,1=indexed,2=rgb,3=grey,+8=rle packed

    unsigned short colourmapstart;     // first colour map entry in palette
    unsigned short colourmaplength;    // number of colours in palette
    unsigned char  colourmapbits;      // number of bits per palette entry 15,16,24,32

    short          xstart;             // image x origin
    short          ystart;             // image y origin
    short          width;              // image width in pixels
    short          height;             // image height in pixels
    unsigned char  bits;               // image bits per pixel 8,16,24,32
    unsigned char  descriptor;         // image descriptor bits (vh flip bits)
} __attribute__((packed));
typedef struct struct_tgaHeader t_tgaHeader;


GLuint loadTgaTexture(const char* filename)
{
	FILE* tgaFile;
	t_tgaHeader tgaHeader;
	unsigned char* imageData = NULL;

	tgaFile = fopen(filename, "rb");
	if (tgaFile == NULL)
	{
		return 0;
	}
	fread(&tgaHeader, sizeof (t_tgaHeader), 1, tgaFile);

	if ((tgaHeader.colourmaptype != 0) || (tgaHeader.imagetype != 2)
		|| ((tgaHeader.bits != 24) && (tgaHeader.bits != 32)))
	{
		// 24-bit or 32-bit supported only
		fclose(tgaFile);
		return 0;
	}
	else
	{
		imageData = (unsigned char*)malloc(((tgaHeader.bits >> 3) * tgaHeader.width * tgaHeader.height) * sizeof (unsigned char));
		fread(imageData, sizeof (unsigned char), (tgaHeader.bits >> 3) * tgaHeader.width * tgaHeader.height, tgaFile);
	}
	fclose(tgaFile);

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	GLint internalFormat = GL_RGBA;
	GLint format = GL_BGRA;
	if (tgaHeader.bits == 24)
	{
		internalFormat = GL_RGB;
		format = GL_BGR;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, tgaHeader.width, tgaHeader.height, 0, format, GL_UNSIGNED_BYTE, (void*)imageData);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	free(imageData);

	return textureID;
}


GLuint loadPngTexture(const char* file_name, int* width, int* height)
{
    png_byte header[8];

    FILE *fp = fopen(file_name, "rb");
    if (fp == 0)
    {
        perror(file_name);
        return 0;
    }

    // read the header
    fread(header, 1, 8, fp);

    if (png_sig_cmp(header, 0, 8))
    {
        fprintf(stderr, "error: %s is not a PNG.\n", file_name);
        fclose(fp);
        return 0;
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr)
    {
        fprintf(stderr, "error: png_create_read_struct returned 0.\n");
        fclose(fp);
        return 0;
    }

    // create png info struct
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
    {
        fprintf(stderr, "error: png_create_info_struct returned 0.\n");
        png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
        fclose(fp);
        return 0;
    }

    // create png info struct
    png_infop end_info = png_create_info_struct(png_ptr);
    if (!end_info)
    {
        fprintf(stderr, "error: png_create_info_struct returned 0.\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
        fclose(fp);
        return 0;
    }

    // the code in this if statement gets called if libpng encounters an error
    if (setjmp(png_jmpbuf(png_ptr))) {
        fprintf(stderr, "error from libpng\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        fclose(fp);
        return 0;
    }

    // init png reading
    png_init_io(png_ptr, fp);

    // let libpng know you already read the first 8 bytes
    png_set_sig_bytes(png_ptr, 8);

    // read all the info up to the image data
    png_read_info(png_ptr, info_ptr);

    // variables to pass to get info
    int bit_depth, color_type;
    png_uint_32 temp_width, temp_height;

    // get info about png
    png_get_IHDR(png_ptr, info_ptr, &temp_width, &temp_height, &bit_depth, &color_type,
        NULL, NULL, NULL);

    if (width){ *width = temp_width; }
    if (height){ *height = temp_height; }

    // Update the png info struct.
    png_read_update_info(png_ptr, info_ptr);

    // Row size in bytes.
    int rowbytes = png_get_rowbytes(png_ptr, info_ptr);

    // glTexImage2d requires rows to be 4-byte aligned
    rowbytes += 3 - ((rowbytes-1) % 4);

    // Allocate the image_data as a big block, to be given to opengl
    png_byte * image_data;
    image_data = (png_byte *) malloc(rowbytes * temp_height * sizeof(png_byte)+15);
    if (image_data == NULL)
    {
        fprintf(stderr, "error: could not allocate memory for PNG image data\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        fclose(fp);
        return 0;
    }

    // row_pointers is for pointing to image_data for reading the png with libpng
    png_bytep * row_pointers = (png_byte **) malloc(temp_height * sizeof(png_bytep));
    if (row_pointers == NULL)
    {
        fprintf(stderr, "error: could not allocate memory for PNG row pointers\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        free(image_data);
        fclose(fp);
        return 0;
    }

    // set the individual row_pointers to point at the correct offsets of image_data
    unsigned int i;
    for (i = 0; i < temp_height; i++)
    {
        row_pointers[temp_height - 1 - i] = image_data + i * rowbytes;
    }

    // read the png into image_data through row_pointers
    png_read_image(png_ptr, row_pointers);

    // Generate the OpenGL texture object
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, temp_width, temp_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    free(image_data);
    free(row_pointers);
    fclose(fp);
    return texture;
}
