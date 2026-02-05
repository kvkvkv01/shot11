/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   capture_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvkvkv <kvkvkv@student.42.rio>             #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 00:00:00 by kvkvkv            #+#    #+#             */
/*   Updated: 2026/02/04 00:00:00 by kvkvkv           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "capture_internal.h"
#include <stdlib.h>
#include <string.h>

#ifndef PW_RENDERFULLCONTENT
# define PW_RENDERFULLCONTENT 0x00000002
#endif

t_bool	capture_try_print(HWND hwnd, HDC hdc, BYTE *bits, size_t size)
{
	size_t	i;

	if (!PrintWindow(hwnd, hdc, PW_RENDERFULLCONTENT))
		return (FALSE);
	i = 0;
	while (i + 3 < size)
	{
		if (bits[i] || bits[i + 1] || bits[i + 2] || bits[i + 3])
			return (TRUE);
		i += 4;
	}
	return (FALSE);
}

void	capture_try_blt(HDC hdc, const RECT *b, int w, int h)
{
	HDC	hdc_screen;

	hdc_screen = GetDC(NULL);
	if (!hdc_screen)
		return ;
	BitBlt(hdc, 0, 0, w, h, hdc_screen, b->left, b->top,
		SRCCOPY | CAPTUREBLT);
	ReleaseDC(NULL, hdc_screen);
}

static void	capture_patch_alpha0(BYTE *dst, const BYTE *src, size_t size)
{
	size_t	i;

	if (!dst || !src)
		return ;
	i = 0;
	while (i + 3 < size)
	{
		if (dst[i + 3] == 0)
		{
			dst[i] = src[i];
			dst[i + 1] = src[i + 1];
			dst[i + 2] = src[i + 2];
		}
		i += 4;
	}
}

void	capture_fill_alpha(BYTE *bits, size_t size)
{
	size_t	i;

	if (!bits)
		return ;
	i = 3;
	while (i < size)
	{
		bits[i] = 0xFF;
		i += 4;
	}
}

t_bool	capture_merge_print(HWND hwnd, struct s_capture *cap, const RECT *b)
{
	BYTE	*print_copy;

	if (!capture_try_print(hwnd, cap->hdc, (BYTE *)cap->bits, cap->size))
	{
		capture_try_blt(cap->hdc, b, cap->width, cap->height);
		return (FALSE);
	}
	print_copy = (BYTE *)malloc(cap->size);
	if (print_copy)
	{
		memcpy(print_copy, cap->bits, cap->size);
		capture_try_blt(cap->hdc, b, cap->width, cap->height);
		capture_patch_alpha0(print_copy,
			(const BYTE *)cap->bits, cap->size);
		memcpy(cap->bits, print_copy, cap->size);
		free(print_copy);
	}
	else
		capture_try_blt(cap->hdc, b, cap->width, cap->height);
	return (TRUE);
}
