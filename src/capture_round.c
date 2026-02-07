/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   capture_round.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvkvkv <kvkvkv@student.42.rio>             #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 00:00:00 by kvkvkv            #+#    #+#             */
/*   Updated: 2026/02/07 00:00:00 by kvkvkv           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "capture_internal.h"
#include <dwmapi.h>

#ifndef DWMWA_WINDOW_CORNER_PREFERENCE
# define DWMWA_WINDOW_CORNER_PREFERENCE 33
#endif

static int	capture_corner_radius(HWND hwnd)
{
	int		pref;
	UINT	dpi;
	int		logical;

	pref = 0;
	if (FAILED(DwmGetWindowAttribute(hwnd,
				DWMWA_WINDOW_CORNER_PREFERENCE, &pref, sizeof(pref))))
		return (0);
	if (pref == 1)
		return (0);
	dpi = GetDpiForWindow(hwnd);
	if (dpi == 0)
		dpi = 96;
	logical = 8;
	if (pref == 3)
		logical = 4;
	return (logical * (int)dpi / 96);
}

static void	capture_clear_pixel(BYTE *bits, int w, int x, int y)
{
	int	off;

	off = (y * w + x) * 4;
	bits[off] = 0;
	bits[off + 1] = 0;
	bits[off + 2] = 0;
	bits[off + 3] = 0;
}

static void	capture_clear_corners(BYTE *bits, int w, int h, int r)
{
	int	dx;
	int	dy;
	int	a;
	int	b;

	dy = 0;
	while (dy < r)
	{
		b = 2 * r - 2 * dy - 1;
		dx = 0;
		while (dx < r)
		{
			a = 2 * r - 2 * dx - 1;
			if (a * a + b * b > 4 * r * r)
			{
				capture_clear_pixel(bits, w, dx, dy);
				capture_clear_pixel(bits, w, w - 1 - dx, dy);
				capture_clear_pixel(bits, w, dx, h - 1 - dy);
				capture_clear_pixel(bits, w, w - 1 - dx, h - 1 - dy);
			}
			dx++;
		}
		dy++;
	}
}

void	capture_round_corners(HWND hwnd, BYTE *bits, int w, int h)
{
	int	r;

	r = capture_corner_radius(hwnd);
	if (r <= 0 || r > w / 2 || r > h / 2)
		return ;
	capture_clear_corners(bits, w, h, r);
}
