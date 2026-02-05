/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   png_wic_open.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvkvkv <kvkvkv@student.42.rio>             #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 00:00:00 by kvkvkv            #+#    #+#             */
/*   Updated: 2026/02/04 00:00:00 by kvkvkv           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "png_internal.h"

static t_bool	wic_create_factory(t_wic *w)
{
	HRESULT	hr;

	hr = CoCreateInstance(&CLSID_WICImagingFactory, NULL,
			CLSCTX_INPROC_SERVER, &IID_IWICImagingFactory,
			(void **)&w->factory);
	return (SUCCEEDED(hr));
}

static t_bool	wic_create_stream(t_wic *w, const wchar_t *path)
{
	HRESULT	hr;

	hr = w->factory->lpVtbl->CreateStream(w->factory, &w->stream);
	if (SUCCEEDED(hr))
		hr = w->stream->lpVtbl->InitializeFromFilename(w->stream,
				path, GENERIC_WRITE);
	return (SUCCEEDED(hr));
}

static t_bool	wic_create_encoder(t_wic *w)
{
	IWICImagingFactory	*fac;
	IWICBitmapEncoder	*enc;
	HRESULT				hr;

	fac = w->factory;
	hr = fac->lpVtbl->CreateEncoder(fac,
			&GUID_ContainerFormatPng, NULL, &w->encoder);
	if (SUCCEEDED(hr))
	{
		enc = w->encoder;
		hr = enc->lpVtbl->Initialize(enc, w->stream, WICBitmapEncoderNoCache);
	}
	return (SUCCEEDED(hr));
}

static t_bool	wic_create_frame(t_wic *w)
{
	HRESULT	hr;

	hr = w->encoder->lpVtbl->CreateNewFrame(w->encoder,
			&w->frame, &w->props);
	if (SUCCEEDED(hr))
		hr = w->frame->lpVtbl->Initialize(w->frame, w->props);
	return (SUCCEEDED(hr));
}

t_bool	wic_open(t_wic *w, const wchar_t *path)
{
	if (!wic_create_factory(w))
		return (FALSE);
	if (!wic_create_stream(w, path))
		return (FALSE);
	if (!wic_create_encoder(w))
		return (FALSE);
	if (!wic_create_frame(w))
		return (FALSE);
	return (TRUE);
}
