/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clipboard.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvkvkv <kvkvkv@student.42.rio>             #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 00:00:00 by kvkvkv            #+#    #+#             */
/*   Updated: 2026/02/07 00:00:00 by kvkvkv           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "clipboard.h"

static t_bool	clipboard_open(void)
{
	int	i;

	i = 0;
	while (i < 10)
	{
		if (OpenClipboard(NULL))
			return (TRUE);
		Sleep(10);
		i++;
	}
	return (FALSE);
}

static HGLOBAL	clipboard_alloc_file(HANDLE f, DWORD sz)
{
	HGLOBAL	hm;
	DWORD	rd;

	hm = GlobalAlloc(GHND, sz);
	rd = 0;
	if (hm)
	{
		ReadFile(f, GlobalLock(hm), sz, &rd, NULL);
		GlobalUnlock(hm);
	}
	if (!hm || rd != sz)
	{
		if (hm)
			GlobalFree(hm);
		return (NULL);
	}
	return (hm);
}

static HGLOBAL	clipboard_read_png(const wchar_t *path)
{
	HANDLE	f;
	DWORD	sz;
	HGLOBAL	hm;

	f = CreateFileW(path, GENERIC_READ, FILE_SHARE_READ,
			NULL, OPEN_EXISTING, 0, NULL);
	if (f == INVALID_HANDLE_VALUE)
		return (NULL);
	sz = GetFileSize(f, NULL);
	hm = NULL;
	if (sz > 0 && sz != INVALID_FILE_SIZE)
		hm = clipboard_alloc_file(f, sz);
	CloseHandle(f);
	return (hm);
}

t_bool	clipboard_set_png(const wchar_t *path)
{
	UINT	fmt;
	HGLOBAL	hmem;

	if (!path)
		return (FALSE);
	fmt = RegisterClipboardFormatW(L"PNG");
	if (!fmt)
		return (FALSE);
	hmem = clipboard_read_png(path);
	if (!hmem)
		return (FALSE);
	if (!clipboard_open())
	{
		GlobalFree(hmem);
		return (FALSE);
	}
	EmptyClipboard();
	if (!SetClipboardData(fmt, hmem))
	{
		CloseClipboard();
		GlobalFree(hmem);
		return (FALSE);
	}
	CloseClipboard();
	return (TRUE);
}
