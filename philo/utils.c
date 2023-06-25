/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbenaiss <zbenaiss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 01:11:39 by zbenaiss          #+#    #+#             */
/*   Updated: 2023/06/25 01:51:50 by zbenaiss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long	gettime(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

void	ft_sleep(long long time)
{
	long long	start_point;

	start_point = gettime();
	while ((gettime() - start_point) < time)
	{
		usleep(50);
	}
}

int	ft_atoi(const char *str)
{
	int	operator;
	int	i;
	int	number;

	i = 0;
	number = 0;
	operator = 1;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			operator = -1;
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

void	free_data(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_num)
		pthread_mutex_destroy(&data->forks[i++]);
	pthread_mutex_destroy(&data->print_lock);
	pthread_mutex_destroy(&data->dead_lock);
	pthread_mutex_destroy(&data->meal_lock);
	pthread_mutex_destroy(&data->last_meal_lock);
	free(data->forks);
	free(data->philos);
	free(data);
}
