#include <stdio.h>
#include <unistd.h>

int	main(void)
{
	int	second;
	int	microsecond;

	second = 5;
	printf("%ds止めます。\n", second);
	sleep(second);
	printf("%ds止めました。\n", second);

	microsecond = 5 * 1000000;
	printf("%dμs止めます。\n", microsecond);
	usleep(microsecond);
	printf("%dμs止めました。\n", microsecond);
}