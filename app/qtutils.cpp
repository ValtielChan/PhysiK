#include "qtutils.h"
#include <cstring>
#include <QFile>
#include <QTextStream>
#include <QColor>
#include <QGLWidget>
#include <glm/ext.hpp>

QString QtUtils::fileToString(const QString &filename)
{
    QFile f(filename);
    if(!f.open(QFile::ReadOnly | QFile::Text))
        return QString();
    QTextStream in(&f);
    return in.readAll();
}

EditorImage* QtUtils::loadImage(const QString &filename)
{
    EditorImage* img = new EditorImage(filename);
    QImage glImage = QGLWidget::convertToGLFormat(img->myQImage);
    img->depth = glImage.depth();
    img->width = glImage.width();
    img->height = glImage.height();
    img->allocate(glImage.byteCount());
    memcpy(img->pixels, glImage.bits(), glImage.byteCount());
    return img;
}

QColor QtUtils::toQColor(const glm::vec3 &color)
{
    return QColor((int)(color.r*255), (int)(color.g*255), (int)(color.b*255));
}

glm::vec3 QtUtils::toVec3(const QColor &color)
{
    return glm::vec3(color.redF(), color.greenF(), color.blueF());
}

EditorImage::EditorImage(int myWidth, int myHeight, float frequency, float amplitude)
{
    width = myWidth;
    height = myHeight;
    depth = 32;
    Image::allocate(width*height*depth);

    GLubyte *data = (GLubyte*)Image::pixels;

    float xFactor = 1.0f / (width - 1);
    float yFactor = 1.0f / (height - 1);

    for( int row = 0; row < height; row++ ) {
        for( int col = 0 ; col < width; col++ ) {
            float x = xFactor * col;
            float y = yFactor * row;
            float sum = 0.0f;
            float freq = frequency;
            float scale = amplitude;

            // Compute the sum for each octave
            for( int oct = 0; oct < 4 ; oct++ ) {
                glm::vec2 p(x * freq, y * freq);
                float val = glm::perlin(p, glm::vec2(freq)) / scale;
                sum += val;
                float result = (sum + 1.0f)/ 2.0f;

                // Store in texture buffer
                data[((row * width + col) * 4) + oct] = (GLubyte) ( result * 255.0f );
                freq *= 2.0f;   // Double the frequency
                scale *= amplitude;     // Next power of b
            }
        }
    }
    myQImage = QImage((uchar*)data, myWidth, myHeight, QImage::Format_RGB32);
}
