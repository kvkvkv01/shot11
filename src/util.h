/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvkvkv <kvkvkv@student.42.rio>             #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 00:00:00 by kvkvkv            #+#    #+#             */
/*   Updated: 2026/02/04 00:00:00 by kvkvkv           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef UTIL_H
# define UTIL_H

# include <windows.h>
# include <stddef.h>

typedef int				t_bool;

typedef struct s_image
{
	int		width;
	int		height;
	int		stride;
	BYTE	*pixels;
}	t_image;

t_bool	image_create(t_image *img, int width, int height);
void	image_free(t_image *img);
void	log_debug(const wchar_t *fmt, ...);
wchar_t	*sanitize_filename(const wchar_t *title);
t_bool	get_timestamp(wchar_t *out, size_t out_cch);

#endif
