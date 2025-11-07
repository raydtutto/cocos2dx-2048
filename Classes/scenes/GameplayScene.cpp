//
// Created by Chocobo on 07/11/2025.
//

#include "GameplayScene.h"
#include "cocostudio/ActionTimeline/CSLoader.h"

using namespace cocos2d;

GameplayScene* GameplayScene::create() {
    GameplayScene *pScene = new(std::nothrow) GameplayScene();
    if (pScene && pScene->initWithCSB("GUI/cocosstudio/widgets/gameplayScene.csb")) {
        pScene->autorelease();
        return pScene;
    }
    CC_SAFE_DELETE(pScene);
    return nullptr;
}

bool GameplayScene::initWithCSB(const std::string& path) {
    auto scene = CSLoader::createNode(path);
    if (scene && SceneBase::init()) {
        CC_SAFE_RELEASE(mRootNode);
        mRootNode = scene;
        addChild(mRootNode);
        return true;
    }

    return false;
}
