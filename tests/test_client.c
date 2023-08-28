/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_client.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fporciel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/28 12:47:30 by fporciel          #+#    #+#             */
/*   Updated: 2023/08/28 13:49:10 by fporciel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stddef.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

static const char	*message;

int	ft_isdigit(int c)
{
	return ((c >= 48) && (c <= 57));
}

static int	ft_power(int base, int exp)
{
	int	power;
	int	current_power;

	if ((base == 0) && (exp != 0))
		return (0);
	else if (exp == 0)
		return (1);
	else
	{
		power = 1;
		current_power = base;
		while (exp)
		{
			if (exp & 1)
				power *= current_power;
			current_power *= current_power;
			exp >>= 1;
		}
	}
	return (power);
}

static int	ft_chkatoi(char *nptr)
{
	int	i;

	i = 0;
	while (!ft_isdigit((char)nptr[i]) && (nptr[i] != 0))
	{
		if (!(((char)nptr[i] == 43) || ((char)nptr[i] == 45))
			&& !(((char)nptr[i] == 32)
				|| (((char)nptr[i] >= 9) && ((char)nptr[i] <= 13))))
			return (0);
		if (((char)nptr[i] == 45) && (ft_isdigit((char)nptr[i + 1])))
			return (-1);
		if ((((char)nptr[i] == 43) || ((char)nptr[i] == 45))
			&& (!ft_isdigit((char)nptr[i + 1])))
			return (0);
		i++;
	}
	return (1);
}

int	ft_atoi(const char *nptr)
{
	int	i;
	int	j;
	int	num;
	int	exp;

	i = 0;
	num = 0;
	j = 0;
	while (!ft_isdigit((char)nptr[i]) && (nptr[i] != 0))
		i++;
	j = i;
	while (ft_isdigit((char)nptr[i]))
		i++;
	exp = ((i - j) - 1);
	while (j < i)
	{
		num = num + (((char)nptr[j] - 48) * ft_power(10, exp));
		exp--;
		j++;
	}
	return (num * ft_chkatoi((char *)nptr));
}

static int	mt_kill(int signum, int pid)
{
	if (kill(pid, signum) == -1)
		return (exit(EXIT_FAILURE), 0);
	return (0);
}

static int	mt_isdigital(char *str)
{
	if (str)
	{
		while (*str)
		{
			if (!ft_isdigit(*str))
				return (0);
			str++;
		}
		return (1);
	}
	return (0);
}

static void	mt_client_handler(int signum, siginfo_t *info, void *context)
{
	static int	bitindex = 6;
	static int	counter = 0;

	(void)context;
	if (signum == SIGUSR1)
	{
		if (bitindex < 0)
		{
			counter++;
			bitindex = 7;
		}
		if ((message[counter] >> bitindex) & 1)
		{
			bitindex--;
			mt_kill(SIGUSR2, info->si_pid);
		}
		else
		{
			bitindex--;
			mt_kill(SIGUSR1, info->si_pid);
		}
	}
	else
		exit(EXIT_SUCCESS);
}

static void	mt_send_first_bit(int pid)
{
	if ((message[0] >> 7) & 1)
		mt_kill(SIGUSR2, pid);
	else
		mt_kill(SIGUSR1, pid);
}

int	main(int argc, char **argv)
{
	struct sigaction	sig_data;

	if ((argc != 3) || !mt_isdigital(argv[1]))
		return (exit(EXIT_FAILURE), 0);
	message = argv[2];
	sig_data.sa_handler = 0;
	sig_data.sa_flags = SA_SIGINFO;
	if (sigemptyset(&sig_data.sa_mask) == -1)
		return (exit(EXIT_FAILURE), 0);
	sig_data.sa_sigaction = mt_client_handler;
	if ((sigaction(SIGUSR1, &sig_data, NULL) == -1)
			|| (sigaction(SIGUSR2, &sig_data, NULL) == -1))
		return (exit(EXIT_FAILURE), 0);
	mt_send_first_bit(ft_atoi(argv[1]));
	while (1)
		pause();
	return (0);
}
