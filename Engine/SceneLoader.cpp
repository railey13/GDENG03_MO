#include "SceneLoader.h"
#include <iostream>

#include <fstream>
#include <string>
#include <sstream>
#include <filesystem>
#include <yaml-cpp/yaml.h>
#include "AppWindow.h"
#include "GameObjectTypes.h"
#include "TextureComponent.h"
#include "PhysicsSystem.h"
#include "PhysicsComponent.h"

#include <vector>
SceneLoader* SceneLoader::sharedInstance = NULL;

SceneLoader::SceneLoader()
{
}

SceneLoader::~SceneLoader()
{
    sharedInstance = nullptr;
}

void SceneLoader::initialize() {
    if (sharedInstance != NULL) throw std::exception("SceneLoader already exists");

    sharedInstance = new SceneLoader();
    //sharedInstance->loadScene("test");
}

void SceneLoader::destroy()
{
    delete sharedInstance;
}

SceneLoader* SceneLoader::get()
{
    return sharedInstance;
}

void SceneLoader::loadScene(const char* filePath)
{
    AppWindow::get()->sceneLoading = true;
    AppWindow::get()->DestroyAllObjects();
    AppWindow::get()->gamecamera = false;


    try
    {
        YAML::Node scene = YAML::LoadFile(filePath);

        std::cout << "Successfully opened " << filePath << "\n";

        YAML::Node objects = scene["Scene Objects"];

        GameObject* Parent = nullptr;

        for (const YAML::Node& object : objects)
        {
            
            loadGameObject(object,Parent);
          
            
        }
    }
    catch (const YAML::Exception& e)
    {
        std::cout << "Failed to load YAML scene: "
            << e.what() << "\n";

        
        return;
    }

    AppWindow::get()->sceneLoading = false;
    
}

void SceneLoader::loadGameObject(const YAML::Node& object, GameObject*Parent)
{
    // -------------------------
            // GameObject Type
            // -------------------------

    std::string type = object["GameObjectType"].as<std::string>();

    GameObjectTypes objectToSpawn;

    if (type == "CUBE")
    {
        objectToSpawn = GameObjectTypes::CUBE;
    }
    else if (type == "SPHERE")
    {
        objectToSpawn = GameObjectTypes::SPHERE;
    }
    else if (type == "PLANE")
    {
        objectToSpawn = GameObjectTypes::PLANE;
    }
    else if (type == "CAPSULE")
    {
        objectToSpawn = GameObjectTypes::CAPSULE;
    }
    else if (type == "GAME_CAMERA")
    {
        objectToSpawn = GameObjectTypes::GAME_CAMERA;
    }
    else if (type == "ARMADILLO")
    {
        objectToSpawn = GameObjectTypes::ARMADILLO;
    }
    else if (type == "POT")
    {
        objectToSpawn = GameObjectTypes::POT;
    }
    else if (type == "BUNNY")
    {
        objectToSpawn = GameObjectTypes::BUNNY;
    }
    else if (type == "LUCY")
    {
        objectToSpawn = GameObjectTypes::LUCY;
    }
    else
    {
        std::cout << "Unknown GameObjectType: " << type << "\n";
       
    }

    GameObject* objectCreated =
        AppWindow::get()->SpawnGameObject(objectToSpawn);

    if (Parent) objectCreated->setParent(Parent);

    // -------------------------
    // Name
    // -------------------------

    std::string name = object["Name"].as<std::string>();

    objectCreated->setName(name);


    // -------------------------
    // Position
    // -------------------------

    YAML::Node position = object["Position"];

    float x = position[0].as<float>();
    float y = position[1].as<float>();
    float z = position[2].as<float>();

    objectCreated->getTransform()->setPosition(
        Vector3D(x, y, z)
    );


    // -------------------------
    // Rotation
    // -------------------------

    YAML::Node rotation = object["Rotation"];

    x = rotation[0].as<float>();
    y = rotation[1].as<float>();
    z = rotation[2].as<float>();

    objectCreated->getTransform()->setRotation(
        Vector3D(x, y, z)
    );


    // -------------------------
    // Scale
    // -------------------------

    YAML::Node scale = object["Scale"];

    x = scale[0].as<float>();
    y = scale[1].as<float>();
    z = scale[2].as<float>();

    objectCreated->getTransform()->setScale(
        Vector3D(x, y, z)
    );


    std::cout << "Type: " << type << "\n";
    std::cout << "Name: " << name << "\n";

    // -------------------------
    // Texture
    // -------------------------

    std::string texPath = object["Texture"].as<std::string>();
    
    if (texPath != "NONE") {
        texPath =  std::filesystem::absolute(texPath).string();
        objectCreated->createComponent<TextureComponent>();
        objectCreated->getComponent<TextureComponent>()->setTexturePath(texPath);
    }
    
    // -------------------------
    // RigidBody
    // -------------------------

    std::string rbt = object["RigidBodyType"].as<std::string>();
    if (rbt != "NONE") {

        PhysicsComponent* rb = PhysicsSystem::get()->createComponent(
            objectCreated, PhysicsComponent::BodyType::DYNAMIC);
        rb->addBoxColliderFromScale();
        rb->enableGravity(true);

        if (rbt == "STATIC")
        {
            rb->setBodyType(PhysicsComponent::BodyType::STATIC);
        }
        else if (rbt == "KINEMATIC")
        {
            rb->setBodyType(PhysicsComponent::BodyType::KINEMATIC);
        }
        else if (rbt == "DYNAMIC")
        {
            rb->setBodyType(PhysicsComponent::BodyType::DYNAMIC);
        }

    }
    // -------------------------
    // Children
    // -------------------------
    YAML::Node children = object["Children"];
    if (children && children.IsSequence()) {
        for (const YAML::Node& child : children)
        {
            
            loadGameObject(child, objectCreated);
            

        }
    }
    
}

