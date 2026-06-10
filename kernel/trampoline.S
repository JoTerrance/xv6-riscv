        # AUTOCOMMENT-XV6 (trampoline.S)
        #
        # Codigo de transicion user <-> kernel a muy bajo nivel.
        #
        # Idea clave:
        # Esta pagina se mapea en la MISMA direccion virtual (TRAMPOLINE)
        # tanto en tabla de usuario como de kernel. Asi el CPU puede seguir
        # ejecutando instrucciones aqui durante el cambio de satp.
        #
        # Fases:
        #   uservec  : entra desde user, salva registros, cambia a pagetable kernel.
        #   userret  : vuelve a pagetable user, restaura registros y hace sret.

#include "riscv.h"
#include "memlayout.h"

.section trampsec
.globl trampoline
.globl usertrap
trampoline:
.align 4
.globl uservec
uservec:    
	# Entrada desde user mode:
        # ya estamos en supervisor, pero todavia con pagetable de usuario.

        # Guarda a0 en sscratch para reutilizar a0 como puntero a TRAPFRAME.
        csrw sscratch, a0

        # TRAPFRAME es VA fija en todos los procesos,
        # pero cada uno mapea su propia memoria fisica.
        li a0, TRAPFRAME
        
        # Guarda todo el contexto user en trapframe.
        sd ra, 40(a0)
        sd sp, 48(a0)
        sd gp, 56(a0)
        sd tp, 64(a0)
        sd t0, 72(a0)
        sd t1, 80(a0)
        sd t2, 88(a0)
        sd s0, 96(a0)
        sd s1, 104(a0)
        sd a1, 120(a0)
        sd a2, 128(a0)
        sd a3, 136(a0)
        sd a4, 144(a0)
        sd a5, 152(a0)
        sd a6, 160(a0)
        sd a7, 168(a0)
        sd s2, 176(a0)
        sd s3, 184(a0)
        sd s4, 192(a0)
        sd s5, 200(a0)
        sd s6, 208(a0)
        sd s7, 216(a0)
        sd s8, 224(a0)
        sd s9, 232(a0)
        sd s10, 240(a0)
        sd s11, 248(a0)
        sd t3, 256(a0)
        sd t4, 264(a0)
        sd t5, 272(a0)
        sd t6, 280(a0)

	# save the user a0 in p->trapframe->a0
        csrr t0, sscratch
        sd t0, 112(a0)

        # Cambia a stack de kernel del proceso actual.
        ld sp, 8(a0)

        # Restaura hartid en tp.
        ld tp, 32(a0)

        # Carga puntero a usertrap() (handler C).
        ld t0, 16(a0)

        # Carga satp de kernel.
        ld t1, 0(a0)

        # Ordena memoria antes del cambio de pagetable.
        sfence.vma zero, zero

        # Instala pagetable de kernel.
        csrw satp, t1

        # Limpia TLB de entradas potencialmente obsoletas.
        sfence.vma zero, zero

        # Salta al handler C.
        jalr t0

.globl userret
userret:
        # Vuelta desde usertrap(): a0 trae satp de usuario.

        # Activa tabla de paginas de usuario.
        sfence.vma zero, zero
        csrw satp, a0
        sfence.vma zero, zero

        li a0, TRAPFRAME

        # Restaura todos los registros salvo a0.
        ld ra, 40(a0)
        ld sp, 48(a0)
        ld gp, 56(a0)
        ld tp, 64(a0)
        ld t0, 72(a0)
        ld t1, 80(a0)
        ld t2, 88(a0)
        ld s0, 96(a0)
        ld s1, 104(a0)
        ld a1, 120(a0)
        ld a2, 128(a0)
        ld a3, 136(a0)
        ld a4, 144(a0)
        ld a5, 152(a0)
        ld a6, 160(a0)
        ld a7, 168(a0)
        ld s2, 176(a0)
        ld s3, 184(a0)
        ld s4, 192(a0)
        ld s5, 200(a0)
        ld s6, 208(a0)
        ld s7, 216(a0)
        ld s8, 224(a0)
        ld s9, 232(a0)
        ld s10, 240(a0)
        ld s11, 248(a0)
        ld t3, 256(a0)
        ld t4, 264(a0)
        ld t5, 272(a0)
        ld t6, 280(a0)

	# Restaura a0 original de usuario.
        ld a0, 112(a0)
        
        # Retorna a user mode en sepc configurado por prepare_return().
        sret
