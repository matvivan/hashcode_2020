/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matvivan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/19 10:17:03 by matvivan          #+#    #+#             */
/*   Updated: 2020/02/20 09:11:51 by matvivan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <fcntl.h>

void put_num(int fd, int num)
{
	char c;

	if (num / 10)
		put_num(fd, num / 10);
	c = num % 10 + '0';
	write(fd, &c, 1);
}

int read_num(int fd)
{
	int num;
	char c;

	num = 0;
	while (read(fd, &c, 1) && c > ' ')
		num = num * 10 + (c - '0');
	return (num);
}

char *out_name(char *str)
{
	int i;

	i = 0;
	while (str[i])
		++i;
	if (i > 3)
	{
		str[--i] = 't'; 
		str[--i] = 'u'; 
		str[--i] = 'o';
		str[--i] = '.';
		return (str);
	}
	else
		return ("file.out");
}

void print_solution(int fd, int *vector, int K)
{
	int i;

	i = 0;
	put_num(fd, K);
	write(fd, "\n", 1);
	while (i < K)
	{
		if (vector[i]) /* if element not punched */
		{
			put_num(fd, i); /* print index of element */
			write(fd, " ", (i < K - 1));
		}
		++i;
	}
	write(fd, "\n", 1);
}

void punch_elements(int *vector, int target)
{
	int i;
	int sum;

	i = 0;
	sum = 0;
	while (sum < target)
		sum += vector[i++];
	if (sum == target) /* if finded minimal vector */
		while (--i)
			vector[i] = 0; /* punch whole vector */
	else if (i > 1)
	{
		vector[--i] = 0; /* punch greatest element */
		punch_elements(vector, target - sum); /* and try again */
	}
}

void find_solution(int fd_in, int fd_out, int MAX, int N)
{
	int K;
	int sum;
	int vector[N];

	K = 0;
	sum = 0;
	while (K < N && sum < MAX)
	{
		vector[K] = read_num(fd_in);
		sum += vector[K++];
	}
	if (sum > MAX)
		punch_elements(vector, sum - MAX);
	print_solution(fd_out, vector, K);
}

int main(int ac, char **av)
{
	int fd_in;
	int fd_out;
	int MAX;
	int N_types;

	if (ac > 1)
	{
		fd_in = open(av[1], O_RDONLY);
		if (!read(fd_in, NULL, 0))
		{
			MAX = read_num(fd_in);
			N_types = read_num(fd_in);
			if ((fd_out = open(out_name(av[1]), O_RDWR | O_CREAT, 0644)))
			{
				find_solution(fd_in, fd_out, MAX, N_types);
			}
			else
				return write(1, "error\tCannot create file.\n", 26);
		}
		else
			return write(1, "error\tWrong input file.\n", 24);
	}
	else
		return write(1, "usage: ./a.out <filename>\n", 26);
	return (0);
}
