/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvkvkv <kvkvkv@student.42.rio>             #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 00:00:00 by kvkvkv            #+#    #+#             */
/*   Updated: 2026/02/05 00:00:00 by kvkvkv           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "render.h"
#include "magick.h"
#include "png.h"
#include "util.h"

static t_bool	render_temp_path(wchar_t *out, size_t cch)
{
	wchar_t	dir[MAX_PATH];
	DWORD	n;

	if (!out || cch < MAX_PATH)
		return (FALSE);
	n = GetTempPathW(ARRAYSIZE(dir), dir);
	if (n == 0 || n >= ARRAYSIZE(dir))
		return (FALSE);
	if (!GetTempFileNameW(dir, L"sh11", GetTickCount(), out))
		return (FALSE);
	n = (DWORD)lstrlenW(out);
	if (n > 3)
	{
		out[n - 3] = L'p';
		out[n - 2] = L'n';
		out[n - 1] = L'g';
	}
	return (TRUE);
}

static t_bool	render_magick(const t_image *win, const wchar_t *out_path)
{
	wchar_t	tmp[MAX_PATH];

	if (!render_temp_path(tmp, ARRAYSIZE(tmp)))
		return (FALSE);
	if (!save_png(tmp, win))
		return (FALSE);
	if (!magick_shadow_png(tmp, out_path))
	{
		DeleteFileW(tmp);
		return (FALSE);
	}
	DeleteFileW(tmp);
	return (TRUE);
}

t_bool	render_shadowed(const t_image *win, const wchar_t *out_path)
{
	if (!render_magick(win, out_path))
	{
		log_debug(L"ImageMagick failed");
		return (FALSE);
	}
	return (TRUE);
}
