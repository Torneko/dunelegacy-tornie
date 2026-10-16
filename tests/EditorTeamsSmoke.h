#pragma once
#include <MapEditor/PlayerSettingsWindow.h>
#include <MapEditor/MapEditor.h>

struct Release541EditorFixture {
    static void verify(const std::string& output,const std::string& mod) {
        auto require=[](bool ok,const char* message) {
            if(!ok) throw std::runtime_error(std::string("Editor teams: ")+message);
        };
        for(const bool legacy:{false,true}) {
            MapEditor editor;MapInfo info;
            if(legacy) info.mapSeed=12345;
            editor.setMap(MapData(64,64,Terrain_Rock),info);
            const auto order=getFactionSelectionOrder();const int house=order[2];
            editor.getPlayers()[house].bActive=true;editor.getPlayers()[house].brain="Team1";
            auto window=std::make_unique<PlayerSettingsWindow>(&editor,static_cast<HOUSETYPE>(order[0]));
            if(!legacy && isArcaKevMod(mod)) for(std::size_t i=0;i<order.size();++i)
                require(window->playerWidgets[order[i]].playerCheckbox.getText()==fmt::sprintf(_("Player %d:"),i+1),"ArcaKev player/faction numbering");
            auto& row=window->playerWidgets[house];auto& team=row.teamDropDownBox;
            require(team.getNumEntries()==(legacy ? 14 : 12),"twelve teams not offered");
            require(team.getSelectedEntry()=="Team1","saved team selection lost");
            const auto position=window->centralVBox.getWidgetPosition(&row.playerHBox)
                +row.playerHBox.getWidgetPosition(&team);
            const int scrollOffset=std::min(24,std::max(0,window->centralVBox.getMinimumSize().y-window->centralScroll.getSize().y));
            window->centralScroll.handleMouseWheel(5,5,false);
            require(window->centralScroll.handleMouseLeft(position.x+5,position.y-scrollOffset+5,true),"team popup did not open");
            const int entryHeight=GUIStyle::getInstance().getListBoxEntryHeight();
            const int index=legacy ? 3 : 1; // Team2, after Human/CPU in v1.
            require(window->centralScroll.handleMouseLeftOverlay(position.x+5,
                position.y-scrollOffset+team.getSize().y+2+index*entryHeight,true),"team popup click was swallowed");
            require(team.getSelectedEntry()=="Team2","team popup chose the wrong entry");
            team.setSelectedItem(team.getNumEntries()-1);
            window->onOK();window.reset();
            require(editor.getPlayers()[house].brain=="Team12","team twelve not applied");
            const auto file=(std::filesystem::path(output)/(mod+(legacy ? "-teams-v1.ini" : "-teams-v2.ini"))).string();
            editor.saveMap(file);editor.loadMap(file);
            require(editor.getPlayers()[house].brain=="Team12","team twelve not preserved in map");
        }
        if(mod=="ArcaKev") {
            MapEditor editor;editor.setMap(MapData(64,64,Terrain_Rock),MapInfo());
            for(auto& player:editor.getPlayers()) player.bActive=false;
            const auto order=getFactionSelectionOrder();
            for(std::size_t i=0;i<order.size();++i) {
                auto& player=editor.getPlayers()[order[i]];
                player.bActive=true;player.bAnyHouse=true;player.credits=2000+i;
                editor.getUnitList().emplace_back(500+i,static_cast<HOUSETYPE>(order[i]),Unit_Tank,256,Coord(1+2*i,10),0,GUARD);
            }
            const auto file=(std::filesystem::path(output)/"ArcaKev-player-order.ini").string();
            editor.saveMap(file);editor.loadMap(file);
            for(std::size_t i=0;i<order.size();++i) {
                require(editor.getPlayers()[order[i]].credits==2000+static_cast<int>(i),"ArcaKev numbered slot changed faction on load");
                const auto unit=std::find_if(editor.getUnitList().begin(),editor.getUnitList().end(),[&](const auto& unit){return unit.position==Coord(1+2*i,10);});
                require(unit!=editor.getUnitList().end() && unit->house==order[i],"ArcaKev map unit ownership changed");
            }
        }
        SDL_Log("EDITOR TEAMS PASS: %s, actual scrolled popup interaction, twelve teams, v1/v2 map roundtrip",mod.c_str());
    }
};
