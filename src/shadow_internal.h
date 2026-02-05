/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shadow_internal.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvkvkv <kvkvkv@student.42.rio>             #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 00:00:00 by kvkvkv            #+#    #+#             */
/*   Updated: 2026/02/04 00:00:00 by kvkvkv           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef SHADOW_INTERNAL_H
# define SHADOW_INTERNAL_H

# include "shadow.h"

typedef struct s_shadow_state
{
	int	pad;
	int	dst_w;
	int	dst_h;
	int	win_x;
	int	win_y;
	int	sh_x;
	int	sh_y;
}	t_shadow_state;

typedef struct s_blur_task
{
	const BYTE	*src;
	BYTE		*dst;
	int			w;
	int			h;
	int			r;
}	t_blur_task;

struct s_blur_state
{
	int	w;
	int	h;
	int	r;
	int	wsize;
	int	ti;
	int	li;
	int	ri;
	int	fv;
	int	lv;
	int	val;
};

t_bool	shadow_alloc_image(t_image *out, const t_shadow_state *s);
void	shadow_calc_state(const t_image *src, const t_shadow_params *p,
			t_shadow_state *s);
BYTE	*shadow_make_alpha(int w, int h, const t_shadow_params *p);
void	shadow_draw_shadow(const BYTE *alpha, const t_image *src,
			t_image *out, const t_shadow_state *s);
void	shadow_draw_window(const t_image *src, t_image *out,
			t_shadow_state *s);
void	shadow_boxes_for_gauss(double sigma, int n, int sizes[3]);
void	shadow_box_blur_h(t_blur_task *t);
void	shadow_box_blur_v(t_blur_task *t);
t_bool	shadow_blur_alpha(BYTE *data, int w, int h, float sigma);

#endif
