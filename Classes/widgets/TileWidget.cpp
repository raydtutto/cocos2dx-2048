#include "TileWidget.h"

using namespace cocos2d;
using namespace cocos2d::ui;

std::unordered_map<int, std::string> TileWidget::imageList = {
    {0, "GUI/cocosstudio/img/tile0.png"},
    {2, "GUI/cocosstudio/img/tile2.png"},
    {4, "GUI/cocosstudio/img/tile4.png"},
    {8, "GUI/cocosstudio/img/tile8.png"},
    {16, "GUI/cocosstudio/img/tile16.png"},
    {32, "GUI/cocosstudio/img/tile32.png"},
    {64, "GUI/cocosstudio/img/tile64.png"},
    {128, "GUI/cocosstudio/img/tile128.png"},
    {256, "GUI/cocosstudio/img/tile256.png"},
    {512, "GUI/cocosstudio/img/tile512.png"},
    {1024, "GUI/cocosstudio/img/tile1024.png"},
    {2048, "GUI/cocosstudio/img/tile2048.png"},
    {4096, "GUI/cocosstudio/img/tile4096.png"}
};

TileWidget * TileWidget::create(int num) {
    auto* pTile = new(std::nothrow) TileWidget();
    if (pTile && pTile->initWithNum(num)) {
        pTile->autorelease();
        return pTile;
    }
    CC_SAFE_DELETE(pTile);
    return nullptr;
}

bool TileWidget::initWithNum(int num) {
    if (num >= 0 && imageList.count(num) > 0U && ImageView::init()) {
        loadTexture(imageList[num], TextureResType::LOCAL);
        return true;
    }
    CCASSERT(false, "Cannot initialize this tile.");
    return false;
}
