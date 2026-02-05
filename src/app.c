/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvkvkv <kvkvkv@student.42.rio>             #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 00:00:00 by kvkvkv            #+#    #+#             */
/*   Updated: 2026/02/04 00:00:00 by kvkvkv           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "app.h"
#include "capture.h"
#include "shadow.h"
#include "clipboard.h"
#include "png.h"
#include "path.h"
#include "util.h"
#include <stdlib.h>

static t_bool	app_is_window_ok(HWND hwnd)
{
	if (!hwnd)
		return (FALSE);
	if (!IsWindowVisible(hwnd))
		return (FALSE);
	if (IsIconic(hwnd))
		return (FALSE);
	return (TRUE);
}

static t_bool	app_get_title(HWND hwnd, wchar_t *title, size_t size)
{
	if (!title || size == 0)
		return (FALSE);
	if (GetWindowTextW(hwnd, title, (int)size) == 0)
		lstrcpyW(title, L"Window");
	return (TRUE);
}

static t_bool	app_capture_shadow(HWND hwnd, t_image *out)
{
	RECT			bounds;
	t_image			win;
	t_shadow_params	params;

	if (!get_window_bounds(hwnd, &bounds))
		return (FALSE);
	if (!capture_window_bgra(hwnd, &bounds, &win))
		return (FALSE);
	shadow_params_default(&params);
	if (!create_shadowed_image(&win, out, &params))
	{
		image_free(&win);
		return (FALSE);
	}
	image_free(&win);
	return (TRUE);
}

static void	app_save(HWND hwnd, const t_image *img)
{
	wchar_t	title[512];
	wchar_t	*folder;
	wchar_t	*path;

	folder = NULL;
	path = NULL;
	if (!app_get_title(hwnd, title, ARRAYSIZE(title)))
		return ;
	if (!get_screenshots_folder(&folder))
		return ;
	if (!build_screenshot_path(folder, title, &path))
	{
		free(folder);
		return ;
	}
	if (!save_png(path, img))
		log_debug(L"Failed to save PNG");
	if (!clipboard_set_image(img))
		log_debug(L"Failed to copy to clipboard");
	free(path);
	free(folder);
}

void	app_handle_hotkey(void)
{
	HWND		hwnd;
	t_image		out;

	hwnd = GetForegroundWindow();
	if (!app_is_window_ok(hwnd))
		return ;
	if (!app_capture_shadow(hwnd, &out))
		return ;
	app_save(hwnd, &out);
	image_free(&out);
}
