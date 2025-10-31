/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdiez-cu <vdiez-cu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 11:13:05 by vdiez-cu          #+#    #+#             */
/*   Updated: 2025/10/31 13:26:40 by vdiez-cu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_swap(pthread_mutex_t **a, pthread_mutex_t **b)
{
	uintptr_t		pa;
	uintptr_t		pb;
	pthread_mutex_t	*tmp;

	pa = (uintptr_t)(*a);
	pb = (uintptr_t)(*b);
	if (pa > pb)
	{
		tmp = *a;
		*a = *b;
		*b = tmp;
	}
}

void	ft_print_action(t_table *table, int id_philo, const char *action)
{
	long long	time;

	pthread_mutex_lock(&table->dep_mutex);
	if (table->someone_died)
	{
		pthread_mutex_unlock(&table->dep_mutex);
		return ;
	}
	pthread_mutex_lock(&table->print_mutex);
	time = ft_time_now_ms() - table->start_time_ms;
	printf("%lld %d %s\n", time, id_philo, action);
	pthread_mutex_unlock(&table->print_mutex);
	pthread_mutex_unlock(&table->dep_mutex);
}

long long	ft_time_now_ms(void)
{
	struct timeval	actual_time;
	long long		milliseconds;

	gettimeofday(&actual_time, NULL);
	milliseconds = actual_time.tv_sec * 1000 + actual_time.tv_usec / 1000;
	return (milliseconds);
}

int	ft_atoi(const char *str)
{
	int	i;
	int	sign;
	int	result;

	i = 0;
	result = 0;
	sign = 1;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign *= -1;
		i++;
	}
	while ((str[i] >= '0') && (str[i] <= '9'))
	{
		result = result * 10 + (str[i]) - '0';
		i++;
	}
	return (result * sign);
}

int	ft_is_positive_digits(const char *s)
{
	int	i;

	if (!s || s[0] == '\0')
		return (0);
	i = 0;
	if (s[i] == '+')
		i++;
	if (s[i] == '\0')
		return (0);
	while (s[i])
	{
		if (!(s[i] >= '0' && s[i] <= '9'))
			return (0);
		i++;
	}
	if (ft_atoi(s) <= 0)
		return (0);
	return (1);
}
