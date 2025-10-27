/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printf_wrap.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 19:24:12 by hyamamot          #+#    #+#             */
/*   Updated: 2025/10/24 19:24:14 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

int	printf_wrap(char *str, void *args)
{
	t_args			*a;
	char			*output;
	struct timeval	curr;

	a = (t_args *)args;
	pthread_mutex_lock(&a->shared->mtx_printf);
	if (stop(args))
	{
		pthread_mutex_unlock(&a->shared->mtx_printf);
		return (1);
	}
	pthread_mutex_lock(&a->mtx_last_eat_ts);
	gettimeofday(&curr, NULL);
	*(a->last_eat_ts) = curr;
	pthread_mutex_unlock(&a->mtx_last_eat_ts);
	output = ft_itoa(timestamp_ms(a->shared->start, curr));
	printf("%s ", output);
	free(output);
	output = ft_itoa(a->tid + 1);
	printf("%s ", output);
	free(output);
	printf("%s\n", str);
	pthread_mutex_unlock(&a->shared->mtx_printf);
	return (0);
}
