/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_log.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvkvkv <kvkvkv@student.42.rio>             #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 00:00:00 by kvkvkv            #+#    #+#             */
/*   Updated: 2026/02/04 00:00:00 by kvkvkv           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "util.h"
#include <strsafe.h>
#include <stdarg.h>

#ifdef DEBUG

void	log_debug(const wchar_t *fmt, ...)
{
	wchar_t	buf[1024];
	va_list	args;

	if (!fmt)
		return ;
	va_start(args, fmt);
	StringCchVPrintfW(buf, ARRAYSIZE(buf), fmt, args);
	va_end(args);
	OutputDebugStringW(buf);
	OutputDebugStringW(L"\n");
}

#else

void	log_debug(const wchar_t *fmt, ...)
{
	(void)fmt;
}
#endif
