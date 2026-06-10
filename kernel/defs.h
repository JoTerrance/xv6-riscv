/*
 * AUTOCOMMENT-XV6: comentario agregado automaticamente para explicar el archivo.
 * Archivo: kernel/defs.h
 *
 * Explicacion clara y facil:
 *   - Prototipos globales del kernel.
 *   - Reune declaraciones de funciones compartidas entre modulos para que el compilador conozca firmas y tipos cruzados.
 *
 * Como leer este archivo:
 *   1) Busca las estructuras principales y entiende que estado guardan.
 *   2) Revisa las funciones publicas (las que llaman otros modulos).
 *   3) Luego estudia helpers internos para ver el flujo completo.
 *
 * Nota:
 *   Estos comentarios son una guia pedagogica; la verdad final siempre es el codigo.
 */

// clang-format off
struct buf;
struct context;
struct file;
struct inode;
struct pipe;
struct proc;
struct spinlock;
struct sleeplock;
struct stat;
struct superblock;

// bio.c
void            binit(void);
// AUTOCOMMENT-FUNC-PROTO: binit -> inicializa estado de kernel antes de que el resto del modulo lo use.
struct buf*     bread(uint, uint);
void            brelse(struct buf*);
// AUTOCOMMENT-FUNC-PROTO: brelse -> declaracion publica del modulo de kernel; su implementacion define validaciones y efectos.
void            bwrite(struct buf*);
void            bpin(struct buf*);
// AUTOCOMMENT-FUNC-PROTO: bpin -> declaracion publica del modulo de kernel; su implementacion define validaciones y efectos.
void            bunpin(struct buf*);

// console.c
void            consoleinit(void);
// AUTOCOMMENT-FUNC-PROTO: consoleinit -> inicializa estado de kernel antes de que el resto del modulo lo use.
void            consoleintr(int);
void            consputc(int);
// AUTOCOMMENT-FUNC-PROTO: consputc -> declaracion publica del modulo de kernel; su implementacion define validaciones y efectos.

// exec.c
int             kexec(char*, char**);
// AUTOCOMMENT-FUNC-PROTO: kexec -> declaracion publica del modulo de kernel; su implementacion define validaciones y efectos.

// file.c
struct file*    filealloc(void);
// AUTOCOMMENT-FUNC-PROTO: filealloc -> reserva recursos de kernel y devuelve referencia lista para usar.
void            fileclose(struct file*);
struct file*    filedup(struct file*);
// AUTOCOMMENT-FUNC-PROTO: filedup -> declaracion publica del modulo de kernel; su implementacion define validaciones y efectos.
void            fileinit(void);
int             fileread(struct file*, uint64, int n);
// AUTOCOMMENT-FUNC-PROTO: fileread -> declaracion publica del modulo de kernel; su implementacion define validaciones y efectos.
int             filestat(struct file*, uint64 addr);
int             filewrite(struct file*, uint64, int n);
// AUTOCOMMENT-FUNC-PROTO: filewrite -> declaracion publica del modulo de kernel; su implementacion define validaciones y efectos.

// fs.c
void            fsinit(int);
// AUTOCOMMENT-FUNC-PROTO: fsinit -> inicializa estado de kernel antes de que el resto del modulo lo use.
int             dirlink(struct inode*, char*, uint);
struct inode*   dirlookup(struct inode*, char*, uint*);
// AUTOCOMMENT-FUNC-PROTO: dirlookup -> declaracion publica del modulo de kernel; su implementacion define validaciones y efectos.
struct inode*   ialloc(uint, short);
struct inode*   idup(struct inode*);
// AUTOCOMMENT-FUNC-PROTO: idup -> declaracion publica del modulo de kernel; su implementacion define validaciones y efectos.
void            iinit();
void            ilock(struct inode*);
// AUTOCOMMENT-FUNC-PROTO: ilock -> declaracion publica del modulo de kernel; su implementacion define validaciones y efectos.
void            iput(struct inode*);
void            iunlock(struct inode*);
// AUTOCOMMENT-FUNC-PROTO: iunlock -> declaracion publica del modulo de kernel; su implementacion define validaciones y efectos.
void            iunlockput(struct inode*);
void            iupdate(struct inode*);
// AUTOCOMMENT-FUNC-PROTO: iupdate -> declaracion publica del modulo de kernel; su implementacion define validaciones y efectos.
int             namecmp(const char*, const char*);
struct inode*   namei(char*);
// AUTOCOMMENT-FUNC-PROTO: namei -> declaracion publica del modulo de kernel; su implementacion define validaciones y efectos.
struct inode*   nameiparent(char*, char*);
int             readi(struct inode*, int, uint64, uint, uint);
// AUTOCOMMENT-FUNC-PROTO: readi -> declaracion publica del modulo de kernel; su implementacion define validaciones y efectos.
void            stati(struct inode*, struct stat*);
int             writei(struct inode*, int, uint64, uint, uint);
// AUTOCOMMENT-FUNC-PROTO: writei -> declaracion publica del modulo de kernel; su implementacion define validaciones y efectos.
void            itrunc(struct inode*);
void            ireclaim(int);
// AUTOCOMMENT-FUNC-PROTO: ireclaim -> declaracion publica del modulo de kernel; su implementacion define validaciones y efectos.

