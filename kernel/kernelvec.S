        # AUTOCOMMENT-XV6 (kernelvec.S)
        #
        # Entrada de traps cuando ya estamos en modo supervisor.
        # Flujo:
        #   1) Guardar registros volatiles en el stack de kernel.
        #   2) Llamar a kerneltrap() (C) para despachar causa.
        #   3) Restaurar contexto y volver con sret.
        #
.globl kerneltrap
.globl kernelvec
.align 4
kernelvec:
        # Reserva 256 bytes para marco temporal de registros.
        addi sp, sp, -256

        # Guarda registros caller-saved.
        # No se guarda sp (ya se usa como base del marco) ni tp
        # porque tp contiene hartid y puede cambiar tras scheduler.
        sd ra, 0(sp)
        # sd sp, 8(sp)
        sd gp, 16(sp)
        # sd tp, 24(sp)
        sd t0, 32(sp)
        sd t1, 40(sp)
        sd t2, 48(sp)
        sd a0, 72(sp)
        sd a1, 80(sp)
        sd a2, 88(sp)
        sd a3, 96(sp)
        sd a4, 104(sp)
        sd a5, 112(sp)
        sd a6, 120(sp)
        sd a7, 128(sp)
        sd t3, 216(sp)
        sd t4, 224(sp)
        sd t5, 232(sp)
        sd t6, 240(sp)

        # Delega decision de trap/interrupcion a C.
        call kerneltrap

        # Restaura registros salvados.
        ld ra, 0(sp)
        # ld sp, 8(sp)
        ld gp, 16(sp)
        # not tp (contains hartid), in case we moved CPUs
        ld t0, 32(sp)
        ld t1, 40(sp)
        ld t2, 48(sp)
        ld a0, 72(sp)
        ld a1, 80(sp)
        ld a2, 88(sp)
        ld a3, 96(sp)
        ld a4, 104(sp)
        ld a5, 112(sp)
        ld a6, 120(sp)
        ld a7, 128(sp)
        ld t3, 216(sp)
        ld t4, 224(sp)
        ld t5, 232(sp)
        ld t6, 240(sp)

        # Libera marco temporal.
        addi sp, sp, 256

        # Retorna al punto interrumpido (kernel o user segun CSRs).
        sret
