#ifndef ENTITY
#define ENTITY

class Entity
{
public:
    virtual void draw(const glm::mat4 viewMatrix, const glm::mat4 projectionMatrix) = 0;
};

#endif // ENTITY

