/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   capture_internal.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvkvkv <kvkvkv@student.42.rio>             #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 00:00:00 by kvkvkv            #+#    #+#             */
/*   Updated: 2026/02/04 00:00:00 by kvkvkv           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef CAPTURE_INTERNAL_H
# define CAPTURE_INTERNAL_H

# include "capture.h"

struct s_capture
{
	HDC		hdc;
	HBITMAP	hbm;
	HGDIOBJ	old;
	void	*bits;
	int		width;
	int		height;
	size_t	size;
};

t_bool	capture_try_print(HWND hwnd, HDC hdc, BYTE *bits, size_t size);
void	capture_try_blt(HDC hdc, const RECT *b, int w, int h);
void	capture_fill_alpha(BYTE *bits, size_t size);
void	capture_fix_print_offset(HWND hwnd, const RECT *ext,
			struct s_capture *cap);
void	capture_round_corners(HWND hwnd, BYTE *bits, int w, int h);
t_bool	capture_merge_print(HWND hwnd, struct s_capture *cap,
			const RECT *b);

#endif
