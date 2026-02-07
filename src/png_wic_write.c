/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   png_wic_write.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvkvkv <kvkvkv@student.42.rio>             #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 00:00:00 by kvkvkv            #+#    #+#             */
/*   Updated: 2026/02/04 00:00:00 by kvkvkv           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "png_internal.h"

static void	safe_release(IUnknown **obj)
{
	if (obj && *obj)
	{
		(*obj)->lpVtbl->Release(*obj);
		*obj = NULL;
	}
}

static HRESULT	wic_make_bitmap(t_wic *w, const t_image *img)
{
	IWICImagingFactory	*fac;
	WICPixelFormatGUID	*fmt;

	fac = w->factory;
	fmt = (WICPixelFormatGUID *)&GUID_WICPixelFormat32bppBGRA;
	return (fac->lpVtbl->CreateBitmapFromMemory(fac,
			(UINT)img->width, (UINT)img->height, fmt,
			(UINT)img->stride,
			(UINT)img->size,
		img->pixels, &w->bitmap));
}

void	wic_release(t_wic *w)
{
	safe_release((IUnknown **)&w->bitmap);
	safe_release((IUnknown **)&w->props);
	safe_release((IUnknown **)&w->frame);
	safe_release((IUnknown **)&w->encoder);
	safe_release((IUnknown **)&w->stream);
	safe_release((IUnknown **)&w->factory);
}

t_bool	wic_write(t_wic *w, const t_image *img)
{
	WICPixelFormatGUID		format;
	IWICBitmapFrameEncode	*frame;
	IWICBitmapSource		*src;
	HRESULT					hr;

	frame = w->frame;
	hr = frame->lpVtbl->SetSize(frame, (UINT)img->width,
			(UINT)img->height);
	if (SUCCEEDED(hr))
	{
		format = GUID_WICPixelFormat32bppBGRA;
		hr = frame->lpVtbl->SetPixelFormat(frame, &format);
	}
	if (SUCCEEDED(hr))
		hr = wic_make_bitmap(w, img);
	if (SUCCEEDED(hr))
	{
		src = (IWICBitmapSource *)w->bitmap;
		hr = frame->lpVtbl->WriteSource(frame, src, NULL);
	}
	return (SUCCEEDED(hr));
}

t_bool	wic_commit(t_wic *w)
{
	HRESULT	hr;

	hr = w->frame->lpVtbl->Commit(w->frame);
	if (SUCCEEDED(hr))
		hr = w->encoder->lpVtbl->Commit(w->encoder);
	return (SUCCEEDED(hr));
}
