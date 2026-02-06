/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   capture_shift.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvkvkv <kvkvkv@student.42.rio>             #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 00:00:00 by kvkvkv            #+#    #+#             */
/*   Updated: 2026/02/06 00:00:00 by kvkvkv           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "capture_internal.h"
#include <string.h>

static void	capture_get_offset(HWND hwnd, const RECT *ext, int *dx, int *dy)
{
	RECT	full;

	*dx = 0;
	*dy = 0;
	if (!GetWindowRect(hwnd, &full))
		return ;
	if (ext->left > full.left)
		*dx = ext->left - full.left;
	if (ext->top > full.top)
		*dy = ext->top - full.top;
}

static void	capture_shift_rows(BYTE *bits, int stride, int h, int dy)
{
	size_t	keep;

	keep = (size_t)(h - dy) * stride;
	memmove(bits, bits + (size_t)dy * stride, keep);
	ZeroMemory(bits + keep, (size_t)dy * stride);
}

static void	capture_shift_cols(BYTE *bits, int w, int h, int dx)
{
	int		row;
	size_t	keep;
	BYTE	*ptr;

	keep = (size_t)(w - dx) * 4;
	row = 0;
	while (row < h)
	{
		ptr = bits + (size_t)row * w * 4;
		memmove(ptr, ptr + (size_t)dx * 4, keep);
		ZeroMemory(ptr + keep, (size_t)dx * 4);
		row++;
	}
}

void	capture_fix_print_offset(HWND hwnd, const RECT *ext,
		struct s_capture *cap)
{
	int	dx;
	int	dy;

	capture_get_offset(hwnd, ext, &dx, &dy);
	if (dx <= 0 && dy <= 0)
		return ;
	if (dx >= cap->width || dy >= cap->height)
		return ;
	if (dy > 0)
		capture_shift_rows((BYTE *)cap->bits, cap->width * 4,
			cap->height, dy);
	if (dx > 0)
		capture_shift_cols((BYTE *)cap->bits, cap->width,
			cap->height - dy, dx);
}
