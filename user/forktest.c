/*
 * AUTOCOMMENT-XV6: comentario agregado automaticamente para explicar el archivo.
 * Archivo: user/forktest.c
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

// Test that fork fails gracefully.
// Tiny executable so that the limit can be filling the proc table.

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define N 1000

void
print(const char *s)
{
/*
 * AUTOCOMMENT-FUNC-DEF: print
 * Explicacion facil:
 *   print cumple una tarea puntual dentro de programa de usuario.
 *   Para entenderla rapido, mira que recibe, que valida y que efecto deja al terminar.
 */
  write(1, s, strlen(s));
}

void
forktest(void)
{
/*
 * AUTOCOMMENT-FUNC-DEF: forktest
 * Explicacion facil:
 *   Gestiona ciclo de vida de procesos dentro de programas de usuario.
 *   Actualiza estado, recursos y coordinacion con padre/hijos o scheduler.
 */
  int n, pid;

  print("fork test\n");

  for (n = 0; n < N; n++) {
    pid = fork();
    if (pid < 0)
      break;
    if (pid == 0)
      exit(0);
  }

  if (n == N) {
    print("fork claimed to work N times!\n");
    exit(1);
  }

  for (; n > 0; n--) {
    if (wait(0) < 0) {
      print("wait stopped early\n");
      exit(1);
    }
  }

  if (wait(0) != -1) {
    print("wait got too many\n");
    exit(1);
  }

  print("fork test OK\n");
}

int
main(void)
{
/*
 * AUTOCOMMENT-FUNC-DEF: main
 * Explicacion facil:
 *   Punto de entrada en este modulo de programas de usuario.
 *   Coordina inicializacion, llamadas principales y el flujo base de ejecucion.
 */
  forktest();
  exit(0);
}
