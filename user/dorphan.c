/*
 * AUTOCOMMENT-XV6: comentario agregado automaticamente para explicar el archivo.
 * Archivo: user/dorphan.c
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

// Create an orphaned directory and check if test-xv6.py recovers it.

#define BUFSZ 500

char buf[BUFSZ];

int
main(int argc, char **argv)
{
/*
 * AUTOCOMMENT-FUNC-DEF: main
 * Explicacion facil:
 *   Punto de entrada en este modulo de programas de usuario.
 *   Coordina inicializacion, llamadas principales y el flujo base de ejecucion.
 */
  char *s = argv[0];

  if (mkdir("dd") != 0) {
    printf("%s: mkdir dd failed\n", s);
    exit(1);
  }

  if (chdir("dd") != 0) {
    printf("%s: chdir dd failed\n", s);
    exit(1);
  }

  if (unlink("../dd") < 0) {
    printf("%s: unlink failed\n", s);
    exit(1);
  }
  printf("wait for kill and reclaim\n");
  // sit around until killed
  for (;;)
    pause(1000);
}