void SceneLoader::SaveScene(const std::string& filePath)
{
    YAML::Emitter out;

    out << YAML::BeginMap;

    out << YAML::Key << "Scene Objects";
    out << YAML::Value << YAML::BeginSeq;

    std::vector<GameObject*> sceneObjects = AppWindow::get()->getGameObjects();

    for (GameObject* object : sceneObjects)
    {
        if (object->getParent() == nullptr)
        {
            SaveGameObject(out, object);
        }
      
    }
    out << YAML::EndSeq;
    out << YAML::EndMap;

    std::ofstream file(filePath);

    if (file.is_open())
    {
        file << out.c_str();
        file.close();
    }

}

void SceneLoader::SaveGameObject(YAML::Emitter& out, GameObject* object)
{
    out << YAML::BeginMap;

    GameObjectTypes objectType = object->m_gameObjectType;
    std::string objectTypeString;
    if (objectType == GameObjectTypes::CUBE)
    {
        objectTypeString = "CUBE";
    }
    else if (objectType == GameObjectTypes::SPHERE)
    {
        objectTypeString = "SPHERE";
    }
    else if (objectType == GameObjectTypes::PLANE)
    {
        objectTypeString = "PLANE";
    }
    else if (objectType == GameObjectTypes::CAPSULE)
    {
        objectTypeString = "CAPSULE";
    }
    else if (objectType == GameObjectTypes::GAME_CAMERA)
    {
        objectTypeString = "GAME_CAMERA";
    }
    else if (objectType == GameObjectTypes::ARMADILLO)
    {
        objectTypeString = "ARMADILLO";
    }
    else if (objectType == GameObjectTypes::BUNNY)
    {
        objectTypeString = "BUNNY";
    }
    else if (objectType == GameObjectTypes::POT)
    {
        objectTypeString = "POT";
    }
    else if (objectType == GameObjectTypes::LUCY)
    {
        objectTypeString = "LUCY";
    }
    out << YAML::Key << "GameObjectType"
        << YAML::Value << objectTypeString;

    out << YAML::Key << "Name"
        << YAML::Value << object->getName();

    out << YAML::Key << "Position"
        << YAML::Value << YAML::Flow
        << YAML::BeginSeq
        << object->getTransform()->getPosition().m_x
        << object->getTransform()->getPosition().m_y
        << object->getTransform()->getPosition().m_z
        << YAML::EndSeq;

    out << YAML::Key << "Rotation"
        << YAML::Value << YAML::Flow
        << YAML::BeginSeq
        << object->getTransform()->getRotation().m_x
        << object->getTransform()->getRotation().m_y
        << object->getTransform()->getRotation().m_z
        << YAML::EndSeq;

    out << YAML::Key << "Scale"
        << YAML::Value << YAML::Flow
        << YAML::BeginSeq
        << object->getTransform()->getScale().m_x
        << object->getTransform()->getScale().m_y
        << object->getTransform()->getScale().m_z
        << YAML::EndSeq;

    std::string TexPath;
    if (object->getComponent<TextureComponent>() == NULL) TexPath = "NONE";
    else {
        TexPath = object->getComponent<TextureComponent>()->getTexturePath();
        TexPath = std::filesystem::relative(TexPath, std::filesystem::current_path()).string();
    }
    out << YAML::Key << "Texture"
        << YAML::Value << TexPath;


    PhysicsComponent::BodyType bt;
    std::string bodyType;
    if (object->getComponent<PhysicsComponent>() == NULL) {
        bodyType = "NONE";

    }
    else {
        bt = object->getComponent<PhysicsComponent>()->getBodyType();
        if (bt == PhysicsComponent::BodyType::STATIC)
        {
            bodyType = "STATIC";
        }
        else if (bt == PhysicsComponent::BodyType::KINEMATIC)
        {
            bodyType = "KINIMATIC";
        }
        else if (bt == PhysicsComponent::BodyType::DYNAMIC)
        {
            bodyType = "DYNAMIC";
        }
    }
    out << YAML::Key << "RigidBodyType"
        << YAML::Value << bodyType;


    
    if (!object->getChildren().empty())
    {
        out << YAML::Key << "Children";
        out << YAML::Value << YAML::BeginSeq;

        for (GameObject* child : object->getChildren())
        {
            SaveGameObject(out, child);
        }

        out << YAML::EndSeq;
    }
    else
    {
        out << YAML::Key << "Children"
            << YAML::Value << "NONE";
    }

    out << YAML::EndMap;

}


