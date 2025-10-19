/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printf_wrap.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 14:40:19 by hyamamot          #+#    #+#             */
/*   Updated: 2025/10/17 14:40:20 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	nbrlen(int nbr)
{
	int	len;

	len = 0;
	if (nbr == 0)
		return (1);
	while (nbr)
	{
		len++;
		nbr /= 10;
	}
	return (len);
}

char	*nbrtostr(int nbr)
{
	char	*str;
	int		len;
	int		i;

	len = nbrlen(nbr);
	str = (char *)malloc(sizeof(char) * (len + 1));
	i = 0;
	while (i < len)
	{
		str[len - i - 1] = *ft_itoa(nbr % 10);
		nbr /= 10;
		i++;
	}
	str[len] = '\0';
	return (str);
}

void	printf_wrap(char *str, void *args)
{
	char			*output;
	t_args			*a;
	struct timeval	timestamp;

	a = (t_args *)args;
	pthread_mutex_lock(&a->shared->mtx_printf);
	if (stop(args))
	{
		pthread_mutex_unlock(&a->shared->mtx_printf);
		return ;
	}
	pthread_mutex_lock(&a->mtx_last_eat_ts);
	gettimeofday(&timestamp, NULL);
	*(a->last_eat_ts) = timestamp;
	pthread_mutex_unlock(&a->mtx_last_eat_ts);
	output = nbrtostr(timestamp_ms(a->shared->start, timestamp));
	printf("%s ", output);
	free(output);
	output = nbrtostr(a->tid + 1);
	printf("%s ", output);
	free(output);
	printf("%s\n", str);
	pthread_mutex_unlock(&a->shared->mtx_printf);
	return ;
}
