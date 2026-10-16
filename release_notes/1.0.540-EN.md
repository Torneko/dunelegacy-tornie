# Dune Legacy Tornie 1.0.540 — Linux Mint startup

[Windows 64-bit ZIP](https://github.com/Torneko/dunelegacy-tornie/releases/download/v1.0.540/DuneLegacyTornie-1.0.540-Windows-x64.zip) · [Linux Mint AppImage](https://github.com/Torneko/dunelegacy-tornie/releases/download/v1.0.540/DuneLegacyTornie-1.0.540-Linux-Mint-Compatible.AppImage)

The statically linked AppImage incorrectly required shared SDL2_mixer and SDL2_ttf libraries on the host. Without those packages it exited before creating a window. The redundant check is removed; the system loader already validates linked dependencies.

Version 1.0.539 only supplied the standard AppImage and omitted the previous Mint compatibility launcher. This release restores **DuneLegacyTornie-1.0.540-Linux-Mint-Compatible.AppImage**: X11, software rendering, PulseAudio and the previous Mint profile, retaining saves and achievements. Existing configuration files are preserved.

The standard AppImage also detects Linux Mint. The engine honors explicit software rendering and falls back to it when acceleration is unavailable. Launcher logs record drivers and exit status; the log path is printed when launched from a terminal.

CI now checks the **actual rendered main menu**, after resource initialization, for both packaged AppImages in French and English using PulseAudio under Xvfb. Another check exercises software fallback without an accelerated graphics driver. Engine tests and the three-player UDP regression remain enabled. These checks do not replace confirmation on the reported Mint 22.3 / kernel 6.17 PC.

The VPN-free host relay from 1.0.539 remains included. Protocol **10**, save format **9831**, gameplay rules and resources are retained. Use the same version and mod on every participant. Without FUSE, run `./DuneLegacyTornie-1.0.540-Linux-Mint-Compatible.AppImage --appimage-extract-and-run`.


## Windows 64-bit

**DuneLegacyTornie-1.0.540-Windows-x64.zip** now provides the same 1.0.540 version as Linux, including the Internet host relay (protocol 10). Fully extract the archive into a new folder, then run `dunelegacy.exe`. Every participant needs the same version and mod.

Windows CI checks engine tests, the UDP scenario with guest links blocked, mod resources and achievements in French/English, then the installed main menu in both languages. Human matches across three independent Internet connections still await confirmation.
