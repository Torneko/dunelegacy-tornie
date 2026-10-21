/*
 *  This file is part of Dune Legacy.
 *
 *  Dune Legacy is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  Dune Legacy is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Dune Legacy.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef RADARINTERFACE_H
#define RADARINTERFACE_H

#include "DefaultStructureInterface.h"

#include <FileClasses/FontManager.h>
#include <FileClasses/TextManager.h>

#include <House.h>
#include <globals.h>
#include <Command.h>
#include <structures/Radar.h>
#include <mod/ModManager.h>
#include <players/HumanPlayer.h>
#include <GUI/ResearchButton.h>
#include <array>

#include <GUI/Label.h>
#include <GUI/VBox.h>

#include <misc/string_util.h>

class RadarInterface : public DefaultStructureInterface {
public:
    static RadarInterface* create(int objectID) {
        RadarInterface* tmp = new RadarInterface(objectID);
        tmp->pAllocated = true;
        return tmp;
    }

protected:
    explicit RadarInterface(int objectID) : DefaultStructureInterface(objectID) {
        Uint32 color = getHouseColorRGB(getHouseVisualHouse(pLocalHouse->getHouseID()), 3);

        mainHBox.addWidget(&textVBox);

        friendlyUnitsLabel.setTextFontSize(12);
        friendlyUnitsLabel.setTextColor(color);
        textVBox.addWidget(&friendlyUnitsLabel, (Sint32)18);
        enemyUnitsLabel.setTextFontSize(12);
        enemyUnitsLabel.setTextColor(color);
        textVBox.addWidget(&enemyUnitsLabel, (Sint32)18);


        for(int kind=0;kind<radar::Count;++kind) {
            researchButtons[kind].setVisible(false);
            researchButtons[kind].setTextColor(color);
            researchButtons[kind].setPrice(radar::cost(kind));
            researchButtons[kind].setOnClick([objectID,kind] {
                auto* building=currentGame->getObjectManager().getObject(objectID);
                if(building && pLocalPlayer && currentGame->canControlHouse(building->getOwner()))
                    currentGame->getCommandManager().addCommand(Command(pLocalPlayer->getPlayerID(),CMD_RADAR_RESEARCH,objectID,kind));
            });
            textVBox.addWidget(&researchButtons[kind],38);
        }
        textVBox.addWidget(Spacer::create(), 0.99);
    }

    /**
        This method updates the object interface.
        If the object doesn't exists anymore then update returns false.
        \return true = everything ok, false = the object container should be removed
    */
    bool update() override
    {
        ObjectBase* pObject = currentGame->getObjectManager().getObject(objectID);
        if(pObject == nullptr) {
            return false;
        }

        House* pOwner = pObject->getOwner();

        friendlyUnitsLabel.setText(" " + _("Friend") + ": " + std::to_string(pOwner->getNumVisibleFriendlyUnits()));
        enemyUnitsLabel.setText(" " + _("Enemy") + ": " + std::to_string(pOwner->getNumVisibleEnemyUnits()));


        const bool french=settings.general.language=="fr";
        for(int kind=0;kind<radar::Count;++kind) {
            auto& button=researchButtons[kind];
            button.setVisible(ModManager::instance().isTornieContentActive() && currentGame->techLevel >= radar::level(kind));
            std::string name=kind==radar::BaseExpansion?"Base +2":kind==radar::ExtendedVision?"Vision +2":specialName(pOwner,french);
            std::string status;
            if(pOwner->hasRadarResearch(kind))status="OK";
            else if(pOwner->getRadarResearchActive()==kind)
                status=std::to_string(100-100*pOwner->getRadarResearchRemaining()/MILLI2CYCLES(radar::seconds(kind)*1000))+" %";
            if(button.getText()!=name)button.setText(name);
            button.setStatus(status);
            button.setEnabled(currentGame->canControlHouse(pOwner) && pOwner->canStartRadarResearch(kind));
            std::string description=kind==radar::BaseExpansion
                ?(french?"Construction : +2 cases autour de la base.":"Build range: +2 tiles around the base.")
                :kind==radar::ExtendedVision
                ?(french?"Vision : +2 cases, hors unités aériennes.":"Sight: +2 tiles, excluding aircraft.")
                :specialDescription(pOwner,french);
            description+=" "+std::to_string(radar::cost(kind))+(french?" crédits, ":" credits, ")+std::to_string(radar::seconds(kind))
                +(french?" s, niveau ":" s, level ")+std::to_string(radar::level(kind))+(kind==radar::Specialization?" + IX.":".");
            if(button.getTooltipText()!=description)button.setTooltipText(description);
        }
        return DefaultStructureInterface::update();
    }

