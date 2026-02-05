cmake_minimum_required(VERSION 3.16)
project(shot11 C)
set(CMAKE_C_STANDARD 90)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(shot11
  src/main.c
  src/app.c
  src/app_save.c
  src/capture.c
  src/capture_bgra.c
  src/capture_utils.c
  src/shadow.c
  src/shadow_calc.c
  src/shadow_alpha.c
  src/shadow_draw.c
  src/shadow_blur_boxes.c
  src/shadow_blur_h.c
  src/shadow_blur_v.c
  src/shadow_blur_alpha.c
  src/clipboard.c
  src/png.c
  src/png_load.c
  src/png_wic_open.c
  src/png_wic_write.c
  src/path.c
  src/magick.c
  src/render.c
  src/util.c
  src/util_mem.c
  src/util_str.c
  src/util_time.c
  src/util_log.c
)

target_link_libraries(shot11
  user32 gdi32 dwmapi ole32 shell32 windowscodecs
)
