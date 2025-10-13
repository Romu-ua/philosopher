// pthread_mutex_init と pthread_mutex_lock, 

#include <stdio.h>
#include <pthread.h>

int	a = 0;
pthread_mutex_t	mutex;

void *func_thread(void *p)
{
	printf("start %d\n", *(int *)p);

	int	i = 0;
	for (i = 0; i < 10000; i++) {
		pthread_mutex_lock(&mutex);
		int	next = a + 1;
		int	now = a;
		a = next;
		pthread_mutex_unlock(&mutex);

		if (now + 1 != next) {
			printf("other thread change %d %d\n", a + 1, next);
		}
	}
	
	return (0);
}

int	main(void)
{
	pthread_mutex_init(&mutex, NULL);

	int	b = 42;

	pthread_t pthread;
	pthread_create(&pthread, NULL, &func_thread, &b);

	int i = 0;
	for (i = 0; i < 10000; i++) {
		pthread_mutex_lock(&mutex);
		int next = a + 1;
		int now = a;
		a = next;
		pthread_mutex_unlock(&mutex);
		if (now + 1 != next) {
			printf("other thread change %d %d\n", a + 1, next);
		}
	}
	pthread_join(pthread, NULL);
	printf("a=%d\n", a);

	return (0);
}

