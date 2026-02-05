/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shadow_blur_boxes.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvkvkv <kvkvkv@student.42.rio>             #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 00:00:00 by kvkvkv            #+#    #+#             */
/*   Updated: 2026/02/04 00:00:00 by kvkvkv           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "shadow_internal.h"
#include <math.h>

static int	shadow_round(double v)
{
	if (v >= 0.0)
		return ((int)(v + 0.5));
	return ((int)(v - 0.5));
}

static double	shadow_m_ideal(double sigma, int n, int wl)
{
	double	m_ideal;

	m_ideal = 12.0 * sigma * sigma;
	m_ideal = m_ideal - (double)n * wl * wl;
	m_ideal = m_ideal - 4.0 * (double)n * wl - 3.0 * (double)n;
	m_ideal = m_ideal / (-4.0 * wl - 4.0);
	return (m_ideal);
}

void	shadow_boxes_for_gauss(double sigma, int n, int sizes[3])
{
	double	w_ideal;
	int		wl;
	int		wu;
	int		m;
	int		i;

	w_ideal = sqrt((12.0 * sigma * sigma / (double)n) + 1.0);
	wl = (int)floor(w_ideal);
	if ((wl % 2) == 0)
		wl--;
	wu = wl + 2;
	m = shadow_round(shadow_m_ideal(sigma, n, wl));
	i = 0;
	while (i < n)
	{
		if (i < m)
			sizes[i] = wl;
		else
			sizes[i] = wu;
		i++;
	}
}
