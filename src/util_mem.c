/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_mem.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvkvkv <kvkvkv@student.42.rio>             #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 00:00:00 by kvkvkv            #+#    #+#             */
/*   Updated: 2026/02/04 00:00:00 by kvkvkv           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "util.h"
#include <stdlib.h>

t_bool	image_create(t_image *img, int width, int height)
{
	size_t	stride;
	size_t	size;
	BYTE	*pixels;

	if (!img || width <= 0 || height <= 0)
		return (FALSE);
	stride = (size_t)width * 4;
	if (stride / 4 != (size_t)width)
		return (FALSE);
	if (stride > INT_MAX)
		return (FALSE);
	size = stride * (size_t)height;
	if (size / stride != (size_t)height)
		return (FALSE);
	pixels = (BYTE *)malloc(size);
	if (!pixels)
		return (FALSE);
	img->width = width;
	img->height = height;
	img->stride = (int)stride;
	img->size = size;
	img->pixels = pixels;
	return (TRUE);
}

void	image_free(t_image *img)
{
	if (!img)
		return ;
	if (img->pixels)
		free(img->pixels);
	img->pixels = NULL;
	img->width = 0;
	img->height = 0;
	img->stride = 0;
	img->size = 0;
}
