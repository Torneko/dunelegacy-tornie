// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once
#include <GUI/TextButton.h>
#include <globals.h>
#include <algorithm>

// Preserve a lobby's minimum layout width and translate both drawing and input.
class HorizontalScrollView : public Widget {
public:
    HorizontalScrollView() {
        enableResizing(true,true);
        left.setText("<");right.setText(">");
        left.setOnClick([this]{scrollBy(-std::max(48,getSize().x/2));});
        right.setOnClick([this]{scrollBy(std::max(48,getSize().x/2));});
    }
    ~HorizontalScrollView() override { if(content)content->setParent(nullptr); }
    void setContent(Widget* value) { if(content)content->setParent(nullptr);content=value;if(content)content->setParent(this);resize(getSize()); }
    void removeChildWidget(Widget* value) override { if(value==content)content=nullptr; }
    Point getMinimumSize() const override { return Point(64,48); }
    void resize(Point size) override { resize(size.x,size.y); }
    void resize(Uint32 width,Uint32 height) override {
        Widget::resize(width,height);
        const int minimum=content ? content->getMinimumSize().x : 0;
        maximum=std::max(0,minimum-static_cast<int>(width));offset=std::clamp(offset,0,maximum);
        if(content)content->resize(std::max(static_cast<int>(width),minimum),viewHeight());
        left.resize(28,24);right.resize(28,24);left.setEnabled(offset>0);right.setEnabled(offset<maximum);
    }
    void resizeAll() override { resize(getSize());if(getParent())getParent()->resizeAll(); }
    int getScrollOffset() const { return offset; }
    int getMaximumOffset() const { return maximum; }
    void scrollBy(int amount) { offset=std::clamp(offset+amount,0,maximum);left.setEnabled(offset>0);right.setEnabled(offset<maximum); }
    bool handleMouseLeft(Sint32 x,Sint32 y,bool pressed) override {
        if(!inside(x,y))return false;
        if(maximum && y>=viewHeight()) {
            if(x<28)return left.handleMouseLeft(x,y-viewHeight(),pressed);
            if(x>=getSize().x-28)return right.handleMouseLeft(x-getSize().x+28,y-viewHeight(),pressed);
            return true;
        }
        return content && content->handleMouseLeft(x+offset,y,pressed);
    }
    bool handleMouseRight(Sint32 x,Sint32 y,bool pressed) override { return inside(x,y)&&y<viewHeight()&&content&&content->handleMouseRight(x+offset,y,pressed); }
    void handleMouseMovement(Sint32 x,Sint32 y,bool overlay) override {
        left.handleMouseMovement(x,y-viewHeight(),overlay);right.handleMouseMovement(x-getSize().x+28,y-viewHeight(),overlay);
        if(content)content->handleMouseMovement(x+offset,y,overlay||!inside(x,y)||y>=viewHeight());
    }
    bool handleMouseWheel(Sint32 x,Sint32 y,bool up) override {
        if(!inside(x,y))return false;
        if(y<viewHeight()&&content&&content->handleMouseWheel(x+offset,y,up))return true;
        if(maximum){scrollBy(up?-48:48);return true;}return false;
    }
    bool handleKeyPress(SDL_KeyboardEvent& key) override { return content&&content->handleKeyPress(key); }
    bool handleMouseMovementOverlay(Sint32 x,Sint32 y) override {return inside(x,y)&&y<viewHeight()&&content&&content->handleMouseMovementOverlay(x+offset,y);}
    bool handleMouseLeftOverlay(Sint32 x,Sint32 y,bool pressed) override {return inside(x,y)&&y<viewHeight()&&content&&content->handleMouseLeftOverlay(x+offset,y,pressed);}
    bool handleMouseRightOverlay(Sint32 x,Sint32 y,bool pressed) override {return inside(x,y)&&y<viewHeight()&&content&&content->handleMouseRightOverlay(x+offset,y,pressed);}
    bool handleMouseWheelOverlay(Sint32 x,Sint32 y,bool up) override {return inside(x,y)&&y<viewHeight()&&content&&content->handleMouseWheelOverlay(x+offset,y,up);}
    void draw(Point position) override { paint(position,false); }
    void drawOverlay(Point position) override { paint(position,true); }
private:
    bool inside(int x,int y) const { return x>=0&&y>=0&&x<getSize().x&&y<getSize().y; }
    int viewHeight() const { return std::max(0,getSize().y-(maximum?24:0)); }
    void paint(Point position,bool overlay) {
        if(!isVisible()||!content)return;
        SDL_Rect old{},clip{position.x,position.y,getSize().x,viewHeight()};
        const bool clipped=SDL_RenderIsClipEnabled(renderer);SDL_RenderGetClipRect(renderer,&old);
        if(clipped)SDL_IntersectRect(&old,&clip,&clip);
        SDL_RenderSetClipRect(renderer,&clip);
        if(overlay)content->drawOverlay(position+Point(-offset,0));else content->draw(position+Point(-offset,0));
        SDL_RenderSetClipRect(renderer,clipped?&old:nullptr);
        if(!overlay&&maximum){left.draw(position+Point(0,viewHeight()));right.draw(position+Point(getSize().x-28,viewHeight()));}
    }
    Widget* content=nullptr;
    TextButton left,right;
    int offset=0,maximum=0;
};
