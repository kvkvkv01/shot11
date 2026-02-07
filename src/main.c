/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvkvkv <kvkvkv@student.42.rio>             #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 00:00:00 by kvkvkv            #+#    #+#             */
/*   Updated: 2026/02/07 00:00:00 by kvkvkv           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <windows.h>

#include "app.h"
#include "tray.h"
#include "util.h"

#define HOTKEY_ID 1

static LRESULT CALLBACK	wnd_proc(HWND hw, UINT msg, WPARAM wp,
		LPARAM lp)
{
	if (msg == WM_HOTKEY && wp == HOTKEY_ID)
		app_handle_hotkey();
	else if (msg == WM_TRAY)
		tray_handle_msg(hw, lp);
	else if (msg == WM_COMMAND && LOWORD(wp) == IDM_EXIT)
		DestroyWindow(hw);
	else if (msg == WM_DESTROY)
	{
		tray_destroy(hw);
		PostQuitMessage(0);
	}
	else
		return (DefWindowProcW(hw, msg, wp, lp));
	return (0);
}

static t_bool	app_register_class(HINSTANCE inst)
{
	WNDCLASSW	wc;

	ZeroMemory(&wc, sizeof(wc));
	wc.lpfnWndProc = wnd_proc;
	wc.hInstance = inst;
	wc.lpszClassName = L"Shot11Class";
	return (RegisterClassW(&wc) != 0);
}

static HWND	app_init_window(HINSTANCE inst)
{
	HWND	hwnd;

	hwnd = CreateWindowExW(0, L"Shot11Class", L"Shot11", 0,
			0, 0, 0, 0, HWND_MESSAGE, NULL, inst, NULL);
	if (!hwnd)
		return (NULL);
	if (!RegisterHotKey(hwnd, HOTKEY_ID,
				MOD_CONTROL | MOD_ALT, 'S'))
	{
		DestroyWindow(hwnd);
		return (NULL);
	}
	tray_create(hwnd);
	return (hwnd);
}

static void	message_loop(void)
{
	MSG	msg;

	while (GetMessageW(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
}

int WINAPI	WinMain(HINSTANCE inst, HINSTANCE prev,
		LPSTR cmd, int show)
{
	HWND	hwnd;
	HRESULT	hr;

	(void)prev;
	(void)cmd;
	(void)show;
	SetProcessDpiAwarenessContext(
		DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
	hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED
		| COINIT_DISABLE_OLE1DDE);
	if (FAILED(hr))
	{
		MessageBoxW(NULL, L"COM init failed",
			L"Shot11", MB_OK | MB_ICONERROR);
		return (1);
	}
	if (!app_register_class(inst))
		return (1);
	hwnd = app_init_window(inst);
	if (!hwnd)
		return (1);
	message_loop();
	CoUninitialize();
	return (0);
}
