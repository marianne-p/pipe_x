#ifndef PIPEX_H
# define PIPEX_H

# include "./Libft/libft.h"
# include <fcntl.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <sys/types.h>

enum    e_pipex_err
{
    SCCS = 0,
    WRNG_ARGS = -1,
    INF_ERR = -2,
    FILE_NO_OPEN = -3,
    PIPE_ERR = -4,
    ALL_ERR = -5,
    DUP2_ERR = -6,
    NO_CMD = -7,
    EXEC_ERR = -8,
};

typedef struct s_pipex
{
    char    **cmd1;
    char    *path1;
    char    **cmd2;
    char    *path2;
    char    **envp;
    char    *filename;
    int     infile;
    int     outfile;
    int     fd[2];
    int     pid;
    char    **multipath;
}   t_pipex;

t_pipex *init_pipex(t_pipex *data, char **argv, char **envp);
void    run_processes(t_pipex **data_ptr, t_pipex *data);
int     error_no_free(char *str, int err);
int     error_ppx_free(t_pipex **data_ptr, char *str, int err);
void    free_all(t_pipex **data_ptr);
int     err_free_all(t_pipex **data_ptr, char *str, int err);

#endif
