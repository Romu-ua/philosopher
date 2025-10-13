// pthread_create と pthread_join

#include <stdio.h>
#include <pthread.h>

int	a = 0;

void	*func_thread(void *p)
{
	printf("start %d\n", *(int *)p);

	int	i = 0;
	for (i = 0; i < 10000; i++) {
		int	next = a + 1;
		int	now = a;
		a = next;
		if (now + 1 != next) {
			printf("other thread change %d %d\n", a+1, next);
		}
	}
	return (0);
}

int	main(void)
{
	int			b;
	pthread_t	pthread;

	b = 42;
	pthread_create(&pthread, NULL, &func_thread, &b);
	int	i = 0;
	for (i = 0; i < 10000; i++) {
		int	next = a + 1;
		int now = a;
		a = next;
		if (now + 1 != next) {
			printf("other hread change %d %d\n", a+1, next);
		}
	}

	pthread_join(pthread, NULL);
	printf("a=%d\n", a);

	return (0);
}
