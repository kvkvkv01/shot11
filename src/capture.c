/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   capture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvkvkv <kvkvkv@student.42.rio>             #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 00:00:00 by kvkvkv            #+#    #+#             */
/*   Updated: 2026/02/04 00:00:00 by kvkvkv           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "capture.h"
#include <dwmapi.h>

t_bool	get_window_bounds(HWND hwnd, RECT *out)
{
	RECT	rect;
	HRESULT	hr;

	if (!hwnd || !out)
		return (FALSE);
	rect.left = 0;
	rect.top = 0;
	rect.right = 0;
	rect.bottom = 0;
	hr = DwmGetWindowAttribute(hwnd, DWMWA_EXTENDED_FRAME_BOUNDS,
			&rect, sizeof(rect));
	if (SUCCEEDED(hr))
	{
		*out = rect;
		return (TRUE);
	}
	if (GetWindowRect(hwnd, &rect))
	{
		*out = rect;
		return (TRUE);
	}
	return (FALSE);
}
