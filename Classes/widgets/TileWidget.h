#ifndef INC_2048_GAME_TILEWIDGET_H
#define INC_2048_GAME_TILEWIDGET_H
#include "ui/UIImageView.h"
#include <string>
#include <unordered_map>


namespace cocos2d::ui {
    class Text;
}

class TileWidget : public cocos2d::ui::ImageView {
public:
    static TileWidget* create(int num = 0, const std::string& info = "");
    void updateTile(int num, const std::string& info = "");
protected:
    bool initWithNum(int num, const std::string& info = "");
private:
    static std::unordered_map<int, std::string> imageList;

    cocos2d::ui::ImageView* mImg{nullptr};// number image
    cocos2d::ui::Text* mText{nullptr};// debug helper
};


#endif //INC_2048_GAME_TILEWIDGET_H