# Dune Legacy Tornie 1.0.539 — Internet multiplayer

Linux release, network protocol **10**. Every participant needs **1.0.539** and the same mod. Version 1.0.538/protocol 9 cannot join this build. Save format remains **9831** and deterministic gameplay rules are preserved.

- The third player is no longer expelled when the host is reachable but guests cannot communicate directly. After the direct connection attempt (five seconds), the session automatically switches to a host relay, without a VPN or an additional relay service.
- The host forwards chat, commands and selections with their original sender identity. Each message has one route after switching. Player lists, departures and reconnections remain consistent.
- Lobby settings, team changes, game start, mod information and configuration negotiation continue through the host. Command cycles and game epochs are preserved; previous-game packets are rejected. Latency buffering allows for both relay legs.
- Every guest must still reach the host. The relay cannot fix an unreachable host and ends when the host leaves.

Validation: CTest engine tests; real NetworkManager/UDP regression with three participants, with guest links open and then filtered; lobby receipt, chat without echoes, twenty synchronization command cycles, selections, start, stale epochs, forged-envelope rejection, a fourth guest, departure/reconnection and host loss. Linux CI runs these checks before building DEB, RPM, TAR.GZ and AppImage packages on Ubuntu 22.04.

These automated tests use loopback sockets and datagram filtering; a human match across three separate Internet connections remains to be checked. Historical two-PC testing applies to 1.0.538. Units, buildings, technology, factions and achievements retain their rules.
