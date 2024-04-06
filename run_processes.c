/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_processes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpihur <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 17:00:08 by mpihur            #+#    #+#             */
/*   Updated: 2024/04/06 17:00:26 by mpihur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include "libft.h"

static void    free_split(char **split, int i)
{
    while (split[i])
    {
        free(split[i]);
        i++;
    }
    free(split);
    write(2, "Split\n", 6);
}

void    free_all(t_pipex **data_ptr)
{
    if ((*data_ptr)->cmd1 != NULL)
        free_split((*data_ptr)->cmd1, 0);
    if ((*data_ptr)->cmd2 != NULL)
        free_split((*data_ptr)->cmd2, 0);
    if ((*data_ptr)->multipath != NULL)
        free_split((*data_ptr)->multipath, 0);
    if ((*data_ptr)->path1)
        free((*data_ptr)->path1);
    if ((*data_ptr)->path2)
        free((*data_ptr)->path2);
    close((*data_ptr)->fd[0]);
    close((*data_ptr)->fd[1]);
}

char	*find_cmd_path(t_pipex **data_ptr, char *cmd, int i)
{
	char	*temp;
	char	*the_path;

    write(2, "In find_cmd_path\n", ft_strlen("In find_cmd_path\n"));
	while ((*data_ptr)->multipath[i])
	{
		temp = ft_strjoin((*data_ptr)->multipath[i], "/");
		the_path = ft_strjoin(temp, cmd);
		free(temp);
		if (access(the_path, F_OK | X_OK) == 0)
        {
            write(2, "cmd found\n", ft_strlen("cmd found\n"));
			return (the_path);
        }
		free(the_path);
		i++;
	}
    write(2, "cmd not found\n", ft_strlen("cmd not found\n"));
    return (NULL);
}

/*diff: close(fd[0]), dup2 directions*/
void    run_first_cmd(char **cmd, t_pipex **data_ptr, t_pipex *data, char **path)
{
    close(data->fd[0]);
    if (dup2(data->infile, STDIN_FILENO) < 0 || dup2(data->fd[1], STDOUT_FILENO) < 0)
        exit(err_free_all(data_ptr, "Dup2 issue\n", DUP2_ERR));
    *path = find_cmd_path(data_ptr, cmd[0], 0);
    if (*path == NULL)
        exit(err_free_all(data_ptr, "Cmd1 not found\n", EXEC_ERR));
    write(2, "R_1st_Exec\n", 11);
    write(2, *path, ft_strlen(*path));
    write(2, "\n", 1);
    if (execve(*path, cmd, data->envp) < 0)
        exit(err_free_all(data_ptr, "Execve issue\n", EXEC_ERR));
}

/*diff: open outfile, dup2 redirect to outfile*/
void    run_last_cmd(char **cmd, t_pipex **data_ptr, t_pipex *data, char **path)
{
    close(data->fd[1]);
    data->outfile = open(data->filename, O_RDWR | O_CREAT | O_TRUNC, 0777);
    if (data->outfile < 0)
        exit(err_free_all(data_ptr, "Outfile: OPEN ISSUE\n", FILE_NO_OPEN));
    if (dup2(data->fd[0], STDIN_FILENO) < 0 || dup2(data->outfile, STDOUT_FILENO) < 0)
        exit(err_free_all(data_ptr, "DUP2 error\n", DUP2_ERR));
    *path = find_cmd_path(data_ptr, cmd[0], 0);
    if (*path == NULL)
        exit(err_free_all(data_ptr, "Cmd2 not found\n", EXEC_ERR));
    write(2, "R_Exec\n", 7);
    write(2, *path, ft_strlen(*path));
    if (execve(*path, cmd, data->envp) < 0)
        exit(err_free_all(data_ptr, "EXECVE error\n", EXEC_ERR));
}

void    run_processes(t_pipex **data_ptr, t_pipex *data)
{
    data->pid = fork();
    if (data->pid == 0)
        run_first_cmd(data->cmd1, data_ptr, data, &(data->path1));
    else
    {
        waitpid(data->pid, NULL, 0);
        run_last_cmd(data->cmd2, data_ptr, data, &(data->path2));
    }
    close(data->fd[0]);
    close(data->fd[1]);
    free_all(data_ptr);
}
