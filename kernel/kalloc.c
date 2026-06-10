/*
 * AUTOCOMMENT-XV6: comentario agregado automaticamente para explicar el archivo.
 * Archivo: kernel/kalloc.c
 *
 * Explicacion clara y facil:
 *   - Asignador de paginas fisicas del kernel.
 *   - Entrega y recupera paginas de memoria con una free list protegida por lock para uso de procesos y tablas.
 *
 * Como leer este archivo:
 *   1) Busca las estructuras principales y entiende que estado guardan.
 *   2) Revisa las funciones publicas (las que llaman otros modulos).
 *   3) Luego estudia helpers internos para ver el flujo completo.
 *
 * Nota:
 *   Estos comentarios son una guia pedagogica; la verdad final siempre es el codigo.
 */

// Physical memory allocator, for user processes,
// kernel stacks, page-table pages,
// and pipe buffers. Allocates whole 4096-byte pages.

#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "riscv.h"
#include "defs.h"

void freerange(void *pa_start, void *pa_end);

extern char end[]; // first address after kernel.
                   // defined by kernel.ld.

struct run {
  struct run *next;
};

struct {
  struct spinlock lock;
  struct run *freelist;
} kmem;

void
kinit()
{
/*
 * AUTOCOMMENT-FUNC-DEF: kinit
 * Explicacion facil:
 *   Prepara estado inicial de memoria fisica para que el resto del codigo funcione bien.
 *   Suele crear estructuras base, locks y valores por defecto antes de usarlos.
 */
  initlock(&kmem.lock, "kmem");
  freerange(end, (void *)PHYSTOP);
}

void
freerange(void *pa_start, void *pa_end)
{
/*
 * AUTOCOMMENT-FUNC-DEF: freerange
 * Explicacion facil:
 *   Libera recursos previamente asignados en asignacion de memoria fisica.
 *   Evita fugas y deja estructuras en estado coherente para usos futuros.
 */
  char *p;
  p = (char *)PGROUNDUP((uint64)pa_start);
  for (; p + PGSIZE <= (char *)pa_end; p += PGSIZE)
    kfree(p);
}

// Free the page of physical memory pointed at by pa,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void
kfree(void *pa)
{
/*
 * AUTOCOMMENT-FUNC-DEF: kfree
 * Explicacion facil:
 *   Libera recursos previamente asignados en asignacion de memoria fisica.
 *   Evita fugas y deja estructuras en estado coherente para usos futuros.
 */
  struct run *r;

  if (((uint64)pa % PGSIZE) != 0 || (char *)pa < end || (uint64)pa >= PHYSTOP)
    panic("kfree");

  // Fill with junk to catch dangling refs.
  memset(pa, 1, PGSIZE);

  r = (struct run *)pa;

  acquire(&kmem.lock);
  r->next = kmem.freelist;
  kmem.freelist = r;
  release(&kmem.lock);
}

// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
void *
kalloc(void)
{
/*
 * AUTOCOMMENT-FUNC-DEF: kalloc
 * Explicacion facil:
 *   Reserva recursos necesarios para asignacion de memoria fisica.
 *   Si no hay espacio suficiente, falla de forma controlada para mantener consistencia.
 */
  struct run *r;

  acquire(&kmem.lock);
  r = kmem.freelist;
  if (r)
    kmem.freelist = r->next;
  release(&kmem.lock);

  if (r)
    memset((char *)r, 5, PGSIZE); // fill with junk
  return (void *)r;
}
