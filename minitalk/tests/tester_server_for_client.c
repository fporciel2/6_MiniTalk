/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tester_server_for_client.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fporciel <fporciel@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/27 19:02:48 by fporciel          #+#    #+#             */
/*   Updated: 2023/08/27 19:28:47 by fporciel         ###   ########.fr       */
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

void	ft_putnbr_fd(int n, int fd)
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

void	ft_putstr_fd(char *s, int fd)
{
	size_t	strlen;

	if (s == NULL)
		return ;
	strlen = ft_strlen(s);
	write(fd, s, strlen);
}

static int	mt_kill(int signum, int pid, char **message)
{
	(void)message;
	if ((signum == SIGUSR2) && (message != NULL))
	{
		free(*message);
		*message = NULL;
		kill(pid, signum);
		return (exit(EXIT_FAILURE));
	}
	if (kill(pid, signum) == -1)
	{
		if (message != NULL)
		{
			free(*message);
			*message = NULL;
		}
		return (exit(EXIT_FAILURE));
	}
}

static void	mt_put_message(char **message, int *end_flag)
{
	*end_flag = 1;
	ft_putstr_fd(*message, 1);
	write(1, "\n", 1);
	free(*message);
	*message = NULL;
}

static char	*mt_addchar(char **message, char character, int client_pid)
{
	char	*new_message;
	int		messagelen;
	int		count;

	(void)client_pid;
	messagelen = (int)ft_strlen(*message);
	count = 0;
	new_message = malloc((2 + messagelen) * sizeof(*new_message));
	if (new_message == NULL)
	{
		mt_kill(SIGUSR2, client_pid, message);
		return (NULL);
	}
	if (*message)
	{
		while (count < messagelen)
		{
			new_message[count] = *message[count];
			count++;
		}
		free(*message);
	}
	new_message[count] = character;
	new_message[count + 1] = 0;
	return (new_message);
}

static void	mt_server_handler(int signum, siginfo_t *info, void *context)
{
	static char	character = 0;
	static char	*message = NULL;
	static int	bitIndex = 7;
	static char	end_flag = 0;

	if (signum == 1)
		character |= (1 << bitIndex);
	bitIndex--;
	if (bitIndex < 0)
	{
		message = mt_addchar(&message, character, 0);
		bitIndex = 7;
		if (character == 0)
			mt_putmessage(&message, &end_flag);
		else
			character = 0;
	}
	if (end_flag == 0)
		mt_kill(SIGUSR1, info->si_pid, &message);
	else
	{
		mt_kill(SIGUSR2, info->si_pid, NULL);
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
		return (exit(EXIT_FAILURE));
	sig_data.sa_sigaction = mt_server_handler;
	if ((sigaction(SIGUSR1, &sig_data, NULL) == -1)
			|| (sigaction(SIGUSR2, &sig_data, NULL) == -1))
		return (exit(EXIT_FAILURE));
	ft_putstr_fd("\nSERVER PID: ", 1);
	ft_putnbr_fd(pid, 1);
	write(1, "\n", 1);
	while (1)
		pause();
	return (0);
}
