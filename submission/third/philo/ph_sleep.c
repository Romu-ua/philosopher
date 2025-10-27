/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ph_sleep.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 19:23:56 by hyamamot          #+#    #+#             */
/*   Updated: 2025/10/24 19:23:57 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

int	ph_sleep(void *args)
{
	t_args			*a;
	struct timeval	curr;
	int				ts;

	a = (t_args *)args;
	pthread_mutex_lock(&a->shared->mtx_printf);
	if (stop(args))
	{
		pthread_mutex_unlock(&a->shared->mtx_printf);
		return (1);
	}
	gettimeofday(&curr, NULL);
	ts = timestamp_ms(a->shared->start, curr);
	printf("%d %d is sleeping\n", ts, a->tid + 1);
	pthread_mutex_unlock(&a->shared->mtx_printf);
	ft_msleep(a->shared->time_to_sleep);
	return (0);
}
