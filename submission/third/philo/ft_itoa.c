/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 12:21:08 by hyamamot          #+#    #+#             */
/*   Updated: 2025/10/27 12:21:08 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

char	*is_zero(char *result)
{
	result = (char *)malloc(sizeof(char) * 2);
	if (!result)
		return (NULL);
	result[0] = '0';
	result[1] = '\0';
	return (result);
}

int	count_len(int n)
{
	int	len;

	len = 1;
	if (n < 0)
	{
		n = -n;
		len++;
	}
	while (n > 9)
	{
		n /= 10;
		len++;
	}
	return (len);
}

char	*is_nmalloc(int len, char *result)
{
	result = (char *)malloc(sizeof(char) * (len + 1));
	if (!result)
		return (NULL);
	return (result);
}

void	is_set(int n, int len, int is_negative, char *result)
{
	result[len--] = '\0';
	while (len >= 0)
	{
		result[len] = n % 10 + '0';
		n /= 10;
		len--;
	}
	if (is_negative)
		result[0] = '-';
}

char	*ft_itoa(int n)
{
	char			*result;
	int				is_negative;
	const int		len = count_len(n);

	result = NULL;
	if (n == 0)
	{
		result = is_zero(result);
		if (!result)
			return (NULL);
		return (result);
	}
	if (n == INT_MIN)
	{
		result = ft_strdup("-2147483648");
		return (result);
	}
	is_negative = (n < 0);
	if (is_negative)
		n = -n;
	result = is_nmalloc(len, result);
	if (!result)
		return (NULL);
	is_set(n, len, is_negative, result);
	return (result);
}
