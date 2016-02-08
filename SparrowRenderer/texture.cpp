#include "texture.h"
#include "glassert.h"
#include "image.h"

Texture::Texture(GLenum format,
                 GLenum internal_format,
                 int width,
                 int height,
                 GLenum dataType,
                 GLenum texTarget) :
    m_target(texTarget),
    m_format(format),
    m_internal_format(internal_format),
    m_width(width),
    m_height(height),
    m_dataType(dataType)
{
    glAssert(glGenTextures(1, &texId));
    glAssert(glBindTexture(m_target, texId));
    switch(m_target)
    {
    case GL_TEXTURE_2D :
        glAssert(glTexImage2D(m_target, 0, internal_format, width, height, 0, format, dataType, NULL));
        setWrap(GL_REPEAT);
        setFiltering(GL_LINEAR);
        break;
    case GL_TEXTURE_RECTANGLE :
        glAssert(glTexImage2D(m_target, 0, internal_format, width, height, 0, format, dataType, NULL));
        setWrap(GL_CLAMP_TO_EDGE);
        setFiltering(GL_LINEAR);
        break;
    case GL_TEXTURE_CUBE_MAP :
        for(int i=0; i<6; ++i)
            glAssert(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internal_format, width, height, 0, format, dataType, NULL));
        setWrap(GL_CLAMP_TO_EDGE);
        setFiltering(GL_LINEAR);
        break;
    }
}

Texture::Texture(Image* myImage) :
    m_target(GL_TEXTURE_2D),
    m_width(myImage->width),
    m_height(myImage->height),
    m_dataType(GL_UNSIGNED_BYTE)
{
    glAssert(glGenTextures(1, &texId));
    glAssert(glBindTexture(m_target, texId));
    initPixels(myImage, GL_TEXTURE_2D);
    setWrap(GL_REPEAT);
    setFiltering(GL_LINEAR);
}

Texture::Texture(Image* myCubemapImages[6]) :
    m_target(GL_TEXTURE_CUBE_MAP),
    m_width(myCubemapImages[0]->width),
    m_height(myCubemapImages[0]->height),
    m_dataType(GL_UNSIGNED_BYTE)
{
    glAssert(glGenTextures(1, &texId));
    glAssert(glBindTexture(m_target, texId));
    for(int i=0; i<6; ++i)
        initPixels(myCubemapImages[i], GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);
    setWrap(GL_CLAMP_TO_EDGE);
    setFiltering(GL_LINEAR);
}

Texture::Texture(Texture* tex, bool halfDim) :
    m_target(tex->m_target),
    m_format(tex->m_format),
    m_internal_format(tex->m_internal_format),
    m_width(tex->m_width),
    m_height(tex->m_height),
    m_dataType(tex->m_dataType)
{
    glAssert(glGenTextures(1, &texId));
    glAssert(glBindTexture(m_target, texId));
    if(halfDim)
    {
        m_width /= 2;
        m_height /= 2;
    }
    glAssert(glTexImage2D(m_target, 0, m_internal_format, m_width, m_height, 0, m_format, m_dataType, NULL));
}

Texture::~Texture()
{
    glAssert(glDeleteTextures(1, &texId));
}

void Texture::initPixels(Image* myImage, GLenum target)
{
    switch(myImage->depth)
    {
    case 32:
        m_format = GL_RGBA;
        m_internal_format = GL_RGBA;
        break;
    case 24:
        m_format = GL_RGB;
        m_internal_format = GL_RGB;
        break;
    case 8:
        m_format = GL_RED;
        m_internal_format = GL_R8;
        break;
    }
    glAssert(glTexImage2D(target, 0, m_internal_format, m_width, m_height, 0, m_format, m_dataType, myImage->pixels));
}

void Texture::setWrap(GLint wrap)
{
    glAssert(glTexParameteri(m_target, GL_TEXTURE_WRAP_S, wrap));
    glAssert(glTexParameteri(m_target, GL_TEXTURE_WRAP_T, wrap));
    glAssert(glTexParameteri(m_target, GL_TEXTURE_WRAP_R, wrap));
}

void Texture::setFiltering(GLint filter)
{
    glAssert(glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, filter));
    glAssert(glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, filter));
}

void Texture::bind(int slot)
{
    glAssert(glActiveTexture(GL_TEXTURE0+slot));
    glAssert(glBindTexture(m_target, texId));
}

