/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shadow.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvkvkv <kvkvkv@student.42.rio>             #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 00:00:00 by kvkvkv            #+#    #+#             */
/*   Updated: 2026/02/04 00:00:00 by kvkvkv           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "shadow_internal.h"
#include <stdlib.h>

void	shadow_params_default(t_shadow_params *p)
{
	if (!p)
		return ;
	p->sigma = 50.0f;
	p->off_x = 30;
	p->off_y = 30;
	p->opacity = 0.5f;
}

t_bool	create_shadowed_image(const t_image *src, t_image *out,
		const t_shadow_params *p)
{
	t_shadow_state	st;
	BYTE			*alpha;

	if (!src || !out || !p || !src->pixels)
		return (FALSE);
	shadow_calc_state(src, p, &st);
	if (!shadow_alloc_image(out, &st))
		return (FALSE);
	alpha = shadow_make_alpha(src->width, src->height, p);
	if (!alpha)
	{
		image_free(out);
		return (FALSE);
	}
	shadow_draw_shadow(alpha, src, out, &st);
	shadow_draw_window(src, out, &st);
	free(alpha);
	return (TRUE);
}
