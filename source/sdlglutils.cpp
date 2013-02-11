#include "sdlglutils.h"
#include <SDL/SDL.h>
#include <GL/glu.h>

SDL_Surface * flipSurface(SDL_Surface * surface);

GLuint loadTexture(const char * filename,bool useMipMap)
{
    GLuint glID;
    SDL_Surface * picture_surface = NULL;
    SDL_Surface *gl_surface = NULL;
    SDL_Surface * gl_fliped_surface = NULL;
    Uint32 rmask, gmask, bmask, amask;

    picture_surface = SDL_LoadBMP(filename);
    if (picture_surface == NULL)
        return 0;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN

    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else

    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif

    SDL_PixelFormat format = *(picture_surface->format);
    format.BitsPerPixel = 32;
    format.BytesPerPixel = 4;
    format.Rmask = rmask;
    format.Gmask = gmask;
    format.Bmask = bmask;
    format.Amask = amask;

    gl_surface = SDL_ConvertSurface(picture_surface,&format,SDL_SWSURFACE);

    gl_fliped_surface = flipSurface(gl_surface);

    glGenTextures(1, &glID);

    glBindTexture(GL_TEXTURE_2D, glID);


    if (useMipMap)
    {

        gluBuild2DMipmaps(GL_TEXTURE_2D, 4, gl_fliped_surface->w,
                          gl_fliped_surface->h, GL_RGBA,GL_UNSIGNED_BYTE,
                          gl_fliped_surface->pixels);

        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,
                        GL_LINEAR_MIPMAP_LINEAR);

    }
    else
    {
        glTexImage2D(GL_TEXTURE_2D, 0, 4, gl_fliped_surface->w,
                     gl_fliped_surface->h, 0, GL_RGBA,GL_UNSIGNED_BYTE,
                     gl_fliped_surface->pixels);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);


    SDL_FreeSurface(gl_fliped_surface);
    SDL_FreeSurface(gl_surface);
    SDL_FreeSurface(picture_surface);

    return glID;
}

int takeScreenshot(const char * filename)
{
    GLint viewport[4];
    Uint32 rmask, gmask, bmask, amask;
    SDL_Surface * picture, * finalpicture;

    glGetIntegerv(GL_VIEWPORT, viewport);

#if SDL_BYTEORDER == SDL_BIG_ENDIAN

    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else

    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif

    picture = SDL_CreateRGBSurface(SDL_SWSURFACE,viewport[2],viewport[3], 32,
                                   rmask, gmask, bmask, amask);
    SDL_LockSurface(picture);
    glReadPixels(viewport[0],viewport[1],viewport[2],viewport[3],GL_RGBA,
                 GL_UNSIGNED_BYTE,picture->pixels);
    SDL_UnlockSurface(picture);

    finalpicture = flipSurface(picture);

    if (SDL_SaveBMP(finalpicture, filename))
    {
        return -1;
    }
    SDL_FreeSurface(finalpicture);
    SDL_FreeSurface(picture);

    return 0;
}

SDL_Surface * flipSurface(SDL_Surface * surface)
{
    int current_line,pitch;
    SDL_Surface * fliped_surface = SDL_CreateRGBSurface(SDL_SWSURFACE,
                                   surface->w,surface->h,
                                   surface->format->BitsPerPixel,
                                   surface->format->Rmask,
                                   surface->format->Gmask,
                                   surface->format->Bmask,
                                   surface->format->Amask);



    SDL_LockSurface(surface);
    SDL_LockSurface(fliped_surface);

    pitch = surface->pitch;
    for (current_line = 0; current_line < surface->h; current_line ++)
    {
        memcpy(&((unsigned char* )fliped_surface->pixels)[current_line*pitch],
               &((unsigned char* )surface->pixels)[(surface->h - 1  -
                                                    current_line)*pitch],
               pitch);
    }

    SDL_UnlockSurface(fliped_surface);
    SDL_UnlockSurface(surface);
    return fliped_surface;
}

void drawAxis(double scale)
{
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glPushMatrix();
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glEnable(GL_LINE_SMOOTH);
    glLineWidth(2);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glScaled(scale,scale,scale);
    glBegin(GL_LINES);
    glColor3ub(255,0,0);
    glVertex3i(0,0,0);
    glVertex3i(1,0,0);
    glColor3ub(0,255,0);
    glVertex3i(0,0,0);
    glVertex3i(0,1,0);
    glColor3ub(0,0,255);
    glVertex3i(0,0,0);
    glVertex3i(0,0,1);
    glEnd();
    glPopMatrix();
    glPopAttrib();
}

