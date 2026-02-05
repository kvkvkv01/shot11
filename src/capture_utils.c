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
