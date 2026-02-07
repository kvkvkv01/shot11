call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat" >nul 2>&1
cd /d "D:\Projects\shot11"
echo BUILDING...
cl.exe /nologo /W4 /DUNICODE /D_UNICODE /Od /Zi /Fe:shot11.exe src\main.c src\app.c src\app_save.c src\capture.c src\capture_bgra.c src\capture_utils.c src\capture_shift.c src\capture_round.c src\shadow.c src\shadow_calc.c src\shadow_alpha.c src\shadow_draw.c src\shadow_blur_boxes.c src\shadow_blur_h.c src\shadow_blur_v.c src\shadow_blur_alpha.c src\clipboard.c src\png.c src\png_load.c src\png_wic_open.c src\png_wic_write.c src\path.c src\magick.c src\render.c src\tray.c src\util.c src\util_mem.c src\util_str.c src\util_time.c src\util_log.c /link /SUBSYSTEM:WINDOWS user32.lib gdi32.lib dwmapi.lib ole32.lib shell32.lib windowscodecs.lib
echo DONE %ERRORLEVEL%