int initFullScreen(unsigned int * width,unsigned int * height)
{
    SDL_Rect ** modes;

    modes = SDL_ListModes(NULL,SDL_FULLSCREEN|SDL_OPENGL);
    if ((modes == (SDL_Rect **)0)||(modes == (SDL_Rect **)-1))
        return 0;

    if (width != NULL)
        *width = modes[0]->w;
    if (height != NULL)
        *height = modes[0]->h;
    if (SDL_SetVideoMode(modes[0]->w,
                         modes[0]->h,
                         SDL_GetVideoInfo()->vfmt->BitsPerPixel,
                         SDL_FULLSCREEN|SDL_OPENGL) == NULL)
        return -1;
    else
    {
        return 0;
    }
}

int XPMFromImage(const char * imagefile, const char * XPMfile)
{
    SDL_Surface * image,*image32bits;
    FILE * xpm;
    Uint32 pixel;
    Uint8 r,g,b,a;
    int x,y;
    unsigned int w;
    char * xpm_name;
    Uint32 rmask, gmask, bmask, amask;

    image = SDL_LoadBMP(imagefile);
    if (image == NULL)
        return -1;

    #if SDL_BYTEORDER == SDL_BIG_ENDIAN

    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else

    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif

    image32bits = SDL_CreateRGBSurface(SDL_SWSURFACE,
                                      image->w,image->h,
                                      32,rmask, gmask, bmask, amask);

    SDL_BlitSurface(image,NULL,image32bits,NULL);
    SDL_FreeSurface(image);

    xpm = fopen(XPMfile,"w");

    xpm_name = (char*)malloc(strlen(imagefile)*sizeof(char));
    strcpy(xpm_name,imagefile);
    if (strchr(xpm_name,'.') != NULL)
        *strchr(xpm_name,'.') = '\0';
    fprintf(xpm,"const char *%s[] =\n",xpm_name);
    free(xpm_name);

    fprintf(xpm,"\t{\n");
    fprintf(xpm,"\t\t/* width height num_colors chars_per_pixel */\n");
    w = ((image->w%8) == 0)?image32bits->w:8*(image32bits->w/8+1);

    fprintf(xpm,"\t\t\" %d %d 3 1 \",\n",w,image32bits->h);
    fprintf(xpm,"\t\t/* colors */\n");
    fprintf(xpm,"\t\t\"X c #000000\",\n");
    fprintf(xpm,"\t\t\". c #ffffff\",\n");
    fprintf(xpm,"\t\t\"  c None\",\n");
    fprintf(xpm,"\t\t/* pixels */\n");

    SDL_LockSurface(image32bits);

    for (y = 0; y < image32bits->h; y ++)
    {
        fprintf(xpm,"\t\t\"");
        for (x = 0; x < image32bits->w ; x ++)
        {
            pixel = ((Uint32*)image32bits->pixels)[x+y*image32bits->pitch/4];
            SDL_GetRGBA(pixel,image32bits->format,&r,&g,&b,&a);
            if (a < 128)
                fprintf(xpm," ");
            else if ((r >= 128)||(g >= 128)||(b >= 128))
                fprintf(xpm,".");
            else
                fprintf(xpm,"X");
        }
        for (x = image32bits->w ; (unsigned int)x < w ; x ++)
            fprintf(xpm," ");
        fprintf(xpm,"\",\n");
    }

    SDL_UnlockSurface(image32bits);
    SDL_FreeSurface(image32bits);
    fprintf(xpm,"\t\t\"0,0\"\n");
    fprintf(xpm,"\t};\n");
    return 0;
}

SDL_Cursor * cursorFromXPM(const char * xpm[])
{
    int i, row, col;
    int width, height;
    Uint8 * data;
    Uint8 * mask;
    int hot_x, hot_y;
    SDL_Cursor * cursor = NULL;

    sscanf(xpm[0], "%d %d", &width, &height);
    data = (Uint8*)calloc(width/8*height,sizeof(Uint8));
    mask = (Uint8*)calloc(width/8*height,sizeof(Uint8));

    i = -1;
    for ( row=0; row<height; ++row )
    {
        for ( col=0; col<width; ++col )
        {
            if ( col % 8 )
            {
                data[i] <<= 1;
                mask[i] <<= 1;
            }
            else
            {
                ++i;
                data[i] = mask[i] = 0;
            }
            switch (xpm[4+row][col])
            {
                case 'X':
                data[i] |= 0x01;
                mask[i] |= 0x01;
                break;
                case '.':
                mask[i] |= 0x01;
                break;
                case ' ':
                break;
            }
        }
    }
    sscanf(xpm[4+row], "%d,%d", &hot_x, &hot_y);
    cursor = SDL_CreateCursor(data, mask, width, height, hot_x, hot_y);
    free(data);
    free(mask);
    return cursor;
}
