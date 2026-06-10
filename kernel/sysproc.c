/*
 * AUTOCOMMENT-XV6: comentario agregado automaticamente para explicar el archivo.
 * Archivo: kernel/sysproc.c
 *
 * Explicacion clara y facil:
 *   - Syscalls de procesos y memoria.
 *   - Implementa fork/exit/wait/kill/getpid/sbrk/sleep/uptime para control de procesos.
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
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "vm.h"

uint64
sys_exit(void)
{
/*
 * AUTOCOMMENT-FUNC-DEF: sys_exit
 * Explicacion facil:
 *   Coordina estados de procesos y planificacion dentro de syscalls de procesos.
 *   Toca locks y transiciones de estado, por eso conviene seguir el orden exacto de pasos.
 */
  int n;
  argint(0, &n);
  kexit(n);
  return 0; // not reached
}

uint64
sys_getpid(void)
{
/*
 * AUTOCOMMENT-FUNC-DEF: sys_getpid
 * Explicacion facil:
 *   sys_getpid cumple una tarea puntual dentro de syscalls de procesos.
 *   Para entenderla rapido, mira que recibe, que valida y que efecto deja al terminar.
 */
  return myproc()->pid;
}

uint64
sys_fork(void)
{
/*
 * AUTOCOMMENT-FUNC-DEF: sys_fork
 * Explicacion facil:
 *   Coordina estados de procesos y planificacion dentro de syscalls de procesos.
 *   Toca locks y transiciones de estado, por eso conviene seguir el orden exacto de pasos.
 */
  return kfork();
}

uint64
sys_wait(void)
{
/*
 * AUTOCOMMENT-FUNC-DEF: sys_wait
 * Explicacion facil:
 *   Coordina estados de procesos y planificacion dentro de syscalls de procesos.
 *   Toca locks y transiciones de estado, por eso conviene seguir el orden exacto de pasos.
 */
  uint64 p;
  argaddr(0, &p);
  return kwait(p);
}

uint64
sys_sbrk(void)
{
/*
 * AUTOCOMMENT-FUNC-DEF: sys_sbrk
 * Explicacion facil:
 *   sys_sbrk cumple una tarea puntual dentro de syscalls de procesos.
 *   Para entenderla rapido, mira que recibe, que valida y que efecto deja al terminar.
 */
  uint64 addr;
  int t;
  int n;

  argint(0, &n);
  argint(1, &t);
  addr = myproc()->sz;

  if (t == SBRK_EAGER || n < 0) {
    if (growproc(n) < 0) {
      return -1;
    }
  } else {
    // Lazily allocate memory for this process: increase its memory
    // size but don't allocate memory. If the processes uses the
    // memory, vmfault() will allocate it.
    if (addr + n < addr)
      return -1;
    if (addr + n > TRAPFRAME)
      return -1;
    myproc()->sz += n;
  }
  return addr;
}

uint64
sys_pause(void)
{
/*
 * AUTOCOMMENT-FUNC-DEF: sys_pause
 * Explicacion facil:
 *   sys_pause cumple una tarea puntual dentro de syscalls de procesos.
 *   Para entenderla rapido, mira que recibe, que valida y que efecto deja al terminar.
 */
  int n;
  uint ticks0;

  argint(0, &n);
  if (n < 0)
    n = 0;
  acquire(&tickslock);
  ticks0 = ticks;
  while (ticks - ticks0 < n) {
    if (killed(myproc())) {
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
/*
 * AUTOCOMMENT-FUNC-DEF: sys_kill
 * Explicacion facil:
 *   Coordina estados de procesos y planificacion dentro de syscalls de procesos.
 *   Toca locks y transiciones de estado, por eso conviene seguir el orden exacto de pasos.
 */
  int pid;

  argint(0, &pid);
  return kkill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
/*
 * AUTOCOMMENT-FUNC-DEF: sys_uptime
 * Explicacion facil:
 *   sys_uptime cumple una tarea puntual dentro de syscalls de procesos.
 *   Para entenderla rapido, mira que recibe, que valida y que efecto deja al terminar.
 */
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
