// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once
#include <GUI/TextButton.h>
#include <globals.h>
#include <array>
#include <algorithm>

class CompletionButton : public TextButton {
public:
    void setCompleted(bool value) { completed=value; }
    bool isCompleted() const { return completed; }
    void draw(Point position) override {
        TextButton::draw(position);
        if(!completed||!isVisible())return;
        constexpr int x[]={0,2,9,4,6,0,-6,-4,-9,-2};
        constexpr int y[]={-9,-3,-3,1,8,4,8,1,-3,-3};
        Uint8 r,g,b,a;SDL_GetRenderDrawColor(renderer,&r,&g,&b,&a);
        SDL_SetRenderDrawColor(renderer,255,220,0,255);
        for(int row=-9;row<=8;++row){
            std::array<int,10> intersections{};int count=0;
            for(int i=0;i<10;++i){int j=(i+1)%10;if((y[i]<=row&&y[j]>row)||(y[j]<=row&&y[i]>row))intersections[count++]=x[i]+(row-y[i])*(x[j]-x[i])/(y[j]-y[i]);}
            std::sort(intersections.begin(),intersections.begin()+count);
            for(int i=0;i+1<count;i+=2)SDL_RenderDrawLine(renderer,position.x+14+intersections[i],position.y+getSize().y/2+row,position.x+14+intersections[i+1],position.y+getSize().y/2+row);
        }
        SDL_SetRenderDrawColor(renderer,r,g,b,a);
    }
private:
    bool completed=false;
};