private:
    friend struct RadarResearchFixture;
    static std::string specialName(const House* house,bool fr) {
        switch(getHouseFactionIdentity(static_cast<HOUSETYPE>(house->getFactionID()))) {
            case HOUSE_HARKONNEN:return fr?"Dégâts":"Damage";
            case HOUSE_ATREIDES:case HOUSE_FREMEN:return fr?"Troopers rapides":"Fast Troopers";
            case HOUSE_ORDOS:return fr?"Véhicules rapides":"Fast vehicles";
            case HOUSE_SARDAUKAR:return fr?"Équipages":"Tank crews";
            case HOUSE_MERCENARY:return fr?"Prix réduits":"Lower prices";
            case HOUSE_NEUTRAL:return fr?"Réparation":"Repairs";
            case HOUSE_REBELS:return fr?"Portée +1":"Range +1";
            case HOUSE_WILDSPADE:return fr?"Aéronefs":"Aircraft";
            case HOUSE_KLESHMERSH:return fr?"Feu +2 s":"Fire +2 s";
            case HOUSE_CUSTOM:return fr?"Blindage":"Armour";
            case HOUSE_THARPIQUE:return fr?"Éclaireurs":"Scouting";
            default:return fr?"Spécialisation":"Specialization";
        }
    }
    static std::string specialDescription(const House* house,bool fr) {
        switch(getHouseFactionIdentity(static_cast<HOUSETYPE>(house->getFactionID()))) {
            case HOUSE_HARKONNEN:return fr?"Combat Tank, explosion du Devastator et missile du palais : dégâts +20 %.":"Combat Tank, Devastator explosion and Palace missile: +20% damage.";
            case HOUSE_ATREIDES:case HOUSE_FREMEN:return fr?"Troopers et escouades : déplacement +15 %.":"Troopers and squads: +15% movement speed.";
            case HOUSE_ORDOS:return fr?"Véhicules légers : déplacement +15 %.":"Light vehicles: +15% movement speed.";
            case HOUSE_SARDAUKAR:return fr?"Chars détruits : 35 % de chance de sauver un Trooper ; remplace le soldat habituel.":"Destroyed tanks: 35% chance to rescue one Trooper, replacing the ordinary Soldier roll.";
            case HOUSE_MERCENARY:return fr?"Infanterie, véhicules légers et aéronefs : prix -10 %.":"Infantry, light vehicles and aircraft: -10% purchase price.";
            case HOUSE_NEUTRAL:return fr?"Bâtiments et chantier de réparation : +25 % de vitesse, coût par point inchangé.":"Building and Repair Yard repairs: +25% speed, unchanged credits per health point.";
            case HOUSE_REBELS:return fr?"Véhicules armés au sol : portée +1 case.":"Armed ground vehicles: +1 tile weapon range.";
            case HOUSE_WILDSPADE:return fr?"Ornithoptères et Carryalls, variantes comprises : résistance +25 %.":"Ornithopters and Carryalls, including variants: +25% health.";
            case HOUSE_KLESHMERSH:return fr?"Incendies persistants : +2 secondes au même rythme de dégâts.":"Persistent flames: +2 seconds with unchanged damage rate.";
            case HOUSE_CUSTOM:return fr?"Chars classiques et variantes : dégâts subis -20 %, déplacement -10 %.":"Conventional tanks and variants: -20% incoming damage, -10% movement speed.";
            case HOUSE_THARPIQUE:return fr?"Éclaireurs et artillerie : vision +2, cumulable avec la recherche commune.":"Scouts and artillery: +2 sight, stacking with the common research.";
            default:return "";
        }
    }
    std::array<ResearchButton,radar::Count> researchButtons;
    VBox    textVBox;

    Label   friendlyUnitsLabel;
    Label   enemyUnitsLabel;

};

#endif // RADARINTERFACE_H
