/*
 * AUTOCOMMENT-XV6: comentario agregado automaticamente para explicar el archivo.
 * Archivo: user/sh.c
 *
 * Explicacion clara y facil:
 *   - Este archivo implementa el shell de xv6.
 *   - Convierte texto en estructuras de comando y luego las ejecuta.
 *
 * Guia rapida de lectura:
 *   1) Las estructuras de comando modelan EXEC, REDIR, PIPE, LIST y BACK.
 *   2) Las funciones parse* construyen un arbol de ejecucion.
 *   3) runcmd interpreta ese arbol y lanza procesos con fork/exec.
 */

// Shell.

#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"

// Parsed command representation
#define EXEC  1
#define REDIR 2
#define PIPE  3
#define LIST  4
#define BACK  5

#define MAXARGS 10

struct cmd {
  int type;
};

struct execcmd {
  int type;
  char *argv[MAXARGS];
  char *eargv[MAXARGS];
};

struct redircmd {
  int type;
  struct cmd *cmd;
  char *file;
  char *efile;
  int mode;
  int fd;
};

struct pipecmd {
  int type;
  struct cmd *left;
  struct cmd *right;
};

struct listcmd {
  int type;
  struct cmd *left;
  struct cmd *right;
};

struct backcmd {
  int type;
  struct cmd *cmd;
};

int fork1(void); // Fork but panics on failure.
void panic(char *);
struct cmd *parsecmd(char *);
void runcmd(struct cmd *) __attribute__((noreturn));

// Execute cmd.  Never returns.
void
runcmd(struct cmd *cmd)
{
/*
 * AUTOCOMMENT-FUNC-DEF: runcmd
 * Explicacion facil:
 *   Esta funcion participa en el flujo interno del shell de xv6.
 *   Conviene seguir que token procesa y que nodo de comando construye o ejecuta.
 */
  int p[2];
  struct backcmd *bcmd;
  struct execcmd *ecmd;
  struct listcmd *lcmd;
  struct pipecmd *pcmd;
  struct redircmd *rcmd;

  if (cmd == 0)
    exit(1);

  switch (cmd->type) {
  default:
    panic("runcmd");

  case EXEC:
    ecmd = (struct execcmd *)cmd;
    if (ecmd->argv[0] == 0)
      exit(1);
    exec(ecmd->argv[0], ecmd->argv);
    fprintf(2, "exec %s failed\n", ecmd->argv[0]);
    break;

  case REDIR:
    rcmd = (struct redircmd *)cmd;
    close(rcmd->fd);
    if (open(rcmd->file, rcmd->mode) < 0) {
      fprintf(2, "open %s failed\n", rcmd->file);
      exit(1);
    }
    runcmd(rcmd->cmd);
    break;

  case LIST:
    lcmd = (struct listcmd *)cmd;
    if (fork1() == 0)
      runcmd(lcmd->left);
    wait(0);
    runcmd(lcmd->right);
    break;

  case PIPE:
    pcmd = (struct pipecmd *)cmd;
    if (pipe(p) < 0)
      panic("pipe");
    if (fork1() == 0) {
      close(1);
      dup(p[1]);
      close(p[0]);
      close(p[1]);
      runcmd(pcmd->left);
    }
    if (fork1() == 0) {
      close(0);
      dup(p[0]);
      close(p[0]);
      close(p[1]);
      runcmd(pcmd->right);
    }
    close(p[0]);
    close(p[1]);
    wait(0);
    wait(0);
    break;

  case BACK:
    bcmd = (struct backcmd *)cmd;
    if (fork1() == 0)
      runcmd(bcmd->cmd);
    break;
  }
  exit(0);
}

int
getcmd(char *buf, int nbuf)
{
/*
 * AUTOCOMMENT-FUNC-DEF: getcmd
 * Explicacion facil:
 *   Esta funcion participa en el flujo interno del shell de xv6.
 *   Conviene seguir que token procesa y que nodo de comando construye o ejecuta.
 */
  write(2, "$ ", 2);
  memset(buf, 0, nbuf);
  gets(buf, nbuf);
  if (buf[0] == 0) // EOF
    return -1;
  return 0;
}

