#pragma once
#include <GUI/dune/GameOptionsWindow.h>
struct Release543OptionsFixture {
    static void verify(const std::string& output,const std::string& mod) {
        const auto require=[](bool ok,const char* why){if(!ok)throw std::runtime_error(std::string("Release 543 options: ")+why);};
        auto rules=effectiveGameOptions;GameOptionsWindow window(rules);
        window.draw();
        const auto pos=window.vboxLeft.getWidgetPosition(&window.concreteRequiredCheckbox);
        require(pos.x>=0 && pos.y>=0 && pos.x+window.concreteRequiredCheckbox.getSize().x<=window.vboxLeft.getSize().x,"concrete checkbox exceeds its left column");
        require(window.getSize().x<=getRendererWidth() && window.getSize().y<=getRendererHeight(),"general rules dialog exceeds screen");
        auto screenshot=renderReadSurface(renderer);require(screenshot!=nullptr,"options capture");
        const auto path=(std::filesystem::path(output)/(mod+"-general-options.png")).string();require(SavePNG(screenshot.get(),path.c_str())==0,"options preview");
    }
};
