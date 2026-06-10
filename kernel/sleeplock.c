/*
 * AUTOCOMMENT-XV6: comentario agregado automaticamente para explicar el archivo.
 * Archivo: kernel/sleeplock.c
 *
 * Explicacion clara y facil:
 *   - Locks bloqueantes (sleep locks).
 *   - Permite esperar durmiendo en lugar de girar activamente cuando la seccion critica puede tardar.
 *
 * Como leer este archivo:
 *   1) Busca las estructuras principales y entiende que estado guardan.
 *   2) Revisa las funciones publicas (las que llaman otros modulos).
 *   3) Luego estudia helpers internos para ver el flujo completo.
 *
 * Nota:
 *   Estos comentarios son una guia pedagogica; la verdad final siempre es el codigo.
 */

// Sleeping locks

#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "sleeplock.h"

void
initsleeplock(struct sleeplock *lk, char *name)
{
/*
 * AUTOCOMMENT-FUNC-DEF: initsleeplock
 * Explicacion facil:
 *   Coordina estados de procesos y planificacion dentro de modulo.
 *   Toca locks y transiciones de estado, por eso conviene seguir el orden exacto de pasos.
 */
  initlock(&lk->lk, "sleep lock");
  lk->name = name;
  lk->locked = 0;
  lk->pid = 0;
}

void
acquiresleep(struct sleeplock *lk)
{
/*
 * AUTOCOMMENT-FUNC-DEF: acquiresleep
 * Explicacion facil:
 *   Sincroniza acceso concurrente dentro de sleeplocks.
 *   Controla locks o estados de espera para prevenir carreras y bloqueos incorrectos.
 */
  acquire(&lk->lk);
  while (lk->locked) {
    sleep(lk, &lk->lk);
  }
  lk->locked = 1;
  lk->pid = myproc()->pid;
  release(&lk->lk);
}

void
releasesleep(struct sleeplock *lk)
{
/*
 * AUTOCOMMENT-FUNC-DEF: releasesleep
 * Explicacion facil:
 *   Sincroniza acceso concurrente dentro de sleeplocks.
 *   Controla locks o estados de espera para prevenir carreras y bloqueos incorrectos.
 */
  acquire(&lk->lk);
  lk->locked = 0;
  lk->pid = 0;
  wakeup(lk);
  release(&lk->lk);
}

int
holdingsleep(struct sleeplock *lk)
{
/*
 * AUTOCOMMENT-FUNC-DEF: holdingsleep
 * Explicacion facil:
 *   Coordina estados de procesos y planificacion dentro de modulo.
 *   Toca locks y transiciones de estado, por eso conviene seguir el orden exacto de pasos.
 */
  int r;

  acquire(&lk->lk);
  r = lk->locked && (lk->pid == myproc()->pid);
  release(&lk->lk);
  return r;
}
