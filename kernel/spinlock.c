/*
 * AUTOCOMMENT-XV6: comentario agregado automaticamente para explicar el archivo.
 * Archivo: kernel/spinlock.c
 *
 * Explicacion clara y facil:
 *   - Spinlocks del kernel.
 *   - Sincronizacion de baja latencia para secciones criticas cortas deshabilitando interrupciones cuando corresponde.
 *
 * Como leer este archivo:
 *   1) Busca las estructuras principales y entiende que estado guardan.
 *   2) Revisa las funciones publicas (las que llaman otros modulos).
 *   3) Luego estudia helpers internos para ver el flujo completo.
 *
 * Nota:
 *   Estos comentarios son una guia pedagogica; la verdad final siempre es el codigo.
 */

// Mutual exclusion spin locks.

#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "riscv.h"
#include "proc.h"
#include "defs.h"

void
initlock(struct spinlock *lk, char *name)
{
/*
 * AUTOCOMMENT-FUNC-DEF: initlock
 * Explicacion facil:
 *   initlock cumple una tarea puntual dentro de modulo.
 *   Para entenderla rapido, mira que recibe, que valida y que efecto deja al terminar.
 */
  lk->name = name;
  lk->locked = 0;
  lk->cpu = 0;
}

// Acquire the lock.
// Loops (spins) until the lock is acquired.
void
acquire(struct spinlock *lk)
{
/*
 * AUTOCOMMENT-FUNC-DEF: acquire
 * Explicacion facil:
 *   Sincroniza acceso concurrente dentro de spinlocks.
 *   Controla locks o estados de espera para prevenir carreras y bloqueos incorrectos.
 */
  push_off(); // disable interrupts to avoid deadlock.
  if (holding(lk))
    panic("acquire");

  // On RISC-V, __atomic_exchange_n turns into an atomic swap:
  //   a5 = 1
  //   s1 = &lk->locked
  //   amoswap.w.aq a5, a5, (s1)
  //
  // Passing __ATOMIC_ACQUIRE to __atomic_exchange_n tells
  // the C compiler and the processor to not move loads or stores
  // past this point, to ensure that the critical section's memory
  // references happen strictly after the lock is acquired.
  while (__atomic_exchange_n(&lk->locked, 1, __ATOMIC_ACQUIRE) != 0)
    ;

  // Record info about lock acquisition for holding() and debugging.
  lk->cpu = mycpu();
}

// Release the lock.
void
release(struct spinlock *lk)
{
/*
 * AUTOCOMMENT-FUNC-DEF: release
 * Explicacion facil:
 *   Sincroniza acceso concurrente dentro de spinlocks.
 *   Controla locks o estados de espera para prevenir carreras y bloqueos incorrectos.
 */
  if (!holding(lk))
    panic("release");

  lk->cpu = 0;

  // Release the lock, equivalent to lk->locked = 0.
  //
  // This code doesn't use a C assignment, since the C standard
  // implies that an assignment might be implemented with
  // multiple store instructions.
  //
  // On RISC-V, __atomic_store_n turns into a single atomic store:
  //   s1 = &lk->locked
  //   sw zero,0(s1)
  //
  // The __ATOMIC_RELEASE argument to __atomic_store_n tells the
  // the C compiler and the CPU to not move loads or stores past
  // this point, to ensure that all the stores in the critical
  // section are visible to other CPUs before the lock is released,
  // and that loads in the critical section occur strictly before
  // the lock is released.
  //
  // On RISC-V, this generates a fence instruction before the store:
  //   fence rw,w
  __atomic_store_n(&lk->locked, 0, __ATOMIC_RELEASE);

  pop_off();
}

// Check whether this cpu is holding the lock.
// Interrupts must be off.
int
holding(struct spinlock *lk)
{
/*
 * AUTOCOMMENT-FUNC-DEF: holding
 * Explicacion facil:
 *   holding cumple una tarea puntual dentro de modulo.
 *   Para entenderla rapido, mira que recibe, que valida y que efecto deja al terminar.
 */
  int r;
  r = (lk->locked && lk->cpu == mycpu());
  return r;
}

// push_off/pop_off are like intr_off()/intr_on() except that they are matched:
// it takes two pop_off()s to undo two push_off()s.  Also, if interrupts
// are initially off, then push_off, pop_off leaves them off.

void
push_off(void)
{
/*
 * AUTOCOMMENT-FUNC-DEF: push_off
 * Explicacion facil:
 *   push_off cumple una tarea puntual dentro de modulo.
 *   Para entenderla rapido, mira que recibe, que valida y que efecto deja al terminar.
 */
  int old = intr_get();

  // disable interrupts to prevent an involuntary context
  // switch while using mycpu().
  intr_off();

  if (mycpu()->noff == 0)
    mycpu()->intena = old;
  mycpu()->noff += 1;
}

void
pop_off(void)
{
/*
 * AUTOCOMMENT-FUNC-DEF: pop_off
 * Explicacion facil:
 *   pop_off cumple una tarea puntual dentro de modulo.
 *   Para entenderla rapido, mira que recibe, que valida y que efecto deja al terminar.
 */
  struct cpu *c = mycpu();
  if (intr_get())
    panic("pop_off - interruptible");
  if (c->noff < 1)
    panic("pop_off");
  c->noff -= 1;
  if (c->noff == 0 && c->intena)
    intr_on();
}
