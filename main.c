/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*													+:+ +:+		 +:+	 */
/*   By: mpihur <marvin@42.fr>					  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2024/04/06 16:59:46 by mpihur			#+#	#+#			 */
/*   Updated: 2024/04/10 14:03:19 by mpihur           ###   ########.fr       */
/*																			*/
/* ************************************************************************** */

#include "pipex.h"
#include "libft.h"

int	error_no_free(char *str, int err)
{
	write(2, str, ft_strlen(str));
	perror(NULL);
	return (err);
}

int	error_ppx_free(t_pipex **data_ptr, char *str, int err)
{
	free(*data_ptr);
	return (error_no_free(str, err));
}

int	err_free_all(t_pipex **data_ptr, char *str, int err)
{
	free_all(data_ptr);
	return (error_ppx_free(data_ptr, str, err));
}

/*
1. Basic error check (argc num, infile access (F_OK, R_OK))
2. Initializing the t_pipex (open files, pipe())
3. Creating a child process. 
4. Child: close read side fd[0], check the cmd1
	, dup2 (infile = STDIN, STDOUT = fd[1]), send to execve.
5. Parent: close fd[1], check the cmd2, dup2 (), send to execve.
6. Free t_pipex data. 
*/

int	main(int argc, char **argv, char **envp)
{
	t_pipex	*data;

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
