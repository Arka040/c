#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <pwd.h>

enum
{
	MAXARG = 10,
	BUFSIZE = 80,
	EXEC = 1,
	PIPE = 2,
	RDR = 3,
	BCKGR = 4,
	LIST = 5,
	CND = 6,
};

typedef struct {
	int type;
}cmd;

typedef struct {
	int type;
	char **argv;
}execcmd;

typedef struct {
	int type;
	cmd *left;
	cmd *right;
}pipecmd;

typedef struct {
	int type;
	cmd *command;
	char *infile;
	char *outfile;
	char *apndfile;
}rdrcmd;

typedef struct {
	int type;
	cmd *left;
	cmd *right;
}bckgrcmd;

typedef struct {
	int type;
	cmd *left;
	cmd *right;
}listcmd;

typedef struct {
	int type;
	int mode;
	cmd *command;
	cmd *nextcommand;
}cndcmd;

void getstr(char *str);
int getnext(char **ps, char *endps, char **start, char **end);
int checknext(char **s, char *ends, char *symbols);
cmd *parserdr(char **s, char *ends, cmd *pipe);
cmd *parsexec(char **s, char *ends);
cmd *parspipe(char **s, char *ends);
cmd *parsecmd(char **s, char *ends);
cmd *parselist(char **s, char *ends);
cmd *parsecond(char **s, char *ends, cmd *node);
void runcmd(cmd *node);

int main(int argc, char **argv) {
    char *str = malloc(BUFSIZE);
    char *s;
    char *ends;
    cmd *node;
    pid_t p;
    int cdflag = 0;
    system("clear");
    while (1) {
        printf("$ ");
        getstr(str);
        if (strcmp(str, "exit") == 0) {
			break;
        }
        s = str;
        ends = s + strlen(s);
        node = parselist(&s, ends);
        if (strcmp(s, "") != 0) {
            printf("error\n");
            break;
        }
        if (str[0] = 'c' && str[1] == 'd' && str[2] == ' ') {
            if (chdir(str + 3) != 0) {
                printf("error\n");
            }
            continue;
        }
        if (!(p = fork())) {
            runcmd(node);
        }
        waitpid(p, 0, 0);
	}
    free(str);
    return 0;
}

void getstr(char *str)
{
    char c;
    int space = 1;
    while (((c = getchar()) != EOF)&&(c != '\n')) {
        if (c == ' ') {
            if (space == 0) {
                *str = c;
                str++;
                space = 1;
            }
        } else {
            space = 0;
            *str = c;                                               
            str++;
        }
    }
    *str = '\0';
}

int getnext(char **ps, char *endps, char **start, char **end)
{	
	char *s;
	s = *ps;
	while (*s == ' ')
		s++;	
	if (start)
		*start = s;
	int result = *s;	
	switch (*s) {
		case '(':
		case ')':
		case '<':
		case ';':
			s++;
			break;
		case '>':
			s++;
			if (*s == '>') {
				s++;
				result = 'a'; // >>
			}
			break;
		case '|':
			s++;
			if (*s == '|') {
				s++;
				result = '+'; // ||
			}
			break;
		case '&':
			s++;
			if (*s == '&') {
				s++;
				result = '*'; // &&
			}
			break;
		case '\0':
			result = 0;
			break;
		default:
			result = 'w'; // word
			while ((strchr("()&|><; ", *s) == 0) && (s < endps)){
				s++;
			}
			break;
	}
	if (end)
		*end = s;
	*ps = s;
	return result;
}

int checknext(char **s, char *ends, char *symbols)
{
	char *tmp;
	tmp = *s;
	int a = getnext(&tmp, ends, 0, 0);
	if (strchr(symbols, a) && (a != '\0'))
		return 1;
	else
		return 0;
}

cmd *parserdr(char **s, char *ends, cmd *pipe)
{
	char *start, *end;
	int next;
	char buf[BUFSIZE];
	rdrcmd *result = malloc(sizeof(rdrcmd));
	result->type = RDR;
	result->infile =  NULL;
	result->outfile = NULL;
	result->apndfile = NULL;
	result->command = (cmd *) pipe;
	do {
		next = getnext(s, ends, 0 ,0);
		getnext(s, ends, &start , &end);
		memcpy(buf, start, strlen(start) - strlen(end));
		buf[strlen(start) - strlen(end)] = '\0';
		if (next == '<') {
			result->infile = malloc(strlen(buf)+1);
			strcpy(result->infile, buf);
		} else if (next == '>') {
			result->outfile = malloc(strlen(buf)+1);
			strcpy(result->outfile, buf);
		} else if (next == 'a') {
			result->apndfile = malloc(strlen(buf)+1);
			strcpy(result->apndfile, buf);
		}
	} while (checknext(s, ends, "<>a"));
	
	return (cmd *) result;
}

cmd *parsexec(char **s, char *ends)
{
	char *start, *end;
	char **args = malloc(10 * sizeof(char *));
	int i = 0;
	for (i = 0; i < 10; i++)
		args[i] = malloc(BUFSIZE);
	
	i = 0;
	char buf[BUFSIZE];
	while (checknext(s, ends, "w")) {
		getnext(s, ends, &start, &end);
		memcpy(buf, start, strlen(start) - strlen(end));
		buf[strlen(start) - strlen(end)] = '\0';
		strcpy(args[i++], buf);
	}
	if (i == 0)
		exit(1);
	args[i] = 0;
	execcmd *result = malloc(sizeof(execcmd));
	result->type = EXEC;
	result->argv = args;
	
	return  (cmd *) result;
}

