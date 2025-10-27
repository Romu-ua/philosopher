/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 19:41:51 by hyamamot          #+#    #+#             */
/*   Updated: 2025/10/24 19:41:52 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

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
