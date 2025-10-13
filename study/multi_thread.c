// 単一変数を5つのスレッドで更新していく

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct s_shared {
	long			counter;
	pthread_mutex_t	mtx;
}	t_shared;

typedef struct s_args {
	int			tid;
	long		loops;
	t_shared	*shared;
}	t_args;

void	*worker(void *arg)
{
	t_args	*a = (t_args *)arg;

	for (long i = 0; i < a->loops; i++) {
		pthread_mutex_lock(&a->shared->mtx);
		a->shared->counter++;
		pthread_mutex_unlock(&a->shared->mtx);
	}
	return (NULL);
}

int	main(void)
{
	const int	N = 5;
	const long	LOOPS = 100000;
	t_shared	*shared;

	shared = malloc(sizeof(*shared));
	if (!shared) 
		return (1);
	shared->counter = 0;
	pthread_mutex_init(&shared->mtx, NULL);

	pthread_t	th[N];
	t_args		args[N];

	// Nの数だけスレッドを作成する
	for (int i = 0; i < N; i++) {
		args[i].tid = i;
		args[i].loops = LOOPS;
		args[i].shared = shared;
		if (pthread_create(&th[i], NULL, worker, &args[i]) != 0) {
			perror("pthread_creaet");
			return (1);
		}
	};
	for (int i = 0; i < N; i++) {
		pthread_join(th[i], NULL);
	};

	printf("counter = %ld (expected %ld)\n", shared->counter, N * LOOPS);

	pthread_mutex_destroy(&shared->mtx);
	free(shared);
	return (0);
}
