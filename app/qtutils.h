#ifndef QTUTILS_H
#define QTUTILS_H

#include <glm/vec3.hpp>
#include <SparrowRenderer/image.h>
#include <QImage>
#include <SparrowRenderer/texture.h>

class Texture;
class QString;
class QColor;

struct EditorImage : public Image
{
    EditorImage(const QString &filename) : myQImage(filename) {}
    EditorImage(int myWidth, int myHeight, float frequency, float amplitude);
    QImage myQImage;
    Texture* texture;
    void initGL()
    {
        texture = new Texture(this);
    }
};

class QtUtils
{
public:
    static QString fileToString(const QString &filename);
    static EditorImage* loadImage(const QString &filename);
    static QColor toQColor(const glm::vec3 &color);
    static glm::vec3 toVec3(const QColor &color);
};

#endif // QTUTILS_H
