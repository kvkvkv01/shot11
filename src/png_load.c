/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   png_load.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvkvkv <kvkvkv@student.42.rio>             #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 00:00:00 by kvkvkv            #+#    #+#             */
/*   Updated: 2026/02/05 00:00:00 by kvkvkv           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "png.h"
#include <wincodec.h>

static void	wic_release(IUnknown **obj)
{
	if (obj && *obj)
	{
		(*obj)->lpVtbl->Release(*obj);
		*obj = NULL;
	}
}

static t_bool	wic_get_frame(IWICImagingFactory *fac, const wchar_t *path,
		IWICBitmapFrameDecode **frame)
{
	IWICBitmapDecoder	*dec;
	HRESULT				hr;

	dec = NULL;
	hr = fac->lpVtbl->CreateDecoderFromFilename(fac, path, NULL,
			GENERIC_READ, WICDecodeMetadataCacheOnLoad, &dec);
	if (SUCCEEDED(hr))
		hr = dec->lpVtbl->GetFrame(dec, 0, frame);
	wic_release((IUnknown **)&dec);
	return (SUCCEEDED(hr));
}

static t_bool	wic_make_converter(IWICImagingFactory *fac,
		IWICBitmapFrameDecode *frame, IWICFormatConverter **conv)
{
	HRESULT	hr;

	hr = fac->lpVtbl->CreateFormatConverter(fac, conv);
	if (SUCCEEDED(hr))
		hr = (*conv)->lpVtbl->Initialize(*conv,
				(IWICBitmapSource *)frame,
				&GUID_WICPixelFormat32bppBGRA,
				WICBitmapDitherTypeNone, NULL,
				0.0, WICBitmapPaletteTypeCustom);
	return (SUCCEEDED(hr));
}

static t_bool	wic_copy_pixels(IWICBitmapSource *src, t_image *out)
{
	UINT	w;
	UINT	h;

	if (FAILED(src->lpVtbl->GetSize(src, &w, &h)))
		return (FALSE);
	if (!image_create(out, (int)w, (int)h))
		return (FALSE);
	if (FAILED(src->lpVtbl->CopyPixels(src, NULL, (UINT)out->stride,
				(UINT)out->size, out->pixels)))
	{
		image_free(out);
		return (FALSE);
	}
	return (TRUE);
}

t_bool	load_png(const wchar_t *path, t_image *out)
{
	IWICImagingFactory		*fac;
	IWICBitmapFrameDecode	*frame;
	IWICFormatConverter		*conv;
	HRESULT					hr;
	t_bool					ok;

	fac = NULL;
	frame = NULL;
	conv = NULL;
	ok = FALSE;
	if (!path || !out)
		return (FALSE);
	hr = CoCreateInstance(&CLSID_WICImagingFactory, NULL,
			CLSCTX_INPROC_SERVER, &IID_IWICImagingFactory,
			(void **)&fac);
	if (SUCCEEDED(hr) && wic_get_frame(fac, path, &frame)
		&& wic_make_converter(fac, frame, &conv)
		&& wic_copy_pixels((IWICBitmapSource *)conv, out))
		ok = TRUE;
	wic_release((IUnknown **)&conv);
	wic_release((IUnknown **)&frame);
	wic_release((IUnknown **)&fac);
	return (ok);
}
