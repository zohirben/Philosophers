/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbenaiss <zbenaiss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 01:11:25 by zbenaiss          #+#    #+#             */
/*   Updated: 2023/06/25 01:49:46 by zbenaiss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_thinking(t_philos *philo)
{
	pthread_mutex_lock(&(philo->data->dead_lock));
	if (philo->data->dead != 1)
	{
		pthread_mutex_unlock(&(philo->data->dead_lock));
		pthread_mutex_lock(&(philo->data->print_lock));
		printf("%lli %i is thinking\n", (gettime() - philo->start_time),
			philo->id);
		pthread_mutex_unlock(&(philo->data->print_lock));
		return (0);
	}
	else
	{
		pthread_mutex_unlock(&(philo->data->dead_lock));
		return (1);
	}
}

int	ft_eating(t_philos *philo)
{
	pthread_mutex_lock(&(philo->data->dead_lock));
	if (philo->data->dead != 1)
	{
		pthread_mutex_unlock(&(philo->data->dead_lock));
		pthread_mutex_lock(&(philo->data->print_lock));
		printf("%lli %i is eating\n", gettime() - philo->start_time, philo->id);
		pthread_mutex_unlock(&(philo->data->print_lock));
		pthread_mutex_lock(&(philo->data->last_meal_lock));
		philo->last_meal_time = gettime();
		pthread_mutex_unlock(&(philo->data->last_meal_lock));
		pthread_mutex_lock(&(philo->data->meal_lock));
		philo->meals_eaten++;
		pthread_mutex_unlock(&(philo->data->meal_lock));
		ft_sleep(philo->eat_time);
		return (0);
	}
	else
	{
		pthread_mutex_unlock(&(philo->data->dead_lock));
		return (1);
	}
}

int	ft_sleeping(t_philos *philo)
{
	pthread_mutex_lock(&(philo->data->dead_lock));
	if (philo->data->dead != 1)
	{
		pthread_mutex_unlock(&(philo->data->dead_lock));
		pthread_mutex_lock(&(philo->data->print_lock));
		printf("%lli %i is sleeping\n", gettime() - philo->start_time,
			philo->id);
		pthread_mutex_unlock(&(philo->data->print_lock));
		ft_sleep(philo->sleep_time);
		return (0);
	}
	else
	{
		pthread_mutex_unlock(&(philo->data->dead_lock));
		return (1);
	}
}

int	ft_forks(t_philos *philo)
{
	pthread_mutex_lock(&(philo->data->dead_lock));
	if (philo->data->dead != 1)
	{
		pthread_mutex_unlock(&(philo->data->dead_lock));
		pthread_mutex_lock(philo->l_fork);
		pthread_mutex_lock(&(philo->data->print_lock));
		printf("%lli %i has taken a fork\n", gettime() - philo->start_time,
			philo->id);
		pthread_mutex_unlock(&(philo->data->print_lock));
		pthread_mutex_lock(philo->r_fork);
		pthread_mutex_lock(&(philo->data->print_lock));
		printf("%lli %i has taken a fork\n", gettime() - philo->start_time,
			philo->id);
		pthread_mutex_unlock(&(philo->data->print_lock));
		return (0);
	}
	else
	{
		pthread_mutex_unlock(&(philo->data->dead_lock));
		return (1);
	}
}

int	ft_dropf(t_philos *philo)
{
	pthread_mutex_unlock(philo->l_fork);
	pthread_mutex_unlock(philo->r_fork);
	return (0);
}
