#ifndef INC_2048_GAME_TILEWIDGET_H
#define INC_2048_GAME_TILEWIDGET_H
#include "ui/UIWidget.h"
#include <string>
#include <unordered_map>

#include "ui/UIButton.h"


namespace cocos2d::ui {
    class ImageView;
    class Text;
}

class TileWidget : public cocos2d::ui::Widget {
public:
    static TileWidget* create(int num = 0, const std::string& info = "");
    void updateTile(int num, const std::string& info = "");
    void moveTile(cocos2d::Vec2 position);
    cocos2d::Vec2 getTilePosition();
protected:
    bool initWithNum(int num, const std::string& info = "");
private:
    static std::unordered_map<int, std::string> imageList;

    cocos2d::ui::ImageView* mImg{nullptr};// number image
    cocos2d::ui::Text* mText{nullptr};// debug helper
    cocos2d::Vec2 mPos;
};


#endif //INC_2048_GAME_TILEWIDGET_H