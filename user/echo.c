/*
 * AUTOCOMMENT-XV6: comentario agregado automaticamente para explicar el archivo.
 * Archivo: user/echo.c
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
#include "user/user.h"

int
main(int argc, char *argv[])
{
/*
 * AUTOCOMMENT-FUNC-DEF: main
 * Explicacion facil:
 *   Punto de entrada en este modulo de programas de usuario.
 *   Coordina inicializacion, llamadas principales y el flujo base de ejecucion.
 */
  int i;

  for (i = 1; i < argc; i++) {
    write(1, argv[i], strlen(argv[i]));
    if (i + 1 < argc) {
      write(1, " ", 1);
    } else {
      write(1, "\n", 1);
    }
  }
  exit(0);
}
