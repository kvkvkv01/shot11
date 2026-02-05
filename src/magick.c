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
#include "magick.h"
#include <strsafe.h>
#include <stdlib.h>

static t_bool	magick_find(wchar_t *out, size_t cch)
{
	DWORD	n;

	if (!out || cch == 0)
		return (FALSE);
	n = SearchPathW(NULL, L"magick.exe", NULL,
			(DWORD)cch, out, NULL);
	if (n == 0 || n >= (DWORD)cch)
		return (FALSE);
	return (TRUE);
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

t_bool	magick_available(void)
{
	wchar_t	path[MAX_PATH];

	return (magick_find(path, ARRAYSIZE(path)));
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
				L"-shadow 50x50+30+30 ) +swap -background none "
				L"-layers merge +repage \"%s\"",
				exe, in_path, out_path)))
		return (FALSE);
	return (magick_run(cmd));
}
