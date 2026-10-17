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

#include <Menu/ModEditorMenu.h>

#include <globals.h>
#include <config.h>
#include <FileClasses/GFXManager.h>
#include <FileClasses/TextManager.h>
#include <GUI/MsgBox.h>
#include <mod/ModManager.h>

#include <functional>

ModEditorMenu::ModEditorMenu(const std::string& modName)
    : MenuBase(), modName(modName), modified(false) {
    
    modPath = ModManager::instance().getModPath(modName);
    
    // Set up window
    SDL_Texture* pBackground = pGFXManager->getUIGraphic(UI_MenuBackground);
    setBackground(pBackground);
    resize(getTextureSize(pBackground));

    setWindowWidget(&windowWidget);

    windowWidget.addWidget(&mainVBox, 
        Point((getRendererWidth() - 580) / 2, (getRendererHeight() - 450) / 2),
        Point(580, 450));

    // Title
    titleLabel.setText(_("EDIT MOD"));
    titleLabel.setAlignment(Alignment_HCenter);
    mainVBox.addWidget(&titleLabel);
    mainVBox.addWidget(VSpacer::create(20));

    // Display Name row
    mainVBox.addWidget(&nameRow, 25);
    displayNameLabel.setText(_("Display Name:"));
    nameRow.addWidget(&displayNameLabel, 120);
    nameRow.addWidget(HSpacer::create(10));
    displayNameTextBox.setOnTextChange([this](bool) { modified = true; });
    nameRow.addWidget(&displayNameTextBox, 250);
    nameRow.addWidget(Spacer::create());
    
    mainVBox.addWidget(VSpacer::create(10));
    
    // Version row
    mainVBox.addWidget(&versionRow, 25);
    versionLabel.setText(_("Version:"));
    versionRow.addWidget(&versionLabel, 120);
    versionRow.addWidget(HSpacer::create(10));
    versionTextBox.setOnTextChange([this](bool) { modified = true; });
    versionRow.addWidget(&versionTextBox, 150);
    versionRow.addWidget(Spacer::create());
    
    mainVBox.addWidget(VSpacer::create(25));
    
    customContentCheckbox.setText(_("Custom units and buildings"));
    customContentCheckbox.setTooltipText(_("Disable to use Neutral vanilla units, buildings and technologies for every faction. Spice options remain independent."));
    greenSpiceCheckbox.setText(_("Green spice"));
    redSpiceCheckbox.setText(_("Red spice"));
    purpleSpiceCheckbox.setText(_("Purple spice"));
    blueSpiceCheckbox.setText(_("Blue spice"));
    for(auto* checkbox : {&customContentCheckbox, &greenSpiceCheckbox, &redSpiceCheckbox, &purpleSpiceCheckbox, &blueSpiceCheckbox}) {
        checkbox->setOnClick([this]() { modified = true; });
        mainVBox.addWidget(checkbox, 20);
    }
    mainVBox.addWidget(VSpacer::create(8));

    instructionsLabel1.setText(settings.general.language == "fr"
        ? "Ces options s'appliquent aux nouvelles parties de ce mod."
        : "These options apply to new games using this mod.");
    mainVBox.addWidget(&instructionsLabel1, 24);
    pathLabel.setText(_("Mod folder: ") + modName);
    mainVBox.addWidget(&pathLabel, 20);
    mainVBox.addWidget(Spacer::create());

    // Buttons at bottom
    mainVBox.addWidget(&buttonHBox, 30);
    
    buttonHBox.addWidget(Spacer::create());
    
    saveButton.setText(_("SAVE"));
    saveButton.setOnClick(std::bind(&ModEditorMenu::onSave, this));
    buttonHBox.addWidget(&saveButton, 120);
    
    buttonHBox.addWidget(HSpacer::create(20));
    
    cancelButton.setText(_("CANCEL"));
    cancelButton.setOnClick(std::bind(&ModEditorMenu::onCancel, this));
    buttonHBox.addWidget(&cancelButton, 120);
    
    buttonHBox.addWidget(Spacer::create());

    // Load current mod info
    loadModInfo();
}

ModEditorMenu::~ModEditorMenu() = default;

void ModEditorMenu::loadModInfo() {
    ModInfo info = ModManager::instance().getModInfo(modName);
    displayNameTextBox.setText(info.displayName);
    versionTextBox.setText(info.version);
    customContentCheckbox.setChecked(info.content.customUnitsAndBuildings);
    greenSpiceCheckbox.setChecked(info.content.greenSpice);
    redSpiceCheckbox.setChecked(info.content.redSpice);
    purpleSpiceCheckbox.setChecked(info.content.purpleSpice);
    blueSpiceCheckbox.setChecked(info.content.blueSpice);
    const auto allowed=ModContentOptions::legacy(modName);
    if(isTornieFamilyMod(modName)) {
        greenSpiceCheckbox.setEnabled(allowed.greenSpice);
        redSpiceCheckbox.setEnabled(allowed.redSpice);
        purpleSpiceCheckbox.setEnabled(allowed.purpleSpice);
        blueSpiceCheckbox.setEnabled(allowed.blueSpice);
    }
    modified = false;
}

void ModEditorMenu::saveModInfo() {
    ModInfo info = ModManager::instance().getModInfo(modName);
    info.displayName = displayNameTextBox.getText();
    info.version = versionTextBox.getText();
    info.content.customUnitsAndBuildings = customContentCheckbox.isChecked();
    info.content.greenSpice = greenSpiceCheckbox.isChecked();
    info.content.redSpice = redSpiceCheckbox.isChecked();
    info.content.purpleSpice = purpleSpiceCheckbox.isChecked();
    info.content.blueSpice = blueSpiceCheckbox.isChecked();
    ModManager::instance().writeModInfo(modPath, info);
    if(ModManager::instance().getActiveModName() == modName) {
        ModManager::instance().setActiveMod(modName);
        effectiveGameOptions = ModManager::instance().loadEffectiveGameOptions(settings.gameOptions);
    }
    modified = false;
}

void ModEditorMenu::onSave() {
    saveModInfo();
    openWindow(MsgBox::create(_("Mod info saved.")));
    quit();
}

void ModEditorMenu::onCancel() {
    if (modified) {
        // Could add confirmation dialog here
    }
    quit();
}