int
main(void)
{
/*
 * AUTOCOMMENT-FUNC-DEF: main
 * Explicacion facil:
 *   Esta funcion participa en el flujo interno del shell de xv6.
 *   Conviene seguir que token procesa y que nodo de comando construye o ejecuta.
 */
  static char buf[100];
  int fd;

  // Ensure that three file descriptors are open.
  while ((fd = open("console", O_RDWR)) >= 0) {
    if (fd >= 3) {
      close(fd);
      break;
    }
  }

  // Read and run input commands.
  while (getcmd(buf, sizeof(buf)) >= 0) {
    char *cmd = buf;
    while (*cmd == ' ' || *cmd == '\t')
      cmd++;
    if (*cmd == '\n') // is a blank command
      continue;
    if (cmd[0] == 'c' && cmd[1] == 'd' && cmd[2] == ' ') {
      // Chdir must be called by the parent, not the child.
      cmd[strlen(cmd) - 1] = 0; // chop \n
      if (chdir(cmd + 3) < 0)
        fprintf(2, "cannot cd %s\n", cmd + 3);
    } else {
      if (fork1() == 0)
        runcmd(parsecmd(cmd));
      wait(0);
    }
  }
  exit(0);
}

void
panic(char *s)
{
/*
 * AUTOCOMMENT-FUNC-DEF: panic
 * Explicacion facil:
 *   Esta funcion participa en el flujo interno del shell de xv6.
 *   Conviene seguir que token procesa y que nodo de comando construye o ejecuta.
 */
  fprintf(2, "%s\n", s);
  exit(1);
}

int
fork1(void)
{
/*
 * AUTOCOMMENT-FUNC-DEF: fork1
 * Explicacion facil:
 *   Esta funcion participa en el flujo interno del shell de xv6.
 *   Conviene seguir que token procesa y que nodo de comando construye o ejecuta.
 */
  int pid;

  pid = fork();
  if (pid == -1)
    panic("fork");
  return pid;
}

//PAGEBREAK!
// Constructors

struct cmd *
execcmd(void)
{
/*
 * AUTOCOMMENT-FUNC-DEF: execcmd
 * Explicacion facil:
 *   Esta funcion participa en el flujo interno del shell de xv6.
 *   Conviene seguir que token procesa y que nodo de comando construye o ejecuta.
 */
  struct execcmd *cmd;

  cmd = malloc(sizeof(*cmd));
  memset(cmd, 0, sizeof(*cmd));
  cmd->type = EXEC;
  return (struct cmd *)cmd;
}

struct cmd *
redircmd(struct cmd *subcmd, char *file, char *efile, int mode, int fd)
{
/*
 * AUTOCOMMENT-FUNC-DEF: redircmd
 * Explicacion facil:
 *   Esta funcion participa en el flujo interno del shell de xv6.
 *   Conviene seguir que token procesa y que nodo de comando construye o ejecuta.
 */
  struct redircmd *cmd;

  cmd = malloc(sizeof(*cmd));
  memset(cmd, 0, sizeof(*cmd));
  cmd->type = REDIR;
  cmd->cmd = subcmd;
  cmd->file = file;
  cmd->efile = efile;
  cmd->mode = mode;
  cmd->fd = fd;
  return (struct cmd *)cmd;
}

struct cmd *
pipecmd(struct cmd *left, struct cmd *right)
{
/*
 * AUTOCOMMENT-FUNC-DEF: pipecmd
 * Explicacion facil:
 *   Esta funcion participa en el flujo interno del shell de xv6.
 *   Conviene seguir que token procesa y que nodo de comando construye o ejecuta.
 */
  struct pipecmd *cmd;

  cmd = malloc(sizeof(*cmd));
  memset(cmd, 0, sizeof(*cmd));
  cmd->type = PIPE;
  cmd->left = left;
  cmd->right = right;
  return (struct cmd *)cmd;
}

struct cmd *
listcmd(struct cmd *left, struct cmd *right)
{
/*
 * AUTOCOMMENT-FUNC-DEF: listcmd
 * Explicacion facil:
 *   Esta funcion participa en el flujo interno del shell de xv6.
 *   Conviene seguir que token procesa y que nodo de comando construye o ejecuta.
 */
  struct listcmd *cmd;

  cmd = malloc(sizeof(*cmd));
  memset(cmd, 0, sizeof(*cmd));
  cmd->type = LIST;
  cmd->left = left;
  cmd->right = right;
  return (struct cmd *)cmd;
}

