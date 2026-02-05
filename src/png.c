/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   png.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvkvkv <kvkvkv@student.42.rio>             #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 00:00:00 by kvkvkv            #+#    #+#             */
/*   Updated: 2026/02/04 00:00:00 by kvkvkv           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "png_internal.h"

t_bool	save_png(const wchar_t *path, const t_image *img)
{
	t_wic	w;
	t_bool	ok;

	if (!path || !img || !img->pixels)
		return (FALSE);
	ZeroMemory(&w, sizeof(w));
	ok = wic_open(&w, path);
	if (ok)
		ok = wic_write(&w, img);
	if (ok)
		ok = wic_commit(&w);
	wic_release(&w);
	return (ok);
}
