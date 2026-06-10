/*
 * AUTOCOMMENT-XV6: comentario agregado automaticamente para explicar el archivo.
 * Archivo: kernel/string.c
 *
 * Explicacion clara y facil:
 *   - Rutinas basicas de memoria/cadena.
 *   - Incluye funciones tipo memcpy/memset/strcmp usadas ampliamente en kernel y usuario.
 *
 * Como leer este archivo:
 *   1) Busca las estructuras principales y entiende que estado guardan.
 *   2) Revisa las funciones publicas (las que llaman otros modulos).
 *   3) Luego estudia helpers internos para ver el flujo completo.
 *
 * Nota:
 *   Estos comentarios son una guia pedagogica; la verdad final siempre es el codigo.
 */

#include "types.h"

void *
memset(void *dst, int c, uint n)
{
/*
 * AUTOCOMMENT-FUNC-DEF: memset
 * Explicacion facil:
 *   memset cumple una tarea puntual dentro de modulo.
 *   Para entenderla rapido, mira que recibe, que valida y que efecto deja al terminar.
 */
  char *cdst = (char *)dst;
  int i;
  for (i = 0; i < n; i++) {
    cdst[i] = c;
  }
  return dst;
}

int
memcmp(const void *v1, const void *v2, uint n)
{
/*
 * AUTOCOMMENT-FUNC-DEF: memcmp
 * Explicacion facil:
 *   memcmp cumple una tarea puntual dentro de modulo.
 *   Para entenderla rapido, mira que recibe, que valida y que efecto deja al terminar.
 */
  const uchar *s1, *s2;

  s1 = v1;
  s2 = v2;
  while (n-- > 0) {
    if (*s1 != *s2)
      return *s1 - *s2;
    s1++, s2++;
  }

  return 0;
}

void *
memmove(void *dst, const void *src, uint n)
{
/*
 * AUTOCOMMENT-FUNC-DEF: memmove
 * Explicacion facil:
 *   memmove cumple una tarea puntual dentro de modulo.
 *   Para entenderla rapido, mira que recibe, que valida y que efecto deja al terminar.
 */
  const char *s;
  char *d;

  if (n == 0)
    return dst;

  s = src;
  d = dst;
  if (s < d && s + n > d) {
    s += n;
    d += n;
    while (n-- > 0)
      *--d = *--s;
  } else
    while (n-- > 0)
      *d++ = *s++;

  return dst;
}

// memcpy exists to placate GCC.  Use memmove.
void *
memcpy(void *dst, const void *src, uint n)
{
/*
 * AUTOCOMMENT-FUNC-DEF: memcpy
 * Explicacion facil:
 *   memcpy cumple una tarea puntual dentro de modulo.
 *   Para entenderla rapido, mira que recibe, que valida y que efecto deja al terminar.
 */
  return memmove(dst, src, n);
}

int
strncmp(const char *p, const char *q, uint n)
{
/*
 * AUTOCOMMENT-FUNC-DEF: strncmp
 * Explicacion facil:
 *   strncmp cumple una tarea puntual dentro de modulo.
 *   Para entenderla rapido, mira que recibe, que valida y que efecto deja al terminar.
 */
  while (n > 0 && *p && *p == *q)
    n--, p++, q++;
  if (n == 0)
    return 0;
  return (uchar)*p - (uchar)*q;
}

char *
strncpy(char *s, const char *t, int n)
{
/*
 * AUTOCOMMENT-FUNC-DEF: strncpy
 * Explicacion facil:
 *   strncpy cumple una tarea puntual dentro de modulo.
 *   Para entenderla rapido, mira que recibe, que valida y que efecto deja al terminar.
 */
  char *os;

  os = s;
  while (n-- > 0 && (*s++ = *t++) != 0)
    ;
  while (n-- > 0)
    *s++ = 0;
  return os;
}

// Like strncpy but guaranteed to NUL-terminate.
char *
safestrcpy(char *s, const char *t, int n)
{
/*
 * AUTOCOMMENT-FUNC-DEF: safestrcpy
 * Explicacion facil:
 *   safestrcpy cumple una tarea puntual dentro de modulo.
 *   Para entenderla rapido, mira que recibe, que valida y que efecto deja al terminar.
 */
  char *os;

  os = s;
  if (n <= 0)
    return os;
  while (--n > 0 && (*s++ = *t++) != 0)
    ;
  *s = 0;
  return os;
}

int
strlen(const char *s)
{
/*
 * AUTOCOMMENT-FUNC-DEF: strlen
 * Explicacion facil:
 *   strlen cumple una tarea puntual dentro de modulo.
 *   Para entenderla rapido, mira que recibe, que valida y que efecto deja al terminar.
 */
  int n;

  for (n = 0; s[n]; n++)
    ;
  return n;
}
