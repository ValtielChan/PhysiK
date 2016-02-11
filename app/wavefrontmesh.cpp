#include "wavefrontmesh.h"
#include <QFile>
#include <QTextStream>
#include <QFileInfo>
#include <cstdio>
#include <QCoreApplication>
#include <QProgressBar>
#include <QPushButton>
#include <QSpacerItem>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <SparrowRenderer/phongmaterial.h>
#include "qtutils.h"
#include <SparrowRenderer/texture.h>

void TextureLoader::run()
{
    while(images.size() < todos.size())
    {
        ToLoad &todo = todos[images.size()];
        QString texturePath = path + "/" + todo.filename;
        if(QFile(texturePath).exists())
            images.push_back(QtUtils::loadImage(texturePath));
        else
        {
            fprintf(stderr, "can't load texture : %s\n", texturePath.toStdString().c_str());
            images.push_back(NULL);
        }
        emit progressChanged(images.size() < todos.size() ? QString("Loading %1 ...").arg(todo.filename) : "Done.",
                             (images.size()*100) / todos.size());
        if(canceled)
        {
            for(int i=0; i<images.size(); ++i)
            {
                delete(images[i]);
                delete(todos[i].target);
            }
            break;
        }
    }
}

void TextureLoader::gl_finish()
{
    for(int i=0; i<images.size(); ++i)
    {
        images[i]->initGL();
        todos[i].target = images[i]->texture;
        delete images[i];
    }
}

std::vector<Mesh*> WavefrontMesh::loadMesh(QString wavefrontFilename, bool noDialog)
{
    std::vector<Mesh*> meshes;
    MeshLoader* loader = new MeshLoader(wavefrontFilename);
    QFileInfo fileInfo(wavefrontFilename);
    loader->textureLoader.setTexturePath(fileInfo.absolutePath());
    if(!noDialog)
    {
        setWindowTitle(QString("Loading %1").arg(fileInfo.fileName()));
        connect(loader, SIGNAL(progressChanged(int)), meshProgressBar, SLOT(setValue(int)));
        connect(loader, SIGNAL(finished()), this, SLOT(accept()));
        connect(&(loader->textureLoader), SIGNAL(progressChanged(QString,int)), this, SLOT(textureProgress(QString,int)));
    }
    loader->start();
    if(noDialog)
    {
        loader->wait();
        loader->textureLoader.gl_finish();
        meshes = loader->getMeshes();
    }
    else
    {
        int ret = exec();
        if(ret == Accepted)
        {
            loader->textureLoader.gl_finish();
            meshes = loader->getMeshes();
        }
        else
        {
            loader->cancelLoading();
            loader->wait();
        }
    }
    delete(loader);
    for(Mesh* m : meshes)
    {
        if(!m->hasNormals())
            m->computeNormals();
        if(m->material->getFlags() & NORMAL_MAP_FLAG)
            m->computeTangents();
        m->mergeVertices();
    }
    return meshes;
}

void WavefrontMesh::textureProgress(QString label, int progress)
{
    textureLabel->setText(label);
    textureProgressBar->setValue(progress);
}

WavefrontMesh::WavefrontMesh(QWidget* parent) :
    QDialog(parent)
{
    QGridLayout *gridLayout;
    QPushButton *cancelButton;
    QSpacerItem *verticalSpacer;
    QSpacerItem *horizontalSpacer;
    QVBoxLayout *verticalLayout;
    QLabel *label;

    if (objectName().isEmpty())
        resize(400, 150);
    setModal(true);
    gridLayout = new QGridLayout(this);

    cancelButton = new QPushButton(this);
    cancelButton->setText("Cancel");
    gridLayout->addWidget(cancelButton, 2, 1, 1, 1);

    verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    gridLayout->addItem(verticalSpacer, 1, 1, 1, 1);
    horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    gridLayout->addItem(horizontalSpacer, 2, 0, 1, 1);

    groupBox = new QGroupBox(this);
    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
    groupBox->setSizePolicy(sizePolicy);
    verticalLayout = new QVBoxLayout(groupBox);

    textureLabel = new QLabel(groupBox);
    sizePolicy.setHeightForWidth(textureLabel->sizePolicy().hasHeightForWidth());
    textureLabel->setSizePolicy(sizePolicy);
    verticalLayout->addWidget(textureLabel);

    textureProgressBar = new QProgressBar(groupBox);
    textureProgressBar->setValue(0);
    verticalLayout->addWidget(textureProgressBar);

    label = new QLabel(groupBox);
    label->setText("Loading Mesh...");
    sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
    label->setSizePolicy(sizePolicy);
    verticalLayout->addWidget(label);

    meshProgressBar = new QProgressBar(groupBox);
    meshProgressBar->setValue(0);
    verticalLayout->addWidget(meshProgressBar);

    gridLayout->addWidget(groupBox, 0, 0, 1, 2);

    connect(cancelButton, SIGNAL(pressed()), this, SLOT(reject()));
}

