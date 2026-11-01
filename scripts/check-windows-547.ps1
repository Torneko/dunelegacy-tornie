param([string]$Flag='--verify-release547',[ValidateSet('fr','en')][string]$Language='fr')
$ErrorActionPreference='Stop'
$projectRoot=Split-Path -Parent $PSScriptRoot
$runtime=Join-Path $projectRoot 'windows-build-546\bin'
$profile=Join-Path $projectRoot ('reports\native-547-profile\'+$Language)
$output=Join-Path $projectRoot ('reports\native-547\'+$Language+'\'+$Flag.TrimStart('-'))
New-Item -ItemType Directory -Force -Path $output,(Join-Path $profile 'mods') | Out-Null
$env:SMOKE_RUNTIME_MODS=Join-Path $runtime 'mods'
$env:SMOKE_PROFILE_MODS=Join-Path $profile 'mods'
@'
from pathlib import Path
import os,shutil
source=Path(os.environ['SMOKE_RUNTIME_MODS']);target=Path(os.environ['SMOKE_PROFILE_MODS'])
for path in source.rglob('*'):
    if not path.is_file():continue
    dest=target/path.relative_to(source)
    if dest.exists() and dest.stat().st_size==path.stat().st_size and dest.read_bytes()==path.read_bytes():continue
    dest.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(path,dest)
'@ | & 'C:\msys64\mingw64\bin\python.exe' -
if($LASTEXITCODE -ne 0) {throw 'Smoke profile synchronization failed'}
Set-Content -LiteralPath (Join-Path $profile 'mods\active_mod.txt') -Value 'Tornie' -Encoding utf8
Set-Content -LiteralPath (Join-Path $profile 'Dune Legacy.ini') -Encoding utf8 -Value "[General]`nLanguage=$Language`nPlay Intro=false`n[Video]`nFullscreen=false`n[Audio]`nPlay Music=false`n"
$env:DUNELEGACY_USER_DIR=$profile
$env:SDL_RENDER_DRIVER='software'
$env:SDL_AUDIODRIVER='dummy'
$env:PATH=($env:PATH -split ';' | Where-Object { $_ -notlike '*msys64*' }) -join ';'
$env:DUNELEGACY_SMOKE_DIR=$output
$verification=Start-Process -FilePath (Join-Path $runtime 'dunelegacy.exe') -WorkingDirectory $runtime `
    -ArgumentList '--window',$Flag -WindowStyle Hidden -PassThru -Wait `
    -RedirectStandardOutput (Join-Path $output 'stdout.log') -RedirectStandardError (Join-Path $output 'stderr.log')
$log=Join-Path $profile 'Dune Legacy.log'
if(Test-Path -LiteralPath $log) {
    Copy-Item -LiteralPath $log -Destination (Join-Path $output 'runtime.log') -Force
    Get-Content -LiteralPath $log -Tail 12
}
if($verification.ExitCode -ne 0) {throw ('Native validation failed, exit='+$verification.ExitCode)}
if($Flag -eq '--verify-release547' -and -not (Select-String -LiteralPath $log -Pattern 'RELEASE 547 PASS:')) {throw 'Release 547 runtime checks did not complete'}
