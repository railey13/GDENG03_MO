#include "CameraHandler.h"

CameraHandler* CameraHandler::sharedInstance = NULL;

CameraHandler* CameraHandler::get() {
    return sharedInstance;
}

void CameraHandler::initialize() {
    sharedInstance = new CameraHandler();
}

void CameraHandler::destroy() {
    delete sharedInstance;
}

CameraHandler::CameraHandler() {
    m_sceneCamera = new Camera();
}

CameraHandler::~CameraHandler() {

}

CameraHandler::CameraHandler(CameraHandler const&) {

}

void CameraHandler::setGameCamera(GameCamera* gameCamera) {
    m_gameCamera = gameCamera;
}

Camera* CameraHandler::getSceneCamera() {
    return m_sceneCamera;
}

GameCamera* CameraHandler::getGameCamera() {
    return m_gameCamera;
}
