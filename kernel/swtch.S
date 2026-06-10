# AUTOCOMMENT-XV6 (swtch.S)
#
# Cambio de contexto minimo entre dos hilos de kernel.
#
# Firma:
#   void swtch(struct context *old, struct context *new);
#
# Convencion:
# - a0 apunta al contexto donde guardar el estado actual.
# - a1 apunta al contexto desde donde cargar el estado siguiente.
# - Solo guarda registros callee-saved + ra/sp (los necesarios para retomar).


.globl swtch
swtch:
        # Guarda contexto actual en *old.
        sd ra, 0(a0)
        sd sp, 8(a0)
        sd s0, 16(a0)
        sd s1, 24(a0)
        sd s2, 32(a0)
        sd s3, 40(a0)
        sd s4, 48(a0)
        sd s5, 56(a0)
        sd s6, 64(a0)
        sd s7, 72(a0)
        sd s8, 80(a0)
        sd s9, 88(a0)
        sd s10, 96(a0)
        sd s11, 104(a0)

        # Carga contexto destino desde *new.
        ld ra, 0(a1)
        ld sp, 8(a1)
        ld s0, 16(a1)
        ld s1, 24(a1)
        ld s2, 32(a1)
        ld s3, 40(a1)
        ld s4, 48(a1)
        ld s5, 56(a1)
        ld s6, 64(a1)
        ld s7, 72(a1)
        ld s8, 80(a1)
        ld s9, 88(a1)
        ld s10, 96(a1)
        ld s11, 104(a1)
        
        # Retorna al ra del contexto nuevo.
        ret

	