struct cmd *
backcmd(struct cmd *subcmd)
{
/*
 * AUTOCOMMENT-FUNC-DEF: backcmd
 * Explicacion facil:
 *   Esta funcion participa en el flujo interno del shell de xv6.
 *   Conviene seguir que token procesa y que nodo de comando construye o ejecuta.
 */
  struct backcmd *cmd;

  cmd = malloc(sizeof(*cmd));
  memset(cmd, 0, sizeof(*cmd));
  cmd->type = BACK;
  cmd->cmd = subcmd;
  return (struct cmd *)cmd;
}
//PAGEBREAK!
// Parsing

char whitespace[] = " \t\r\n\v";
char symbols[] = "<|>&;()";

int
gettoken(char **ps, char *es, char **q, char **eq)
{
/*
 * AUTOCOMMENT-FUNC-DEF: gettoken
 * Explicacion facil:
 *   Esta funcion participa en el flujo interno del shell de xv6.
 *   Conviene seguir que token procesa y que nodo de comando construye o ejecuta.
 */
  char *s;
  int ret;

  s = *ps;
  while (s < es && strchr(whitespace, *s))
    s++;
  if (q)
    *q = s;
  ret = *s;
  switch (*s) {
  case 0:
    break;
  case '|':
  case '(':
  case ')':
  case ';':
  case '&':
  case '<':
    s++;
    break;
  case '>':
    s++;
    if (*s == '>') {
      ret = '+';
      s++;
    }
    break;
  default:
    ret = 'a';
    while (s < es && !strchr(whitespace, *s) && !strchr(symbols, *s))
      s++;
    break;
  }
  if (eq)
    *eq = s;

  while (s < es && strchr(whitespace, *s))
    s++;
  *ps = s;
  return ret;
}

int
peek(char **ps, char *es, char *toks)
{
/*
 * AUTOCOMMENT-FUNC-DEF: peek
 * Explicacion facil:
 *   Esta funcion participa en el flujo interno del shell de xv6.
 *   Conviene seguir que token procesa y que nodo de comando construye o ejecuta.
 */
  char *s;

  s = *ps;
  while (s < es && strchr(whitespace, *s))
    s++;
  *ps = s;
  return *s && strchr(toks, *s);
}

struct cmd *parseline(char **, char *);
struct cmd *parsepipe(char **, char *);
struct cmd *parseexec(char **, char *);
struct cmd *nulterminate(struct cmd *);

struct cmd *
parsecmd(char *s)
{
/*
 * AUTOCOMMENT-FUNC-DEF: parsecmd
 * Explicacion facil:
 *   Esta funcion participa en el flujo interno del shell de xv6.
 *   Conviene seguir que token procesa y que nodo de comando construye o ejecuta.
 */
  char *es;
  struct cmd *cmd;

  es = s + strlen(s);
  cmd = parseline(&s, es);
  peek(&s, es, "");
  if (s != es) {
    fprintf(2, "leftovers: %s\n", s);
    panic("syntax");
  }
  nulterminate(cmd);
  return cmd;
}

struct cmd *
parseline(char **ps, char *es)
{
/*
 * AUTOCOMMENT-FUNC-DEF: parseline
 * Explicacion facil:
 *   Esta funcion participa en el flujo interno del shell de xv6.
 *   Conviene seguir que token procesa y que nodo de comando construye o ejecuta.
 */
  struct cmd *cmd;

  cmd = parsepipe(ps, es);
  while (peek(ps, es, "&")) {
    gettoken(ps, es, 0, 0);
    cmd = backcmd(cmd);
  }
  if (peek(ps, es, ";")) {
    gettoken(ps, es, 0, 0);
    cmd = listcmd(cmd, parseline(ps, es));
  }
  return cmd;
}

struct cmd *
parsepipe(char **ps, char *es)
{
/*
 * AUTOCOMMENT-FUNC-DEF: parsepipe
 * Explicacion facil:
 *   Esta funcion participa en el flujo interno del shell de xv6.
 *   Conviene seguir que token procesa y que nodo de comando construye o ejecuta.
 */
  struct cmd *cmd;

  cmd = parseexec(ps, es);
  if (peek(ps, es, "|")) {
    gettoken(ps, es, 0, 0);
    cmd = pipecmd(cmd, parsepipe(ps, es));
  }
  return cmd;
}

