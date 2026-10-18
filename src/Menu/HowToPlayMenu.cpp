/*
 *  This file is part of Dune Legacy Tornie.
 *
 *  Licensed under GPL-2.0-or-later.
 */

#include <Menu/HowToPlayMenu.h>
#include <Menu/MainMenuButtonColor.h>

#include <globals.h>

#include <FileClasses/GFXManager.h>
#include <FileClasses/TextManager.h>

// RTS guide for the standalone Tornie edition.
static const char* kHowToPlayBody =
    "DUNE LEGACY TORNIE\n"
    "Build a base, harvest spice, and command your army on Arrakis.\n"
    "\n"
    "1) CHOOSE A GAME\n"
    "Play a campaign, a custom game, multiplayer or the map editor.\n"
    "\n"
    "2) BUILD YOUR ECONOMY\n"
    "Start with a Construction Yard, Windtraps and a Refinery.\n"
    "Harvesters collect spice; Silos expand storage. Build on rock.\n"
    "Keep enough power. Concrete avoids the bare-rock health penalty.\n"
    "The optional bottom-left counter shows your own Harvesters.\n"
    "\n"
    "3) DEFEND AND EXPAND\n"
    "Upgrade factories and build IX to unlock advanced units.\n"
    "Repair, Upgrade and Sell occupy the structure's top toolbar.\n"
    "Selling is on by default in every mod except Vanilla.\n"
    "Sales refund 50%, or 75% for Easy Mode allies, without storage.\n"
    "\n"
    "4) MODS\n"
    "Choose Vanilla, Tornie, Tornie Lite, Jericho, Jericho Lite,\n"
    "ArcaKev or ArcaKev Lite in MODS. Full mods have twelve factions;\n"
    "Lite editions have six. Each has its own campaigns and rules.\n"
    "All keep normal spice. Tornie/Lite add green and purple spice;\n"
    "Jericho/Lite add red and blue; ArcaKev/Lite add green and blue.\n"
    "Mod checkboxes also control the editor and generated maps.\n"
    "Custom units have blue stars only in the editor, not portraits.\n"
    "Achievements are shared; ArcaKev and Lite have campaign awards.\n"
    "\n"
    "5) CO-OP AND ALLIANCES\n"
    "Co-op supports four allied factions and up to eight controllers\n"
    "in Archon, freely assigned to factions by the host.\n"
    "The host selects enemy AI, Easy Mode and +5/+15 enemy forces.\n"
    "Easy starts at mission 2, adds 500 credits each mission and\n"
    "reduces allied purchases by 25 credits (minimum 1).\n"
    "Only enabled allies receive starting forces and reinforcements.\n"
    "Custom and multiplayer lobbies have separate options for allied\n"
    "control and building on allied land. Ownership and funds stay\n"
    "separate. Co-op options sit to the right of chat.\n"
    "Load manual or automatic saves in the host lobby; guests rejoin.\n"
    "Everyone needs the same game revision, mod and network protocol.\n";

