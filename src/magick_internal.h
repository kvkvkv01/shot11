/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   magick_internal.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvkvkv <kvkvkv@student.42.rio>             #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 00:00:00 by kvkvkv            #+#    #+#             */
/*   Updated: 2026/02/05 00:00:00 by kvkvkv           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef MAGICK_INTERNAL_H
# define MAGICK_INTERNAL_H

# include "magick.h"

typedef struct s_magick_dir
{
	wchar_t				pat[MAX_PATH];
	WIN32_FIND_DATAW	fd;
	HANDLE				h;
}	t_magick_dir;

#endif
