/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_pipex.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpihur <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 14:02:35 by mpihur            #+#    #+#             */
/*   Updated: 2024/04/10 14:03:39 by mpihur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

char	*get_paths(char **envp)
{
	while (*envp)
	{
		if (ft_strncmp(*envp, "PATH", 4) == 0)
			return (*envp + 5);
		envp++;
	}
	return (NULL);
}

t_pipex	*init_pipex(t_pipex *data, char **argv, char **envp)
{
	data = (t_pipex *)malloc(sizeof(t_pipex));
	if (data == NULL)
		exit(error_no_free("Malloc error: init_pipex\n", ALL_ERR));
	data->cmd1 = NULL;
	data->cmd2 = NULL;
	data->multipath = NULL;
	data->infile = open(argv[1], O_RDONLY);
	if (data->infile == -1)
		exit(error_ppx_free(&data, "Infile: OPEN ERROR\n", FILE_NO_OPEN));
	if (pipe(data->fd) < 0)
		exit(error_ppx_free(&data, "PIPE NOT OPENED\n", PIPE_ERR));
	data->cmd1 = ft_split(argv[2], ' ');
	data->cmd2 = ft_split(argv[3], ' ');
	data->envp = envp;
	data->multipath = ft_split(get_paths(envp), ':');
	if (data->multipath == NULL || data->cmd1 == NULL || data->cmd2 == NULL)
		exit(err_free_all(&data, "INVALID CMD1/CMD2 provided\n", NO_CMD));
	data->filename = argv[4];
	data->path1 = find_cmd_path(&data, data->cmd1[0], 0);
	if (data->path1 == NULL)
		exit(err_free_all(&data, "Cmd1 not found\n", EXEC_ERR));
	data->path2 = find_cmd_path(&data, data->cmd2[0], 0);
	if (data->path2 == NULL)
		exit(err_free_all(&data, "Cmd2 not found\n", EXEC_ERR));
	return (data);
}
