#pragma once

#include "GameObject.h"
#include "Vector3D.h"
#include <yaml-cpp/yaml.h>

class SceneLoader
{
public:
    SceneLoader();
    ~SceneLoader();
    static void initialize();
    static void destroy();
    static SceneLoader* get();
private:
    SceneLoader (SceneLoader* const&);
    SceneLoader& operator=(SceneLoader* const&) {};
    static SceneLoader* sharedInstance;

public:
    void loadScene(const char* filePath);
    void loadGameObject(const YAML::Node& object, GameObject* Parent);
    void SaveScene(const std::string& filePath);
    void SaveGameObject(YAML::Emitter& out, GameObject* object);
};


