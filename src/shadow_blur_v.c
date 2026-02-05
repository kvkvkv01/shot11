/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shadow_blur_v.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvkvkv <kvkvkv@student.42.rio>             #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 00:00:00 by kvkvkv            #+#    #+#             */
/*   Updated: 2026/02/04 00:00:00 by kvkvkv           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "shadow_internal.h"

static void	blur_v_prepare(const t_blur_task *t,
			struct s_blur_state *s, int x)
{
	int	i;

	s->ti = x;
	s->li = s->ti;
	s->ri = s->ti + t->r * t->w;
	s->fv = t->src[s->ti];
	s->lv = t->src[s->ti + t->w * (t->h - 1)];
	s->val = (t->r + 1) * s->fv;
	i = 0;
	while (i < t->r)
	{
		s->val += t->src[s->ti + i * t->w];
		i++;
	}
}

static void	blur_v_top(const t_blur_task *t, struct s_blur_state *s)
{
	int	y;

	y = 0;
	while (y <= t->r)
	{
		s->val += t->src[s->ri] - s->fv;
		t->dst[s->ti] = (BYTE)(s->val / s->wsize);
		s->ri += t->w;
		s->ti += t->w;
		y++;
	}
}

static void	blur_v_mid(const t_blur_task *t, struct s_blur_state *s)
{
	int	y;

	y = t->r + 1;
	while (y < t->h - t->r)
	{
		s->val += t->src[s->ri] - t->src[s->li];
		t->dst[s->ti] = (BYTE)(s->val / s->wsize);
		s->li += t->w;
		s->ri += t->w;
		s->ti += t->w;
		y++;
	}
}

static void	blur_v_bottom(const t_blur_task *t, struct s_blur_state *s)
{
	int	y;

	y = t->h - t->r;
	while (y < t->h)
	{
		s->val += s->lv - t->src[s->li];
		t->dst[s->ti] = (BYTE)(s->val / s->wsize);
		s->li += t->w;
		s->ti += t->w;
		y++;
	}
}

void	shadow_box_blur_v(t_blur_task *t)
{
	struct s_blur_state	s;
	int					x;

	s.w = t->w;
	s.h = t->h;
	s.r = t->r;
	s.wsize = t->r + t->r + 1;
	x = 0;
	while (x < t->w)
	{
		blur_v_prepare(t, &s, x);
		blur_v_top(t, &s);
		blur_v_mid(t, &s);
		blur_v_bottom(t, &s);
		x++;
	}
}
