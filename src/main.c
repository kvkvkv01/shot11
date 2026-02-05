/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvkvkv <kvkvkv@student.42.rio>             #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 00:00:00 by kvkvkv            #+#    #+#             */
/*   Updated: 2026/02/04 00:00:00 by kvkvkv           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <windows.h>

#include "app.h"
#include "util.h"

#define HOTKEY_ID 1

static t_bool	init_com(void)
{
	HRESULT	hr;

	hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED
			| COINIT_DISABLE_OLE1DDE);
	if (SUCCEEDED(hr))
		return (TRUE);
	return (FALSE);
}

static t_bool	register_hotkey(void)
{
	if (!RegisterHotKey(NULL, HOTKEY_ID, MOD_CONTROL | MOD_ALT, 'S'))
		return (FALSE);
	return (TRUE);
}

static void	message_loop(void)
{
	MSG	msg;
	int	ret;

	ret = (int)GetMessageW(&msg, NULL, 0, 0);
	while (ret > 0)
	{
		if (msg.message == WM_HOTKEY && msg.wParam == HOTKEY_ID)
			app_handle_hotkey();
		ret = (int)GetMessageW(&msg, NULL, 0, 0);
	}
}

int	main(void)
{
	t_bool	com_ok;

	SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
	com_ok = init_com();
	if (!register_hotkey())
		log_debug(L"RegisterHotKey failed");
	message_loop();
	UnregisterHotKey(NULL, HOTKEY_ID);
	if (com_ok)
		CoUninitialize();
	return (0);
}