// kalloc.c
void*           kalloc(void);
// AUTOCOMMENT-FUNC-PROTO: kalloc -> reserva recursos de kernel y devuelve referencia lista para usar.
void            kfree(void *);
void            kinit(void);
// AUTOCOMMENT-FUNC-PROTO: kinit -> inicializa estado de kernel antes de que el resto del modulo lo use.

// log.c
void            initlog(int, struct superblock*);
// AUTOCOMMENT-FUNC-PROTO: initlog -> declaracion publica del modulo de kernel; su implementacion define validaciones y efectos.
void            log_write(struct buf*);
void            begin_op(void);
// AUTOCOMMENT-FUNC-PROTO: begin_op -> declaracion publica del modulo de kernel; su implementacion define validaciones y efectos.
void            end_op(void);

// pipe.c
int             pipealloc(struct file**, struct file**);
// AUTOCOMMENT-FUNC-PROTO: pipealloc -> reserva recursos de kernel y devuelve referencia lista para usar.
void            pipeclose(struct pipe*, int);
int             piperead(struct pipe*, uint64, int);
// AUTOCOMMENT-FUNC-PROTO: piperead -> declaracion publica del modulo de kernel; su implementacion define validaciones y efectos.
int             pipewrite(struct pipe*, uint64, int);

// printf.c
int             printf(char*, ...) __attribute__ ((format (printf, 1, 2)));
// AUTOCOMMENT-FUNC-PROTO: printf -> declaracion publica del modulo de kernel; su implementacion define validaciones y efectos.
void            panic(char*) __attribute__((noreturn));
void            printfinit(void);
// AUTOCOMMENT-FUNC-PROTO: printfinit -> inicializa estado de kernel antes de que el resto del modulo lo use.

// proc.c
int             cpuid(void);
// AUTOCOMMENT-FUNC-PROTO: cpuid -> declaracion publica del modulo de kernel; su implementacion define validaciones y efectos.
void            kexit(int);
int             kfork(void);
// AUTOCOMMENT-FUNC-PROTO: kfork -> declaracion publica del modulo de kernel; su implementacion define validaciones y efectos.
int             growproc(int);
void            proc_mapstacks(pagetable_t);
// AUTOCOMMENT-FUNC-PROTO: proc_mapstacks -> declaracion publica del modulo de kernel; su implementacion define validaciones y efectos.
pagetable_t     proc_pagetable(struct proc *);
void            proc_freepagetable(pagetable_t, uint64);
// AUTOCOMMENT-FUNC-PROTO: proc_freepagetable -> declaracion publica del modulo de kernel; su implementacion define validaciones y efectos.
int             kkill(int);
int             killed(struct proc*);
// AUTOCOMMENT-FUNC-PROTO: killed -> declaracion publica del modulo de kernel; su implementacion define validaciones y efectos.
void            setkilled(struct proc*);
struct cpu*     mycpu(void);
// AUTOCOMMENT-FUNC-PROTO: mycpu -> declaracion publica del modulo de kernel; su implementacion define validaciones y efectos.
struct proc*    myproc();
void            procinit(void);
// AUTOCOMMENT-FUNC-PROTO: procinit -> inicializa estado de kernel antes de que el resto del modulo lo use.
void            scheduler(void) __attribute__((noreturn));
void            sched(void);
// AUTOCOMMENT-FUNC-PROTO: sched -> declaracion publica del modulo de kernel; su implementacion define validaciones y efectos.
void            sleep(void*, struct spinlock*);
void            userinit(void);
// AUTOCOMMENT-FUNC-PROTO: userinit -> inicializa estado de kernel antes de que el resto del modulo lo use.
int             kwait(uint64);
void            wakeup(void*);
// AUTOCOMMENT-FUNC-PROTO: wakeup -> declaracion publica del modulo de kernel; su implementacion define validaciones y efectos.
void            yield(void);
int             either_copyout(int user_dst, uint64 dst, void *src, uint64 len);
// AUTOCOMMENT-FUNC-PROTO: either_copyout -> declaracion publica del modulo de kernel; su implementacion define validaciones y efectos.
int             either_copyin(void *dst, int user_src, uint64 src, uint64 len);
void            procdump(void);
// AUTOCOMMENT-FUNC-PROTO: procdump -> declaracion publica del modulo de kernel; su implementacion define validaciones y efectos.

