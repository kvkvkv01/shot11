/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   magick.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvkvkv <kvkvkv@student.42.rio>             #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 00:00:00 by kvkvkv            #+#    #+#             */
/*   Updated: 2026/02/05 00:00:00 by kvkvkv           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "magick_internal.h"
#include <strsafe.h>

static t_bool	magick_try_entry(const wchar_t *root, const wchar_t *name,
			wchar_t *out, size_t cch)
{
	if (FAILED(StringCchPrintfW(out, cch,
				L"%s\\%s\\magick.exe", root, name)))
		return (FALSE);
	if (GetFileAttributesW(out) == INVALID_FILE_ATTRIBUTES)
		return (FALSE);
	return (TRUE);
}

static t_bool	magick_try_dir(const wchar_t *root, wchar_t *out, size_t cch)
{
	t_magick_dir	d;

	if (FAILED(StringCchPrintfW(d.pat, ARRAYSIZE(d.pat),
				L"%s\\ImageMagick-*", root)))
		return (FALSE);
	d.h = FindFirstFileW(d.pat, &d.fd);
	if (d.h == INVALID_HANDLE_VALUE)
		return (FALSE);
	while (1)
	{
		if ((d.fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			&& magick_try_entry(root, d.fd.cFileName, out, cch))
		{
			FindClose(d.h);
			return (TRUE);
		}
		if (!FindNextFileW(d.h, &d.fd))
		{
			FindClose(d.h);
			return (FALSE);
		}
	}
	return (FALSE);
}

static t_bool	magick_find(wchar_t *out, size_t cch)
{
	DWORD	n;

	if (!out || cch == 0)
		return (FALSE);
	n = GetEnvironmentVariableW(L"MAGICK_EXE", out, (DWORD)cch);
	if (n > 0 && n < (DWORD)cch)
	{
		if (GetFileAttributesW(out) != INVALID_FILE_ATTRIBUTES)
			return (TRUE);
	}
	n = SearchPathW(NULL, L"magick.exe", NULL,
			(DWORD)cch, out, NULL);
	if (n > 0 && n < (DWORD)cch)
		return (TRUE);
	if (magick_try_dir(L"C:\\Program Files", out, cch))
		return (TRUE);
	if (magick_try_dir(L"C:\\Program Files (x86)", out, cch))
		return (TRUE);
	return (FALSE);
}

static t_bool	magick_run(wchar_t *cmd)
{
	STARTUPINFOW		si;
	PROCESS_INFORMATION	pi;
	DWORD				code;

	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));
	si.cb = sizeof(si);
	if (!CreateProcessW(NULL, cmd, NULL, NULL, FALSE,
			CREATE_NO_WINDOW, NULL, NULL, &si, &pi))
		return (FALSE);
	WaitForSingleObject(pi.hProcess, INFINITE);
	code = 1;
	if (!GetExitCodeProcess(pi.hProcess, &code))
		code = 1;
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
	return (code == 0);
}

t_bool	magick_shadow_png(const wchar_t *in_path, const wchar_t *out_path)
{
	wchar_t	exe[MAX_PATH];
	wchar_t	cmd[2048];

	if (!in_path || !out_path)
		return (FALSE);
	if (!magick_find(exe, ARRAYSIZE(exe)))
		return (FALSE);
	if (FAILED(StringCchPrintfW(cmd, ARRAYSIZE(cmd),
				L"\"%s\" \"%s\" ( +clone -background black "
				L"-shadow 40x50+0+36 ) +swap -background transparent "
				L"-layers merge +repage \"%s\"",
				exe, in_path, out_path)))
		return (FALSE);
	return (magick_run(cmd));
}
