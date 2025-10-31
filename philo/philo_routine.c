/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdiez-cu <vdiez-cu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 17:46:28 by victor            #+#    #+#             */
/*   Updated: 2025/10/31 13:27:56 by vdiez-cu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_check_philos(t_table *table, t_philo *philos,
				t_parameters *params, t_check_vars *vars)
{
	vars->i = -1;
	vars->full_philos = 0;
	while (++vars->i < params->number_of_philos)
	{
		pthread_mutex_lock(&philos[vars->i].last_eat_mutex);
		vars->now = ft_time_now_ms();
		vars->last_eat = philos[vars->i].last_eat_ms;
		vars->trying = philos[vars->i].trying_to_eat;
		pthread_mutex_unlock(&philos[vars->i].last_eat_mutex);
		if (check_and_handle_dep(table, philos, params, vars) == -1)
			return (-1);
		if (params->count_must_eat != -1)
		{
			pthread_mutex_lock(&philos[vars->i].last_eat_mutex);
			if (philos[vars->i].eat_count >= params->count_must_eat)
				vars->full_philos++;
			pthread_mutex_unlock(&philos[vars->i].last_eat_mutex);
		}
	}
	return (vars->full_philos);
}

void	ft_actions(t_philo *philo, pthread_mutex_t *first_fork,
		pthread_mutex_t *second_fork)
{
	pthread_mutex_lock(&philo->last_eat_mutex);
	philo->trying_to_eat = 0;
	philo->last_eat_ms = ft_time_now_ms();
	philo->eat_count++;
	pthread_mutex_unlock(&philo->last_eat_mutex);
	ft_print_action(philo->table, philo->id_philo, "has taken a fork");
	ft_print_action(philo->table, philo->id_philo, "is eating");
	usleep((philo->parameters->time_to_eat) * 1000);
	pthread_mutex_lock(&philo->last_eat_mutex);
	pthread_mutex_unlock(&philo->last_eat_mutex);
	pthread_mutex_unlock(second_fork);
	pthread_mutex_unlock(first_fork);
	ft_print_action(philo->table, philo->id_philo, "is sleeping");
	usleep((philo->parameters->time_to_sleep) * 1000);
	ft_print_action(philo->table, philo->id_philo, "is thinking");
}

int	ft_philo_loop(t_philo *philo)
{
	pthread_mutex_t	*first_fork;
	pthread_mutex_t	*second_fork;

	first_fork = philo->left_fork;
	second_fork = philo->right_fork;
	ft_swap(&first_fork, &second_fork);
	pthread_mutex_lock(first_fork);
	pthread_mutex_lock(&philo->last_eat_mutex);
	philo->trying_to_eat = 1;
	pthread_mutex_unlock(&philo->last_eat_mutex);
	pthread_mutex_lock(&philo->table->dep_mutex);
	if (philo->table->someone_died)
	{
		pthread_mutex_unlock(&philo->table->dep_mutex);
		pthread_mutex_lock(&philo->last_eat_mutex);
		philo->trying_to_eat = 0;
		pthread_mutex_unlock(&philo->last_eat_mutex);
		pthread_mutex_unlock(first_fork);
		return (1);
	}
	pthread_mutex_unlock(&philo->table->dep_mutex);
	pthread_mutex_lock(second_fork);
	ft_actions(philo, first_fork, second_fork);
	return (0);
}

void	*ft_philo_routine(void *philosophers)
{
	t_philo	*philo;

	philo = (t_philo *)philosophers;
	if (philo->parameters->number_of_philos == 1)
	{
		pthread_mutex_lock(philo->left_fork);
		ft_print_action(philo->table, philo->id_philo, "has taken a fork");
		usleep(philo->parameters->time_to_die * 1000);
		return (pthread_mutex_unlock(philo->left_fork), NULL);
	}
	if (philo->id_philo % 2 == 0)
		usleep((philo->parameters->time_to_eat * 1000) / 2);
	while (1)
	{
		pthread_mutex_lock(&philo->table->dep_mutex);
		if (philo->table->someone_died)
		{
			pthread_mutex_unlock(&philo->table->dep_mutex);
			break ;
		}
		pthread_mutex_unlock(&philo->table->dep_mutex);
		if (ft_philo_loop(philo))
			break ;
	}
	return (NULL);
}
