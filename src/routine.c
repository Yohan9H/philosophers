/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohurteb <yohurteb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 10:48:25 by yohurteb          #+#    #+#             */
/*   Updated: 2024/07/28 15:26:25 by yohurteb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	take_fork(t_philo *philo)
{
	if (philo->num % 2 == 0)
	{
		pthread_mutex_lock(philo->l_fork);
		print_status(philo, "has taken a fork", give_time(philo->data));
		pthread_mutex_lock(philo->r_fork);
		print_status(philo, "has taken a fork", give_time(philo->data));
	}
	else
	{
		pthread_mutex_lock(philo->r_fork);
		print_status(philo, "has taken a fork", give_time(philo->data));
		pthread_mutex_lock(philo->l_fork);
		print_status(philo, "has taken a fork", give_time(philo->data));
	}
}

void	ft_eat(t_philo *philo)
{
	take_fork(philo);
	print_status(philo, "is eating", give_time(philo->data));
	pthread_mutex_lock(&philo->data->eat_lock);
	philo->last_eat = give_time(philo->data);
	pthread_mutex_unlock(&philo->data->eat_lock);
	ft_usleep(philo->time_to_eat, philo->data);
	pthread_mutex_lock(&philo->data->eat_lock);
	philo->eating++;
	pthread_mutex_unlock(&philo->data->eat_lock);
	pthread_mutex_unlock(philo->l_fork);
	pthread_mutex_unlock(philo->r_fork);
}

void	ft_sleep(t_philo *philo)
{
	print_status(philo, "is sleeping", give_time(philo->data));
	ft_usleep(philo->time_to_sleep, philo->data);
}

void	ft_think(t_philo *philo)
{
	print_status(philo, "is thinking", give_time(philo->data));
	ft_usleep(philo->data->set_t_die - philo->data->set_t_eat
		- philo->data->set_t_sleep - 10, philo->data);
}

void	*routine(void *philo_void)
{
	t_philo	*philo;

	philo = (t_philo *)philo_void;
	if (one_philo(philo) == -1)
		return (NULL);
	while (if_dead(philo) == 0 && if_eat_finish(philo) == 0)
	{
		ft_eat(philo);
		if (check_break(philo) == -1)
			break ;
		ft_sleep(philo);
		if (check_break(philo) == -1)
			break ;
		ft_think(philo);
		if (check_break(philo) == -1)
			break ;
	}
	return (NULL);
}
