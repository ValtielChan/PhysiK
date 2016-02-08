#ifndef GLASSERT
#define GLASSERT

/**
 * OpenGL error management class.
 */

#include <iostream>
#include <cassert>

#ifdef RENDER_DEBUG
#define STR(x) #x
#define glAssert(code) \
    code; \
{\
    GLuint err = glGetError(); \
    if (err != GL_NO_ERROR) { \
        std::cerr<<"Erreur OpenGL ("<<__FILE__<<":"<<__LINE__<<", "<<STR(code)<<") : "<<(const char*)gluErrorString (err)<<"("<<err<<")"<<std::endl; \
    } \
}
#else
#define glAssert(code) code;
#endif

#endif // GLASSERT

