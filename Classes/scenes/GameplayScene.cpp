#include "GameplayScene.h"

#include "ccConstants.h"
#include "cocostudio/ActionTimeline/CSLoader.h"
#include "utils/NodeUtils.h"
#include "widgets/TileWidget.h"

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
    gameboard->setContentSize(Size(size.width, size.width));

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

    int count = 0;
    for (int x = 0; x < 4; ++x) {
        for (int y = 0; y < 4; ++y) {
            if (TileWidget* tile = TileWidget::create(2)) {
                gameboard->addChild(tile);
                if (mGrid.count(x) == 0U) {
                    mGrid[x] = {};
                }
                TileInfo tileInfo;
                tileInfo.num = 0;
                tileInfo.pNode = tile;
                tile->setPosition(Vec2((x + 1) * tileSize.width, (y + 1) * tileSize.height));

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
        if (loc.x > mInitTouchPos.x + touchSwipeThreshold) {
            CCLOG("Right");
        } else if (loc.x < mInitTouchPos.x - touchSwipeThreshold) {
            CCLOG("Left");
        } else if (loc.y > mInitTouchPos.y + touchSwipeThreshold) {
            CCLOG("Up");
        } else if (loc.y < mInitTouchPos.y - touchSwipeThreshold) {
            CCLOG("Down");
        }

        mInitTouchPos = cocos2d::Vec2::ZERO;
    };

    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(myListener, this);
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
    // todo new func to handle touches touchHandler()
    // Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(mylistener, this);
    // in all events like onTouchMoved, CCLOG all useful data

    touchHandler();
}