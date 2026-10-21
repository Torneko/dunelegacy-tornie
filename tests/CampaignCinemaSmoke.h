#pragma once
#include <CutScenes/WestwoodLogoVideoEvent.h>
#include <CutScenes/OriginalCredits.h>
#include <CutScenes/Intro.h>
#include <CutScenes/Meanwhile.h>
#include <CutScenes/Finale.h>
#include <CutScenes/CampaignStory.h>
#include <FileClasses/music/MusicPlayer.h>
#include <SoundPlayer.h>
#include <FileClasses/INIFile.h>
#include <fstream>
#include <cstring>
#include <FileClasses/adl/sound_adlib.h>

struct IntroLogoFixture {
    static void verify(const std::string& output) {
        const auto require=[](bool ok,const char* why){if(!ok)throw std::runtime_error(std::string("Original cinema: ")+why);};
        const bool music=settings.audio.playMusic;
        const int volume=settings.audio.musicVolume;
        const bool musicOn=musicPlayer->isMusicOn();
        Palette before=palette;
        settings.audio.playMusic=false;
        {
            WestwoodLogoVideoEvent logo;require(!logo.signature,"muted logo loaded audio");
            for(int i=0;i<80;++i) {
                SDL_SetRenderDrawColor(renderer,0,0,0,255);SDL_RenderClear(renderer);
                require(logo.draw()==100,"original six-tick logo cadence");
                if(i==30) {
                    auto scene=renderReadSurface(renderer);int blue=0;
                    for(int y=0;y<scene->h;++y)for(int x=0;x<scene->w;++x) {
                        Uint8 r,g,b,a;SDL_GetRGBA(getPixel(scene.get(),x,y),scene->format,&r,&g,&b,&a);
                        if(b>r+30 && b>g+20)++blue;
                    }
                    require(blue>100,"Westwood logo is not the original blue logo");
                    require(SavePNG(scene.get(),(std::filesystem::path(output)/"westwood-blue-original.png").string().c_str())==0,"logo preview export");
                }
            }
            require(logo.isFinished(),"muted logo does not finish at eight seconds");
        }
        settings.audio.playMusic=true;settings.audio.musicVolume=37;musicPlayer->setMusic(true);
        int channel=-1,previous=-1;
        {
            WestwoodLogoVideoEvent logo;require(logo.signature && logo.signature->alen>44100,"original DUNE0/4 signature missing");
            for(int i=0;i<10;++i) {
                SDL_RenderClear(renderer);logo.draw();
                require(logo.channel<0,"signature starts before the first animated/full-opacity frame");
            }
            SDL_RenderClear(renderer);logo.draw();
            SoundAdlibPC baseline(pFileManager->openFile("DUNE0.ADL").get());
            auto quiet=sdl2::mix_chunk_ptr(baseline.getSubsong(4));
            require(quiet && quiet->alen==logo.signature->alen,"louder signature changed track duration");
            long double oldEnergy=0,newEnergy=0;
            for(Uint32 i=0;i+1<quiet->alen;i+=2) {
                Sint16 a,b;std::memcpy(&a,quiet->abuf+i,2);std::memcpy(&b,logo.signature->abuf+i,2);
                oldEnergy+=static_cast<long double>(a)*a;newEnergy+=static_cast<long double>(b)*b;
            }
            require(oldEnergy>0 && newEnergy>oldEnergy*3.5 && newEnergy<oldEnergy*4.5,
                "signature is not approximately twice as loud (+6 dB)");
            channel=logo.channel;previous=logo.previousVolume;
            require(channel>=0 && Mix_GetChunk(channel)==logo.signature.get() && Mix_Playing(channel),"signature timing or channel");
            logo.draw();require(logo.channel==channel,"signature restarts after its cue");
            require(Mix_Volume(channel,-1)==37 && settings.audio.playMusic && musicPlayer->isMusicOn(),"signature volume or user preference");
        }
        require(!Mix_Playing(channel) && Mix_Volume(channel,-1)==previous,"skipped logo left its audio playing or volume changed");
        settings.audio.musicVolume=0;
        {
            WestwoodLogoVideoEvent logo;
            for(int i=0;i<11;++i)logo.draw();
            require(logo.channel>=0 && Mix_Volume(logo.channel,-1)==0,"signature ignores zero music volume");
        }
        settings.audio.musicVolume=37;
        {
            OriginalCreditsVideoEvent credits;
            require(credits.music && credits.music->alen>44100,"original DUNE20/2 credits music missing");
            require(credits.pictures.size()==11 && credits.textHeight>200 && credits.initialPlanet,"original credits assets or bitmap font");
            for(int i=0;i<800;++i) {
                SDL_SetRenderDrawColor(renderer,0,0,0,255);SDL_RenderClear(renderer);credits.draw();
                if(i==20 || i==80 || i==250 || i==600) {
                    auto scene=renderReadSurface(renderer);
                    require(SavePNG(scene.get(),(std::filesystem::path(output)/("original-credits-"+settings.general.language+"-"+std::to_string(i)+".png")).string().c_str())==0,"credits preview export");
                }
            }
            channel=credits.channel;previous=credits.previousVolume;
            require(channel>=0 && Mix_GetChunk(channel)==credits.music.get(),"credits original music not playing");
        }
        require(!Mix_Playing(channel) && Mix_Volume(channel,-1)==previous,"credits audio survives exit");
        settings.audio.playMusic=false;musicPlayer->setMusic(false);
        {
            struct TestIntro:Intro {int frame(){return draw();}} intro;
            for(int i=0;i<31;++i){SDL_RenderClear(renderer);intro.frame();}
            SDL_Event escape{};escape.type=SDL_KEYDOWN;escape.key.keysym.sym=SDLK_ESCAPE;SDL_PushEvent(&escape);
            intro.run();
        }
        {
            OriginalCredits credits;
            SDL_Event escape{};escape.type=SDL_KEYDOWN;escape.key.keysym.sym=SDLK_ESCAPE;SDL_PushEvent(&escape);
            credits.run();
        }
        for(int i=0;i<256;++i)require(before[i].r==palette[i].r && before[i].g==palette[i].g && before[i].b==palette[i].b,"original logo/credits changed game palette");
        settings.audio.playMusic=music;settings.audio.musicVolume=volume;musicPlayer->setMusic(musicOn);
        SDL_Log("ORIGINAL CINEMA PASS: %s, blue palette, original AdLib tracks, mute/volume, bitmap credits and skipped audio cleanup",settings.general.language.c_str());
    }
};

