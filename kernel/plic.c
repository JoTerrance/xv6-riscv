/*
 * AUTOCOMMENT-XV6: comentario agregado automaticamente para explicar el archivo.
 * Archivo: kernel/plic.c
 *
 * Explicacion clara y facil:
 *   - Controlador de interrupciones PLIC.
 *   - Inicializa y configura prioridades/habilitacion de interrupciones externas para cada hart de RISC-V.
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
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "defs.h"

//
// the riscv Platform Level Interrupt Controller (PLIC).
//

void
plicinit(void)
{
/*
 * AUTOCOMMENT-FUNC-DEF: plicinit
 * Explicacion facil:
 *   plicinit cumple una tarea puntual dentro de modulo.
 *   Para entenderla rapido, mira que recibe, que valida y que efecto deja al terminar.
 */
  // set desired IRQ priorities non-zero (otherwise disabled).
  *(uint32 *)(PLIC + UART0_IRQ * 4) = 1;
  *(uint32 *)(PLIC + VIRTIO0_IRQ * 4) = 1;
}

void
plicinithart(void)
{
/*
 * AUTOCOMMENT-FUNC-DEF: plicinithart
 * Explicacion facil:
 *   plicinithart cumple una tarea puntual dentro de modulo.
 *   Para entenderla rapido, mira que recibe, que valida y que efecto deja al terminar.
 */
  int hart = cpuid();

  // set enable bits for this hart's S-mode
  // for the uart and virtio disk.
  *(uint32 *)PLIC_SENABLE(hart) = (1 << UART0_IRQ) | (1 << VIRTIO0_IRQ);

  // set this hart's S-mode priority threshold to 0.
  *(uint32 *)PLIC_SPRIORITY(hart) = 0;
}

// ask the PLIC what interrupt we should serve.
int
plic_claim(void)
{
/*
 * AUTOCOMMENT-FUNC-DEF: plic_claim
 * Explicacion facil:
 *   plic_claim cumple una tarea puntual dentro de modulo.
 *   Para entenderla rapido, mira que recibe, que valida y que efecto deja al terminar.
 */
  int hart = cpuid();
  int irq = *(uint32 *)PLIC_SCLAIM(hart);
  return irq;
}

// tell the PLIC we've served this IRQ.
void
plic_complete(int irq)
{
/*
 * AUTOCOMMENT-FUNC-DEF: plic_complete
 * Explicacion facil:
 *   plic_complete cumple una tarea puntual dentro de modulo.
 *   Para entenderla rapido, mira que recibe, que valida y que efecto deja al terminar.
 */
  int hart = cpuid();
  *(uint32 *)PLIC_SCLAIM(hart) = irq;
}
