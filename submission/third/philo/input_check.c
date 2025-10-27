/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_check.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 15:42:54 by hyamamot          #+#    #+#             */
/*   Updated: 2025/10/23 15:42:55 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

int	is_space(int c)
{
	return (c == ' ' || c == '\t' || c == '\n'
		|| c == '\v' || c == '\f' || c == '\r');
}

int	parse_positive_int(char *s)
{
	long long	val;

	while (*s && is_space(*s))
		s++;
	if (*s < '0' || *s > '9')
		return (1);
	val = 0;
	while (*s >= '0' && *s <= '9')
	{
		val = val * 10 + (*s - '0');
		if (val > INT_MAX)
			return (1);
		s++;
	}
	while (*s && is_space(*s))
		s++;
	if (*s != '\0')
		return (1);
	if (val <= 0)
		return (1);
	return (0);
}

int	input_check(int argc, char **argv)
{
	int	i;

	if (argc > 6 || argc < 5)
		return (1);
	i = 1;
	while (i <= 4)
	{
		if (parse_positive_int(argv[i]))
		{
			i++;
			return (1);
		}
		i++;
	}
	if (argc == 6 && parse_positive_int(argv[5]))
		return (1);
	return (0);
}