static const char* kHowToPlayBodyFrench =
    "DUNE LEGACY TORNIE\n"
    "Construisez une base, récoltez l’épice et dirigez votre armée.\n"
    "\n"
    "1) CHOISIR UNE PARTIE\n"
    "Campagne, partie personnalisée, multijoueur ou éditeur de cartes.\n"
    "\n"
    "2) DÉVELOPPER L’ÉCONOMIE\n"
    "Commencez par un chantier, des éoliennes et une raffinerie.\n"
    "Les moissonneuses récoltent l’épice ; les silos la stockent.\n"
    "Construisez sur la roche et alimentez la base. Le béton évite\n"
    "la pénalité de santé. Le compteur facultatif en bas à gauche\n"
    "indique le nombre de vos propres moissonneuses.\n"
    "\n"
    "3) DÉFENDRE ET PROGRESSER\n"
    "Améliorez les usines et construisez IX pour les unités avancées.\n"
    "Barre du bâtiment : Réparer, Améliorer, puis Vendre.\n"
    "La vente est activée par défaut dans tous les mods hors Vanilla.\n"
    "Remboursement : 50 %, ou 75 % pour les alliés en mode facile,\n"
    "sans stockage nécessaire.\n"
    "\n"
    "4) MODS\n"
    "Dans MODS : Vanilla, Tornie, Tornie Lite, Jericho, Jericho Lite,\n"
    "ArcaKev et ArcaKev Lite. Les mods complets ont douze factions ;\n"
    "les éditions Lite en ont six. Chacun a ses campagnes et règles.\n"
    "L’épice normale reste présente. Tornie/Lite : verte et mauve ;\n"
    "Jericho/Lite : rouge et bleue ; ArcaKev/Lite : verte et bleue.\n"
    "Les cases des mods s’appliquent aussi à l’éditeur et aux cartes\n"
    "générées. Les étoiles bleues sont réservées à l’éditeur.\n"
    "Les succès sont communs ; ArcaKev et Lite ont leurs succès\n"
    "de campagnes.\n"
    "\n"
    "5) COOP ET ALLIANCES\n"
    "La coop accueille quatre factions alliées, jusqu’à huit joueurs\n"
    "en archonte, avec une répartition libre par l’hôte.\n"
    "L’hôte règle les IA, le mode facile et les forces +5/+15.\n"
    "Facile : départ à la mission 2, +500 crédits à chaque mission,\n"
    "achats alliés réduits de 25 crédits (minimum 1).\n"
    "Seuls les alliés activés reçoivent unités de départ et renforts.\n"
    "Les lobbies personnalisés et multijoueurs proposent séparément\n"
    "le contrôle des alliés et la construction sur leurs terres.\n"
    "Les propriétaires et les crédits restent distincts.\n"
    "Options coop à droite du chat. Chargez une sauvegarde manuelle\n"
    "ou automatique dans le lobby hôte ; les invités rejoignent.\n"
    "Tous doivent avoir la même révision du jeu, le même mod et\n"
    "le même protocole réseau.\n";

HowToPlayMenu::HowToPlayMenu() : MenuBase()
{
    SDL_Texture* pBackground = pGFXManager->getUIGraphic(UI_MenuBackground);
    setBackground(pBackground);
    resize(getTextureSize(pBackground));

    setWindowWidget(&windowWidget);

    SDL_Texture* pPlanetBackground = pGFXManager->getUIGraphic(UI_PlanetBackground);
    planetPicture.setTexture(pPlanetBackground);
    SDL_Rect dest1 = calcAlignedDrawingRect(pPlanetBackground);
    dest1.y = dest1.y - getHeight(pPlanetBackground) / 2 + 10;
    windowWidget.addWidget(&planetPicture, dest1);

    SDL_Texture* pDuneLegacy = pGFXManager->getUIGraphic(UI_DuneLegacy);
    duneLegacy.setTexture(pDuneLegacy);
    SDL_Rect dest2 = calcAlignedDrawingRect(pDuneLegacy);
    dest2.y = dest2.y + getHeight(pDuneLegacy) / 2 + 28;
    windowWidget.addWidget(&duneLegacy, dest2);

    title.setText(_("HOW TO PLAY"));
    title.setTextFontSize(20);
    title.setAlignment(Alignment_HCenter);
    windowWidget.addWidget(&title,
                           Point(getRendererWidth() / 2 - 280,
                                 getRendererHeight() / 2 - 200),
                           Point(560, 30));

    body.setTextFontSize(14);
    body.setText(settings.general.language == "fr" ? kHowToPlayBodyFrench : kHowToPlayBody);
    body.setAutohideScrollbar(false);
    windowWidget.addWidget(&body,
                           Point(getRendererWidth() / 2 - 320,
                                 getRendererHeight() / 2 - 160),
                           Point(640, 320));

    backButton.setText(_("Back"));
    MainMenuButtonColor::apply(backButton);
    backButton.setOnClick(std::bind(&HowToPlayMenu::onBack, this));
    windowWidget.addWidget(&backButton,
                           Point(getRendererWidth() / 2 - 60,
                                 getRendererHeight() / 2 + 175),
                           Point(120, 30));
}

HowToPlayMenu::~HowToPlayMenu() = default;

void HowToPlayMenu::onBack() {
    quit();
}
