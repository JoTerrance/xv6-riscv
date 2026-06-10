/*
 * AUTOCOMMENT-XV6: comentario agregado automaticamente para explicar el archivo.
 * Archivo: kernel/elf.h
 *
 * Explicacion clara y facil:
 *   - Formato ELF usado por exec.
 *   - Define cabeceras y constantes para leer binarios ELF al cargar programas de usuario en memoria.
 *
 * Como leer este archivo:
 *   1) Busca las estructuras principales y entiende que estado guardan.
 *   2) Revisa las funciones publicas (las que llaman otros modulos).
 *   3) Luego estudia helpers internos para ver el flujo completo.
 *
 * Nota:
 *   Estos comentarios son una guia pedagogica; la verdad final siempre es el codigo.
 */

// Format of an ELF executable file

#define ELF_MAGIC 0x464C457FU // "\x7FELF" in little endian

// File header
struct elfhdr {
  uint magic; // must equal ELF_MAGIC
  uchar elf[12];
  ushort type;
  ushort machine;
  uint version;
  uint64 entry;
  uint64 phoff;
  uint64 shoff;
  uint flags;
  ushort ehsize;
  ushort phentsize;
  ushort phnum;
  ushort shentsize;
  ushort shnum;
  ushort shstrndx;
};

// Program section header
struct proghdr {
  uint32 type;
  uint32 flags;
  uint64 off;
  uint64 vaddr;
  uint64 paddr;
  uint64 filesz;
  uint64 memsz;
  uint64 align;
};

// Values for Proghdr type
#define ELF_PROG_LOAD 1

// Flag bits for Proghdr flags
#define ELF_PROG_FLAG_EXEC  1
#define ELF_PROG_FLAG_WRITE 2
#define ELF_PROG_FLAG_READ  4
