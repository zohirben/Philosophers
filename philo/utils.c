#include "philo.h"

long long gettime(void)
{
	struct timeval time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

void ft_sleep(long long time)
{
	long long start_point;

	start_point = gettime();
	while ((gettime() - start_point) < time)
	{
		usleep(50);
	}
}

int ft_atoi(const char *str)
{
	int operator;
	int i;
	int number;

	i = 0;
	number = 0;
	operator= 1;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			operator= - 1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		number *= 10;
		number += (str[i] - 48);
		i++;
	}
	return (number * operator);
}