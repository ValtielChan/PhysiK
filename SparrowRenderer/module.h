#ifndef MODULE
#define MODULE

class Camera;
class Scene;

class Module
{
public:
    virtual void renderGL(Camera* myCamera, Scene* scene) = 0;
    virtual bool requiresModernOpenGL() {return true;}
    virtual void resize(int width, int height) {}
    virtual ~Module(){}
};

#endif // MODULE