struct CampaignCinemaFixture {
    template<class T> struct Playable : T {
        template<class... Args> explicit Playable(Args... args):T(args...){}
        int frame(){return this->draw();}
    };
    static void verify(const std::string& output,const std::string& mod) {
        const auto require=[](bool ok,const char* why){if(!ok)throw std::runtime_error(std::string("Campaign cinema: ")+why);};
        const bool music=settings.audio.playMusic,sfx=settings.audio.playSFX;
        const bool musicOn=musicPlayer->isMusicOn();
        settings.audio.playMusic=false;musicPlayer->setMusic(false);soundPlayer->setSound(false);
        Palette before=palette;
        std::ofstream report(std::filesystem::path(output)/(mod+"-campaign-story-"+settings.general.language+".tsv"));
        report<<"house\ttemplate\tH-role\tA-role\tO-role\timperial-role\tfirst-frames\tsecond-frames\tfinal-frames\n";
        std::map<int,std::array<int,3>> modelFrames;
        int count=0;
        for(const int house:getFactionSelectionOrder()) {
            const CampaignStory story(house);
            require(story.model>=HOUSE_HARKONNEN && story.model<=HOUSE_ORDOS,"non-original scene model");
            if(mod!="vanilla") {
                INIFile plan(pFileManager->openFile("CampaignPlan.ini").get());
                const auto name=getHouseNameByNumber(static_cast<HOUSETYPE>(house));
                const auto letter=plan.getStringValue(name,"Template","");
                require(story.model==(letter=="H"?HOUSE_HARKONNEN:letter=="A"?HOUSE_ATREIDES:HOUSE_ORDOS),"scene does not follow campaign template");
                require(story.roles[HOUSE_SARDAUKAR]==getHouseByName(plan.getStringValue(name,"Opponent3","")),"wrong imperial opponent");
            }
            const auto expected=getHouseNameByNumber(static_cast<HOUSETYPE>(story.roles[HOUSE_HARKONNEN]))+" / "
                +getHouseNameByNumber(static_cast<HOUSETYPE>(story.roles[HOUSE_ATREIDES]))+" / "
                +getHouseNameByNumber(static_cast<HOUSETYPE>(story.roles[HOUSE_ORDOS]))+" / "
                +getHouseNameByNumber(static_cast<HOUSETYPE>(story.roles[HOUSE_SARDAUKAR]));
            require(story.text("Harkonnen / Atreides / Ordos / Empereur")==expected,"story names chain or wrong roles");
            const auto colours=story.scenePalette();
            for(const int role:{HOUSE_HARKONNEN,HOUSE_ATREIDES,HOUSE_ORDOS,HOUSE_SARDAUKAR})for(int shade=0;shade<8;++shade) {
                const auto actual=colours[houseToPaletteIndex[role]+shade];
                const auto wanted=getHouseColorSDL(getHouseVisualHouse(story.roles[role]),shade);
                require(actual.r==wanted.r && actual.g==wanted.g && actual.b==wanted.b,"story role colour differs from its faction");
            }
            const auto play=[&](auto& movie,const std::string& title) {
                int frames=0;
                while(movie.frame()>0 && ++frames<4000) {
                    if(frames==150 && count==0) {
                        auto scene=renderReadSurface(renderer);
                        require(SavePNG(scene.get(),(std::filesystem::path(output)/(mod+"-"+settings.general.language+"-"+title+".png")).string().c_str())==0,"story frame export");
                    }
                }
                require(frames<4000,"movie never finishes");return frames;
            };
            std::array<int,3> frames;
            {Playable<Meanwhile> movie(house,true);frames[0]=play(movie,"meanwhile-4");}
            {Playable<Meanwhile> movie(house,false);frames[1]=play(movie,"meanwhile-8");}
            {Playable<Finale> movie(house);frames[2]=play(movie,"finale");}
            require(frames[0]>100 && frames[1]>100 && frames[2]>200,"added house lost its dialogue or palace finale");
            const auto found=modelFrames.find(story.model);
            if(found==modelFrames.end())modelFrames.emplace(story.model,frames);
            else require(found->second==frames,"same template has inconsistent cinematic length");
            if(count==0) {
                const int oldColour=getHouseVisualHouse(house);
                setHouseVisualHouse(static_cast<HOUSETYPE>(house),HOUSECOLOR_CUSTOM_APPLE_GREEN);
                {
                    Playable<Finale> bonus(house);
                    for(int i=0;i<frames[2]-60;++i)bonus.frame();
                    auto scene=renderReadSurface(renderer);int painted=0;
                    for(int y=0;y<scene->h;++y)for(int x=0;x<scene->w;++x) {
                        Uint8 r,g,b,a;SDL_GetRGBA(getPixel(scene.get(),x,y),scene->format,&r,&g,&b,&a);
                        for(int shade=0;shade<6;++shade) {
                            const auto c=getHouseColorSDL(HOUSECOLOR_CUSTOM_APPLE_GREEN,shade);
                            if(r==c.r && g==c.g && b==c.b){++painted;break;}
                        }
                    }
                    require(painted>100,"conquered planet ignores the selected bonus colour");
                    require(SavePNG(scene.get(),(std::filesystem::path(output)/(mod+"-"+settings.general.language+"-planet-bonus.png")).string().c_str())==0,"bonus planet preview");
                }
                setHouseVisualHouse(static_cast<HOUSETYPE>(house),oldColour);
            }
            report<<house<<'\t'<<story.model<<'\t'<<story.roles[HOUSE_HARKONNEN]<<'\t'<<story.roles[HOUSE_ATREIDES]<<'\t'
                <<story.roles[HOUSE_ORDOS]<<'\t'<<story.roles[HOUSE_SARDAUKAR]<<'\t'<<frames[0]<<'\t'<<frames[1]<<'\t'<<frames[2]<<'\n';++count;
        }
        for(int i=0;i<256;++i)require(before[i].r==palette[i].r && before[i].g==palette[i].g && before[i].b==palette[i].b,"campaign movie changed global palette");
        settings.audio.playMusic=music;soundPlayer->setSound(sfx);musicPlayer->setMusic(musicOn);
        SDL_Log("CAMPAIGN CINEMA PASS: %s %s, %d houses, templates, names, role colours and complete intermediate/final scenes",mod.c_str(),settings.general.language.c_str(),count);
    }
};
