/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app_save.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvkvkv <kvkvkv@student.42.rio>             #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 00:00:00 by kvkvkv            #+#    #+#             */
/*   Updated: 2026/02/05 00:00:00 by kvkvkv           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "app.h"
#include "render.h"
#include "clipboard.h"
#include "path.h"
#include "util.h"
#include <stdlib.h>

static t_bool	app_get_title(HWND hwnd, wchar_t *title, size_t size)
{
	if (!title || size == 0)
		return (FALSE);
	if (GetWindowTextW(hwnd, title, (int)size) == 0)
		lstrcpyW(title, L"Window");
	return (TRUE);
}

static void	app_free_paths(wchar_t *folder, wchar_t *path)
{
	if (path)
		free(path);
	if (folder)
		free(folder);
}

static void	app_render_clip(const t_image *win, const wchar_t *path)
{
	t_image	out;

	if (!render_shadowed(win, path, &out))
		return ;
	if (!clipboard_set_image(&out))
		log_debug(L"Failed to copy to clipboard");
	image_free(&out);
}

void	app_save(HWND hwnd, const t_image *win)
{
	wchar_t	title[512];
	wchar_t	*folder;
	wchar_t	*path;

	folder = NULL;
	path = NULL;
	if (!app_get_title(hwnd, title, ARRAYSIZE(title)))
		return ;
	if (get_screenshots_folder(&folder))
	{
		if (build_screenshot_path(folder, title, &path))
			app_render_clip(win, path);
	}
	app_free_paths(folder, path);
}
