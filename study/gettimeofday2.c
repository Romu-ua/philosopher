#include <stdio.h>
#include <sys/time.h>

int	timeval_to_ms(struct timeval start, struct timeval curr)
{
	return ((curr.tv_usec - start.tv_usec) / 1000 * 1000);
}

int main(void)
{
	int				range;
	struct timeval	start;
	struct timeval	curr;

	// 10msのrangeで処理を終了させる。
	// tv_usecは6桁。したがって、100000 で 1/10 s
	range = 100;
	gettimeofday(&start, NULL);
	gettimeofday(&curr, NULL);
	printf("start: %d", timeval_to_ms(start, curr));
	while (1)
	{
		gettimeofday(&curr, NULL);
		if (range < timeval_to_ms(start, curr))
		{
			printf("%d break\n", timeval_to_ms(start, curr));
			break ;
		}
		printf("%d in process\n", timeval_to_ms(start, curr));
	}
	return (0);
}
