/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_str.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvkvkv <kvkvkv@student.42.rio>             #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 00:00:00 by kvkvkv            #+#    #+#             */
/*   Updated: 2026/02/04 00:00:00 by kvkvkv           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "util.h"
#include <stdlib.h>
#include <strsafe.h>

#define TITLE_MAX 120

static wchar_t	*dup_wstr(const wchar_t *src)
{
	size_t	len;
	wchar_t	*out;

	if (!src)
		return (NULL);
	len = wcslen(src);
	out = (wchar_t *)malloc((len + 1) * sizeof(wchar_t));
	if (!out)
		return (NULL);
	StringCchCopyW(out, len + 1, src);
	return (out);
}

static t_bool	invalid_char(wchar_t ch)
{
	if (ch < 32)
		return (TRUE);
	if (ch == L'<' || ch == L'>' || ch == L':' || ch == L'"')
		return (TRUE);
	if (ch == L'/' || ch == L'\\' || ch == L'|' || ch == L'?')
		return (TRUE);
	if (ch == L'*')
		return (TRUE);
	return (FALSE);
}

static void	trim_tail(wchar_t *out)
{
	size_t	len;

	len = wcslen(out);
	while (len > 0 && (out[len - 1] == L' ' || out[len - 1] == L'.'))
	{
		out[len - 1] = L'\0';
		len--;
	}
}

static void	copy_sanitized(const wchar_t *title, wchar_t *out)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (title[i] != L'\0' && j < TITLE_MAX)
	{
		if (invalid_char(title[i]))
			out[j] = L'_';
		else
			out[j] = title[i];
		i++;
		j++;
	}
	out[j] = L'\0';
}

wchar_t	*sanitize_filename(const wchar_t *title)
{
	wchar_t	*out;

	if (!title || title[0] == L'\0')
		return (dup_wstr(L"Window"));
	out = (wchar_t *)malloc((TITLE_MAX + 1) * sizeof(wchar_t));
	if (!out)
		return (NULL);
	copy_sanitized(title, out);
	trim_tail(out);
	if (out[0] == L'\0')
	{
		free(out);
		return (dup_wstr(L"Window"));
	}
	return (out);
}