// swtch.S
void            swtch(struct context*, struct context*);
// AUTOCOMMENT-FUNC-PROTO: swtch -> declaracion publica del modulo de kernel; su implementacion define validaciones y efectos.

// spinlock.c
void            acquire(struct spinlock*);
// AUTOCOMMENT-FUNC-PROTO: acquire -> declaracion publica del modulo de kernel; su implementacion define validaciones y efectos.
int             holding(struct spinlock*);
void            initlock(struct spinlock*, char*);
// AUTOCOMMENT-FUNC-PROTO: initlock -> declaracion publica del modulo de kernel; su implementacion define validaciones y efectos.
void            release(struct spinlock*);
void            push_off(void);
// AUTOCOMMENT-FUNC-PROTO: push_off -> declaracion publica del modulo de kernel; su implementacion define validaciones y efectos.
void            pop_off(void);

// sleeplock.c
void            acquiresleep(struct sleeplock*);
// AUTOCOMMENT-FUNC-PROTO: acquiresleep -> declaracion publica del modulo de kernel; su implementacion define validaciones y efectos.
void            releasesleep(struct sleeplock*);
int             holdingsleep(struct sleeplock*);
// AUTOCOMMENT-FUNC-PROTO: holdingsleep -> declaracion publica del modulo de kernel; su implementacion define validaciones y efectos.
void            initsleeplock(struct sleeplock*, char*);

// string.c
int             memcmp(const void*, const void*, uint);
// AUTOCOMMENT-FUNC-PROTO: memcmp -> declaracion publica del modulo de kernel; su implementacion define validaciones y efectos.
void*           memmove(void*, const void*, uint);
void*           memset(void*, int, uint);
// AUTOCOMMENT-FUNC-PROTO: memset -> declaracion publica del modulo de kernel; su implementacion define validaciones y efectos.
char*           safestrcpy(char*, const char*, int);
int             strlen(const char*);
// AUTOCOMMENT-FUNC-PROTO: strlen -> declaracion publica del modulo de kernel; su implementacion define validaciones y efectos.
int             strncmp(const char*, const char*, uint);
char*           strncpy(char*, const char*, int);
// AUTOCOMMENT-FUNC-PROTO: strncpy -> declaracion publica del modulo de kernel; su implementacion define validaciones y efectos.

// syscall.c
void            argint(int, int*);
// AUTOCOMMENT-FUNC-PROTO: argint -> declaracion publica del modulo de kernel; su implementacion define validaciones y efectos.
int             argstr(int, char*, int);
void            argaddr(int, uint64 *);
// AUTOCOMMENT-FUNC-PROTO: argaddr -> declaracion publica del modulo de kernel; su implementacion define validaciones y efectos.
int             fetchstr(uint64, char*, int);
int             fetchaddr(uint64, uint64*);
// AUTOCOMMENT-FUNC-PROTO: fetchaddr -> declaracion publica del modulo de kernel; su implementacion define validaciones y efectos.
void            syscall();

