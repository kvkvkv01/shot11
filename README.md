# shot11

Lightweight hotkey-only tool to capture the active window, add a modern drop
shadow, copy to clipboard, and save as PNG in the Windows Screenshots folder.

**Runtime dependency**
- ImageMagick (the `magick.exe` CLI must be in `PATH`).
- Install with: `winget install ImageMagick.ImageMagick`
- If `magick.exe` is not on `PATH`, set `MAGICK_EXE` to the full path.

**Build (PowerShell)**
```
msvc
.\build.ps1
```

**Hotkey**
- `Ctrl+Alt+S`
