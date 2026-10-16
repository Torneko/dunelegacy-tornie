#pragma once
#include <FileClasses/LoadSavePNG.h>
#include <Menu/BriefingMenu.h>
#include <filesystem>

inline void verifyRebelsMentat(const std::string& output,const std::string& mod) {
    if(mod!="Tornie" && mod!="Jericho" && mod!="ArcaKev" && mod!="vanilla") return;
    auto require=[](bool ok,const std::string& message) {
        if(!ok) throw std::runtime_error("Rebels cat mentat: "+message);
    };
    const auto& info=ModManager::instance().getActiveMentatInfo(HOUSE_REBELS);
    require(info.enabled && info.factionTint && info.backgroundAsset=="MentatCat_Wildspade.png","Rebels did not receive the animated cat");
    auto source=LoadPNG_RW(pFileManager->openFile(info.backgroundAsset).get());
    auto original=sdl2::surface_ptr{SDL_ConvertSurfaceFormat(source.get(),SDL_PIXELFORMAT_RGBA32,0)};
    auto* background=pGFXManager->getUIGraphicSurface(UI_MentatBackground,HOUSE_REBELS);
    auto reference=sdl2::surface_ptr{SDL_ConvertSurfaceFormat(background,SDL_PIXELFORMAT_RGBA32,0)};
    require(reference && reference->w==640 && reference->h==400,"portrait dimensions");
    int changed=0,green=0;
    for(int y=0;y<400;++y) for(int x=0;x<640;++x) {
        const auto* a=static_cast<Uint8*>(original->pixels)+y*original->pitch+x*4;
        const auto* b=static_cast<Uint8*>(reference->pixels)+y*reference->pitch+x*4;
        require(a[3]==b[3],"transparency changed");
        if(std::memcmp(a,b,3)!=0) {
            ++changed;
            if(mod=="Tornie") require(b[1]>=b[0] && b[1]>=b[2],"Tornie Rebels cat is not green");
            else require(b[0]==b[1] && b[1]==b[2],"Rebels cat is not neutral dark grey");
        }
        if(x>=310) require(std::memcmp(a,b,4)==0,"right-hand room or briefing frame changed");
        if(a[1]>40 && a[1]>a[0]*1.3 && a[1]>a[2]*1.3) {
            require(std::memcmp(a,b,4)==0,"green eye/detail changed");++green;
        }
    }
    require(changed>10000 && green>10,"colour check missed the cat or irises");
    auto* eyes=pGFXManager->getMentatEyesAnimation(HOUSE_REBELS);
    auto* mouth=pGFXManager->getMentatMouthAnimation(HOUSE_REBELS);
    require(eyes && mouth && eyes->getNumberOfFrames()==5 && mouth->getNumberOfFrames()==5,"five gaze/speech states");
    for(auto* animation:{eyes,mouth}) {
        const int w=animation==eyes ? 98 : 40,h=animation==eyes ? 32 : 23;
        for(const auto& frame:animation->getFrames()) require(frame && frame->w==w && frame->h==h,"facial patch dimensions/anchors");
    }
    BriefingMenu menu(getRuntimeHouseForIdentity(HOUSE_REBELS),15,BRIEFING);
    menu.setText("");menu.onMentatTextFinished();
    auto screen=sdl2::surface_ptr{SDL_CreateRGBSurfaceWithFormat(0,getRendererWidth(),getRendererHeight(),32,SDL_PIXELFORMAT_RGBA32)};
    for(int state=0;state<5;++state) {
        eyes->setFrameOverride(state);mouth->setFrameOverride(state);
        SDL_RenderSetClipRect(renderer,nullptr);SDL_RenderClear(renderer);menu.draw();
        require(SDL_RenderReadPixels(renderer,nullptr,screen->format->format,screen->pixels,screen->pitch)==0,"actual briefing capture");
        for(int y=174;y<190;++y) for(int x=164;x<201;++x) {
            const auto* a=static_cast<Uint8*>(reference->pixels)+y*reference->pitch+x*4;
            const auto* b=static_cast<Uint8*>(screen->pixels)+(y+menu.getPosition().y)*screen->pitch+(x+menu.getPosition().x)*4;
            require(std::memcmp(a,b,3)==0,"nose moved or Jericho selected another mentat");
        }
        for(int y=310;y<316;++y) for(int x=258;x<264;++x) {
            const auto* a=static_cast<Uint8*>(reference->pixels)+y*reference->pitch+x*4;
            const auto* b=static_cast<Uint8*>(screen->pixels)+(y+menu.getPosition().y)*screen->pitch+(x+menu.getPosition().x)*4;
            require(std::memcmp(a,b,3)==0,"briefing video covers the recoloured shoulder");
        }
        require(SavePNG(screen.get(),(std::filesystem::path(output)/(mod+"-rebels-briefing-"+std::to_string(state)+".png")).string().c_str())==0,"preview PNG");
    }
    require(SavePNG(reference.get(),(std::filesystem::path(output)/(mod+"-rebels-cat.png")).string().c_str())==0,"portrait PNG");
    eyes->resetFrameOverride();mouth->resetFrameOverride();
    menu.setText("Rebels. Briefing.");menu.update();require(mouth->getLoopsLeft()>0,"speech did not animate");
    require(ModManager::instance().getActiveMentatInfo(HOUSE_NEUTRAL).backgroundAsset=="ChaniMentat.png","Neutral portrait changed");
    SDL_Log("REBELS MENTAT PASS: %s, faction tint, five gaze/speech states, stable nose/shoulder, actual faction briefing",mod.c_str());
}