struct cmd *
parseredirs(struct cmd *cmd, char **ps, char *es)
{
/*
 * AUTOCOMMENT-FUNC-DEF: parseredirs
 * Explicacion facil:
 *   Esta funcion participa en el flujo interno del shell de xv6.
 *   Conviene seguir que token procesa y que nodo de comando construye o ejecuta.
 */
  int tok;
  char *q, *eq;

  while (peek(ps, es, "<>")) {
    tok = gettoken(ps, es, 0, 0);
    if (gettoken(ps, es, &q, &eq) != 'a')
      panic("missing file for redirection");
    switch (tok) {
    case '<':
      cmd = redircmd(cmd, q, eq, O_RDONLY, 0);
      break;
    case '>':
      cmd = redircmd(cmd, q, eq, O_WRONLY | O_CREATE | O_TRUNC, 1);
      break;
    case '+': // >>
      cmd = redircmd(cmd, q, eq, O_WRONLY | O_CREATE, 1);
      break;
    }
  }
  return cmd;
}

struct cmd *
parseblock(char **ps, char *es)
{
/*
 * AUTOCOMMENT-FUNC-DEF: parseblock
 * Explicacion facil:
 *   Esta funcion participa en el flujo interno del shell de xv6.
 *   Conviene seguir que token procesa y que nodo de comando construye o ejecuta.
 */
  struct cmd *cmd;

  if (!peek(ps, es, "("))
    panic("parseblock");
  gettoken(ps, es, 0, 0);
  cmd = parseline(ps, es);
  if (!peek(ps, es, ")"))
    panic("syntax - missing )");
  gettoken(ps, es, 0, 0);
  cmd = parseredirs(cmd, ps, es);
  return cmd;
}

struct cmd *
parseexec(char **ps, char *es)
{
/*
 * AUTOCOMMENT-FUNC-DEF: parseexec
 * Explicacion facil:
 *   Esta funcion participa en el flujo interno del shell de xv6.
 *   Conviene seguir que token procesa y que nodo de comando construye o ejecuta.
 */
  char *q, *eq;
  int tok, argc;
  struct execcmd *cmd;
  struct cmd *ret;

  if (peek(ps, es, "("))
    return parseblock(ps, es);

  ret = execcmd();
  cmd = (struct execcmd *)ret;

  argc = 0;
  ret = parseredirs(ret, ps, es);
  while (!peek(ps, es, "|)&;")) {
    if ((tok = gettoken(ps, es, &q, &eq)) == 0)
      break;
    if (tok != 'a')
      panic("syntax");
    cmd->argv[argc] = q;
    cmd->eargv[argc] = eq;
    argc++;
    if (argc >= MAXARGS)
      panic("too many args");
    ret = parseredirs(ret, ps, es);
  }
  cmd->argv[argc] = 0;
  cmd->eargv[argc] = 0;
  return ret;
}

// NUL-terminate all the counted strings.
struct cmd *
nulterminate(struct cmd *cmd)
{
/*
 * AUTOCOMMENT-FUNC-DEF: nulterminate
 * Explicacion facil:
 *   Esta funcion participa en el flujo interno del shell de xv6.
 *   Conviene seguir que token procesa y que nodo de comando construye o ejecuta.
 */
  int i;
  struct backcmd *bcmd;
  struct execcmd *ecmd;
  struct listcmd *lcmd;
  struct pipecmd *pcmd;
  struct redircmd *rcmd;

  if (cmd == 0)
    return 0;

  switch (cmd->type) {
  case EXEC:
    ecmd = (struct execcmd *)cmd;
    for (i = 0; ecmd->argv[i]; i++)
      *ecmd->eargv[i] = 0;
    break;

  case REDIR:
    rcmd = (struct redircmd *)cmd;
    nulterminate(rcmd->cmd);
    *rcmd->efile = 0;
    break;

  case PIPE:
    pcmd = (struct pipecmd *)cmd;
    nulterminate(pcmd->left);
    nulterminate(pcmd->right);
    break;

  case LIST:
    lcmd = (struct listcmd *)cmd;
    nulterminate(lcmd->left);
    nulterminate(lcmd->right);
    break;

  case BACK:
    bcmd = (struct backcmd *)cmd;
    nulterminate(bcmd->cmd);
    break;
  }
  return cmd;
}
