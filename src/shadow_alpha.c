/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shadow_alpha.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvkvkv <kvkvkv@student.42.rio>             #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 00:00:00 by kvkvkv            #+#    #+#             */
/*   Updated: 2026/02/04 00:00:00 by kvkvkv           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "shadow_internal.h"
#include <stdlib.h>
#include <string.h>

static void	shadow_apply_opacity(BYTE *alpha, size_t size, float opacity)
{
	size_t	i;
	double	val;

	i = 0;
	while (i < size)
	{
		val = (double)alpha[i] * opacity;
		if (val < 0.0)
			val = 0.0;
		if (val > 255.0)
			val = 255.0;
		alpha[i] = (BYTE)(val + 0.5);
		i++;
	}
}

BYTE	*shadow_make_alpha(int w, int h, const t_shadow_params *p)
{
	size_t	size;
	BYTE	*alpha;

	size = (size_t)w * (size_t)h;
	alpha = (BYTE *)malloc(size);
	if (!alpha)
		return (NULL);
	memset(alpha, 255, size);
	if (!shadow_blur_alpha(alpha, w, h, p->sigma))
	{
		free(alpha);
		return (NULL);
	}
	shadow_apply_opacity(alpha, size, p->opacity);
	return (alpha);
}
