// SPDX-License-Identifier: GPL-2.0-or-later
#include <CutScenes/CampaignStory.h>
#include <FileClasses/Wsafile.h>
#include <FileClasses/FileManager.h>
#include <FileClasses/INIFile.h>
#include <mod/ModManager.h>
#include <globals.h>
#include <sand.h>
#include <misc/string_util.h>
#include <regex>
#include <stdexcept>

CampaignStory::CampaignStory(int house) : player(house),model(HOUSE_HARKONNEN) {
    if(house<0 || house>=NUM_HOUSES)throw std::invalid_argument("Invalid campaign story house");
    for(int i=0;i<NUM_HOUSES;++i)roles[i]=i;
    const auto identity=getHouseFactionIdentity(static_cast<HOUSETYPE>(house));
    // Legacy campaigns use the same story families as their original briefings.
    const auto family=identity==HOUSE_KLESHMERSH?HOUSE_HARKONNEN:getHouseFallbackHouse(static_cast<HOUSETYPE>(house));
    switch(family) {
        case HOUSE_ATREIDES:case HOUSE_FREMEN:case HOUSE_REBELS:model=HOUSE_ATREIDES;break;
        case HOUSE_ORDOS:case HOUSE_MERCENARY:case HOUSE_NEUTRAL:model=HOUSE_ORDOS;break;
        default:model=HOUSE_HARKONNEN;break;
    }
    bool configured=false;
    if(ModManager::instance().isTornieContentActive() && pFileManager->exists("CampaignPlan.ini")) {
        INIFile plan(pFileManager->openFile("CampaignPlan.ini").get());
        const auto section=getHouseNameByNumber(static_cast<HOUSETYPE>(house));
        const auto letter=plan.getStringValue(section,"Template","");
        if(letter=="H" || letter=="A" || letter=="O") {
            model=letter=="H"?HOUSE_HARKONNEN:letter=="A"?HOUSE_ATREIDES:HOUSE_ORDOS;
            const int first=model==HOUSE_HARKONNEN?HOUSE_ATREIDES:model==HOUSE_ATREIDES?HOUSE_ORDOS:HOUSE_HARKONNEN;
            const int second=model==HOUSE_HARKONNEN?HOUSE_ORDOS:model==HOUSE_ATREIDES?HOUSE_HARKONNEN:HOUSE_ATREIDES;
            const int source[]={first,second,HOUSE_SARDAUKAR};
            for(int i=0;i<3;++i) {
                const auto opponent=getHouseByName(plan.getStringValue(section,"Opponent"+std::to_string(i+1),""));
                if(opponent==HOUSE_INVALID)throw std::runtime_error("Invalid campaign story opponent");
                roles[source[i]]=opponent;
            }
            configured=true;
        }
    }
    if(!configured) {
        if(identity==HOUSE_FREMEN || identity==HOUSE_REBELS) {
            roles[HOUSE_ORDOS]=HOUSE_SARDAUKAR;roles[HOUSE_HARKONNEN]=HOUSE_MERCENARY;
        } else if(identity==HOUSE_SARDAUKAR) {
            roles[HOUSE_ATREIDES]=HOUSE_MERCENARY;roles[HOUSE_ORDOS]=HOUSE_FREMEN;roles[HOUSE_SARDAUKAR]=HOUSE_ATREIDES;
        } else if(identity==HOUSE_MERCENARY || identity==HOUSE_NEUTRAL) {
            roles[HOUSE_ATREIDES]=HOUSE_SARDAUKAR;roles[HOUSE_HARKONNEN]=HOUSE_FREMEN;
        } else if(identity==HOUSE_KLESHMERSH) {
            roles[HOUSE_ATREIDES]=HOUSE_HARKONNEN;roles[HOUSE_ORDOS]=HOUSE_SARDAUKAR;roles[HOUSE_SARDAUKAR]=HOUSE_MERCENARY;
        }
    }
    roles[model]=house;
}
Palette CampaignStory::scenePalette() const {
    Palette result=palette;
    for(const int original:{HOUSE_HARKONNEN,HOUSE_ATREIDES,HOUSE_ORDOS,HOUSE_SARDAUKAR})
        for(int shade=0;shade<8;++shade)
            result[houseToPaletteIndex[original]+shade]=getHouseColorSDL(getHouseVisualHouse(roles[original]),shade);
    return result;
}
void CampaignStory::apply(Wsafile* animation) const {
    if(animation)animation->setPaletteOverride(scenePalette());
}
std::string CampaignStory::text(const std::string& original) const {
    static const std::regex tokens(R"(\b(Harkonnen|Atreides|Ordos|Sardaukars?|Emperor|Empereur)\b)",std::regex::icase);
    std::string result;size_t cursor=0;
    for(std::sregex_iterator i(original.begin(),original.end(),tokens),end;i!=end;++i) {
        result.append(original,cursor,static_cast<size_t>(i->position())-cursor);
        const auto word=strToUpper(i->str());
        const int role=word=="HARKONNEN"?HOUSE_HARKONNEN:word=="ATREIDES"?HOUSE_ATREIDES:word=="ORDOS"?HOUSE_ORDOS:HOUSE_SARDAUKAR;
        result+=getHouseNameByNumber(static_cast<HOUSETYPE>(roles[role]));
        cursor=static_cast<size_t>(i->position()+i->length());
    }
    result.append(original,cursor,std::string::npos);return result;
}
