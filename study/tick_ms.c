// usleepを1msごとにして処理を実行していくと、
// msの単位でスキップが生じる。
// なぜなら、1ms＋αの処理時間が毎回発生しているから。
// どうやって回避するか。

#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>

typedef struct	s_threadargs
{
	long	duration_ms;
	long	interval_ms;
}	t_threadargs;

long long	now_us(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((long long)tv.tv_sec * 1000000LL + (long long)tv.tv_usec);
}

void	*worker(void *arg)
{
	t_threadargs	*args = (t_threadargs *)arg;
	const long long duration_us = args->duration_ms * 1000LL;
	const long long interval_us = args->interval_ms * 1000LL;

	const long long	t0 = now_us();

	while (now_us() - t0 < duration_us)
	{
		printf("[t = %.3f ms]\n", (now_us() - t0) / 1000.0);
		fflush(stdout);
		usleep(interval_us);
	}

	printf("done");
	return (NULL);
}

int	main(void)
{
	pthread_t		th;
	t_threadargs	args;

	args.duration_ms = 10;
	args.interval_ms = 1;

	if (pthread_create(&th, NULL, worker, &args) != 0)
	{
		perror("pthread_create");
		return (1);
	}
	pthread_join(th, NULL);
}
