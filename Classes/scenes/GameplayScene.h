#ifndef INC_2048_GAME_GAMEPLAYSCENE_H
#define INC_2048_GAME_GAMEPLAYSCENE_H

#include "SceneBase.h"

class GameplayScene : public SceneBase {
public:
    static GameplayScene* create();
protected:
    bool initWithCSB(const std::string &path) override;
};


#endif //INC_2048_GAME_GAMEPLAYSCENE_H