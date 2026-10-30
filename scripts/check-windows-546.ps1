param([string]$Flag='--verify-release546')
$ErrorActionPreference='Stop'
$projectRoot=Split-Path -Parent $PSScriptRoot
$runtime=Join-Path $projectRoot 'windows-build-546\bin'
$profile=Join-Path $projectRoot 'reports\native-profile'
$output=Join-Path $projectRoot ('reports\native-546\'+$Flag.TrimStart('-'))
New-Item -ItemType Directory -Force -Path $output,(Join-Path $profile 'mods') | Out-Null
if (-not (Test-Path -LiteralPath (Join-Path $profile 'mods\Tornie\mod.ini'))) {
    Get-ChildItem -LiteralPath (Join-Path $runtime 'mods') -Directory | Copy-Item -Destination (Join-Path $profile 'mods') -Recurse -Force
    Set-Content -LiteralPath (Join-Path $profile 'mods\active_mod.txt') -Value 'Tornie' -Encoding utf8
} else {
    & 'C:\msys64\mingw64\bin\python.exe' (Join-Path $PSScriptRoot 'sync-native-profile-546.py')
    if($LASTEXITCODE -ne 0) {throw 'Native profile synchronization failed'}
}
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
    Get-Content -LiteralPath $log -Tail 15
}
if($verification.ExitCode -ne 0) {throw ('Native validation failed, exit='+$verification.ExitCode)}
