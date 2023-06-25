/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbenaiss <zbenaiss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 01:16:37 by zbenaiss          #+#    #+#             */
/*   Updated: 2023/06/25 01:50:12 by zbenaiss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_isdying(t_philos *philo)
{
	int	time;

	pthread_mutex_lock(&(philo->data->last_meal_lock));
	time = gettime() - philo->last_meal_time;
	pthread_mutex_unlock(&(philo->data->last_meal_lock));
	if (time >= philo->die_time)
	{
		pthread_mutex_lock(&(philo->data->dead_lock));
		philo->data->dead = 1;
		pthread_mutex_lock(&(philo->data->print_lock));
		printf("%lli %i died\n", gettime() - philo->start_time, philo->id);
		return (1);
	}
	return (0);
}

int	ft_food(t_data *data)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (i < data->philo_num)
	{
		pthread_mutex_lock(&data->meal_lock);
		if (data->philos[i].meals_eaten >= data->eat_rounds)
			count++;
		pthread_mutex_unlock(&data->meal_lock);
		i++;
	}
	if (count == data->philo_num)
	{
		pthread_mutex_lock(&data->dead_lock);
		data->dead = 1;
		return (1);
	}
	return (0);
}

int	philo_status(t_data *data)
{
	int	i;

	i = 0;
	while (1)
	{
		if (ft_isdying(&data->philos[i]) == 1)
			break ;
		if (data->infinity == 0 && data->eat_rounds != -1)
		{
			if (ft_food(data) == 1)
				break ;
		}
		i++;
		if (i == data->philo_num - 1)
			i = 0;
	}
	return (0);
}

int	ft_onecase(t_philos *philo)
{
	if (philo->data->philo_num == 1)
	{
		printf("1 1 is thinking\n");
		printf("1 1 has taken a fork\n");
		ft_sleep(philo->die_time);
		printf("%i 1 died\n", philo->die_time);
		return (1);
	}
	return (0);
}
