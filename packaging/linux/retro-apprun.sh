#!/usr/bin/env bash
# Dedicated AppRun for older Linux GPUs (GT 540M, legacy Nouveau).
# This is opt-in for a separate retro package; normal Linux builds are unchanged.
set -uo pipefail

appdir="${APPDIR:-$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)}"
export APPDIR="${appdir}"
export PATH="${APPDIR}/usr/bin:${PATH}"
# Include the game's private vcpkg library folder (used by the CMake
# INSTALL_RPATH) as well as linuxdeploy's standard AppDir locations.
# This also covers libraries loaded with dlopen() by other components.
export LD_LIBRARY_PATH="${APPDIR}/usr/lib/dunelegacy:${APPDIR}/usr/lib/x86_64-linux-gnu/dunelegacy:${APPDIR}/usr/lib64/dunelegacy:${APPDIR}/usr/lib:${APPDIR}/usr/lib/x86_64-linux-gnu:${APPDIR}/usr/lib64${LD_LIBRARY_PATH:+:${LD_LIBRARY_PATH}}"
export XDG_DATA_DIRS="${APPDIR}/usr/share${XDG_DATA_DIRS:+:${XDG_DATA_DIRS}}"

# The C++ engine must accept software rendering, not merely SDL's hint.
export DUNELEGACY_RETRO_SOFTWARE="${DUNELEGACY_RETRO_SOFTWARE:-1}"
if [[ -n "${DISPLAY:-}" && -z "${SDL_VIDEODRIVER:-}" ]]; then
    export SDL_VIDEODRIVER=x11
fi
# Use only when sound devices/drivers cause startup crashes.
if [[ "${DUNELEGACY_RETRO_NO_AUDIO:-0}" == 1 ]]; then
    export SDL_AUDIODRIVER=dummy
fi

state_root="${XDG_STATE_HOME:-${HOME:-/tmp}/.local/state}"
log_dir="${state_root}/DuneLegacyTornie"
if ! mkdir -p "${log_dir}" 2>/dev/null; then
    log_dir="/tmp/DuneLegacyTornie-${UID:-0}"
    mkdir -p "${log_dir}"
fi
log_file="${log_dir}/retro-$(date +%Y%m%d-%H%M%S)-$$.log"

{
    printf 'Dune Legacy Tornie 1.0.538 retro launcher\n'
    printf 'Kernel: %s\n' "$(uname -r 2>/dev/null || true)"
    printf 'Video backend: %s\n' "${SDL_VIDEODRIVER:-auto}"
    printf 'Renderer: %s\n' "${DUNELEGACY_RETRO_SOFTWARE:-0}"
    printf 'Audio backend: %s\n' "${SDL_AUDIODRIVER:-auto}"
    printf 'AppDir: %s\n' "${APPDIR}"
    printf '%s\n' '--- game log ---'
} > "${log_file}"

printf 'Retro log: %s\n' "${log_file}"
"${APPDIR}/usr/bin/dunelegacy" --window "$@" 2>&1 | tee -a "${log_file}"
status=${PIPESTATUS[0]}
printf 'Game exit status: %s\n' "${status}" | tee -a "${log_file}"
exit "${status}"
