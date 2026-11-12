# Verifica o ambiente de desenvolvimento. Com -Fix, corrige o que estiver faltando.
#   powershell -ExecutionPolicy Bypass -File scripts/doctor.ps1
#   powershell -ExecutionPolicy Bypass -File scripts/doctor.ps1 -Fix
# -Elevated e uso interno: marca a copia aberta pelo UAC.
param([switch]$Fix, [switch]$Elevated)

$ErrorActionPreference = 'Stop'
$Root   = Split-Path -Parent $PSScriptRoot
$Msys   = 'C:\msys64'
$Bash   = "$Msys\usr\bin\bash.exe"
$Ucrt   = "$Msys\ucrt64\bin"
$IsAdmin = ([Security.Principal.WindowsPrincipal][Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole(
    [Security.Principal.WindowsBuiltInRole]::Administrator)

# -Fix instala coisas (MSYS2, Chocolatey, fonte) que exigem admin: reabre o script elevado.
if ($Fix -and -not $IsAdmin) {
    Write-Host 'O -Fix precisa de administrador. Pedindo elevacao (UAC)...' -ForegroundColor Yellow
    $argList = "-NoProfile -ExecutionPolicy Bypass -File `"$PSCommandPath`" -Fix -Elevated"
    try {
        $proc = Start-Process powershell -Verb RunAs -ArgumentList $argList -Wait -PassThru
    } catch {
        Write-Host 'Elevacao recusada. Nada foi alterado.' -ForegroundColor Red
        exit 1
    }
    Write-Host "Execucao elevada terminou com codigo $($proc.ExitCode)."
    exit $proc.ExitCode
}

$script:Failures = 0

function Report([string]$Name, [bool]$Ok, [string]$Hint = '') {
    if ($Ok) {
        Write-Host "  [OK] $Name" -ForegroundColor Green
    } else {
        Write-Host "  [X]  $Name" -ForegroundColor Red
        if ($Hint) { Write-Host "       -> $Hint" -ForegroundColor DarkGray }
    }
}

# Roda a verificacao; se falhar e -Fix estiver ligado, tenta corrigir e verifica de novo.
# $Arg e repassado para $Test e $Repair (evita GetNewClosure, que perde o escopo do script).
function Check([string]$Name, [scriptblock]$Test, [string]$Hint, [scriptblock]$Repair = $null, $Arg = $null) {
    $ok = [bool](& $Test $Arg)
    if (-not $ok -and $Fix -and $Repair) {
        Write-Host "  ...  $Name - corrigindo" -ForegroundColor Yellow
        try { & $Repair $Arg; $ok = [bool](& $Test $Arg) }
        catch { Write-Host "       $($_.Exception.Message)" -ForegroundColor Red }
    }
    Report $Name $ok $Hint
    if (-not $ok) { $script:Failures++ }
}

function Pacman([string[]]$Packages) {
    & $Bash -lc "pacman -S --needed --noconfirm $($Packages -join ' ')"
    if ($LASTEXITCODE -ne 0) { throw "pacman falhou (codigo $LASTEXITCODE)" }
}

function Require-Admin([string]$What) {
    if (-not $IsAdmin) { throw "$What precisa de um terminal como administrador." }
}

# --- MSYS2 -------------------------------------------------------------------
Write-Host "`nMSYS2" -ForegroundColor Cyan
Check 'MSYS2 instalado' { Test-Path $Bash } 'winget install -e --id MSYS2.MSYS2' {
    winget install -e --id MSYS2.MSYS2 --accept-source-agreements --accept-package-agreements
    if (Test-Path $Bash) {
        & $Bash -lc 'pacman -Syu --noconfirm'
        & $Bash -lc 'pacman -Syu --noconfirm'
    }
}

$msysOk = Test-Path $Bash
$tools = [ordered]@{
    'git'              = @("$Msys\usr\bin\git.exe",        'git')
    'gcc'              = @("$Ucrt\gcc.exe",                'mingw-w64-ucrt-x86_64-gcc')
    'gdb'              = @("$Ucrt\gdb.exe",                'mingw-w64-ucrt-x86_64-gdb')
    'cmake'            = @("$Ucrt\cmake.exe",              'mingw-w64-ucrt-x86_64-cmake')
    'ninja'            = @("$Ucrt\ninja.exe",              'mingw-w64-ucrt-x86_64-ninja')
    'glslangValidator' = @("$Ucrt\glslangValidator.exe",   'mingw-w64-ucrt-x86_64-glslang')
}
foreach ($name in $tools.Keys) {
    $tool = $tools[$name]
    $repair = if ($msysOk) { { param($t) Pacman @($t[1]) } } else { $null }
    # git vale qualquer um no PATH (ex.: Git for Windows), nao so o do MSYS2.
    $test = if ($name -eq 'git') { { param($t) (Test-Path $t[0]) -or [bool](Get-Command git -ErrorAction SilentlyContinue) } }
            else { { param($t) Test-Path $t[0] } }
    Check $name $test "pacman -S --needed $($tool[1])" $repair $tool
}

Check "$Ucrt no PATH do usuario" {
    ([Environment]::GetEnvironmentVariable('Path', 'User') -split ';') -contains $Ucrt
} "adicionar $Ucrt ao PATH do usuario" {
    $userPath = [Environment]::GetEnvironmentVariable('Path', 'User')
    [Environment]::SetEnvironmentVariable('Path', ($userPath.TrimEnd(';') + ";$Ucrt"), 'User')
    $env:Path = "$Ucrt;$env:Path"
    Write-Host '       Reabra o terminal/VS Code para o PATH novo valer.' -ForegroundColor DarkGray
}

# --- Fonte -------------------------------------------------------------------
Write-Host "`nFonte" -ForegroundColor Cyan
# Logo apos instalar, o choco ainda nao esta no PATH desta sessao; por isso o caminho fixo.
$Choco = "$env:ProgramData\chocolatey\bin\choco.exe"
function Find-Choco {
    $cmd = Get-Command choco -ErrorAction SilentlyContinue
    if ($cmd) { return $cmd.Source }
    if (Test-Path $Choco) { return $Choco }
    $null
}

Check 'Chocolatey instalado' { [bool](Find-Choco) } 'https://chocolatey.org/install  (terminal como administrador)' {
    Require-Admin 'Instalar o Chocolatey'
    Set-ExecutionPolicy Bypass -Scope Process -Force
    [Net.ServicePointManager]::SecurityProtocol = [Net.ServicePointManager]::SecurityProtocol -bor 3072
    Invoke-Expression ((New-Object Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1'))
}

Check 'Fira Code instalada' {
    $keys = 'HKCU:\Software\Microsoft\Windows NT\CurrentVersion\Fonts',
            'HKLM:\Software\Microsoft\Windows NT\CurrentVersion\Fonts'
    foreach ($k in $keys) {
        if (Test-Path $k) {
            $names = (Get-Item $k).GetValueNames()
            if ($names | Where-Object { $_ -like 'Fira Code*' }) { return $true }
        }
    }
    $false
} 'choco install firacode -y  (terminal como administrador)' {
    $chocoExe = Find-Choco
    if (-not $chocoExe) { throw 'Chocolatey nao encontrado (veja o item acima).' }
    Require-Admin 'Instalar a Fira Code'
    & $chocoExe install firacode -y
    if ($LASTEXITCODE -ne 0) { throw "choco falhou (codigo $LASTEXITCODE)" }
}

# --- VS Code -----------------------------------------------------------------
Write-Host "`nVS Code" -ForegroundColor Cyan
$code = Get-Command code -ErrorAction SilentlyContinue
if (-not $code) {
    Report 'comando code no PATH' $false 'VS Code > Ctrl+Shift+P > "Shell Command: Install code command in PATH"'
    $script:Failures++
} else {
    # Regex em vez de ConvertFrom-Json: o extensions.json aceita virgula sobrando.
    $json = Get-Content "$Root\.vscode\extensions.json" -Raw
    $block = [regex]::Match($json, '"recommendations"\s*:\s*\[(.*?)\]', 'Singleline').Groups[1].Value
    $wanted = [regex]::Matches($block, '"([^"]+)"') | ForEach-Object { $_.Groups[1].Value }
    $state = @{ Installed = @(& code --list-extensions) }
    foreach ($ext in $wanted) {
        Check "extensao $ext" { param($e) $state.Installed -contains $e } "code --install-extension $ext" {
            param($e)
            & code --install-extension $e | Out-Null
            $state.Installed = @(& code --list-extensions)
        } $ext
    }
}

# --- Projeto -----------------------------------------------------------------
Write-Host "`nProjeto" -ForegroundColor Cyan
Check 'build configurado (build/build.ninja)' { Test-Path "$Root\build\build.ninja" } `
    'cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug' {
    $cmake = if (Test-Path "$Ucrt\cmake.exe") { "$Ucrt\cmake.exe" } else { 'cmake' }
    $env:Path = "$Ucrt;$env:Path"
    & $cmake -S $Root -B "$Root\build" -G Ninja -DCMAKE_BUILD_TYPE=Debug
    if ($LASTEXITCODE -ne 0) { throw "cmake falhou (codigo $LASTEXITCODE)" }
}

# --- Resultado ---------------------------------------------------------------
Write-Host ''
if ($script:Failures -eq 0) {
    Write-Host 'Tudo certo.' -ForegroundColor Green
} else {
    $msg = "$($script:Failures) problema(s)."
    if (-not $Fix) { $msg += ' Rode com -Fix (ou a task "doctor: fix") para corrigir.' }
    Write-Host $msg -ForegroundColor Red
}
# A janela elevada fecha sozinha ao terminar; segura para dar tempo de ler.
if ($Elevated) { Read-Host 'Enter para fechar' | Out-Null }
exit [int]($script:Failures -ne 0)
