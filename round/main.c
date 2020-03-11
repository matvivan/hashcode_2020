/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matvivan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/20 11:46:31 by matvivan          #+#    #+#             */
/*   Updated: 2020/02/20 13:29:39 by matvivan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
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

void print_line(int fd, int *ARRAY, int LIBR)
{
	int i;

	i = 0;
	while (i < LIBR)
	{
		if (ARRAY[i]) /* if element not punched */
		{
			put_num(fd, i); /* print index of element */
			write(fd, " ", 1);
			put_num(fd, i);
			write(fd, " ", (i < LIBR - 1));
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
	print_line(fd_out, vector, K);
}

int *get_scores(int fd_in, int BOOKS)
{
	int i;
	int *score;

	score = (int *)malloc(BOOKS * sizeof(int));
	i = 0;
	while (i < BOOKS)
		score[i++] = read_num(fd_in);
	return (score);
}

int get_pbd(int fd_in, int amount, int max_bpd)
{
	int i;
	int IDs[amount];
	int SIGN;
	int BPD;

	SIGN = read_num(fd_in);
	BPD  = read_num(fd_in);
	i = 0;
	while (i < amount)
		IDs[i++] = read_num(fd_in);
	if (BPD < max_bpd)
		return (BPD);
	else
		return (0);
}

void get_library(int fd_in, int LIBR, int DAYS)
{
	int i;
	int list_libr[LIBR];

	i = 0;
	while (i < LIBR)
		list_libr[i++] = get_bpd(fd_in, read_num(fd_in), DAYS);
}

int main(int ac, char **av)
{
	int fd_in;
	int fd_out;
	int BOOKS;
	int LIBR;
	int DAYS;

	if (ac > 1)
	{
		fd_in = open(av[1], O_RDONLY);
		if (!read(fd_in, NULL, 0))
		{
			BOOKS = read_num(fd_in);
			LIBR = read_num(fd_in);
			DAYS = read_num(fd_in);
			if ((fd_out = open(out_name(av[1]), O_RDWR | O_CREAT, 0644)))
			{
				get_scores(fd_in, BOOKS);
				get_library(fd_in, LIBR, DAYS);
				print_line(fd_out, 
//				order_signup(BOOKS, LIBR, DAYS);
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
