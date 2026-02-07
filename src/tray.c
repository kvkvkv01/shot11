/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tray.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvkvkv <kvkvkv@student.42.rio>             #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 00:00:00 by kvkvkv            #+#    #+#             */
/*   Updated: 2026/02/07 00:00:00 by kvkvkv           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "tray.h"
#include <shellapi.h>

static HICON	tray_create_icon(void)
{
	BYTE	and_mask[32];
	BYTE	xor_mask[1024];
	int		i;

	ZeroMemory(and_mask, sizeof(and_mask));
	i = 0;
	while (i < 256)
	{
		xor_mask[i * 4 + 0] = 80;
		xor_mask[i * 4 + 1] = 80;
		xor_mask[i * 4 + 2] = 80;
		xor_mask[i * 4 + 3] = 255;
		i++;
	}
	return (CreateIcon(GetModuleHandle(NULL), 16, 16,
				1, 32, and_mask, xor_mask));
}

static void	tray_show_menu(HWND hwnd)
{
	HMENU	menu;
	POINT	pt;

	menu = CreatePopupMenu();
	if (!menu)
		return ;
	AppendMenuW(menu, MF_STRING, IDM_EXIT, L"Exit");
	GetCursorPos(&pt);
	SetForegroundWindow(hwnd);
	TrackPopupMenu(menu, TPM_RIGHTBUTTON, pt.x, pt.y,
		0, hwnd, NULL);
	DestroyMenu(menu);
}

void	tray_create(HWND hwnd)
{
	NOTIFYICONDATAW	nid;

	ZeroMemory(&nid, sizeof(nid));
	nid.cbSize = sizeof(nid);
	nid.hWnd = hwnd;
	nid.uID = 1;
	nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
	nid.uCallbackMessage = WM_TRAY;
	nid.hIcon = tray_create_icon();
	lstrcpyW(nid.szTip, L"Shot11 - Ctrl+Alt+S");
	Shell_NotifyIconW(NIM_ADD, &nid);
	nid.uVersion = NOTIFYICON_VERSION_4;
	Shell_NotifyIconW(NIM_SETVERSION, &nid);
	if (nid.hIcon)
		DestroyIcon(nid.hIcon);
}

void	tray_destroy(HWND hwnd)
{
	NOTIFYICONDATAW	nid;

	ZeroMemory(&nid, sizeof(nid));
	nid.cbSize = sizeof(nid);
	nid.hWnd = hwnd;
	nid.uID = 1;
	Shell_NotifyIconW(NIM_DELETE, &nid);
}

void	tray_handle_msg(HWND hwnd, LPARAM lp)
{
	if (LOWORD(lp) == WM_RBUTTONUP)
		tray_show_menu(hwnd);
	if (LOWORD(lp) == WM_CONTEXTMENU)
		tray_show_menu(hwnd);
}
