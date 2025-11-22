#include "TileWidget.h"

#include "ui/UIText.h"
#include "utils/NodeUtils.h"

using namespace cocos2d;
using namespace cocos2d::ui;

namespace {
    constexpr auto _tileBg = "GUI/cocosstudio/img/tile0.png";
}

std::unordered_map<int, std::string> TileWidget::imageList = {
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

TileWidget * TileWidget::create(int num, const std::string& info) {
    auto* pTile = new(std::nothrow) TileWidget();
    if (pTile && pTile->initWithNum(num, info)) {
        pTile->autorelease();
        return pTile;
    }
    CC_SAFE_DELETE(pTile);
    return nullptr;
}

void TileWidget::updateTile(int num, const std::string &info) {
    if (imageList.count(num) > 0U) {
        mImg->loadTexture(imageList[num], TextureResType::LOCAL);
        mText->setTextColor({0,0,0,255});
    } else {
        mText->setTextColor({255,255,255,255});
    }
    mImg->setVisible(imageList.count(num) > 0U);

    mText->setString(info);
}

bool TileWidget::initWithNum(int num, const std::string& info) {
    if (!ImageView::init())
        return false;
    setContentSize(Size{136.f,136.f});
    loadTexture(_tileBg, TextureResType::LOCAL); // bg

    // load image
    mImg = ImageView::create();
    addChild(mImg);
    mImg->setAnchorPoint({0.5f, 0.5f});
    mImg->setPosition({136/2, 136/2});

    // load text
    mText = Text::create();
    addChild(mText);
    mText->setPosition({136/2, 136/4});

    updateTile(num, info);

    return true;
}