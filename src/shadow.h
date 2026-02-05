/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shadow.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvkvkv <kvkvkv@student.42.rio>             #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 00:00:00 by kvkvkv            #+#    #+#             */
/*   Updated: 2026/02/04 00:00:00 by kvkvkv           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef SHADOW_H
# define SHADOW_H

# include "util.h"

typedef struct s_shadow_params
{
	float	sigma;
	int		off_x;
	int		off_y;
	float	opacity;
}	t_shadow_params;

void	shadow_params_default(t_shadow_params *p);
t_bool	create_shadowed_image(const t_image *src, t_image *out,
			const t_shadow_params *p);

#endif
