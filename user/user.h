/*
 * AUTOCOMMENT-XV6: comentario agregado automaticamente para explicar el archivo.
 * Archivo: user/user.h
 *
 * Explicacion clara y facil:
 *   - Interfaz de usuario para syscalls y utilidades.
 *   - Declara funciones de libreria minima y wrappers de llamadas al sistema usados por programas de usuario.
 *
 * Como leer este archivo:
 *   1) Busca las estructuras principales y entiende que estado guardan.
 *   2) Revisa las funciones publicas (las que llaman otros modulos).
 *   3) Luego estudia helpers internos para ver el flujo completo.
 *
 * Nota:
 *   Estos comentarios son una guia pedagogica; la verdad final siempre es el codigo.
 */

#define SBRK_ERROR ((char *)-1)

struct stat;

// system calls
int fork(void);
// AUTOCOMMENT-FUNC-PROTO: fork -> interfaz de syscall/usuario; define contrato de uso y resultado esperado.
int exit(int) __attribute__((noreturn));
int wait(int *);
// AUTOCOMMENT-FUNC-PROTO: wait -> interfaz de syscall/usuario; define contrato de uso y resultado esperado.
int pipe(int *);
int write(int, const void *, int);
// AUTOCOMMENT-FUNC-PROTO: write -> interfaz de syscall/usuario; define contrato de uso y resultado esperado.
int read(int, void *, int);
int close(int);
// AUTOCOMMENT-FUNC-PROTO: close -> interfaz de syscall/usuario; define contrato de uso y resultado esperado.
int kill(int);
int exec(const char *, char **);
// AUTOCOMMENT-FUNC-PROTO: exec -> interfaz de syscall/usuario; define contrato de uso y resultado esperado.
int open(const char *, int);
int mknod(const char *, short, short);
// AUTOCOMMENT-FUNC-PROTO: mknod -> declaracion publica del modulo de programas de usuario; su implementacion define validaciones y efectos.
int unlink(const char *);
int fstat(int fd, struct stat *);
// AUTOCOMMENT-FUNC-PROTO: fstat -> declaracion publica del modulo de programas de usuario; su implementacion define validaciones y efectos.
int link(const char *, const char *);
int mkdir(const char *);
// AUTOCOMMENT-FUNC-PROTO: mkdir -> interfaz de syscall/usuario; define contrato de uso y resultado esperado.
int chdir(const char *);
int dup(int);
// AUTOCOMMENT-FUNC-PROTO: dup -> declaracion publica del modulo de programas de usuario; su implementacion define validaciones y efectos.
int getpid(void);
char *sys_sbrk(int, int);
// AUTOCOMMENT-FUNC-PROTO: sys_sbrk -> declaracion publica del modulo de programas de usuario; su implementacion define validaciones y efectos.
int pause(int);
int uptime(void);
// AUTOCOMMENT-FUNC-PROTO: uptime -> interfaz de syscall/usuario; define contrato de uso y resultado esperado.

// ulib.c
int stat(const char *, struct stat *);
// AUTOCOMMENT-FUNC-PROTO: stat -> declaracion publica del modulo de programas de usuario; su implementacion define validaciones y efectos.
char *strcpy(char *, const char *);
void *memmove(void *, const void *, int);
// AUTOCOMMENT-FUNC-PROTO: memmove -> declaracion publica del modulo de programas de usuario; su implementacion define validaciones y efectos.
char *strchr(const char *, char c);
int strcmp(const char *, const char *);
// AUTOCOMMENT-FUNC-PROTO: strcmp -> declaracion publica del modulo de programas de usuario; su implementacion define validaciones y efectos.
char *gets(char *, int max);
uint strlen(const char *);
// AUTOCOMMENT-FUNC-PROTO: strlen -> declaracion publica del modulo de programas de usuario; su implementacion define validaciones y efectos.
void *memset(void *, int, uint);
int atoi(const char *);
// AUTOCOMMENT-FUNC-PROTO: atoi -> declaracion publica del modulo de programas de usuario; su implementacion define validaciones y efectos.
int memcmp(const void *, const void *, uint);
void *memcpy(void *, const void *, uint);
// AUTOCOMMENT-FUNC-PROTO: memcpy -> declaracion publica del modulo de programas de usuario; su implementacion define validaciones y efectos.
char *sbrk(int);
char *sbrklazy(int);
// AUTOCOMMENT-FUNC-PROTO: sbrklazy -> declaracion publica del modulo de programas de usuario; su implementacion define validaciones y efectos.

// printf.c
void fprintf(int, const char *, ...) __attribute__((format(printf, 2, 3)));
// AUTOCOMMENT-FUNC-PROTO: fprintf -> declaracion publica del modulo de programas de usuario; su implementacion define validaciones y efectos.
void printf(const char *, ...) __attribute__((format(printf, 1, 2)));

// umalloc.c
void *malloc(uint);
// AUTOCOMMENT-FUNC-PROTO: malloc -> declaracion publica del modulo de programas de usuario; su implementacion define validaciones y efectos.
void free(void *);
