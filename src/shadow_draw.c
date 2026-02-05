/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shadow_draw.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvkvkv <kvkvkv@student.42.rio>             #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 00:00:00 by kvkvkv            #+#    #+#             */
/*   Updated: 2026/02/04 00:00:00 by kvkvkv           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "shadow_internal.h"

static void	shadow_draw_shadow_row(const BYTE *row, BYTE *dst, int w)
{
	int		x;
	BYTE	a;

	x = 0;
	while (x < w)
	{
		a = row[x];
		if (a)
		{
			dst[0] = 0;
			dst[1] = 0;
			dst[2] = 0;
			dst[3] = a;
		}
		dst += 4;
		x++;
	}
}

void	shadow_draw_shadow(const BYTE *alpha, const t_image *src,
			t_image *out, const t_shadow_state *s)
{
	int			y;
	BYTE		*dst;
	const BYTE	*row;

	y = 0;
	while (y < src->height)
	{
		dst = out->pixels + (size_t)(s->sh_y + y) * out->stride
			+ (size_t)s->sh_x * 4;
		row = alpha + (size_t)y * src->width;
		shadow_draw_shadow_row(row, dst, src->width);
		y++;
	}
}

static void	shadow_draw_window_row(const BYTE *srow, BYTE *drow, int w)
{
	int	x;

	x = 0;
	while (x < w)
	{
		drow[0] = srow[0];
		drow[1] = srow[1];
		drow[2] = srow[2];
		drow[3] = 255;
		srow += 4;
		drow += 4;
		x++;
	}
}

void	shadow_draw_window(const t_image *src, t_image *out,
			t_shadow_state *s)
{
	int		y;
	BYTE	*drow;
	BYTE	*srow;

	y = 0;
	while (y < src->height)
	{
		srow = src->pixels + (size_t)y * src->stride;
		drow = out->pixels + (size_t)(s->win_y + y) * out->stride
			+ (size_t)s->win_x * 4;
		shadow_draw_window_row(srow, drow, src->width);
		y++;
	}
}
