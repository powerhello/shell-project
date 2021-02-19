/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_pipes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctycho <ctycho@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/16 13:05:16 by ctycho            #+#    #+#             */
/*   Updated: 2021/02/19 17:41:37 by ctycho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int		mini_bin1(t_mini *s, int i)
{
	char		**bin = NULL;
	char		*line = NULL;
	int			m = 0;
	DIR				*folder;
	struct dirent	*command;
	int				flag;

	flag = 0;
	// s->tmp = NULL;
	while (ft_strncmp(s->env[m], "PATH=", 5) != 0)
		m++;
	line = ft_substr(s->env[m], 5, ft_strlen(s->env[m]));
	bin = ft_split(line, ':');
	ft_memdel_1d(line);
	if (s->mass3d[i][0][0] == '/')
		s->var.bin = ft_strdup(s->mass3d[i][0]);
	else
	{
		m = 0;
		while (bin[m] && flag == 0)
		{
			folder = opendir(bin[m]);
			while ((command = readdir(folder)))
			{
				if (ft_strcmp(command->d_name, s->mass3d[i][0]) == 0)
				{
					flag = 1;
					line = ft_strjoin(bin[m], "/");
					s->var.bin = ft_strjoin(line, s->mass3d[i][0]);
					ft_memdel_1d(line);
				}
			}
			closedir(folder);
			m++;
		}
	}
	ft_memdel_2d((void**)bin);
	if (folder == NULL)
	{
		write(1, "error\n", 6);
		exit (127);
	}
	return (flag);
}

int					mini_pipes(t_mini *s)
{
	int		tmpin = 0;
	int		tmpout = 0;
	int		fdin = 0;
	int		i;
	int				status;
	int		res = 0;

	tmpin = dup(0);
	tmpout = dup(1);
	// if (infile)
	// 	fdin = open(infile, O_RDONLY);
	// else
		fdin = dup(0);
	int	ret;
	int	fdout;
	for(i = 0; i < s->pipe.count_commands; i++)
	{
		res = 0;
		res = mini_bin1(s, i);
		dup2(fdin, 0);
		close(fdin);
		if (i == s->pipe.count_commands - 1)
		{
			// write(1, "#\n", 2);
			// if (outfile)
			// 	fdout = open();
			// else
			fdout = dup(tmpout);
		}
		else
		{
			int fdpipe[2];
			pipe(fdpipe);
			fdout = fdpipe[1];
			fdin = fdpipe[0];
		}
		dup2(fdout, 1);
		close(fdout);
		ret = fork();
		if (ret == 0)
		{
			execve(s->var.bin, s->mass3d[i], s->env);
			ft_error(s->mass3d[i][0], 1);
			exit (1);
		}
		if (res)
			ft_memdel_1d(s->var.bin);
	}
	dup2(tmpin, 0);
	dup2(tmpout, 1);
	close(tmpin);
	close(tmpout);
	if (waitpid(ret, &status, 0) > 0)
		return (status);
	return (0);
}