void MeshLoader::run()
{
    MaterialMap materials;
    std::vector<glm::vec3> pos;
    std::vector<glm::vec3> norm;
    std::vector<glm::vec2> tex;
    QString line;
    QStringList list;
    QStringList faceList;
    Material* defaultMat = materials.get("default");
    if(defaultMat == NULL)
    {
        defaultMat = new PhongMaterial();
        materials.add("default", defaultMat);
    }
    Material* currentMat = defaultMat;

    QFile f(filename);
    QFileInfo info(f);
    if(!f.open(QFile::ReadOnly | QFile::Text))
    {
        fprintf(stderr, "can't load %s.\n", filename.toStdString().c_str());
        return;
    }
    QTextStream in(&f);

    quint64 readChars = 0;
    int currentProgress = 0;

    MeshBuilder* currentMesh = new MeshBuilder();
    meshes.push_back(currentMesh);
    currentMesh->setMaterial(currentMat);

    line = in.readLine();
    while(!line.isNull())
    {
        if(line.isEmpty())
        {
            line = in.readLine();
            continue;
        }

        // update progress
        if(canceled)
        {
            textureLoader.cancel();
            textureLoader.wait();
            return;
        }
        readChars += line.size() + 1;
        int step = readChars*100/info.size();
        if(step != currentProgress)
        {
            currentProgress = step;
            emit progressChanged(step);
        }

        switch(line.at(0).toLatin1())
        {
        case 'v':
            //vertex attribute
            list = line.split(QChar(' '));
            switch(line.at(1).toLatin1())
            {
            case ' ':
                // vertex position
                pos.push_back(glm::vec3(list[1].toFloat(), list[2].toFloat(), list[3].toFloat()));
                break;
            case 't':
                // texCoord
                tex.push_back(glm::vec2(list[1].toFloat(), list[2].toFloat()));
                break;
            case 'n':
                // normal
                norm.push_back(glm::vec3(list[1].toFloat(), list[2].toFloat(), list[3].toFloat()));
                break;
            }
            break;
        case 'f':
            {
                // face
                list = line.split(QChar(' '));
                int nb_vertices = currentMesh->positions.size();
                currentMesh->addTriangle(nb_vertices, nb_vertices+1, nb_vertices+2);
                for(int i=0; i<3; ++i)
                {
                    faceList = list[i+1].split(QChar('/'));
                    if(norm.size() == 0)
                    {
                        if(tex.size() == 0)
                            currentMesh->addPosition(pos[faceList[0].toInt()-1]);
                        else
                            currentMesh->addVertex(pos[faceList[0].toInt()-1],
                                                   tex[faceList[1].toInt()-1]);
                    }
                    else
                    {
                        if(tex.size() == 0)
                            currentMesh->addVertex(pos[faceList[0].toInt()-1],
                                                   norm[faceList[2].toInt()-1]);
                        else
                            currentMesh->addVertex(pos[faceList[0].toInt()-1],
                                                   norm[faceList[2].toInt()-1],
                                                   tex[faceList[1].toInt()-1]);
                    }
                }
            }
            break;
        case 'g':
            currentMesh = new MeshBuilder();
            meshes.push_back(currentMesh);
            currentMesh->setMaterial(currentMat);
            break;
        case 'm':
            // mtllib
            loadMTL(info.absolutePath().append("/").append(line.split(' ').back()), materials);
            break;
        case 'u':
            // usemtl
        {
            std::string mat_name = line.split(' ').back().toStdString();
            currentMat = materials.get(mat_name);
            if(currentMat == NULL)
            {
                fprintf(stderr, "cannot find any material named : %s.\n", mat_name.c_str());
                currentMat = new PhongMaterial();
                materials.add(mat_name, currentMat);
            }
            currentMesh->setMaterial(currentMat);
        }
            break;
        default:
        case '#':
            // comment
            break;
        }
        line = in.readLine();
    }

    for(int i=0; i<meshes.size(); ++i)
    {
        if(meshes[i]->indices.size() == 0)
        {
            meshes[i] = meshes.back();
            meshes.pop_back();
            --i;
        }
    }

    // TODO : optimisation -> merge groups with same material
    textureLoader.wait();

    ok = true;
}

