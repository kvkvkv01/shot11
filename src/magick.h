/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   magick.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvkvkv <kvkvkv@student.42.rio>             #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 00:00:00 by kvkvkv            #+#    #+#             */
/*   Updated: 2026/02/05 00:00:00 by kvkvkv           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef MAGICK_H
# define MAGICK_H

# include "util.h"

t_bool	magick_available(void);
t_bool	magick_shadow_png(const wchar_t *in_path,
			const wchar_t *out_path);

#endif
