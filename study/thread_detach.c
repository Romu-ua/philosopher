// pthread_detach

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void	*worker(void *arg)
{
	printf("Worker thread started\n");
	sleep(1);
	printf("Worker thread finishled\n");
	return (NULL); // detachするので戻り値は誰も受け取らない。
}

int	main(void)
{
	pthread_t	th;

	pthread_create(&th, NULL, worker, NULL);
	pthread_detach(th);

	pthread_exit(NULL);
}
