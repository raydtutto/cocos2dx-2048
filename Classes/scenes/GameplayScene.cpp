#include "GameplayScene.h"

#include "cocostudio/ActionTimeline/CSLoader.h"
#include "utils/NodeUtils.h"
#include "widgets/TileWidget.h"

#include <random>
#include <tuple>

using namespace cocos2d;

namespace {
    constexpr auto touchSwipeThreshold = 70.f;
    constexpr std::pair<int, int> gridSize{4, 4};
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

GameplayScene::~GameplayScene() {
    if (mKeyListener) {
        mKeyListener->release();
        mKeyListener = nullptr;
    }
}

bool GameplayScene::initWithCSB(const std::string& path) {
    auto scene = CSLoader::createNode(path);
    if (scene && SceneBase::init()) {
        CC_SAFE_RELEASE(mRootNode);
        mRootNode = scene;
        addChild(mRootNode);

        mKeyListener = EventListenerKeyboard::create();
        if (mKeyListener) {
            mKeyListener->onKeyPressed = CC_CALLBACK_2(GameplayScene::keyPressed, this);
            mKeyListener->onKeyReleased = CC_CALLBACK_2(GameplayScene::keyReleased, this);
            _eventDispatcher->addEventListenerWithSceneGraphPriority(mKeyListener, this);
        } else {
            CCASSERT(mKeyListener, "mKeyListener is null");
        }

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
    for (int x = 0; x < gridSize.first; ++x) {
        for (int y = 0; y < gridSize.second; ++y) {
            int nextNum = 0;
            if ((randomPos.first.first == x && randomPos.first.second == y) || (randomPos.second.first == x && randomPos.second.second == y)) {
                nextNum = 2;
            }

            if (TileWidget* tile = TileWidget::create(nextNum, StringUtils::format("x%d,y%d", x, y))) {
                gameboard->addChild(tile);
                if (mGrid.count(x) == 0U) {
                    mGrid[x] = {};
                }
                TileInfo tileInfo;
                tileInfo.num = nextNum;
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

        mInitTouchPos = Vec2::ZERO;
        onMove(dir);
    };

    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(myListener, this);
}

void GameplayScene::onMove(eDirection dir) {
    // todo move without merging
    if (dir == eDirection::DOWN) {
        for (int x = 0; x < gridSize.first; ++x) {
            std::vector<TileInfo*> line;
            line.push_back(&mGrid[x][0]);
            line.push_back(&mGrid[x][1]);
            line.push_back(&mGrid[x][2]);
            line.push_back(&mGrid[x][3]);
            proceedOnMove(line);
        }
    }
}

void GameplayScene::proceedOnMove(std::vector<TileInfo*> line) {
    CCLOG("ProceedOnMove");
    auto size = static_cast<int>(line.size());
    int current = size - 1;
    while (current >= 0) {
        if (line[current]->num == 0) {
            current--;
            continue;
        }
        int next = current - 1;
        while (next >= 0) {
            if (line[current]->num == line[next]->num) {
                CCLOG("Merge");
                int res = line[current]->num + line[next]->num;
                line[next]->pNode->updateTile(res);
                line[next]->num = res;
                line[current]->pNode->updateTile(0);
                line[current]->num = 0;
            } else if (line[next]->num > 0) {
                CCLOG("Stop");
            } else {
                int res = line[current]->num + line[next]->num;
                line[next]->pNode->updateTile(res);
                line[next]->num = res;
                line[current]->pNode->updateTile(0);
                line[current]->num = 0;
                CCLOG("Moved");
            }
            next--;
        }
        current--;
    }
}

void GameplayScene::onEnter() {
    SceneBase::onEnter();
    CCLOG("GameplayScene::onEnter");

    fillGrid();
    touchHandler();
}

void GameplayScene::keyPressed(EventKeyboard::KeyCode keyCode, Event*) {
    //
}

void GameplayScene::keyReleased(EventKeyboard::KeyCode keyCode, Event*) {
    switch (keyCode) {
        case EventKeyboard::KeyCode::KEY_UP_ARROW:
        case EventKeyboard::KeyCode::KEY_W:
            CCLOG("UP");
            onMove(eDirection::UP);
            break;
        case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
        case EventKeyboard::KeyCode::KEY_S:
            CCLOG("DOWN");
            onMove(eDirection::DOWN);
            break;
        case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
        case EventKeyboard::KeyCode::KEY_A:
            CCLOG("LEFT");
            onMove(eDirection::LEFT);
            break;
        case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
        case EventKeyboard::KeyCode::KEY_D:
            CCLOG("RIGHT");
            onMove(eDirection::RIGHT);
            break;
        default:
            break;
    }
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
