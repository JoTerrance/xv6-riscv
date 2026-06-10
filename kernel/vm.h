/*
 * AUTOCOMMENT-XV6: comentario agregado automaticamente para explicar el archivo.
 * Archivo: kernel/vm.h
 *
 * Explicacion clara y facil:
 *   - Declaraciones de memoria virtual.
 *   - Prototipos y tipos compartidos para operaciones de paginacion y mapeo.
 *
 * Como leer este archivo:
 *   1) Busca las estructuras principales y entiende que estado guardan.
 *   2) Revisa las funciones publicas (las que llaman otros modulos).
 *   3) Luego estudia helpers internos para ver el flujo completo.
 *
 * Nota:
 *   Estos comentarios son una guia pedagogica; la verdad final siempre es el codigo.
 */

#define SBRK_EAGER 1
#define SBRK_LAZY  2
