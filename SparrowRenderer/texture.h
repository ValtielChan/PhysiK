#ifndef TEXTURE_H
#define TEXTURE_H

#include "glew.h"
#include <string>

class Image;

class Texture
{
private:
    GLuint texId;
    GLenum m_target;
    GLenum m_format;
    GLenum m_internal_format;
    int m_width;
    int m_height;
    GLenum m_dataType;

    void initPixels(Image* myImage, GLenum textureSlot);
public:
    // creates a 2D texture from perlin noise
    Texture();
    // creates an empty 2D texture
    Texture(GLenum format = GL_RGBA,
            GLenum internal_format = GL_RGBA,
            int width = 512,
            int height = 512,
            GLenum dataType = GL_UNSIGNED_BYTE,
            GLenum texTarget = GL_TEXTURE_2D);
    // creates a standard texture from an image
    Texture(Image* myImage);
    // creates a cubeMap from 6 images
	Texture(Image* myCubemapImages[6]);
    // creates a texture from another
    Texture(Texture* tex, bool halfDim = false);

    ~Texture();
    void bind(int slot);
    GLuint getId() {return texId;}
    GLenum getTarget() {return m_target;}
    void setWrap(GLint wrap);
    void setFiltering(GLint filter);
    bool isCubeMap() {return m_target == GL_TEXTURE_CUBE_MAP;}

    int getWidth() {return m_width;}
    int getHeight() {return m_height;}
};

#endif // TEXTURE_H
