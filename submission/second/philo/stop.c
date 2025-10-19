/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 17:26:01 by hyamamot          #+#    #+#             */
/*   Updated: 2025/10/12 17:26:02 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	stop(void *args)
{
	t_args	*a;
	int		flag;

	flag = 0;
	a = (t_args *)args;
	pthread_mutex_lock(&a->mtx_last_eat_ts);
	if (a->shared->is_die)
		flag = 1;
	pthread_mutex_unlock(&a->mtx_last_eat_ts);
	return (flag);
}
