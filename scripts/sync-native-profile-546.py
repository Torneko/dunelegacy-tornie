from pathlib import Path
import shutil

root=Path(__file__).resolve().parents[1]
source=root/'windows-build-546/bin/mods'
target=root/'reports/native-profile/mods'
changed=0
for item in source.rglob('*'):
    if not item.is_file():continue
    dest=target/item.relative_to(source)
    # Native file copying preserves timestamps; avoid rewriting the complete
    # profile for every code-only test iteration on a synchronized drive.
    if dest.is_file() and dest.stat().st_size==item.stat().st_size:
        if dest.read_bytes()==item.read_bytes():continue
    dest.parent.mkdir(parents=True,exist_ok=True)
    shutil.copy2(item,dest);changed+=1
print(f'Isolated native profile: {changed} changed mod files synchronized')
