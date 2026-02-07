/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tray.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvkvkv <kvkvkv@student.42.rio>             #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 00:00:00 by kvkvkv            #+#    #+#             */
/*   Updated: 2026/02/07 00:00:00 by kvkvkv           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef TRAY_H
# define TRAY_H

# include <windows.h>

# define WM_TRAY	(WM_APP + 1)
# define IDM_EXIT	1001

void	tray_create(HWND hwnd);
void	tray_destroy(HWND hwnd);
void	tray_handle_msg(HWND hwnd, LPARAM lp);

#endif
