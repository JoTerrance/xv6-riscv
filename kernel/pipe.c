/*
 * AUTOCOMMENT-XV6: comentario agregado automaticamente para explicar el archivo.
 * Archivo: kernel/pipe.c
 *
 * Explicacion clara y facil:
 *   - Implementacion de pipes.
 *   - Ofrece comunicacion productor/consumidor entre procesos con buffer circular y bloqueo adecuado.
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
#include "spinlock.h"
#include "proc.h"
#include "fs.h"
#include "sleeplock.h"
#include "file.h"

#define PIPESIZE 512

struct pipe {
  struct spinlock lock;
  char data[PIPESIZE];
  uint nread;    // number of bytes read
  uint nwrite;   // number of bytes written
  int readopen;  // read fd is still open
  int writeopen; // write fd is still open
};

int
pipealloc(struct file **f0, struct file **f1)
{
/*
 * AUTOCOMMENT-FUNC-DEF: pipealloc
 * Explicacion facil:
 *   Reserva o libera recursos de pipes segun haga falta.
 *   Si hay error, corta temprano para no dejar estructuras en estado inconsistente.
 */
  struct pipe *pi;

  pi = 0;
  *f0 = *f1 = 0;
  if ((*f0 = filealloc()) == 0 || (*f1 = filealloc()) == 0)
    goto bad;
  if ((pi = (struct pipe *)kalloc()) == 0)
    goto bad;
  pi->readopen = 1;
  pi->writeopen = 1;
  pi->nwrite = 0;
  pi->nread = 0;
  initlock(&pi->lock, "pipe");
  (*f0)->type = FD_PIPE;
  (*f0)->readable = 1;
  (*f0)->writable = 0;
  (*f0)->pipe = pi;
  (*f1)->type = FD_PIPE;
  (*f1)->readable = 0;
  (*f1)->writable = 1;
  (*f1)->pipe = pi;
  return 0;

bad:
  if (pi)
    kfree((char *)pi);
  if (*f0)
    fileclose(*f0);
  if (*f1)
    fileclose(*f1);
  return -1;
}

void
pipeclose(struct pipe *pi, int writable)
{
/*
 * AUTOCOMMENT-FUNC-DEF: pipeclose
 * Explicacion facil:
 *   pipeclose cumple una tarea puntual dentro de pipes.
 *   Para entenderla rapido, mira que recibe, que valida y que efecto deja al terminar.
 */
  acquire(&pi->lock);
  if (writable) {
    pi->writeopen = 0;
    wakeup(&pi->nread);
  } else {
    pi->readopen = 0;
    wakeup(&pi->nwrite);
  }
  if (pi->readopen == 0 && pi->writeopen == 0) {
    release(&pi->lock);
    kfree((char *)pi);
  } else
    release(&pi->lock);
}

int
pipewrite(struct pipe *pi, uint64 addr, int n)
{
/*
 * AUTOCOMMENT-FUNC-DEF: pipewrite
 * Explicacion facil:
 *   Realiza transferencia de datos en el contexto de pipes.
 *   Valida limites y mueve bytes de forma segura entre estructuras o espacios de memoria.
 */
  int i = 0;
  struct proc *pr = myproc();

  acquire(&pi->lock);
  while (i < n) {
    if (pi->readopen == 0 || killed(pr)) {
      release(&pi->lock);
      return -1;
    }
    if (pi->nwrite == pi->nread + PIPESIZE) { //DOC: pipewrite-full
      wakeup(&pi->nread);
      sleep(&pi->nwrite, &pi->lock);
    } else {
      char ch;
      if (copyin(pr->pagetable, &ch, addr + i, 1) == -1)
        break;
      pi->data[pi->nwrite++ % PIPESIZE] = ch;
      i++;
    }
  }
  wakeup(&pi->nread);
  release(&pi->lock);

  return i;
}

int
piperead(struct pipe *pi, uint64 addr, int n)
{
/*
 * AUTOCOMMENT-FUNC-DEF: piperead
 * Explicacion facil:
 *   Realiza transferencia de datos en el contexto de pipes.
 *   Valida limites y mueve bytes de forma segura entre estructuras o espacios de memoria.
 */
  int i;
  struct proc *pr = myproc();
  char ch;

  acquire(&pi->lock);
  while (pi->nread == pi->nwrite && pi->writeopen) { //DOC: pipe-empty
    if (killed(pr)) {
      release(&pi->lock);
      return -1;
    }
    sleep(&pi->nread, &pi->lock); //DOC: piperead-sleep
  }
  for (i = 0; i < n; i++) { //DOC: piperead-copy
    if (pi->nread == pi->nwrite)
      break;
    ch = pi->data[pi->nread % PIPESIZE];
    if (copyout(pr->pagetable, addr + i, &ch, 1) == -1) {
      if (i == 0)
        i = -1;
      break;
    }
    pi->nread++;
  }
  wakeup(&pi->nwrite); //DOC: piperead-wakeup
  release(&pi->lock);
  return i;
}
