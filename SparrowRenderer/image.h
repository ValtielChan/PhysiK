#ifndef IMAGE
#define IMAGE

#include <cstddef>
#include <glm/gtc/noise.hpp>

struct Image
{
    int width;
    int height;
    int depth;
    void* pixels;

    Image() : pixels(NULL) {}

    Image(int myDepth, int myWidth, int myHeight, float frequency, float amplitude) :
        width(myWidth),
        height(myHeight),
        depth(myDepth)
    {
        allocate(width * height * (depth/8));
        unsigned char* data = (unsigned char*)pixels;

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
                    data[((row * width + col) * 4) + oct] =
                          (unsigned char) ( result * 255.0f );
                    freq *= 2.0f;   // Double the frequency
                    scale *= amplitude;     // Next power of b
                }
            }
        }
    }

    ~Image()
    {
        if(pixels != NULL)
            delete[] (char*)pixels;
    }

    void allocate(int size)
    {
        pixels = new char[size];
    }
};

#endif // IMAGE

