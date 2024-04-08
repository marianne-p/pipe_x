/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpihur <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 16:59:46 by mpihur            #+#    #+#             */
/*   Updated: 2024/04/06 16:59:56 by mpihur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include "libft.h"

int     error_no_free(char *str, int err)
{
    write(2, str, ft_strlen(str));
    perror(NULL);
    return (err);
}

int     error_ppx_free(t_pipex **data_ptr, char *str, int err)
{
    free(*data_ptr);
    return (error_no_free(str, err));
}

int     err_free_all(t_pipex **data_ptr, char *str, int err)
{
    free_all(data_ptr);
    //free more in data
    return (error_ppx_free(data_ptr, str, err));
}

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

t_pipex *init_pipex(t_pipex *data, char **argv, char **envp)
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
    write(2, "Finished init\n", ft_strlen("Finished init\n"));
    return (data);
}

/*
1. Basic error check (argc num, infile access (F_OK, R_OK))
2. Initializing the t_pipex (open files, pipe())
3. Creating a child process. 
4. Child: close read side fd[0], check the cmd1, dup2 (infile = STDIN, STDOUT = fd[1]), 
         send to execve.
5. Parent: close fd[1], check the cmd2, dup2 (), send to execve.
6. Free t_pipex data. 
*/

int     main(int argc, char **argv, char **envp)
{
    t_pipex *data;

    data = NULL;
    if (argc == 5)
    {
        if (access(argv[1], F_OK) == -1 || access(argv[1], R_OK) == -1)
            exit(error_no_free("Infile: NO ACC/READ PERM\n", INF_ERR));
        data = init_pipex(data, argv, envp);
        run_processes(&data, data);
    }
    else
        exit(write(2, "NOT ENOUGH ARGS\n", 16));
    return (0);
}
