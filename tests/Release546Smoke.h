#pragma once
#include <MapEditor/LoadMapWindow.h>
#include <MapEditor/ChoamWindow.h>
#include <MapEditor/PlayerSettingsWindow.h>
#include <FileClasses/LoadSavePNG.h>
#include <Campaign/CoopCampaignRuntime.h>
#include <structures/StarPort.h>
#include <misc/OFileStream.h>
#include <Trigger/ReinforcementTrigger.h>
#include <fstream>
#include <set>

struct Release546Fixture {
    static void require(bool ok,const std::string& message) {
        if(!ok) throw std::runtime_error("Release 546: "+message);
    }
    static void frames(const char* directory) {
        require(directory,"missing frame output directory");
        auto& mods=ModManager::instance();const auto previous=mods.getActiveModName();
        int checked=0;
        for(const std::string mod:{"Tornie","Jericho","ArcaKev","TornieLite","JerichoLite","ArcaKevLite","vanilla"}) {
            require(mods.setActiveMod(mod),"frame mod switch");
            for(const auto house:{HOUSE_ATREIDES,HOUSE_ORDOS}) {
                const auto name=getHouseNameByNumber(house);
                auto png=LoadPNG_RW(pFileManager->openFile("CampaignHerald"+name+".png").get());
                auto expected=sdl2::surface_ptr(SDL_CreateRGBSurfaceWithFormat(0,166,182,32,SDL_PIXELFORMAT_ARGB8888));
                SDL_FillRect(expected.get(),nullptr,SDL_MapRGB(expected->format,0,0,0));SDL_Rect square{0,8,166,166};
                SDL_BlitScaled(png.get(),nullptr,expected.get(),&square);
                auto* actual=pGFXManager->getUIGraphicSurface(UI_FactionFrame,house);
                for(int y=0;y<182;++y)for(int x=0;x<166;++x)
                    require((getPixel(actual,x,y)&0xffffff)==(getPixel(expected.get(),x,y)&0xffffff),"Atreides/Ordos frame binding "+mod+name);
                const int colour=getHouseVisualHouse(house);setHouseVisualHouse(house,HOUSECOLOR_GUEST_1);
                for(const unsigned screen:{UI_GameStatsBackground,UI_MapChoiceScreen}) {
                    auto background=pGFXManager->createFactionFrameBackground(screen,house);
                    Release545Smoke::verifyFrames(background.get(),expected.get(),screen);
                    auto capture=sdl2::surface_ptr(SDL_ConvertSurfaceFormat(background.get(),SDL_PIXELFORMAT_RGBA32,0));
                    require(capture && SavePNG(capture.get(),(std::filesystem::path(directory)/(mod+"-"+name+"-"+std::to_string(screen)+".png")).string().c_str())==0,"frame screenshot");++checked;
                }
                setHouseVisualHouse(house,colour);
            }
        }
        mods.setActiveMod(previous);SDL_Log("RELEASE 546 FRAMES PASS: %d score/region compositions, seven mods and bonus colours",checked);
    }
    static void verifyReinforcements(Game& game,const std::vector<int>& allies,int expectedEnemies=2) {
        std::map<int,std::set<Uint32>> times;
        for(const auto& trigger:game.getTriggerManager().getTriggers()) {
            const auto* delivery=dynamic_cast<const ReinforcementTrigger*>(trigger.get());
            if(!delivery)continue;
            require(!delivery->getDroppedUnits().empty(),"empty native reinforcement delivery");
            times[delivery->getHouseID()].insert(delivery->getCycleNumber());
        }
        for(const int house:allies)require(times[house].count(MILLI2CYCLES(5*60000))>0,"allied five-minute reinforcement missing");
        int enemies=0;
        for(const auto& row:times)if(std::find(allies.begin(),allies.end(),row.first)==allies.end()
            && (row.second.count(MILLI2CYCLES(6*60000)) || row.second.count(MILLI2CYCLES(9*60000))
                || row.second.count(MILLI2CYCLES(11*60000)) || row.second.count(MILLI2CYCLES(13*60000))))++enemies;
        require(enemies==expectedEnemies,"primary/support/late reinforcement timers or ownership missing");
    }
    static void verifyOpponents(Game& game,const std::vector<int>& allies,int support,int stage) {
        const int expected=stage==9?4:stage==8?3:2;
        std::set<int> opponents;
        std::vector<Coord> alliedCells,supportCells;
        for(auto* building:structureList) {
            const int owner=building->getOwner()->getHouseID();
            const bool allied=std::find(allies.begin(),allies.end(),owner)!=allies.end();
            if(!allied && building->getItemID()==Structure_ConstructionYard)opponents.insert(owner);
            auto& cells=allied?alliedCells:supportCells;
            if(allied || owner==support)for(int y=0;y<building->getStructureSizeY();++y)for(int x=0;x<building->getStructureSizeX();++x)
                cells.push_back(building->getLocation()+Coord(x,y));
        }
        require(static_cast<int>(opponents.size())==expected,"wrong number of distinct enemy bases at stage "+std::to_string(stage));
        require(opponents.count(support)>0,"persistent support has no live base");
        if(stage==2) {
            for(auto* unit:unitList)if(unit->getItemID()==Unit_MCV
                && std::find(allies.begin(),allies.end(),unit->getOwner()->getHouseID())!=allies.end())
                for(int y=0;y<2;++y)for(int x=0;x<2;++x)alliedCells.push_back(unit->getLocation()+Coord(x,y));
            int clearance=1000;
            for(const auto& a:alliedCells)for(const auto& b:supportCells)clearance=std::min(clearance,std::max(std::abs(a.x-b.x),std::abs(a.y-b.y)));
            require(clearance>=10,"stage-two support buildings are too close to an allied base or MCV: "+std::to_string(clearance));
        }
        verifyReinforcements(game,allies,expected);
    }
    static void run(const char* directory) {
        require(directory,"missing output directory");
        const std::filesystem::path output(directory);
        std::filesystem::create_directories(output);
        auto& mods=ModManager::instance();const auto previous=mods.getActiveModName();
        using Source=LoadMapWindow::MapSource;
        int loaded=0,started=0;
        int coopStarted=0;
        int coopCheckpoints=0;
        std::ofstream report(output/"campaign-starts.csv");
        report << "mod,faction,mission,credits,units,structures\n";
        for(const std::string mod:{"Tornie","Jericho","ArcaKev","TornieLite","JerichoLite","ArcaKevLite","vanilla"}) {
            require(mods.setActiveMod(mod),"mod switch "+mod);
            effectiveGameOptions=mods.loadEffectiveGameOptions(settings.gameOptions);
            const bool revised=mod=="Tornie" || mod=="TornieLite";
            verifyWildspadeTechnology(directory,mod);
            for(const auto source:{Source::VanillaCampaign,Source::VanillaCoopCampaign,Source::Campaign,Source::CoopCampaign}) {
                if(mod=="vanilla" && (source==Source::Campaign || source==Source::CoopCampaign)) continue;
                LoadMapWindow picker(COLOR_DEFAULT,source);
                const int expected=source==Source::VanillaCampaign?198:source==Source::VanillaCoopCampaign?207:
                    (mod.find("Lite")!=std::string::npos?132:264)+(source==Source::CoopCampaign?9:0);
                require(picker.mapSourceDropDown.getSelectedEntryIntData()==static_cast<int>(source),"editor selected source");
                require(picker.mapList.getNumEntries()==expected,"editor map count "+mod+" source="+std::to_string(static_cast<int>(source))+" got="+std::to_string(picker.mapList.getNumEntries()));
                require(!picker.currentMapsAreUserMaps,"bundled map delete protection");
                MapEditor browserEditor;
                for(const auto& path:picker.mapFilepaths) {
                    auto& editor=browserEditor;editor.loadMap(path);
                    require(!editor.getUnitList().empty() || !editor.getStructureList().empty(),"empty editor map "+path);
                    std::set<int> units,buildings;
                    for(const auto& unit:editor.getUnitList()) require(units.insert(unit.id).second,"duplicate unit ID "+path);
                    for(const auto& building:editor.getStructureList()) require(buildings.insert(building.id).second,"duplicate structure ID "+path);
                    const auto order=editor.getPlayerSelectionOrder();
                    for(size_t i=0;i<editor.getPlayers().size();++i) if(editor.getPlayers()[i].bActive)
                        require(std::find(order.begin(),order.end(),static_cast<int>(i))!=order.end(),"loaded campaign owner hidden "+path);
                    ++loaded;
                }
                SDL_RenderSetClipRect(renderer,nullptr);SDL_RenderClear(renderer);picker.draw();
                SavePNG(renderReadSurface(renderer).get(),(output/(mod+"-editor-source-"+std::to_string(static_cast<int>(source))+".png")).string().c_str());
                if(source==Source::VanillaCampaign) {
                    MapEditor editor;editor.loadMap(picker.mapFilepaths.front());
                    const auto before=editor.getChoam();
                    {
                        ChoamWindow window(&editor,HOUSE_ATREIDES);
                        require(window.choamRows.size()==16 && window.choamUnits.size()==31,"CHOAM row capacity");
                        auto& row=window.choamRows.back();
                        row.Checkbox_Unit1.setChecked(true);window.onUnitCheckbox(Unit_RebelHarvester);
                        row.TextBox_Unit1.setValue(7);
                        require(window.rowsScroll.handleMouseWheel(5,5,false),"CHOAM list cannot scroll");
                        for(int i=0;i<25;++i)window.rowsScroll.handleMouseWheel(5,5,false);
                        SDL_RenderClear(renderer);window.draw();
                        SavePNG(renderReadSurface(renderer).get(),(output/(mod+"-choam-last-row.png")).string().c_str());
                        window.onOK();
                    }
                    require(editor.getChoam().at(Unit_RebelHarvester)==7,"CHOAM last unit apply");
                    editor.undoLastOperation();require(editor.getChoam()==before,"CHOAM grouped undo");
                    editor.redoLastOperation();require(editor.getChoam().at(Unit_RebelHarvester)==7,"CHOAM redo");
                    const auto saved=(output/(mod+"-choam-map.ini")).string();editor.saveMap(saved);
                    MapEditor restored;restored.loadMap(saved);
                    require(restored.getChoam()==editor.getChoam(),"CHOAM map save/reload");
                    PlayerSettingsWindow owners(&restored,HOUSE_ATREIDES);
                    owners.onAdvancedBasicToggle();owners.onOK();
                }
            }
            if(mod=="vanilla") continue;
            auto options=effectiveGameOptions;options.easyMode=false;options.chaosMode=false;
            options.immortalHumanPlayer=false;options.instantBuild=false;
            for(const int selected:getFactionSelectionOrder()) {
                const auto faction=static_cast<HOUSETYPE>(selected);
                std::string persistentSupport;
                const std::vector<int> missions=revised?std::vector<int>{1,2,3,4,8,20,21,22}:std::vector<int>{1,8,22};
                for(const int mission:missions) {
                    const std::string name=std::string("scen")+static_cast<char>(std::tolower(getHouseScenarioLetter(faction)))+fmt::sprintf("%03d.ini",mission);
                    const auto path=std::filesystem::path(mods.getModPath(mod))/"campaign"/name;
                    std::ifstream input(path,std::ios::binary);const std::string data((std::istreambuf_iterator<char>(input)),{});
                    require(!data.empty(),"missing scenario "+name);
                    INIFile scenario(path.string());GameInitSettings campaignStart(faction,options);
                    for(const int h:getFactionSelectionOrder()) {
                        const auto house=static_cast<HOUSETYPE>(h);const auto section=getHouseNameByNumber(house);
                        if(!scenario.hasSection(section))continue;
                        const bool human=scenario.getStringValue(section,"Brain","")=="Human";
                        GameInitSettings::HouseInfo info(house,human?1:2);
                        info.addPlayerInfo(GameInitSettings::PlayerInfo(human?settings.general.playerName:"546 AI "+std::to_string(h),human?HUMANPLAYERCLASS:"CampaignAIPlayer"));campaignStart.addHouseInfo(info);
                    }
                    GameInitSettings init(campaignStart,mission,0,0);
                    const auto replay=(output/"campaign-start.rpl").string();
                    {OFileStream stream;require(stream.open(replay),"campaign replay fixture");stream.writeString(settings.general.playerName);init.save(stream);}
                    auto game=std::make_unique<Game>();currentGame=game.get();game->initReplay(replay);
                    require(pLocalHouse && pLocalHouse->getHouseID()==faction,"wrong campaign faction "+mod+name);
                    require(game->getGameInitSettings().getGameType()==GameType::Campaign,"campaign start bypassed campaign rules");
                    if(revised) {
                        const auto support=scenario.getStringValue("BASIC","SupportOpponent","");
                        if(persistentSupport.empty())persistentSupport=support;
                        require(support==persistentSupport,"support faction changed within the campaign");
                        const int stage=scenario.getIntValue("BASIC","TechLevel",0);
                        verifyOpponents(*game,{selected},getHouseByName(support),stage);
                    }
                    require(pLocalHouse->getCredits()==scenario.getIntValue(getHouseNameByNumber(faction),"Credits",-1),"original/revised scenario starting funds");
                    if(mission==1 && revised) {
                        verifyReinforcements(*game,{selected});
                        require(pLocalHouse->getCredits()==1500,"intro funds "+mod+name);
                        int bases=0;for(auto* building:structureList) if(building->getItemID()==Structure_ConstructionYard && building->getOwner()!=pLocalHouse)++bases;
                        require(bases==2,"intro must have two enemy bases "+mod+name);
                        if(getHouseScenarioLetter(faction)=='R') require(pLocalHouse->getNumItems(Structure_ConstructionYard)==0 && pLocalHouse->getNumItems(Structure_StarPort)==1,"Rebels trade opening");
                    }
                    CaptureHuntGameFixture::start(*game);
                    for(int tick=0;tick<(mission==1?150:3);++tick)CaptureHuntGameFixture::fullTick(*game);
                    require(!game->hasFinished(),"premature campaign ending "+mod+name);
                    report<<mod<<','<<selected<<','<<mission<<','<<pLocalHouse->getCredits()<<','<<pLocalHouse->getNumUnits()<<','<<pLocalHouse->getNumStructures()<<'\n';report.flush();
                    if(mission==1 || (revised && (mission==20 || mission==22))) {
                        game->drawScreen();SavePNG(renderReadSurface(renderer).get(),(output/(mod+"-intro-"+std::to_string(selected)+".png")).string().c_str());
                        const auto saved=(output/"campaign-checkpoint.dls").string();const int credits=pLocalHouse->getCredits();require(game->saveGame(saved),"campaign checkpoint save");
                        game.reset();currentGame=nullptr;pLocalHouse=nullptr;pLocalPlayer=nullptr;
                        game=std::make_unique<Game>();currentGame=game.get();game->initGame(GameInitSettings(saved));
                        require(pLocalHouse && pLocalHouse->getCredits()==credits,"campaign checkpoint restore");
                        if(revised)verifyOpponents(*game,{selected},getHouseByName(persistentSupport),scenario.getIntValue("BASIC","TechLevel",0));
                    }
                    game.reset();currentGame=nullptr;pLocalHouse=nullptr;pLocalPlayer=nullptr;++started;
                }
                for(const bool mirror:{false,true}) {
                if(mirror && !(mod=="Tornie" && getHouseScenarioLetter(faction)=='R'))continue;
                for(const bool four:{false,true}) {
                    auto session=coop::CoopCampaignSession::create("release546",mod,getFactionSelectionOrder(),
                        {selected,mirror?static_cast<int>(getHouseByName(persistentSupport)):selected},{settings.general.playerName,"546 Guest"},546,false);
                    if(four) {
                        session.enableExpandedAllies(false);
                        session.configureAlly(2,selected,HUMANPLAYERCLASS,"546 Third");
                        session.configureAlly(3,selected,HUMANPLAYERCLASS,"546 Fourth");
                    }
                    for(int stage=1;stage<=9;++stage) {
                    if(!revised && stage>1)break;
                    if(revised && stage!=1 && stage!=2 && stage!=8 && stage!=9) {session.completeMission(true);continue;}
                    const auto init=coop::makeGameSettings(session,options,"546 revised co-op");
                    session=coop::CoopCampaignSession::fromMapData(init.getFiledata());
                    const auto context=coop::readContext(init.getFiledata());require(context.has_value(),"coop context");
                    auto mapData=sdl2::RWops_ptr{SDL_RWFromConstMem(init.getFiledata().data(),static_cast<int>(init.getFiledata().size()))};
                    INIFile map(mapData.get());
                    auto game=std::make_unique<Game>();currentGame=game.get();game->initGame(init);
                    for(const int index:context->alliedSlots())if(context->slots[index].active()) {
                        const auto* house=game->getHouse(context->slots[index].house);
                        require(house && house->getCredits()==map.getIntValue("Player1","Credits",-1),"coop initial funds before maintenance");
                    }
                    // CHOAM prices and the Starport list are initialized by the
                    // first simulation update, before a player can place orders.
                    CaptureHuntGameFixture::start(*game);CaptureHuntGameFixture::fullTick(*game);
                    std::vector<int> alliedHouses;
                    for(const int index:context->alliedSlots())if(context->slots[index].active())alliedHouses.push_back(context->slots[index].house);
                    if(revised) {
                        require(context->enemyPresent.size()==4,"four stable enemy slots");
                        require(context->slots[3].faction==getHouseByName(persistentSupport),"persistent enemy changed because of guest faction");
                        verifyOpponents(*game,alliedHouses,context->slots[3].house,stage);
                    }
                    for(const int index:context->alliedSlots()) {
                        const auto& slot=context->slots[index];auto* house=game->getHouse(slot.house);
                        if(!slot.active()) {require(!house,"closed co-op ally exists");continue;}
                        require(house,"active co-op house missing after first update");
                        require(house->getNumUnits()>0,"coop intro army");
                        if(revised && stage==1 && getHouseScenarioLetter(faction)=='R') {
                            require(house->getNumItems(Structure_ConstructionYard)==0 && house->getNumItems(Unit_MCV)==0,"trade intro bypassed by a free yard/MCV");
                            require(house->getNumItems(Structure_StarPort)==1 && house->getNumItems(Structure_Refinery)==1,"coop trade economy");
                            StarPort* port=nullptr;for(auto* building:structureList)if(building->getOwner()==house && building->getItemID()==Structure_StarPort)port=dynamic_cast<StarPort*>(building);
                            require(port,"trade starport missing");port->updateBuildList();
                            require(port->isAvailableToBuild(Unit_MCV),"trade MCV unavailable after CHOAM update "+mod+" stock="+std::to_string(house->getChoam().getNumAvailable(Unit_MCV)));
                            const int before=house->getCredits();port->doProduceItem(Unit_MCV,false);
                            require(house->getCredits()<before,"MCV purchase not charged");port->doPlaceOrder();
                        } else if(index!=0) {
                            MCV* vehicle=nullptr;for(auto* unit:unitList)if(unit->getOwner()==house && unit->getItemID()==Unit_MCV)vehicle=dynamic_cast<MCV*>(unit);
                            require(vehicle && vehicle->canDeploy(),"coop guest MCV cannot deploy");
                        }
                    }
                    for(int tick=0;tick<3;++tick)CaptureHuntGameFixture::fullTick(*game);
                    require(!game->hasFinished(),"premature co-op ending");
                    if(revised && stage>=8) {
                        const auto saved=(output/"coop-checkpoint.dls").string();require(game->saveGame(saved),"late cooperative checkpoint save");
                        game.reset();currentGame=nullptr;pLocalHouse=nullptr;pLocalPlayer=nullptr;
                        game=std::make_unique<Game>();currentGame=game.get();
                        GameInitSettings load(getBasename(saved,true),readCompleteFile(saved),"546 late co-op checkpoint");
                        for(const auto& info:init.getHouseInfoList())load.addHouseInfo(info);
                        game->initGame(load);
                        const auto restored=coop::readContext(game->getGameInitSettings().getFiledata());
                        require(restored && restored->slots.size()==context->slots.size(),"late checkpoint dropped participants");
                        require(restored->slots[3].faction==getHouseByName(persistentSupport),"late checkpoint changed support faction");
                        verifyOpponents(*game,alliedHouses,restored->slots[3].house,stage);++coopCheckpoints;
                    }
                    game.reset();currentGame=nullptr;pLocalHouse=nullptr;pLocalPlayer=nullptr;++coopStarted;
                    session.completeMission(true);
                    }
                }
                }
            }
            SDL_Log("RELEASE 546 MOD PASS: %s editor/CHOAM, campaign intros, stages 4/9 and intro checkpoints",mod.c_str());
        }
        mods.setActiveMod(previous);
        SDL_Log("RELEASE 546 R2 MATRIX PASS: %d editor maps; %d solo starts; %d coop starts; %d late cooperative checkpoints",loaded,started,coopStarted,coopCheckpoints);
        SDL_Log("RELEASE 546 R2 PASS: Tornie-only campaign revision, persistent support including mirror guests, stage-two clearance, 3/4 late enemies, %d cooperative checkpoints",coopCheckpoints);
    }
};
