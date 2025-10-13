/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 17:25:46 by hyamamot          #+#    #+#             */
/*   Updated: 2025/10/12 17:25:46 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <sys/time.h>
# include <limits.h>
# include <stdio.h>
# include <stdlib.h>
# include <time.h>
# include <unistd.h>
# include <stdbool.h>

typedef struct s_shared
{
	int				number_of_philosophers;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				number_of_times_each_philosopher_must_eat;
	pthread_mutex_t	*mtx;
	pthread_mutex_t	mtx_waiting;
	int				is_die;
	int				*fin_flags;
	pthread_mutex_t	mtx_fin_flags;
	pthread_mutex_t	mtx_printf;
	struct timeval	start;
}	t_shared;

typedef struct s_args
{
	int				tid;
	t_shared		*shared;
	pthread_mutex_t	mtx_last_eat_ts;
	struct timeval	*last_eat_ts;
}	t_args;

void		*worker(void *args);
void		*die_worker(void *args);
int			stop(void *args);
void		ft_msleep(int ms);
int			timestamp_ms(struct timeval start, struct timeval curr);
void		eat(void *args);
void		think(void *args);
void		philo_sleep(void *args);
int			ft_atoi(const char *nptr);
t_shared	*shared_init(int argc, char **argv, int *is_miss_init);
t_args		*args_init(t_shared *shared, int *is_miss_init);
void		th_init(t_shared *shared, pthread_t **th, int *is_miss_init);
void		destory(t_args *args, pthread_t *th);
void		*util_return(int *is_miss_init, t_shared *shared);
void		*cleanup_return(t_shared *shared);
void		shared_cleanup(t_shared *shared);
#endif