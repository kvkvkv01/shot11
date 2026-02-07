/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shadow_calc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvkvkv <kvkvkv@student.42.rio>             #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 00:00:00 by kvkvkv            #+#    #+#             */
/*   Updated: 2026/02/04 00:00:00 by kvkvkv           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "shadow_internal.h"
#include <string.h>

static int	shadow_abs(int v)
{
	if (v < 0)
		return (-v);
	return (v);
}

t_bool	shadow_alloc_image(t_image *out, const t_shadow_state *s)
{
	if (!image_create(out, s->dst_w, s->dst_h))
		return (FALSE);
	memset(out->pixels, 0, out->size);
	return (TRUE);
}

void	shadow_calc_state(const t_image *src, const t_shadow_params *p,
		t_shadow_state *s)
{
	s->pad = (int)(3.0f * p->sigma + 0.5f);
	s->dst_w = src->width + s->pad * 2 + shadow_abs(p->off_x);
	s->dst_h = src->height + s->pad * 2 + shadow_abs(p->off_y);
	s->win_x = s->pad;
	s->win_y = s->pad;
	if (p->off_x < 0)
		s->win_x = s->pad - p->off_x;
	if (p->off_y < 0)
		s->win_y = s->pad - p->off_y;
	s->sh_x = s->win_x + p->off_x;
	s->sh_y = s->win_y + p->off_y;
}
