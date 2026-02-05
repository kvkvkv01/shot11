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

static t_bool	app_capture_window(HWND hwnd, t_image *out)
{
	RECT	bounds;

	if (!get_window_bounds(hwnd, &bounds))
		return (FALSE);
	return (capture_window_bgra(hwnd, &bounds, out));
}

void	app_handle_hotkey(void)
{
	HWND		hwnd;
	t_image		out;

	hwnd = GetForegroundWindow();
	if (!app_is_window_ok(hwnd))
		return ;
	if (!app_capture_window(hwnd, &out))
		return ;
	app_save(hwnd, &out);
	image_free(&out);
}