// trap.c
extern uint     ticks;
void            trapinit(void);
// AUTOCOMMENT-FUNC-PROTO: trapinit -> inicializa estado de kernel antes de que el resto del modulo lo use.
void            trapinithart(void);
extern struct spinlock tickslock;
void            prepare_return(void);
// AUTOCOMMENT-FUNC-PROTO: prepare_return -> declaracion publica del modulo de kernel; su implementacion define validaciones y efectos.

// uart.c
void            uartinit(void);
// AUTOCOMMENT-FUNC-PROTO: uartinit -> inicializa estado de kernel antes de que el resto del modulo lo use.
void            uartintr(void);
void            uartwrite(char [], int);
// AUTOCOMMENT-FUNC-PROTO: uartwrite -> declaracion publica del modulo de kernel; su implementacion define validaciones y efectos.
void            uartputc_sync(int);

// vm.c
void            kvminit(void);
// AUTOCOMMENT-FUNC-PROTO: kvminit -> inicializa estado de kernel antes de que el resto del modulo lo use.
void            kvminithart(void);
void            kvmmap(pagetable_t, uint64, uint64, uint64, int);
// AUTOCOMMENT-FUNC-PROTO: kvmmap -> declaracion publica del modulo de kernel; su implementacion define validaciones y efectos.
int             mappages(pagetable_t, uint64, uint64, uint64, int);
pagetable_t     uvmcreate(void);
// AUTOCOMMENT-FUNC-PROTO: uvmcreate -> declaracion publica del modulo de kernel; su implementacion define validaciones y efectos.
uint64          uvmalloc(pagetable_t, uint64, uint64, int);
uint64          uvmdealloc(pagetable_t, uint64, uint64);
// AUTOCOMMENT-FUNC-PROTO: uvmdealloc -> declaracion publica del modulo de kernel; su implementacion define validaciones y efectos.
int             uvmcopy(pagetable_t, pagetable_t, uint64);
void            uvmfree(pagetable_t, uint64);
// AUTOCOMMENT-FUNC-PROTO: uvmfree -> declaracion publica del modulo de kernel; su implementacion define validaciones y efectos.
void            uvmunmap(pagetable_t, uint64, uint64, int);
void            uvmclear(pagetable_t, uint64);
// AUTOCOMMENT-FUNC-PROTO: uvmclear -> declaracion publica del modulo de kernel; su implementacion define validaciones y efectos.
pte_t *         walk(pagetable_t, uint64, int);
uint64          walkaddr(pagetable_t, uint64);
// AUTOCOMMENT-FUNC-PROTO: walkaddr -> declaracion publica del modulo de kernel; su implementacion define validaciones y efectos.
int             copyout(pagetable_t, uint64, char *, uint64);
int             copyin(pagetable_t, char *, uint64, uint64);
// AUTOCOMMENT-FUNC-PROTO: copyin -> declaracion publica del modulo de kernel; su implementacion define validaciones y efectos.
int             copyinstr(pagetable_t, char *, uint64, uint64);
int             ismapped(pagetable_t, uint64);
// AUTOCOMMENT-FUNC-PROTO: ismapped -> declaracion publica del modulo de kernel; su implementacion define validaciones y efectos.
uint64          vmfault(pagetable_t, uint64, int);

// plic.c
void            plicinit(void);
// AUTOCOMMENT-FUNC-PROTO: plicinit -> inicializa estado de kernel antes de que el resto del modulo lo use.
void            plicinithart(void);
int             plic_claim(void);
// AUTOCOMMENT-FUNC-PROTO: plic_claim -> declaracion publica del modulo de kernel; su implementacion define validaciones y efectos.
void            plic_complete(int);

// virtio_disk.c
void            virtio_disk_init(void);
// AUTOCOMMENT-FUNC-PROTO: virtio_disk_init -> inicializa estado de kernel antes de que el resto del modulo lo use.
void            virtio_disk_rw(struct buf *, int);
void            virtio_disk_intr(void);
// AUTOCOMMENT-FUNC-PROTO: virtio_disk_intr -> declaracion publica del modulo de kernel; su implementacion define validaciones y efectos.

// number of elements in fixed-size array
#define NELEM(x) (sizeof(x) / sizeof((x)[0]))
