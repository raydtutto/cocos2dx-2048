#ifndef INC_2048_GAME_NODEUTILS_H
#define INC_2048_GAME_NODEUTILS_H

#include "cocos2d.h"

class NodeUtils {
    public:
    static cocos2d::Node* getNodeByName(cocos2d::Node* parent, const std::string& name);
};


#endif //INC_2048_GAME_NODEUTILS_H