bool MeshLoader::loadMTL(QString filename, MaterialMap &materials)
{
    QFile f(filename);
    if(!f.open(QFile::ReadOnly | QFile::Text))
    {
        fprintf(stderr, "can't load %s.\n", filename.toStdString().c_str());
        return false;
    }
    QTextStream in(&f);
    QFileInfo info(f);
    PhongMaterial* mat = NULL;
    bool hasNormalMap = false;

    QString line = in.readLine();
    while(!line.isNull())
    {
        if(line.isEmpty())
        {
            line = in.readLine();
            continue;
        }
        QStringList splitter = line.split(' ');

        if(splitter[0].startsWith("#"))
        {
            // this is a comment
        }
        else if(splitter[0].compare("newmtl") == 0 && splitter.size() == 2)
        {
            mat = new PhongMaterial();
            materials.add(splitter[1].toStdString(), mat);
        }
        else if(splitter[0].compare("Ka") == 0 && splitter.size() == 4)
        {
            mat->ambient.r = splitter[1].toFloat();
            mat->ambient.g = splitter[2].toFloat();
            mat->ambient.b = splitter[3].toFloat();
        }
        else if(splitter[0].compare("Kd") == 0 && splitter.size() == 4)
        {
            mat->diffuse.r = splitter[1].toFloat();
            mat->diffuse.g = splitter[2].toFloat();
            mat->diffuse.b = splitter[3].toFloat();
        }
        else if(splitter[0].compare("Ks") == 0 && splitter.size() == 4)
        {
            mat->specular.r = splitter[1].toFloat();
            mat->specular.g = splitter[2].toFloat();
            mat->specular.b = splitter[3].toFloat();
        }
        else if(splitter[0].compare("Ns") == 0 && splitter.size() == 2)
        {
            mat->shininess = splitter[1].toFloat();
        }
        else if(splitter[0].startsWith("map_") && splitter.size() == 2)
        {
            if(splitter[0].compare("map_Ka") == 0)
                textureLoader.addTextureToLoad(splitter[1], mat->ambient_texture);
            else if(splitter[0].compare("map_Kd") == 0)
                textureLoader.addTextureToLoad(splitter[1], mat->diffuse_texture);
            else if(splitter[0].compare("map_Ks") == 0)
                textureLoader.addTextureToLoad(splitter[1], mat->specular_texture);
            else if(splitter[0].compare("map_Normal") == 0)
            {
                textureLoader.addTextureToLoad(splitter[1], mat->normal_map);
                hasNormalMap = true;
            }
            else if(splitter[0].compare("map_d") == 0)
                textureLoader.addTextureToLoad(splitter[1], mat->alpha_mask);
            else
                fprintf(stderr, "unsupported material property : \"%s\"\n", splitter[0].toStdString().c_str());
        }
        else
            fprintf(stderr, "unsupported material property : \"%s\"\n", splitter[0].toStdString().c_str());

        line = in.readLine();
    }
    if(!textureLoader.isRunning())
        textureLoader.start();
    return hasNormalMap;
}
