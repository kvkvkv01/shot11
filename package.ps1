param(
    [switch]$Sign
)

$ErrorActionPreference = 'Stop'

$sdkBin = (Get-ChildItem 'C:\Program Files (x86)\Windows Kits\10\bin\*\x64\makeappx.exe' |
    Sort-Object FullName -Descending | Select-Object -First 1).DirectoryName

if (-not $sdkBin) {
    throw 'Windows SDK not found. Install Windows 10/11 SDK.'
}

# Require shot11.exe (run build.ps1 -Release first)
if (-not (Test-Path "$PSScriptRoot\shot11.exe")) {
    throw 'shot11.exe not found. Run build.ps1 -Release first.'
}

# Generate grey square PNG assets
$assetsDir = "$PSScriptRoot\Assets"
if (-not (Test-Path $assetsDir)) { New-Item -ItemType Directory -Path $assetsDir | Out-Null }

function New-GreyPng([string]$Path, [int]$Size) {
    Add-Type -AssemblyName System.Drawing
    $bmp = New-Object System.Drawing.Bitmap($Size, $Size)
    $g = [System.Drawing.Graphics]::FromImage($bmp)
    $g.Clear([System.Drawing.Color]::FromArgb(255, 80, 80, 80))
    $g.Dispose()
    $bmp.Save($Path, [System.Drawing.Imaging.ImageFormat]::Png)
    $bmp.Dispose()
}

$sizes = @{
    'StoreLogo.png'          = 50
    'Square44x44Logo.png'    = 44
    'Square150x150Logo.png'  = 150
    'Wide310x150Logo.png'    = 310  # width, height handled below
}

foreach ($entry in $sizes.GetEnumerator()) {
    $path = Join-Path $assetsDir $entry.Key
    if ($entry.Key -eq 'Wide310x150Logo.png') {
        Add-Type -AssemblyName System.Drawing
        $bmp = New-Object System.Drawing.Bitmap(310, 150)
        $g = [System.Drawing.Graphics]::FromImage($bmp)
        $g.Clear([System.Drawing.Color]::FromArgb(255, 80, 80, 80))
        $g.Dispose()
        $bmp.Save($path, [System.Drawing.Imaging.ImageFormat]::Png)
        $bmp.Dispose()
    } else {
        New-GreyPng $path $entry.Value
    }
}

# Stage MSIX layout
$layout = "$PSScriptRoot\msix_stage"
if (Test-Path $layout) { Remove-Item $layout -Recurse -Force }
New-Item -ItemType Directory -Path $layout | Out-Null
New-Item -ItemType Directory -Path "$layout\Assets" | Out-Null

Copy-Item "$PSScriptRoot\shot11.exe" "$layout\"
Copy-Item "$PSScriptRoot\AppxManifest.xml" "$layout\"
Copy-Item "$assetsDir\*" "$layout\Assets\"

# Create MSIX
$msix = "$PSScriptRoot\shot11.msix"
if (Test-Path $msix) { Remove-Item $msix -Force }
& "$sdkBin\makeappx.exe" pack /d $layout /p $msix /o
if ($LASTEXITCODE -ne 0) { throw 'makeappx failed' }

# Sign for local testing (optional)
if ($Sign) {
    $cert = "$PSScriptRoot\shot11_dev.pfx"
    if (-not (Test-Path $cert)) {
        $newCert = New-SelfSignedCertificate `
            -Type Custom `
            -Subject "CN=Shot11Dev" `
            -KeyUsage DigitalSignature `
            -FriendlyName "Shot11 Dev Cert" `
            -CertStoreLocation "Cert:\CurrentUser\My" `
            -TextExtension @("2.5.29.37={text}1.3.6.1.5.5.7.3.3")
        Export-PfxCertificate -Cert $newCert -FilePath $cert -Password (
            ConvertTo-SecureString -String "shot11dev" -Force -AsPlainText)
        Write-Host "Created self-signed cert: $cert (password: shot11dev)"
        Write-Host "To install for sideloading, run:"
        Write-Host "  Import-PfxCertificate -FilePath $cert -Password (ConvertTo-SecureString 'shot11dev' -Force -AsPlainText) -CertStoreLocation Cert:\LocalMachine\TrustedPeople"
    }
    & "$sdkBin\signtool.exe" sign /fd SHA256 /a /f $cert /p "shot11dev" $msix
    if ($LASTEXITCODE -ne 0) { throw 'signtool failed' }
}

# Cleanup
Remove-Item $layout -Recurse -Force

Write-Host ""
Write-Host "Package created: $msix"
if (-not $Sign) {
    Write-Host "Run with -Sign to sign for local sideloading."
}
