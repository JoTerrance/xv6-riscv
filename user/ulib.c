/*
 * AUTOCOMMENT-XV6: comentario agregado automaticamente para explicar el archivo.
 * Archivo: user/ulib.c
 *
 * Explicacion clara y facil:
 *   - Libreria base de usuario.
 *   - Implementa funciones comunes (cadenas, IO simple, wrappers) para evitar duplicacion en cada programa.
 *
 * Como leer este archivo:
 *   1) Busca las estructuras principales y entiende que estado guardan.
 *   2) Revisa las funciones publicas (las que llaman otros modulos).
 *   3) Luego estudia helpers internos para ver el flujo completo.
 *
 * Nota:
 *   Estos comentarios son una guia pedagogica; la verdad final siempre es el codigo.
 */

#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "kernel/riscv.h"
#include "kernel/vm.h"
#include "user/user.h"

//
// wrapper so that it's OK if main() does not call exit().
//
void
start(int argc, char **argv)
{
/*
 * AUTOCOMMENT-FUNC-DEF: start
 * Explicacion facil:
 *   start cumple una tarea puntual dentro de programa de usuario.
 *   Para entenderla rapido, mira que recibe, que valida y que efecto deja al terminar.
 */
  int r;
  extern int main(int argc, char **argv);
  r = main(argc, argv);
  exit(r);
}

char *
strcpy(char *s, const char *t)
{
/*
 * AUTOCOMMENT-FUNC-DEF: strcpy
 * Explicacion facil:
 *   strcpy cumple una tarea puntual dentro de programa de usuario.
 *   Para entenderla rapido, mira que recibe, que valida y que efecto deja al terminar.
 */
  char *os;

  os = s;
  while ((*s++ = *t++) != 0)
    ;
  return os;
}

int
strcmp(const char *p, const char *q)
{
/*
 * AUTOCOMMENT-FUNC-DEF: strcmp
 * Explicacion facil:
 *   strcmp cumple una tarea puntual dentro de programa de usuario.
 *   Para entenderla rapido, mira que recibe, que valida y que efecto deja al terminar.
 */
  while (*p && *p == *q)
    p++, q++;
  return (uchar)*p - (uchar)*q;
}

uint
strlen(const char *s)
{
/*
 * AUTOCOMMENT-FUNC-DEF: strlen
 * Explicacion facil:
 *   strlen cumple una tarea puntual dentro de programa de usuario.
 *   Para entenderla rapido, mira que recibe, que valida y que efecto deja al terminar.
 */
  int n;

  for (n = 0; s[n]; n++)
    ;
  return n;
}

void *
memset(void *dst, int c, uint n)
{
/*
 * AUTOCOMMENT-FUNC-DEF: memset
 * Explicacion facil:
 *   memset cumple una tarea puntual dentro de programa de usuario.
 *   Para entenderla rapido, mira que recibe, que valida y que efecto deja al terminar.
 */
  char *cdst = (char *)dst;
  int i;
  for (i = 0; i < n; i++) {
    cdst[i] = c;
  }
  return dst;
}

char *
strchr(const char *s, char c)
{
/*
 * AUTOCOMMENT-FUNC-DEF: strchr
 * Explicacion facil:
 *   strchr cumple una tarea puntual dentro de programa de usuario.
 *   Para entenderla rapido, mira que recibe, que valida y que efecto deja al terminar.
 */
  for (; *s; s++)
    if (*s == c)
      return (char *)s;
  return 0;
}

char *
gets(char *buf, int max)
{
/*
 * AUTOCOMMENT-FUNC-DEF: gets
 * Explicacion facil:
 *   gets cumple una tarea puntual dentro de programa de usuario.
 *   Para entenderla rapido, mira que recibe, que valida y que efecto deja al terminar.
 */
  int i, cc;
  char c;

  for (i = 0; i + 1 < max;) {
    cc = read(0, &c, 1);
    if (cc < 1)
      break;
    buf[i++] = c;
    if (c == '\n' || c == '\r')
      break;
  }
  buf[i] = '\0';
  return buf;
}

int
stat(const char *n, struct stat *st)
{
/*
 * AUTOCOMMENT-FUNC-DEF: stat
 * Explicacion facil:
 *   stat cumple una tarea puntual dentro de programa de usuario.
 *   Para entenderla rapido, mira que recibe, que valida y que efecto deja al terminar.
 */
  int fd;
  int r;

  fd = open(n, O_RDONLY);
  if (fd < 0)
    return -1;
  r = fstat(fd, st);
  close(fd);
  return r;
}

int
atoi(const char *s)
{
/*
 * AUTOCOMMENT-FUNC-DEF: atoi
 * Explicacion facil:
 *   atoi cumple una tarea puntual dentro de programa de usuario.
 *   Para entenderla rapido, mira que recibe, que valida y que efecto deja al terminar.
 */
  int n;

  n = 0;
  while ('0' <= *s && *s <= '9')
    n = n * 10 + *s++ - '0';
  return n;
}

void *
memmove(void *vdst, const void *vsrc, int n)
{
/*
 * AUTOCOMMENT-FUNC-DEF: memmove
 * Explicacion facil:
 *   memmove cumple una tarea puntual dentro de programa de usuario.
 *   Para entenderla rapido, mira que recibe, que valida y que efecto deja al terminar.
 */
  char *dst;
  const char *src;

  dst = vdst;
  src = vsrc;
  if (src > dst) {
    while (n-- > 0)
      *dst++ = *src++;
  } else {
    dst += n;
    src += n;
    while (n-- > 0)
      *--dst = *--src;
  }
  return vdst;
}

int
memcmp(const void *s1, const void *s2, uint n)
{
/*
 * AUTOCOMMENT-FUNC-DEF: memcmp
 * Explicacion facil:
 *   memcmp cumple una tarea puntual dentro de programa de usuario.
 *   Para entenderla rapido, mira que recibe, que valida y que efecto deja al terminar.
 */
  const char *p1 = s1, *p2 = s2;
  while (n-- > 0) {
    if (*p1 != *p2) {
      return *p1 - *p2;
    }
    p1++;
    p2++;
  }
  return 0;
}

void *
memcpy(void *dst, const void *src, uint n)
{
/*
 * AUTOCOMMENT-FUNC-DEF: memcpy
 * Explicacion facil:
 *   memcpy cumple una tarea puntual dentro de programa de usuario.
 *   Para entenderla rapido, mira que recibe, que valida y que efecto deja al terminar.
 */
  return memmove(dst, src, n);
}

char *
sbrk(int n)
{
/*
 * AUTOCOMMENT-FUNC-DEF: sbrk
 * Explicacion facil:
 *   sbrk cumple una tarea puntual dentro de programa de usuario.
 *   Para entenderla rapido, mira que recibe, que valida y que efecto deja al terminar.
 */
  return sys_sbrk(n, SBRK_EAGER);
}

char *
sbrklazy(int n)
{
/*
 * AUTOCOMMENT-FUNC-DEF: sbrklazy
 * Explicacion facil:
 *   sbrklazy cumple una tarea puntual dentro de programa de usuario.
 *   Para entenderla rapido, mira que recibe, que valida y que efecto deja al terminar.
 */
  return sys_sbrk(n, SBRK_LAZY);
}
