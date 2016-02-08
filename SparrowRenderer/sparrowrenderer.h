#ifndef SPARROWRENDERER_H
#define SPARROWRENDERER_H

#include "glew.h"
#include <vector>
#include <string>
#include <glm/vec3.hpp>

class Camera;
class Module;
class Scene;

class SparrowRenderer
{
public:
    SparrowRenderer() :
        camera(NULL),
        scene(NULL),
        clearColor(0)
    {}

    // main methods
    void initGL(int w, int h, bool forceCrappy = false);
    void destroyGL();
    void resizeGL(int w, int h);
	void renderGL();
    static bool isModernOpenGLAvailable();
    void setClearColor(glm::vec3 color) {clearColor=color;}

    // clock methods
    double getFPS();
    double getModuleTime(int moduleId);

    // modules methods
	void addModule(Module* myModule, std::string name);
	int getNbModules();

    void setModuleEnabled(std::string module, bool isEnabled);

    // camera methods
    void setCamera(Camera* myCamera);
    Camera* getCamera();

    // scene methods
    void setScene(Scene* myScene);
    Scene* getScene();

protected:
    typedef struct s_moduleNode{
        Module* module;
        std::string name;
        bool isEnabled;
        double seconds;

        s_moduleNode(Module* myModule, const std::string &myName) : module(myModule), name(myName), isEnabled(true) {}
    } ModuleNode;

    int width;
    int height;
    Camera* camera;
    Scene* scene;
    std::vector<ModuleNode> modules;
    static bool modernOpenglAvailable;
    glm::vec3 clearColor;

    std::vector<ModuleNode>::iterator getModuleNode(std::string name);
};

#endif // SPARROWRENDERER_H
