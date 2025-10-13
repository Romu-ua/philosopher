#include <stdio.h>
#include <sys/time.h>

// 参考
// struct timeval {
//     time_t tv_sec;            /* Seconds.  */
//     suseconds_t tv_usec;      /* Microseconds.  */
// };
// int gettimeofday(struct timeval *tv, struct timezone *tz);

// macの環境だと time_tはlong suseconds_tはintになっている。
// tv_secはtime()で得られる値と同じ。
// suseconds_tは秒未満の値になる。

// 参考文献より、gettimeofdayは関数自体が非推奨になっている。
// clock_gettime()関数を使用するべきとなっている。

// 第二引数は歴史的遺物で常にNULLを渡すのが通例。

int	main(int argc, char **argv)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	printf("%ld %06d\n", tv.tv_sec, tv.tv_usec);
	gettimeofday(&tv, NULL);
	printf("%ld %06d\n", tv.tv_sec, tv.tv_usec);
	return (0);
}