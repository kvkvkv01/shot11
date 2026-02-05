/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clipboard.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvkvkv <kvkvkv@student.42.rio>             #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 00:00:00 by kvkvkv            #+#    #+#             */
/*   Updated: 2026/02/04 00:00:00 by kvkvkv           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "clipboard.h"
#include <string.h>

static void	fill_v5_header(BITMAPV5HEADER *hdr, const t_image *img)
{
	ZeroMemory(hdr, sizeof(BITMAPV5HEADER));
	hdr->bV5Size = sizeof(BITMAPV5HEADER);
	hdr->bV5Width = img->width;
	hdr->bV5Height = -img->height;
	hdr->bV5Planes = 1;
	hdr->bV5BitCount = 32;
	hdr->bV5Compression = BI_BITFIELDS;
	hdr->bV5RedMask = 0x00FF0000;
	hdr->bV5GreenMask = 0x0000FF00;
	hdr->bV5BlueMask = 0x000000FF;
	hdr->bV5AlphaMask = 0xFF000000;
	hdr->bV5CSType = LCS_sRGB;
}

static t_bool	fill_global_bits(HGLOBAL hmem, const t_image *img,
	size_t data_size)
{
	BITMAPV5HEADER	*hdr;
	BYTE			*bits;

	hdr = (BITMAPV5HEADER *)GlobalLock(hmem);
	if (!hdr)
		return (FALSE);
	fill_v5_header(hdr, img);
	bits = (BYTE *)(hdr + 1);
	memcpy(bits, img->pixels, data_size);
	GlobalUnlock(hmem);
	return (TRUE);
}

static t_bool	clipboard_open(void)
{
	int	i;

	i = 0;
	while (i < 10)
	{
		if (OpenClipboard(NULL))
			return (TRUE);
		Sleep(10);
		i++;
	}
	return (FALSE);
}

static t_bool	write_clipboard(HGLOBAL hmem)
{
	if (!clipboard_open())
		return (FALSE);
	EmptyClipboard();
	if (!SetClipboardData(CF_DIBV5, hmem))
	{
		CloseClipboard();
		return (FALSE);
	}
	CloseClipboard();
	return (TRUE);
}

t_bool	clipboard_set_image(const t_image *img)
{
	size_t	data_size;
	size_t	total_size;
	HGLOBAL	hmem;

	if (!img || !img->pixels || img->width <= 0 || img->height <= 0)
		return (FALSE);
	data_size = (size_t)img->stride * img->height;
	total_size = sizeof(BITMAPV5HEADER) + data_size;
	hmem = GlobalAlloc(GHND, total_size);
	if (!hmem)
		return (FALSE);
	if (!fill_global_bits(hmem, img, data_size))
	{
		GlobalFree(hmem);
		return (FALSE);
	}
	if (!write_clipboard(hmem))
	{
		GlobalFree(hmem);
		return (FALSE);
	}
	return (TRUE);
}
