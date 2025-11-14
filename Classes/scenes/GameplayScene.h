#ifndef INC_2048_GAME_GAMEPLAYSCENE_H
#define INC_2048_GAME_GAMEPLAYSCENE_H


#include "SceneBase.h"
#include <map>

struct TileInfo {
    cocos2d::Node* pNode = nullptr;
    int num = 0;
};

class GameplayScene : public SceneBase {
public:
    static GameplayScene* create();
    void onEnter() override;

protected:
    bool initWithCSB(const std::string &path) override;
    void fillGrid();
    void touchHandler();

private:
    std::map<int, std::map<int, TileInfo>> mGrid;
    cocos2d::Vec2 mInitTouchPos = cocos2d::Vec2::ZERO;
};


#endif //INC_2048_GAME_GAMEPLAYSCENE_H