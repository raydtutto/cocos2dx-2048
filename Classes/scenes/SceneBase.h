#ifndef INC_2048_GAME_SCENEBASE_H
#define INC_2048_GAME_SCENEBASE_H

#include "cocos2d.h"

class SceneBase : public cocos2d::Scene {
protected:
    virtual bool initWithCSB(const std::string& path) = 0;

    cocos2d::Node* mRootNode{nullptr};
};


#endif //INC_2048_GAME_SCENEBASE_H