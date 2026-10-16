#pragma once
#include <Menu/ModEditorMenu.h>
#include <GUI/ScrollView.h>
#include <mod/ModManager.h>
#include <globals.h>
#include <misc/exceptions.h>

struct ModOptionsUIFixture {
    static void verify() {
        auto require = [](bool ok, const char* message) {
            if(!ok) THROW(std::runtime_error, "MOD OPTIONS CHECK: %s", message);
        };
        struct Probe final : Widget {
            Point last;
            Point getMinimumSize() const override { return Point(120,1000); }
            bool handleMouseMovementOverlay(Sint32 x, Sint32 y) override { last=Point(x,y);return true; }
            bool handleMouseLeftOverlay(Sint32 x, Sint32 y, bool) override { last=Point(x,y);return true; }
            bool handleMouseRightOverlay(Sint32 x, Sint32 y, bool) override { last=Point(x,y);return true; }
            bool handleMouseWheelOverlay(Sint32 x, Sint32 y, bool) override { last=Point(x,y);return true; }
        } probe;
        ScrollView scroll;
        scroll.setContent(&probe);
        scroll.resize(120,60);
        scroll.handleMouseWheel(10,10,false);
        const bool routed=scroll.handleMouseLeftOverlay(5,5,true);
        require(routed && probe.last.y>5,"scroll popup click routing");
        require(scroll.handleMouseMovementOverlay(5,5) && probe.last.y>5,"scroll popup hover routing");
        require(scroll.handleMouseRightOverlay(5,5,true) && probe.last.y>5,"scroll popup right click routing");
        require(scroll.handleMouseWheelOverlay(5,5,false) && probe.last.y>5,"scroll popup wheel routing");
        auto& manager=ModManager::instance();
        const auto name=manager.getActiveModName();
        const auto original=manager.getModInfo(name);
        ModEditorMenu editor(name);
        require(editor.mainVBox.getMinimumSize().y<=450,"Save button falls outside minimum supported screen");
        const bool before=editor.greenSpiceCheckbox.isChecked();
        editor.greenSpiceCheckbox.handleMouseLeft(5,5,true);
        editor.greenSpiceCheckbox.handleMouseLeft(5,5,false);
        require(editor.greenSpiceCheckbox.isChecked()!=before,"checkbox input did not change its value");
        editor.saveModInfo();
        require(manager.getModInfo(name).displayName==original.displayName,"mod name quotes changed after save");
        require(manager.getModInfo(name).gameVersion==original.gameVersion,"mod version changed after save");
        require(manager.getModInfo(name).content.greenSpice!=before,"checkbox not written to the profile");
        require(manager.loadEffectiveGameOptions(settings.gameOptions).content.greenSpice!=before,"active rules did not refresh");
        manager.writeModInfo(manager.getModPath(name),original);
        manager.setActiveMod(name);
        effectiveGameOptions=manager.loadEffectiveGameOptions(settings.gameOptions);
        SDL_Log("MOD OPTIONS CHECK PASSED: compact FR/EN editor, checkbox persistence and scroll popup interaction");
    }
};
