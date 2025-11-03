#include "NodeUtils.h"

cocos2d::Node* NodeUtils::getNodeByName(cocos2d::Node *parent, const std::string &name) {
    if (parent == nullptr || name.empty()) {
        return nullptr;
    }

    if (parent->getName() == name) {
        return parent;
    }

    for (auto child : parent->getChildren()) {
        if (child->getName() == name) {
            return child;
        }

        return getNodeByName(child, name);
    }

    return nullptr;
}