/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvkvkv <kvkvkv@student.42.rio>             #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 00:00:00 by kvkvkv            #+#    #+#             */
/*   Updated: 2026/02/04 00:00:00 by kvkvkv           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "path.h"
#include <shlobj.h>
#include <strsafe.h>
#include <stdlib.h>

static t_bool	copy_and_free(PWSTR path, wchar_t **out_path)
{
	size_t	len;
	wchar_t	*out;

	len = wcslen(path);
	out = (wchar_t *)malloc((len + 1) * sizeof(wchar_t));
	if (!out)
	{
		CoTaskMemFree(path);
		return (FALSE);
	}
	StringCchCopyW(out, len + 1, path);
	CoTaskMemFree(path);
	*out_path = out;
	return (TRUE);
}

t_bool	get_screenshots_folder(wchar_t **out_path)
{
	PWSTR	path;
	HRESULT	hr;

	if (!out_path)
		return (FALSE);
	*out_path = NULL;
	path = NULL;
	hr = SHGetKnownFolderPath(&FOLDERID_Screenshots, KF_FLAG_DEFAULT,
			NULL, &path);
	if (FAILED(hr) || !path)
		return (FALSE);
	SHCreateDirectoryExW(NULL, path, NULL);
	return (copy_and_free(path, out_path));
}

static t_bool	alloc_path(const wchar_t *folder, const wchar_t *title,
	const wchar_t *timestamp, wchar_t **out_path)
{
	size_t	len;
	wchar_t	*out;

	len = wcslen(folder);
	len += 1;
	len += wcslen(title);
	len += 3;
	len += wcslen(timestamp);
	len += 4;
	len += 1;
	out = (wchar_t *)malloc(len * sizeof(wchar_t));
	if (!out)
		return (FALSE);
	StringCchPrintfW(out, len, L"%s\\%s - %s.png",
		folder, title, timestamp);
	*out_path = out;
	return (TRUE);
}

t_bool	build_screenshot_path(const wchar_t *folder,
	const wchar_t *title, wchar_t **out_path)
{
	wchar_t	*safe_title;
	wchar_t	timestamp[32];

	if (!folder || !out_path)
		return (FALSE);
	*out_path = NULL;
	safe_title = sanitize_filename(title);
	if (!safe_title)
		return (FALSE);
	if (!get_timestamp(timestamp, ARRAYSIZE(timestamp)))
	{
		free(safe_title);
		return (FALSE);
	}
	if (!alloc_path(folder, safe_title, timestamp, out_path))
	{
		free(safe_title);
		return (FALSE);
	}
	free(safe_title);
	return (TRUE);
}
