/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_the_server_for_the_client.c                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fporciel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/28 11:56:18 by fporciel          #+#    #+#             */
/*   Updated: 2023/08/28 12:02:45 by fporciel         ###   ########.fr       */
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

static void	ft_bzero(void *s, size_t n)
{
	while (n > 0)
	{
		*((unsigned char *)s + (n - 1)) = 0;
		n--;
	}
}

static size_t	ft_strlen(const char *s)
{
	size_t	len;

	if (s == NULL)
		return (0);
	len = 0;
	while (s[len])
		len++;
	return (len);
}

static void	ft_writenbr(char c, int fd)
{
	write(fd, &c, 1);
}

static void	ft_putnbr_fd(int n, int fd)
{
	long int	i;

	i = n;
	if (i < 0)
	{
		i = -i;
		write(fd, "-", 1);
	}
	if (i > 9)
	{
		ft_putnbr_fd((i / 10), fd);
		ft_writenbr(((i % 10) + 48), fd);
	}
	else
		ft_writenbr((i + 48), fd);
}

static void	ft_putstr_fd(char *s, int fd)
{
	size_t	strlen;

	if (s == NULL)
		return ;
	strlen = ft_strlen(s);
	write(fd, s, strlen);
}

static int	mt_kill(int signum, int pid)
{
	if (kill(pid, signum) == -1)
		return (exit(EXIT_FAILURE), 0);
	return (0);
}

static void	mt_addc(char *message, char *message_swap, int *counter, char c)
{
	if (*counter == 20000)
	{
		if (message[0] != 0)
			write(1, message, ft_strlen(message));
		else
			write(1, message_swap, ft_strlen(message_swap));
		*counter = 0;
	}
	if (message[*counter] == 0)
	{
		message[*counter] = c;
		message_swap[*counter] = 0;
	}
	else
	{
		message_swap[*counter] = c;
		message[*counter] = 0;
	}
}

static void mt_set_message(char character, int *end_flag)
{
	static char	message[20001];
	static char	message_swap[20001];
	static int	counter = 0;

	if (character == 0)
	{
		if (message[0] != 0)
			write(1, message, ft_strlen(message));
		else
			write(1, message_swap, ft_strlen(message_swap));
		ft_bzero(message, ft_strlen(message));
		ft_bzero(message_swap, ft_strlen(message_swap));
		(*end_flag)++;
		counter = 0;
	}
	else
	{
		mt_addc(message, message_swap, &counter, character);
		counter++;
	}
}

static void	mt_server_handler(int signum, siginfo_t *info, void *context)
{
	static char	character = 0;
	static int	bitindex = 7;
	static int	end_flag = 0;

	(void)context;
	if (signum == 1)
		character |= (1 << bitindex);
	bitindex--;
	if (bitindex < 0)
	{
		bitindex = 7;
		mt_set_message(character, &end_flag);
		character = 0;
	}
	if (end_flag == 0)
		mt_kill(SIGUSR1, info->si_pid);
	else
	{
		mt_kill(SIGUSR2, info->si_pid);
		end_flag = 0;
	}
}

int	main(void)
{
	struct sigaction	sig_data;
	int					pid;

	pid = getpid();
	sig_data.sa_handler = 0;
	sig_data.sa_flags = SA_SIGINFO;
	if (sigemptyset(&sig_data.sa_mask) == -1)
		return (exit(EXIT_FAILURE), 0);
	sig_data.sa_sigaction = mt_server_handler;
	if ((sigaction(SIGUSR1, &sig_data, NULL) == -1)
		|| (sigaction(SIGUSR2, &sig_data, NULL) == -1))
		return (exit(EXIT_FAILURE), 0);
	ft_putstr_fd("\nSERVER PID: ", 1);
	ft_putnbr_fd(pid, 1);
	write(1, "\n", 1);
	while (1)
		pause();
	return (0);
}
