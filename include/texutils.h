#ifndef TEXUTILS_H
#define TEXUTILS_H

GLuint loadTgaTexture(const char* filename);
GLuint loadPngTexture(const char* file_name, int* width = 0, int* height = 0);

#endif // TEXUTILS_H
