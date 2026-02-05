/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shadow_blur_h.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvkvkv <kvkvkv@student.42.rio>             #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 00:00:00 by kvkvkv            #+#    #+#             */
/*   Updated: 2026/02/04 00:00:00 by kvkvkv           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "shadow_internal.h"

static void	blur_h_prepare(const t_blur_task *t,
			struct s_blur_state *s, int y)
{
	int	i;

	s->ti = y * t->w;
	s->li = s->ti;
	s->ri = s->ti + t->r;
	s->fv = t->src[s->ti];
	s->lv = t->src[s->ti + t->w - 1];
	s->val = (t->r + 1) * s->fv;
	i = 0;
	while (i < t->r)
	{
		s->val += t->src[s->ti + i];
		i++;
	}
}

static void	blur_h_row(const t_blur_task *t, struct s_blur_state *s)
{
	int	x;

	x = 0;
	while (x <= t->r)
	{
		s->val += t->src[s->ri++] - s->fv;
		t->dst[s->ti++] = (BYTE)(s->val / s->wsize);
		x++;
	}
	while (x < t->w - t->r)
	{
		s->val += t->src[s->ri++] - t->src[s->li++];
		t->dst[s->ti++] = (BYTE)(s->val / s->wsize);
		x++;
	}
	while (x < t->w)
	{
		s->val += s->lv - t->src[s->li++];
		t->dst[s->ti++] = (BYTE)(s->val / s->wsize);
		x++;
	}
}

void	shadow_box_blur_h(t_blur_task *t)
{
	struct s_blur_state	s;
	int					y;

	s.w = t->w;
	s.h = t->h;
	s.r = t->r;
	s.wsize = t->r + t->r + 1;
	y = 0;
	while (y < t->h)
	{
		blur_h_prepare(t, &s, y);
		blur_h_row(t, &s);
		y++;
	}
}
