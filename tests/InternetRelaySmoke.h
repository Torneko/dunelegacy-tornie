// Real UDP transport regression, independent of graphics, profiles and external services.
// Included only by a test-enabled NetworkManager build.
namespace {
ENetHost* relayTestFirst = nullptr;
ENetHost* relayTestSecond = nullptr;
unsigned relayTestDropped = 0;
int ENET_CALLBACK blockRelayTestGuestLink(ENetHost* host, ENetEvent*) {
    const auto otherPort = host == relayTestFirst ? relayTestSecond->address.port : relayTestFirst->address.port;
    if(host->receivedAddress.port == otherPort) { ++relayTestDropped; return 1; }
    return 0;
}
}

void NetworkManager::verifyInternetRelayTransport() {
    const auto require = [](bool value, const char* message) {
        if(!value) throw std::runtime_error(std::string("Internet relay regression: ") + message);
    };
    for(bool blocked : {false, true}) {
        NetworkManager server(0, "", true), first(0, "", true), second(0, "", true), extra(0, "", true);
        GameInitSettings lobby;
        int players = 1, firstJoined = 0, secondJoined = 0, extraJoined = 0;
        bool unexpectedDeparture = false;
        server.setGetChangeEventListForNewPlayerCallback([&](const std::string& name) {
            server.updateServer(++players);
            ChangeEventList changes;
            changes.changeEventList.emplace_back(players - 1, name);
            server.sendChangeEventList(changes);
            return changes;
        });
        first.setOnReceiveGameInfo([&](const GameInitSettings&, const ChangeEventList& changes) {
            require(changes.changeEventList.back().newStringValue == "Guest1", "first guest lobby identity");
            ++firstJoined;
        });
        second.setOnReceiveGameInfo([&](const GameInitSettings&, const ChangeEventList& changes) {
            require(changes.changeEventList.back().newStringValue == "Guest2", "second guest lobby identity");
            ++secondJoined;
        });
        first.setOnPeerDisconnected([&](const std::string&, bool, int) { unexpectedDeparture = true; });
        second.setOnPeerDisconnected([&](const std::string&, bool, int) { unexpectedDeparture = true; });
        // Internet mode, but diagnosticsOnly suppresses STUN/UPnP/discovery.
        server.startServer(false, "Internet relay regression", "Host", &lobby, 1, 4);
        const auto pump = [&] { server.update(); first.update(); second.update(); extra.update(); SDL_Delay(1); };
        const auto until = [&](const std::function<bool()>& done) {
            const Uint32 started = SDL_GetTicks();
            while(!done() && SDL_GetTicks() - started < AWAITING_CONNECTION_TIMEOUT + 3000) pump();
            require(done(), "operation timed out");
        };
        const auto settle = [&] {
            const auto started = SDL_GetTicks();
            while(SDL_GetTicks() - started < 120) pump();
        };
        first.connect("127.0.0.1", server.getHost()->address.port, "Guest1");
        until([&] { return firstJoined == 1; });
        relayTestFirst = first.getHost(); relayTestSecond = second.getHost(); relayTestDropped = 0;
        if(blocked) {
            relayTestFirst->intercept = blockRelayTestGuestLink;
            relayTestSecond->intercept = blockRelayTestGuestLink;
        }
        const auto started = SDL_GetTicks();
        second.connect("127.0.0.1", server.getHost()->address.port, "Guest2");
        until([&] { return secondJoined == 1; });
        settle();
        require(!unexpectedDeparture, "a connected guest was expelled during admission");
        require(firstJoined == 1 && secondJoined == 1 && players == 3, "admission callback repeated");
        require(server.hostRelayEnabled == blocked && first.hostRelayEnabled == blocked
            && second.hostRelayEnabled == blocked, "wrong topology selected");
        require(!blocked || relayTestDropped > 0, "guest filter did not block any datagrams");
        require(server.getConnectedPeers() == std::list<std::string>{"Guest1", "Guest2"}, "host roster");
        auto firstNames = first.getConnectedPeers(); firstNames.sort();
        auto secondNames = second.getConnectedPeers(); secondNames.sort();
        require(firstNames == std::list<std::string>{"Guest2", "Host"}, "first guest roster");
        require(secondNames == std::list<std::string>{"Guest1", "Host"}, "second guest roster");
        require(!blocked || (first.peerList.size() == 1 && second.peerList.size() == 1
            && first.awaitingConnectionList.empty() && second.awaitingConnectionList.empty()), "direct links survived relay switch");

        int chats[3] = {}, commands[3] = {}, selections[3] = {}, starts = 0, lobbyUpdates = 0;
        NetworkManager* participants[] = {&server, &first, &second};
        const std::string names[] = {"Host", "Guest1", "Guest2"};
        for(int i = 0; i < 3; ++i) {
            participants[i]->setGameEpoch(7);
            participants[i]->setOnReceiveChatMessage([&, i](const std::string& name, const std::string& message) {
                require(name != names[i] && message == name, "chat source changed or echoed");
                ++chats[i];
            });
            participants[i]->setOnReceiveCommandList([&, i](const std::string& name, const CommandList& list) {
                require(name != names[i] && list.commandList.size() == 20, "command identity/size changed");
                for(size_t cycle = 0; cycle < list.commandList.size(); ++cycle) {
                    const auto& entry = list.commandList[cycle];
                    const auto sender = std::find(std::begin(names), std::end(names), name);
                    require(sender != std::end(names), "unknown command sender");
                    require(entry.cycle == 100 + cycle && entry.commands.size() == 1
                        && entry.commands[0].getPlayerID() == std::distance(std::begin(names), sender)
                        && entry.commands[0].getCommandID() == CMD_TEST_SYNC
                        && entry.commands[0].getParameter() == std::vector<Uint32>{12345 + static_cast<Uint32>(cycle)},
                        "synchronization command cycle or parameters changed");
                }
                ++commands[i];
            });
            participants[i]->setOnReceiveSelectionList([&, i](const std::string& name, const std::set<Uint32>& ids, int group) {
                require(name != names[i] && ids == std::set<Uint32>{42, 87} && group == 3, "selection identity/payload changed");
                ++selections[i];
            });
        }
        first.setOnReceiveChangeEventList([&](const ChangeEventList& list) {
            require(list.changeEventList.front().newValue == 2, "lobby team update changed"); ++lobbyUpdates;
        });
        ChangeEventList changes;
        changes.changeEventList.emplace_back(ChangeEventList::ChangeEvent::EventType::ChangeTeam, 1, 2);
        second.sendChangeEventList(changes); // Guest -> host validation remains the existing lobby path.
        server.setOnReceiveChangeEventList([&](const ChangeEventList& list) { server.sendChangeEventList(list); });
        first.setOnStartGame([&](unsigned) { ++starts; });
        second.setOnStartGame([&](unsigned) { ++starts; });
        server.sendStartGame(1000);
        until([&] { return starts == 2 && lobbyUpdates == 1; });
        server.stopAnnouncing(); // Relay must survive the transition into the game.
        CommandList commandList;
        for(Uint32 cycle = 0; cycle < 20; ++cycle)
            commandList.commandList.emplace_back(100 + cycle, std::vector<Command>{Command(1, CMD_TEST_SYNC, 12345 + cycle)});
        for(int i = 0; i < 3; ++i) {
            participants[i]->sendChatMessage(names[i]);
            for(auto& entry : commandList.commandList)
                entry.commands = {Command(i, CMD_TEST_SYNC, 12345 + entry.cycle - 100)};
            participants[i]->sendCommandList(commandList);
            participants[i]->sendSelectedList({42, 87}, 3);
        }
        until([&] { return chats[0] == 2 && chats[1] == 2 && chats[2] == 2
            && commands[0] == 2 && commands[1] == 2 && commands[2] == 2
            && selections[0] == 2 && selections[1] == 2 && selections[2] == 2; });
        // Old mission packets must neither execute on the host nor be relayed.
        second.setGameEpoch(6);
        second.sendCommandList(commandList); second.sendSelectedList({9}, 1);
        second.setGameEpoch(7);
        // A guest cannot forge a relay envelope or host-only start/transition.
        ENetPacketOStream forged(ENET_PACKET_FLAG_RELIABLE);
        forged.writeUint32(NETWORKPACKET_RELAY); forged.writeString("Host"); forged.writeString("invalid");
        second.sendPacketToHost(forged);
        ENetPacketOStream forgedStart(ENET_PACKET_FLAG_RELIABLE);
        forgedStart.writeUint32(NETWORKPACKET_STARTGAME); forgedStart.writeUint32(0);
        second.sendPacketToHost(forgedStart);
        settle();
        for(int i = 0; i < 3; ++i) require(chats[i] == 2 && commands[i] == 2 && selections[i] == 2,
            "duplicate delivery, stale epoch or forged source accepted");
        require(starts == 2, "guest started the game");
        if(blocked) {
            first.setOnReceiveChangeEventList({});
            // Later admission reuses the relay without another mesh timeout.
            extra.setOnReceiveGameInfo([&](const GameInitSettings&, const ChangeEventList&) { ++extraJoined; });
            extra.connect("127.0.0.1", server.getHost()->address.port, "Guest3");
            until([&] { return extraJoined == 1; });
            require(extra.hostRelayEnabled && extra.getConnectedPeers().size() == 3, "later guest missing relay roster");
            bool departed = false;
            first.setOnPeerDisconnected([&](const std::string& name, bool isHost, int cause) {
                departed = name == "Guest3" && !isHost && cause == NETWORKDISCONNECT_QUIT;
            });
            extra.disconnect();
            until([&] { return departed; });
            server.updateServer(--players);
            require(first.getConnectedPeers().size() == 2, "departed relay guest stayed in roster");
            // The same manager can reconnect after leaving; no stale relay identity.
            until([&] { return extra.connectPeer == nullptr; });
            extra.connect("127.0.0.1", server.getHost()->address.port, "Guest3");
            until([&] { return extraJoined == 2; });
            require(extra.getConnectedPeers().size() == 3, "reconnected roster is stale");
            bool hostLeft = false;
            second.setOnPeerDisconnected([&](const std::string&, bool isHost, int) { hostLeft = isHost; });
            server.disconnect();
            until([&] { return hostLeft && second.connectPeer == nullptr; });
            require(second.getConnectedPeers().empty(), "host loss retained logical peers");
        }
        server.stopServer();
        SDL_Log("NETWORK %s PASSED: join=%u ms, dropped=%u, lobby, chat, 20 synchronized cycles, selections, epochs, authority, departures/rejoin",
            blocked ? "RELAY" : "DIRECT", SDL_GetTicks() - started, relayTestDropped);
    }
}
