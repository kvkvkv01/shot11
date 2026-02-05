/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   png_internal.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvkvkv <kvkvkv@student.42.rio>             #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 00:00:00 by kvkvkv            #+#    #+#             */
/*   Updated: 2026/02/04 00:00:00 by kvkvkv           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef PNG_INTERNAL_H
# define PNG_INTERNAL_H

# include "png.h"
# include <wincodec.h>

typedef struct s_wic
{
	IWICImagingFactory		*factory;
	IWICStream				*stream;
	IWICBitmapEncoder		*encoder;
	IWICBitmapFrameEncode	*frame;
	IPropertyBag2			*props;
	IWICBitmap				*bitmap;
}	t_wic;

t_bool	wic_open(t_wic *w, const wchar_t *path);
t_bool	wic_write(t_wic *w, const t_image *img);
t_bool	wic_commit(t_wic *w);
void	wic_release(t_wic *w);

#endif
