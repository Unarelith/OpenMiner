/*
 * =====================================================================================
 *
 *       Filename:  OpenGL.hpp
 *
 *    Description:
 *
 *        Created:  15/12/2014 04:18:34
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef OPENGL_HPP_
#define OPENGL_HPP_

#define GL_GLEXT_PROTOTYPES
#define GL3_PROTOTYPES 1

#ifdef __APPLE__
	#include <OpenGL/gl.h>
#else
	#ifdef __MINGW32__
		#include <GL/glew.h>
	#else
		#include <GL/gl.h>
	#endif
#endif

#endif // OPENGL_HPP_
