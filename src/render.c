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
#include "shadow.h"
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
	return (TRUE);
}

static t_bool	render_magick(const t_image *win, const wchar_t *out_path,
		t_image *out)
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
	if (!load_png(out_path, out))
		return (FALSE);
	return (TRUE);
}

static t_bool	render_fallback(const t_image *win, const wchar_t *out_path,
		t_image *out)
{
	t_shadow_params	params;

	shadow_params_default(&params);
	if (!create_shadowed_image(win, out, &params))
		return (FALSE);
	if (!save_png(out_path, out))
	{
		image_free(out);
		return (FALSE);
	}
	return (TRUE);
}

t_bool	render_shadowed(const t_image *win, const wchar_t *out_path,
		t_image *out)
{
	if (magick_available())
	{
		if (render_magick(win, out_path, out))
			return (TRUE);
		log_debug(L"ImageMagick failed, using fallback shadow");
	}
	return (render_fallback(win, out_path, out));
}
