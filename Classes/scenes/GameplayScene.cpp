#include "GameplayScene.h"

#include "cocostudio/ActionTimeline/CSLoader.h"
#include "utils/NodeUtils.h"
#include "widgets/TileWidget.h"

#include <random>

using namespace cocos2d;

namespace {
    constexpr auto touchSwipeThreshold = 70.f;
}

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

void GameplayScene::fillGrid() {
    const auto gameboard = NodeUtils::getNodeByName(this,"gameboard");
    if (!gameboard) {
        CCLOG("No gameboard.");
        return;
    }
    const auto size = Director::getInstance()->getWinSize();
    auto boardSize = gameboard->getContentSize();
    auto scale = size.width / boardSize.width;
    gameboard->setScale(scale);
    for (auto& row : mGrid) {
        for (auto& item : row.second) {
            if (item.second.pNode)
                item.second.pNode->removeFromParentAndCleanup(true);
        }
    }
    mGrid.clear();

    // Temporary node just to get actual tile size
    Size tileSize;
    if (TileWidget* tileTemp = TileWidget::create(2)) {
        tileSize = tileTemp->getContentSize();
    }

    const auto randomPos = getStartRandomPosition();

    int count = 0;
    for (int x = 0; x < 4; ++x) {
        for (int y = 0; y < 4; ++y) {
            int nextNum = 0;
            if ((randomPos.first.first == x && randomPos.first.second == y) || (randomPos.second.first == x && randomPos.second.second == y)) {
                nextNum = 2;
            }

            if (TileWidget* tile = TileWidget::create(nextNum)) {
                gameboard->addChild(tile);
                if (mGrid.count(x) == 0U) {
                    mGrid[x] = {};
                }
                TileInfo tileInfo;
                tileInfo.num = 0;
                tileInfo.pNode = tile;
                tile->setAnchorPoint({0.5f, 0.5f});
                tile->setPosition(Vec2(static_cast<float>(x) * tileSize.width + tileSize.width / 2, static_cast<float>(y) * tileSize.height + tileSize.height / 2));

                mGrid[x][y] = tileInfo;
                count++;
            }
        }
    }

    CCLOG("Board filled with %d", count);
}

void GameplayScene::touchHandler() {
    auto myListener = EventListenerTouchOneByOne::create();

    myListener->onTouchBegan = [this](Touch* touch, Event* event) {
        CCLOG("Touch began %f %f", touch->getStartLocation().x, touch->getStartLocation().y);
        mInitTouchPos = touch->getStartLocation();
        return true;
    };

    myListener->onTouchEnded = [this](Touch* touch, Event* event) {
        CCLOG("Touch ended %f %f", touch->getLocation().x, touch->getLocation().y);
        auto loc = touch->getLocation();
        eDirection dir = eDirection::UNDEFINED;
        if (loc.x > mInitTouchPos.x + touchSwipeThreshold) {
            dir = eDirection::RIGHT;
            CCLOG("Right");
        } else if (loc.x < mInitTouchPos.x - touchSwipeThreshold) {
            dir = eDirection::LEFT;
            CCLOG("Left");
        } else if (loc.y > mInitTouchPos.y + touchSwipeThreshold) {
            dir = eDirection::UP;
            CCLOG("Up");
        } else if (loc.y < mInitTouchPos.y - touchSwipeThreshold) {
            dir = eDirection::DOWN;
            CCLOG("Down");
        }

        mInitTouchPos = cocos2d::Vec2::ZERO;
        onMove(dir);
    };

    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(myListener, this);
}

void GameplayScene::onMove(eDirection dir) {
    // todo
}

void GameplayScene::onEnter() {
    SceneBase::onEnter();
    CCLOG("GameplayScene::onEnter");

    // auto gameboard = NodeUtils::getNodeByName(this,"gameboard");
    // if (gameboard) {
    //     auto size = Director::getInstance()->getWinSize();
    //     gameboard->setContentSize(Size(size.width, size.width));
    //     Size tileSize;
    //
    //     if (TileWidget* tile1 = TileWidget::create(2)) {
    //         tileSize = tile1->getContentSize();
    //         gameboard->addChild(tile1);
    //         tile1->setPositionY(128);
    //         tile1->setPositionX(tileSize.width / 2);
    //     }
    //
    //     if (TileWidget* tile2 = TileWidget::create(1)) {
    //         gameboard->addChild(tile2);
    //         tile2->setPositionY(512);
    //         tile2->setPositionX(tileSize.width / 2);
    //     }
    // }

    fillGrid();
    touchHandler();
}

std::pair<std::pair<int, int>, std::pair<int, int>> GameplayScene::getStartRandomPosition() const {
    std::random_device rd;
    std::uniform_int_distribution<int> dist(0, 3);

    std::pair<std::pair<int, int>, std::pair<int, int>> result;
    result.first.first = dist(rd);
    result.second.first = dist(rd);

    while (result.first.first == result.second.first) {
        result.second.first = dist(rd);
    }

    result.first.second = dist(rd);
    result.second.second = dist(rd);

    return result;
}
