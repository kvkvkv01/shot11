/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_time.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvkvkv <kvkvkv@student.42.rio>             #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 00:00:00 by kvkvkv            #+#    #+#             */
/*   Updated: 2026/02/04 00:00:00 by kvkvkv           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "util.h"
#include <strsafe.h>

t_bool	get_timestamp(wchar_t *out, size_t out_cch)
{
	SYSTEMTIME	st;
	HRESULT		hr;

	if (!out || out_cch == 0)
		return (FALSE);
	GetLocalTime(&st);
	hr = StringCchPrintfW(out, out_cch,
			L"%04d%02d%02d_%02d%02d%02d",
			st.wYear, st.wMonth, st.wDay,
			st.wHour, st.wMinute, st.wSecond);
	return (SUCCEEDED(hr));
}
