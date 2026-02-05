/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   capture_bgra.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvkvkv <kvkvkv@student.42.rio>             #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 00:00:00 by kvkvkv            #+#    #+#             */
/*   Updated: 2026/02/04 00:00:00 by kvkvkv           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "capture_internal.h"
#include <string.h>

static void	capture_init_bmi(BITMAPINFO *bmi, int w, int h)
{
	ZeroMemory(bmi, sizeof(*bmi));
	bmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi->bmiHeader.biWidth = w;
	bmi->bmiHeader.biHeight = -h;
	bmi->bmiHeader.biPlanes = 1;
	bmi->bmiHeader.biBitCount = 32;
	bmi->bmiHeader.biCompression = BI_RGB;
}

static t_bool	capture_setup(const RECT *b, struct s_capture *cap)
{
	BITMAPINFO	bmi;

	cap->width = b->right - b->left;
	cap->height = b->bottom - b->top;
	if (cap->width <= 0 || cap->height <= 0)
		return (FALSE);
	cap->hdc = CreateCompatibleDC(NULL);
	if (!cap->hdc)
		return (FALSE);
	cap->bits = NULL;
	capture_init_bmi(&bmi, cap->width, cap->height);
	cap->hbm = CreateDIBSection(cap->hdc, &bmi, DIB_RGB_COLORS,
			&cap->bits, NULL, 0);
	if (!cap->hbm || !cap->bits)
	{
		DeleteDC(cap->hdc);
		return (FALSE);
	}
	cap->old = SelectObject(cap->hdc, cap->hbm);
	cap->size = (size_t)cap->width * (size_t)cap->height * 4;
	return (TRUE);
}

static void	capture_release(struct s_capture *cap)
{
	if (cap->hdc && cap->old)
		SelectObject(cap->hdc, cap->old);
	if (cap->hbm)
		DeleteObject(cap->hbm);
	if (cap->hdc)
		DeleteDC(cap->hdc);
}

t_bool	capture_window_bgra(HWND hwnd, const RECT *bounds, t_image *out_img)
{
	struct s_capture	cap;
	t_bool				ok;

	if (!hwnd || !bounds || !out_img)
		return (FALSE);
	if (!capture_setup(bounds, &cap))
		return (FALSE);
	if (!capture_try_print(hwnd, cap.hdc, (BYTE *)cap.bits, cap.size))
		capture_try_blt(cap.hdc, bounds, cap.width, cap.height);
	ok = image_create(out_img, cap.width, cap.height);
	if (ok)
		memcpy(out_img->pixels, cap.bits,
			(size_t)out_img->stride * out_img->height);
	capture_release(&cap);
	return (ok);
}
