/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clauren <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/01 22:08:41 by clauren           #+#    #+#             */
/*   Updated: 2020/08/13 00:14:09 by clauren          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>

void print(char c)
{
	write(1, &c, 1);
}

void fill(char c, int n)
{
	while(n--)
		print(c);
}

int is_flag(char c)
{
	char *flags = "-+0 ";
	while (*flags)
		if (c == *flags++)
			return (1);
	return (0);
}
int is_digit(char c)
{
	if ((c >= '0') && (c <= '9'))
		return (1);
	return (0);
}

int ft_strlen(char *s)
{
	int i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

int number_len(long long n, int base)
{
	int len;

	len = 1;
	if (n < 0 )
	{
		if (base == 10)
			len++;
		n *= -1;
	}
	while ((n / base) > 0)
	{
		len++;
		n /= base;
	}
	return (len);
}

char *ltoa_base(long long n, int base, int prec)
{
	int len;
	int i;
	char *result;
	char c;
	int flag;

	i = 0;
	flag = 0;
	len = number_len(n, base);
	if ((len < prec) && (flag = 1))
		len = prec;
	if (len == 1 && n == 0 && !prec)
		return (NULL);
	if(!(result = malloc(sizeof(char) * (len + 1))))
		return (NULL);
	if (n < 0)
	{
		n *= -1;
		if (base == 10)
		{
			if (flag)
				len++;
			result[i++] = '-';
		}

	}
	while(i < len && prec)
	{
		result[i++] = '0';
	}
	result[len--] = '\0';
	while(n)
	{
		c = n % base;
		n /= base;
		c = (c > 9) ? c += 39 : c;
		result[len--] = (c + '0');
	}
	return (result);
}

int print_xd(va_list ap, int wid, int prec, char type)
{
	int len = 0;
	int spaces;
	char *num;
	int k;

	k = 0;
	spaces = 0;
	if (type == 'd')
		num = ltoa_base(va_arg(ap, int), 10, prec);
	else if (type == 'x')
		num = ltoa_base(va_arg(ap, unsigned long), 16, prec);
	else
		return (-1);
	if (!num)
		num = calloc(1, 1);
	len = ft_strlen(num);
	if ((len) < wid)
	{
		spaces = wid - (len);
		fill(' ', spaces);
	}
	while(k < len)
		print(num[k++]);
	free(num);
	return (len + spaces);
}

int print_s(va_list ap, int wid, int prec)
{
	int len;
	char *str;
	int i = 0;
	int spaces;

	str = va_arg(ap, char *);
	if (!str)
		str = "(null)";
	len = ft_strlen(str);
	spaces = 0;
	if ((prec != -1) && len > prec)
		len = prec;
	if (len < wid)
	{
		fill(' ', (wid - len));
		spaces = wid - len;
	}
	while (i < len)
	{
		print(str[i++]);
	}
	return (len + spaces);
}

int switcher(va_list ap, int wid, int prec, char type)
{
	if (type == 'd' || type == 'x')
		return(print_xd(ap, wid, prec, type));
	if (type == 's')
		return(print_s(ap, wid, prec));
	return (-1);
}

int parse(char *fmt, va_list ap)
{
	int len;
	int prec;
	int wid;
	char type;

	len = 0;
	while(*fmt)
	{
		if (*fmt != '%')
		{
			len++;
			print(*fmt++);
			continue ;
		}
		prec = -1;
		wid = 0;
		type = ' ';
		fmt++;
		while (is_flag(*fmt))
			fmt++;
		while(is_digit(*fmt))
		{
			wid = 10 * wid + (*fmt - '0');
			fmt++;
		}
		if (*fmt == '.')
		{
			prec = 0;
			fmt++;
			while (is_digit(*fmt))
			{
				prec = 10 * prec + (*fmt - '0');
				fmt++;
			}
		}
		type = *fmt++;
		len += switcher(ap, wid, prec, type);
	}
	return (len);
}

int ft_printf(const char *fmt, ...)
{
	va_list ap;
	int len;
	va_start(ap, fmt);
	len = parse((char *)fmt, ap);
	va_end(ap);
	return (len);
}
