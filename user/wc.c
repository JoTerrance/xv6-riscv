/*
 * AUTOCOMMENT-XV6: comentario agregado automaticamente para explicar el archivo.
 * Archivo: user/wc.c
 *
 * Explicacion clara y facil:
 *   - Programa de usuario tipo utilidad Unix simple.
 *   - Cada archivo implementa un comando pequeno (cat, ls, grep, etc.) para probar y usar xv6 desde shell.
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
#include "user/user.h"

char buf[512];

void
wc(int fd, char *name)
{
/*
 * AUTOCOMMENT-FUNC-DEF: wc
 * Explicacion facil:
 *   wc cumple una tarea puntual dentro de programa de usuario.
 *   Para entenderla rapido, mira que recibe, que valida y que efecto deja al terminar.
 */
  int i, n;
  int l, w, c, inword;

  l = w = c = 0;
  inword = 0;
  while ((n = read(fd, buf, sizeof(buf))) > 0) {
    for (i = 0; i < n; i++) {
      c++;
      if (buf[i] == '\n')
        l++;
      if (strchr(" \r\t\n\v", buf[i]))
        inword = 0;
      else if (!inword) {
        w++;
        inword = 1;
      }
    }
  }
  if (n < 0) {
    printf("wc: read error\n");
    exit(1);
  }
  printf("%d %d %d %s\n", l, w, c, name);
}

int
main(int argc, char *argv[])
{
/*
 * AUTOCOMMENT-FUNC-DEF: main
 * Explicacion facil:
 *   Punto de entrada en este modulo de programas de usuario.
 *   Coordina inicializacion, llamadas principales y el flujo base de ejecucion.
 */
  int fd, i;

  if (argc <= 1) {
    wc(0, "");
    exit(0);
  }

  for (i = 1; i < argc; i++) {
    if ((fd = open(argv[i], O_RDONLY)) < 0) {
      printf("wc: cannot open %s\n", argv[i]);
      exit(1);
    }
    wc(fd, argv[i]);
    close(fd);
  }
  exit(0);
}
