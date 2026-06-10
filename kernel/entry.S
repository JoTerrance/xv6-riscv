        # AUTOCOMMENT-XV6 (entry.S)
        #
        # Este archivo es el primer codigo que ejecuta cada hart al arrancar.
        # Objetivo: preparar un stack valido y saltar a start() en C.
        #
        # qemu -kernel carga el kernel en 0x80000000.
        # kernel.ld ubica este bloque exactamente en esa direccion.
.section .text
.global _entry
_entry:
        # Inicializa stack para ejecutar codigo C.
        # stack0 (en start.c) reserva una pagina por CPU.
        # Formula:
        #   sp = stack0 + (hartid + 1) * 4096
        # El +1 evita que el hart 0 use direccion base nula del arreglo.
        la sp, stack0
        li a0, 1024*4
        csrr a1, mhartid
        addi a1, a1, 1
        mul a0, a0, a1
        add sp, sp, a0
        # Salta al arranque de alto nivel del kernel.
        call start
spin:
        # Si start() retornara (no deberia), queda girando aqui.
        j spin