cmd *parspipe(char **s, char *ends)
{
	cmd *result;
	pipecmd *pipe;
	if (checknext(s, ends, "(")) {
		getnext(s, ends, 0, 0);
		result = parselist(s, ends);
		if (checknext(s, ends, ")"))
			getnext(s, ends, 0, 0);
		else {
			printf("error\n");
			exit(1);
		}
		
	} else
		result = parsexec(s, ends);

	if (checknext(s, ends, "|")) {
		getnext(s, ends, 0, 0);
		pipe = malloc(sizeof(pipecmd));
		pipe->type = PIPE;
		pipe->left = result;
		pipe->right = parspipe(s, ends);
		result = (cmd *) pipe;
	}
	
	return result;
}

cmd *parsecmd(char **s, char *ends)
{
	cmd *node = parspipe(s, ends);
	
	if (checknext(s, ends, "<>a"))
		node = parserdr(s, ends, node);
	
	if (checknext(s, ends, "+*"))
		node = parsecond(s, ends, node);

	return node;
}

cmd *parsecond(char **s, char *ends, cmd *node)
{
	cndcmd *result = malloc(sizeof(cndcmd));
	result->type = CND;
	result->mode = getnext(s, ends, 0, 0);
	result->command = node;
	result->nextcommand = parselist(s, ends);
	
	return (cmd *) result;
}
	
cmd *parselist(char **s, char *ends)
{
	cmd *result;
	result = parsecmd(s, ends);
	if (checknext(s, ends, ";")) {
		getnext(s, ends, 0, 0);
		listcmd *list;
		list = malloc(sizeof(listcmd));
		list->type = LIST;
		list->left = result;
		if (checknext(s, ends, "w")) {
			list->right = parselist(s, ends);
		} else
			list->right = NULL;
		result = (cmd *) list;
	}
	if (checknext(s, ends, "&")) {
		getnext(s, ends, 0, 0);
		bckgrcmd *list;
		list = malloc(sizeof(bckgrcmd));
		list->type = BCKGR;
		list->left = result;
		if (checknext(s, ends, "w")) {
			list->right = parselist(s, ends);
		} else
			list->right = NULL;
		result = (cmd *) list;
	}
	
	return result;	
}

void runcmd(cmd *node)
{
	int status = 0;
	pid_t p;
	switch (node->type) {
		case EXEC:
			execcmd *exec = (execcmd *) node;
			execvp(exec->argv[0], exec->argv);
			exit(1);
			break;
		case PIPE:
			pipecmd *pip = (pipecmd *) node;
			pid_t p1, p2;
			int fd[2];
			pipe(fd);
			if (!(p1 = fork())) {
				close(fd[0]);
				dup2(fd[1], 1);
				close(fd[1]);
				runcmd(pip->left);
			}
			if (!(p2 = fork())) {
				close(fd[1]);
				dup2(fd[0], 0);
				close(fd[0]);
				runcmd(pip->right);
			}
			close(fd[1]);
			close(fd[0]);
			waitpid(p1, 0, 0);
			waitpid(p2, &status, 0);
			if (WIFEXITED(status) != 0 && WEXITSTATUS(status) == 0)
				status = 0;
			else
				status = 1;
			break;
		case LIST:
			listcmd *list = (listcmd *) node;
			if (!(p = fork()))
				runcmd(list->left);
			waitpid(p, 0, 0);
			runcmd(list->right);
			break;
		case RDR:
			rdrcmd *rdr = (rdrcmd *) node;
			int f;
			if (rdr->infile) {
				f = open(rdr->infile, O_RDONLY);
				dup2(f, 0);
				close(f);
			}
			if (rdr->outfile) {
				f = open(rdr->outfile, O_CREAT | O_TRUNC | O_WRONLY, 0600);
				dup2(f, 1);
				close(f);
			}
			if (rdr->apndfile) {
				f = open(rdr->apndfile, O_CREAT | O_APPEND | O_WRONLY, 0600);
				dup2(f, 1);
				close(f);
			}
			runcmd(rdr->command);
			break;
		case BCKGR:
			bckgrcmd *bckgr = (bckgrcmd *) node;
			if (!fork()) {
				signal(SIGINT, SIG_IGN);
				int f = open("/dev/null", O_RDWR | O_TRUNC | O_CREAT, 0666);
				dup2(f, 0);
				dup2(f, 1);
				close(f);
				runcmd(bckgr->left);
			}
			if (bckgr->right)
				runcmd(bckgr->right);
			break;
		
		case CND:
			cndcmd *cond = (cndcmd *) node; 
			if (!(p = fork()))
				runcmd(cond->command);
			waitpid(p, &status, 0);
			
			int res = !(WIFEXITED(status) != 0 && WEXITSTATUS(status));
			if (cond->mode == '+')
				res = !res;
			if (res)
				runcmd(cond->nextcommand);
			break;
	}
	exit(status);
}
