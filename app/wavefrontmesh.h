#ifndef WAVEFRONTMESH_H
#define WAVEFRONTMESH_H

#include <SparrowRenderer/meshbuilder.h>
#include <QDialog>
#include <QThread>
#include <unordered_map>
#include <string>
#include <vector>

class Texture;
class EditorImage;
class QProgressBar;
class QLabel;
class QGroupBox;

class MaterialMap
{
public:
    std::vector<std::string> names;
    std::unordered_map<std::string, Material*> data;

    void add(const std::string &name, Material* t)
    {
        data[name] = t;
        names.push_back(name);
    }

    Material* get(const std::string &name)
    {
        if(data.count(name))
            return data[name];
        else
            return NULL;
    }
};

class TextureLoader : public QThread
{
    Q_OBJECT

    struct ToLoad
    {
        QString filename;
        Texture* &target;
    };

    std::vector<ToLoad> todos;
    std::vector<EditorImage*> images;
    int current;
    bool canceled;

protected:
    virtual void run();

public:
    TextureLoader() : canceled(false) {}
    void addTextureToLoad(QString filename, Texture* &target)
    {
        ToLoad todo = {filename, target};
        todos.push_back(todo);
    }
    void cancel() {canceled = true;}
    void gl_finish();

signals:
    void progressChanged(QString, int);
};

class MeshLoader : public QThread
{
    Q_OBJECT

    bool ok;
    bool canceled;
    QString filename;

    std::vector<Mesh*> meshes;

    bool loadMTL(QString filename, MaterialMap &materials);

protected:
    virtual void run();

public:
    TextureLoader textureLoader;

    MeshLoader(QString wavefrontFilename) :
        ok(false),
        canceled(false),
        filename(wavefrontFilename)
    {}

    std::vector<Mesh*> getMeshes() {return meshes;}

    bool isOk() {return ok;}
    bool isCanceled() {return canceled;}

    void cancelLoading() {canceled = true;}

signals:
    void progressChanged(int);
};

class WavefrontMesh : public QDialog
{
    Q_OBJECT

    QLabel *textureLabel;
    QProgressBar *meshProgressBar;
    QProgressBar *textureProgressBar;
    QGroupBox *groupBox;

private slots:
    void textureProgress(QString, int);

public:
    WavefrontMesh(QWidget *parent = NULL);

    std::vector<Mesh*> loadMesh(QString wavefrontFilename, bool noDialog = false);
};

#endif // WAVEFRONTMESH_H
