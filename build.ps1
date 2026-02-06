param(
    [switch]$Release
)

$ErrorActionPreference = 'Stop'

function Get-VcVars64 {
    $candidates = @(
        "${env:ProgramFiles(x86)}\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars64.bat",
        "$env:ProgramFiles\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars64.bat",
        "${env:ProgramFiles(x86)}\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat",
        "$env:ProgramFiles\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
    )
    foreach ($p in $candidates) {
        if (Test-Path $p) {
            return $p
        }
    }
    $vswhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
    if (Test-Path $vswhere) {
        $vs = & $vswhere -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath
        if ($vs) {
            $p = Join-Path $vs 'VC\Auxiliary\Build\vcvars64.bat'
            if (Test-Path $p) {
                return $p
            }
        }
    }
    return $null
}

function Import-VcVars64 {
    $vcvars = Get-VcVars64
    if (-not $vcvars) {
        throw 'vcvars64.bat not found. Install Visual Studio Build Tools or Community with C++.'
    }
    & $env:COMSPEC /s /c "`"$vcvars`" & set" 2>$null | ForEach-Object {
        if ($_ -match '^(.*?)=(.*)$') {
            Set-Item -Path Env:$($matches[1]) -Value $matches[2]
        }
    }
}

function Test-WindowsHeaders {
    if (-not $env:INCLUDE) {
        return $false
    }
    foreach ($p in $env:INCLUDE.Split(';')) {
        if ($p -and (Test-Path (Join-Path $p 'Windows.h'))) {
            return $true
        }
    }
    return $false
}

if (-not (Get-Command cl.exe -ErrorAction SilentlyContinue)) {
    Import-VcVars64
}
if (-not (Test-WindowsHeaders)) {
    Import-VcVars64
}

$flags = @('/nologo', '/W4', '/DUNICODE', '/D_UNICODE')
if ($Release) {
    $flags += '/O2'
} else {
    $flags += @('/Od', '/Zi')
}

$src = @(
    'src\main.c',
    'src\app.c',
    'src\app_save.c',
    'src\capture.c',
    'src\capture_bgra.c',
    'src\capture_utils.c',
    'src\capture_shift.c',
    'src\shadow.c',
    'src\shadow_calc.c',
    'src\shadow_alpha.c',
    'src\shadow_draw.c',
    'src\shadow_blur_boxes.c',
    'src\shadow_blur_h.c',
    'src\shadow_blur_v.c',
    'src\shadow_blur_alpha.c',
    'src\clipboard.c',
    'src\png.c',
    'src\png_load.c',
    'src\png_wic_open.c',
    'src\png_wic_write.c',
    'src\path.c',
    'src\magick.c',
    'src\render.c',
    'src\util.c',
    'src\util_mem.c',
    'src\util_str.c',
    'src\util_time.c',
    'src\util_log.c'
)

$libs = @(
    'user32.lib',
    'gdi32.lib',
    'dwmapi.lib',
    'ole32.lib',
    'shell32.lib',
    'windowscodecs.lib'
)

& cl.exe @flags /Fe:shot11.exe @src /link @libs
