// 複数piloとforkの管理
// 両サイドのフォーク対して+1をする処理を10000回行う

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct s_shared {
	int				*forks;
	pthread_mutex_t	*mtx;
}	t_shared;

typedef struct s_args {
	int			tid;
	long		loops;
	t_shared	*shared;
}	t_args;

void	*worker(void *args)
{
	t_args *a = (t_args *)args;

	for (int i = 0; i < a->loops; i++)
	{
		pthread_mutex_lock(&(a->shared->mtx[a->tid]));
		a->shared->forks[a->tid] += 1;
		pthread_mutex_unlock(&(a->shared->mtx[a->tid]));
	}
	return (NULL);
}

int	main(void)
{
	const int	N = 5;
	const long	LOOPS = 10000;
	t_shared	*shared;

	shared = (t_shared *)malloc(sizeof(*shared));
	if (!shared)
		return (1);
	shared->forks = (int *)malloc(sizeof(int) * N);
	shared->mtx = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * N);
	for (int i = 0; i < N; i++) {
		pthread_mutex_init(&((shared->mtx)[i]), NULL);
	};

	pthread_t	th[N];
	t_args		args[N];

	for (int i = 0; i < N; i++) {
		args[i].tid = i;
		args[i].loops = LOOPS;
		args[i].shared = shared;
		// philoは両サイドのforkだけが見えていれば良いので全てのfrokのmtxを持つ必要はない。
		// これで不具合が発生したら修正する。
		if (pthread_create(&th[i], NULL, worker, &args[i])) {
			perror("ptherad_create");
			return (1);
		}
	};
	for (int i = 0; i < N; i++) {
		pthread_join(th[i], NULL);
	}

	printf("fin\n");
	for (int i = 0;i < N; i++) {
		pthread_mutex_destroy(&(shared->mtx[i]));
	};

	for (int i = 0; i < N; i++) {
		printf("%d fork cnt is %d\n", i, shared->forks[i]);
	}
	free(shared->forks);
	free(shared->mtx);
	free(shared);
	return (0);
}
