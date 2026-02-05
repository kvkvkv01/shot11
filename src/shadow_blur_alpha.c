/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shadow_blur_alpha.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvkvkv <kvkvkv@student.42.rio>             #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 00:00:00 by kvkvkv            #+#    #+#             */
/*   Updated: 2026/02/04 00:00:00 by kvkvkv           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "shadow_internal.h"
#include <stdlib.h>

static void	shadow_blur_pass(t_blur_task *task, BYTE *tmp)
{
	BYTE	*data;

	data = (BYTE *)task->src;
	task->dst = tmp;
	shadow_box_blur_h(task);
	task->src = tmp;
	task->dst = data;
	shadow_box_blur_v(task);
	task->src = data;
}

static void	shadow_blur_loops(t_blur_task *task, BYTE *tmp, int sizes[3])
{
	int	i;
	int	r;

	i = 0;
	while (i < 3)
	{
		r = (sizes[i] - 1) / 2;
		if (r > 0)
		{
			task->r = r;
			shadow_blur_pass(task, tmp);
		}
		i++;
	}
}

t_bool	shadow_blur_alpha(BYTE *data, int w, int h, float sigma)
{
	int			sizes[3];
	BYTE		*tmp;
	t_blur_task	task;

	shadow_boxes_for_gauss((double)sigma, 3, sizes);
	tmp = (BYTE *)malloc((size_t)w * (size_t)h);
	if (!tmp)
		return (FALSE);
	task.src = data;
	task.w = w;
	task.h = h;
	shadow_blur_loops(&task, tmp, sizes);
	free(tmp);
	return (TRUE);
}
