/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dep_philos.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdiez-cu <vdiez-cu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 13:24:52 by vdiez-cu          #+#    #+#             */
/*   Updated: 2025/10/31 13:34:37 by vdiez-cu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_and_handle_dep(t_table *table, t_philo *philos,
				t_parameters *params, t_check_vars *vars)
{
	if ((!vars->trying && vars->now - vars->last_eat > params->time_to_die)
		|| (vars->trying && vars->now - vars->last_eat
			> params->time_to_die + params->time_to_eat))
	{
		pthread_mutex_lock(&table->dep_mutex);
		if (!table->someone_died)
		{
			table->someone_died = 1;
			pthread_mutex_lock(&table->print_mutex);
			printf("%lld %d died\n", ft_time_now_ms()
				- table->start_time_ms, philos[vars->i].id_philo);
			pthread_mutex_unlock(&table->print_mutex);
		}
		pthread_mutex_unlock(&table->dep_mutex);
		return (-1);
	}
	return (0);
}

void	*ft_check_dep(void *arg)
{
	t_philo			*philos;
	t_table			*table;
	t_parameters	*params;
	t_check_vars	vars;
	int				full_philos;

	philos = (t_philo *)arg;
	table = philos[0].table;
	params = philos[0].parameters;
	memset(&vars, 0, sizeof(vars));
	while (1)
	{
		full_philos = ft_check_philos(table, philos, params, &vars);
		if (full_philos == -1)
			return (NULL);
		if (params->count_must_eat != -1
			&& full_philos == params->number_of_philos)
		{
			pthread_mutex_lock(&table->dep_mutex);
			table->someone_died = 1;
			return (pthread_mutex_unlock(&table->dep_mutex), NULL);
		}
		usleep(1000);
	}
	return (NULL);
}
