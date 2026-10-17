#pragma once
#include <MapEditor/MapEditor.h>

struct EditorOwnershipFixture {
    static void verify(const std::string& output,const std::string& mod) {
        auto require=[](bool ok,const std::string& message) {
            if(!ok) throw std::runtime_error("Editor object owner: "+message);
        };
        for(const bool numbered:{false,true}) {
            MapEditor editor;editor.setMap(MapData(64,32,Terrain_Rock),MapInfo());
            const auto order=getFactionSelectionOrder();
            for(auto& player:editor.players) {
                player.bActive=isCampaignHouseAvailable(player.house);
                player.bAnyHouse=numbered;
            }
            for(std::size_t i=0;i<order.size();++i) {
                const auto house=static_cast<HOUSETYPE>(order[i]);
                editor.units.emplace_back(200+house,house,Unit_Tank,256,Coord(1+3*i,8),0,GUARD);
                editor.structures.emplace_back(100+house,house,Structure_WindTrap,256,Coord(1+3*i,2));
            }
            const auto inspect=[&] {
                auto& ui=*editor.pInterface;ui.onHouseChanges();
                for(const int house:order) {
                    for(const bool unit:{true,false}) {
                        ui.houseDropDownBox.setSelectedItem(0);
                        editor.selectedUnitID=unit ? 200+house : INVALID;
                        editor.selectedStructureID=unit ? INVALID : 100+house;
                        ui.onObjectSelected();
                        require(ui.houseDropDownBox.getSelectedEntryIntData()==house,
                            mod+" selected "+(unit ? "unit" : "building")+" house "+std::to_string(house)
                            +" points to "+std::to_string(ui.houseDropDownBox.getSelectedEntryIntData()));
                        require(ui.house==house,"interface colour changed to another owner");
                    }
                }
            };
            inspect();
            editor.selectedUnitID=editor.selectedStructureID=INVALID;
            const auto file=(std::filesystem::path(output)/(mod+(numbered ? "-owner-numbered.ini" : "-owner-named.ini"))).string();
            editor.saveMap(file);editor.loadMap(file);inspect();
            require(editor.units.size()==order.size() && editor.structures.size()==order.size(),"objects lost on reload");
            // Inactive houses are still selectable by their stable identity.
            editor.players[order.back()].bActive=false;inspect();
        }
        SDL_Log("EDITOR OWNER PASS: %s, every available faction, units/buildings, named/numbered, inactive houses and map reload",mod.c_str());
    }
};
