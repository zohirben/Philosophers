/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbenaiss <zbenaiss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 00:20:32 by zbenaiss          #+#    #+#             */
/*   Updated: 2023/06/25 01:43:16 by zbenaiss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_data(int ac, char **av, t_data *data)
{
	data->philo_num = ft_atoi(av[1]);
	data->time_to_die = ft_atoi(av[2]);
	data->time_to_eat = ft_atoi(av[3]);
	data->time_to_sleep = ft_atoi(av[4]);
	data->dead = 0;
	if (ac == 6)
	{
		data->eat_rounds = ft_atoi(av[5]);
		data->infinity = 0;
		if (data->eat_rounds < 0)
			return (1);
	}
	else
	{
		data->eat_rounds = -1;
		data->infinity = 1;
	}
	if (data->philo_num <= 0 || data->time_to_die <= 0 || data->time_to_eat <= 0
		|| data->time_to_sleep <= 0 || data->philo_num > 200)
		return (1);
	return (0);
}

int	ft_routine_looper(t_philos *philo)
{
	if (ft_thinking(philo) == 1)
		return (1);
	if (ft_forks(philo) == 1)
		return (1);
	if (ft_eating(philo) == 1)
		return (1);
	if (ft_dropf(philo) == 1)
		return (1);
	if (ft_sleeping(philo) == 1)
		return (1);
	return (0);
}

void	*routine(void *philos)
{
	t_philos	*philo;

	philo = (t_philos *)philos;
	while (1)
	{
		pthread_mutex_lock(&(philo->data->dead_lock));
		if (philo->data->dead == 1)
		{
			pthread_mutex_unlock(&(philo->data->dead_lock));
			return (NULL);
		}
		else
		{
			pthread_mutex_unlock(&(philo->data->dead_lock));
			if (ft_routine_looper(philo) == 1)
				return (NULL);
		}
	}
	return (NULL);
}

int	ft_fpack(int ac, char **av, t_data *data)
{
	if (check_data(ac, av, data) == 1)
		return (1);
	if (init_data(data) == 1)
		return (1);
	if (init_philosophers(data) == 1)
		return (1);
	if (init_threads(data) == 1)
		return (1);
	return (0);
}

int	main(int ac, char **av)
{
	t_data	*data;

	if (arg_checker(ac, av) == 1)
		return (1);
	data = malloc(sizeof(*data));
	if (data == NULL)
		return (1);
	if (ft_fpack(ac, av, data) == 1)
	{
		free_data(data);
		return (1);
	}
	free_data(data);
	return (0);
}
