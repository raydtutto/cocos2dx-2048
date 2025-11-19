#ifndef INC_2048_GAME_GAMEPLAYSCENE_H
#define INC_2048_GAME_GAMEPLAYSCENE_H


#include "SceneBase.h"
#include <tuple>
#include <map>

enum class eDirection : char {
    UNDEFINED = -1,
    UP = 0,
    DOWN,
    RIGHT,
    LEFT
};

struct TileInfo {
    cocos2d::Node* pNode{nullptr};
    int num = 0;
};

class GameplayScene : public SceneBase {
public:
    static GameplayScene* create();
    ~GameplayScene();

    void onEnter() override;
    void keyPressed(cocos2d::EventKeyboard::KeyCode, cocos2d::Event*);
    void keyReleased(cocos2d::EventKeyboard::KeyCode, cocos2d::Event*);

protected:
    bool initWithCSB(const std::string &path) override;
    void fillGrid();

    void touchHandler();
    void onMove(eDirection dir);
    std::pair<std::pair<int, int>, std::pair<int, int>> getStartRandomPosition() const;

private:
    std::map<int, std::map<int, TileInfo>> mGrid;
    cocos2d::Vec2 mInitTouchPos = cocos2d::Vec2::ZERO;
    cocos2d::EventListenerKeyboard* mKeyListener{nullptr};
};


#endif //INC_2048_GAME_GAMEPLAYSCENE_H