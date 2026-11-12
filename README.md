## Setup

No VS Code: `Ctrl+Shift+P` > **Tasks: Run Task** > **doctor** verifica o ambiente; **doctor: fix** corrige o que faltar.
O `-Fix` pede elevação (UAC) e roda numa janela de administrador separada.

Pelo terminal:

```cmd
powershell -ExecutionPolicy Bypass -File scripts/doctor.ps1
powershell -ExecutionPolicy Bypass -File scripts/doctor.ps1 -Fix
```

### Manual

```cmd
winget install -e --id MSYS2.MSYS2
C:\msys64\usr\bin\bash -lc "pacman -Syu --noconfirm"
C:\msys64\usr\bin\bash -lc "pacman -Syu --noconfirm"
C:\msys64\usr\bin\bash -lc "pacman -S --needed --noconfirm git mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-gdb mingw-w64-ucrt-x86_64-cmake mingw-w64-ucrt-x86_64-ninja mingw-w64-ucrt-x86_64-glslang"
powershell -c "[Environment]::SetEnvironmentVariable('Path', [Environment]::GetEnvironmentVariable('Path','User') + ';C:\msys64\ucrt64\bin', 'User')"
set PATH=C:\msys64\ucrt64\bin;%PATH%
choco install firacode -y
```

## Build

```cmd
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```
