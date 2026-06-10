/*
 * AUTOCOMMENT-XV6: comentario agregado automaticamente para explicar el archivo.
 * Archivo: kernel/log.c
 *
 * Explicacion clara y facil:
 *   - Logging transaccional del sistema de archivos.
 *   - Asegura consistencia ante fallos registrando cambios de bloques en un log antes de aplicarlos de forma definitiva.
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
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "fs.h"
#include "buf.h"

// Simple logging that allows concurrent FS system calls.
//
// A log transaction contains the updates of multiple FS system
// calls. The logging system only commits when there are
// no FS system calls active. Thus there is never
// any reasoning required about whether a commit might
// write an uncommitted system call's updates to disk.
//
// A system call should call begin_op()/end_op() to mark
// its start and end. Usually begin_op() just increments
// the count of in-progress FS system calls and returns.
// But if it thinks the log is close to running out, it
// sleeps until the last outstanding end_op() commits.
//
// The log is a physical re-do log containing disk blocks.
// The on-disk log format:
//   header block, containing block #s for block A, B, C, ...
//   block A
//   block B
//   block C
//   ...
// Log appends are synchronous.

// Contents of the header block, used for both the on-disk header block
// and to keep track in memory of logged block# before commit.
struct logheader {
  int n;
  int block[LOGBLOCKS];
};

struct log {
  struct spinlock lock;
  int start;
  int outstanding; // how many FS sys calls are executing.
  int committing;  // in commit(), please wait.
  int dev;
  struct logheader lh;
};
struct log log;

static void recover_from_log(void);
static void commit();

void
initlog(int dev, struct superblock *sb)
{
/*
 * AUTOCOMMENT-FUNC-DEF: initlog
 * Explicacion facil:
 *   Prepara estado inicial de log del sistema de archivos para que el resto del codigo funcione bien.
 *   Suele crear estructuras base, locks y valores por defecto antes de usarlos.
 */
  if (sizeof(struct logheader) >= BSIZE)
    panic("initlog: too big logheader");

  initlock(&log.lock, "log");
  log.start = sb->logstart;
  log.dev = dev;
  recover_from_log();
}

// Copy committed blocks from log to their home location
static void
install_trans(int recovering)
{
/*
 * AUTOCOMMENT-FUNC-DEF: install_trans
 * Explicacion facil:
 *   Participa en el flujo transaccional del log del FS.
 *   Ayuda a que los cambios sean atomicos y recuperables despues de un fallo.
 */
  int tail;

  for (tail = 0; tail < log.lh.n; tail++) {
    if (recovering) {
      printf("recovering tail %d dst %d\n", tail, log.lh.block[tail]);
    }
    struct buf *lbuf = bread(log.dev, log.start + tail + 1); // read log block
    struct buf *dbuf = bread(log.dev, log.lh.block[tail]);   // read dst
    memmove(dbuf->data, lbuf->data, BSIZE); // copy block to dst
    bwrite(dbuf);                           // write dst to disk
    if (recovering == 0)
      bunpin(dbuf);
    brelse(lbuf);
    brelse(dbuf);
  }
}

// Read the log header from disk into the in-memory log header
static void
read_head(void)
{
/*
 * AUTOCOMMENT-FUNC-DEF: read_head
 * Explicacion facil:
 *   Hace operaciones de entrada/salida de datos dentro de log del sistema de archivos.
 *   Controla limites y sincronizacion para mantener datos correctos y consistentes.
 */
  struct buf *buf = bread(log.dev, log.start);
  struct logheader *lh = (struct logheader *)(buf->data);
  int i;
  log.lh.n = lh->n;
  for (i = 0; i < log.lh.n; i++) {
    log.lh.block[i] = lh->block[i];
  }
  brelse(buf);
}

// Write in-memory log header to disk.
// This is the true point at which the
// current transaction commits.
static void
write_head(void)
{
/*
 * AUTOCOMMENT-FUNC-DEF: write_head
 * Explicacion facil:
 *   Hace operaciones de entrada/salida de datos dentro de log del sistema de archivos.
 *   Controla limites y sincronizacion para mantener datos correctos y consistentes.
 */
  struct buf *buf = bread(log.dev, log.start);
  struct logheader *hb = (struct logheader *)(buf->data);
  int i;
  hb->n = log.lh.n;
  for (i = 0; i < log.lh.n; i++) {
    hb->block[i] = log.lh.block[i];
  }
  bwrite(buf);
  brelse(buf);
}

static void
recover_from_log(void)
{
/*
 * AUTOCOMMENT-FUNC-DEF: recover_from_log
 * Explicacion facil:
 *   Recupera estado tras crash usando la cabecera de log en disco.
 *   Si hay transaccion pendiente, la reinstala para restaurar consistencia.
 */
  read_head();
  install_trans(1); // if committed, copy from log to disk
  log.lh.n = 0;
  write_head(); // clear the log
}

// called at the start of each FS system call.
void
begin_op(void)
{
/*
 * AUTOCOMMENT-FUNC-DEF: begin_op
 * Explicacion facil:
 *   Inicia una operacion de FS protegida por el log transaccional.
 *   Puede dormir si no hay espacio de log suficiente para garantizar atomicidad.
 */
  acquire(&log.lock);
  while (1) {
    if (log.committing) {
      sleep(&log, &log.lock);
    } else if (log.lh.n + (log.outstanding + 1) * MAXOPBLOCKS > LOGBLOCKS) {
      // this op might exhaust log space; wait for commit.
      sleep(&log, &log.lock);
    } else {
      log.outstanding += 1;
      release(&log.lock);
      break;
    }
  }
}

// called at the end of each FS system call.
// commits if this was the last outstanding operation.
void
end_op(void)
{
/*
 * AUTOCOMMENT-FUNC-DEF: end_op
 * Explicacion facil:
 *   Finaliza una operacion de FS y dispara commit si corresponde.
 *   Coordina concurrencia para que el commit ocurra en momento seguro.
 */
  int do_commit = 0;

  acquire(&log.lock);
  log.outstanding -= 1;
  if (log.committing)
    panic("log.committing");
  if (log.outstanding == 0) {
    do_commit = 1;
    log.committing = 1;
  } else {
    // begin_op() may be waiting for log space,
    // and decrementing log.outstanding has decreased
    // the amount of reserved space.
    wakeup(&log);
  }
  release(&log.lock);

  if (do_commit) {
    // call commit w/o holding locks, since not allowed
    // to sleep with locks.
    commit();
    acquire(&log.lock);
    log.committing = 0;
    wakeup(&log);
    release(&log.lock);
  }
}

// Copy modified blocks from cache to log.
static void
write_log(void)
{
/*
 * AUTOCOMMENT-FUNC-DEF: write_log
 * Explicacion facil:
 *   Hace operaciones de entrada/salida de datos dentro de log del sistema de archivos.
 *   Controla limites y sincronizacion para mantener datos correctos y consistentes.
 */
  int tail;

  for (tail = 0; tail < log.lh.n; tail++) {
    struct buf *to = bread(log.dev, log.start + tail + 1); // log block
    struct buf *from = bread(log.dev, log.lh.block[tail]); // cache block
    memmove(to->data, from->data, BSIZE);
    bwrite(to); // write the log
    brelse(from);
    brelse(to);
  }
}

static void
commit()
{
/*
 * AUTOCOMMENT-FUNC-DEF: commit
 * Explicacion facil:
 *   Aplica transaccion: escribe log, marca cabecera y copia al area final.
 *   Luego limpia cabecera para dejar el sistema consistente tras reinicios.
 */
  if (log.lh.n > 0) {
    write_log();      // Write modified blocks from cache to log
    write_head();     // Write header to disk -- the real commit
    install_trans(0); // Now install writes to home locations
    log.lh.n = 0;
    write_head(); // Erase the transaction from the log
  }
}

// Caller has modified b->data and is done with the buffer.
// Record the block number and pin in the cache by increasing refcnt.
// commit()/write_log() will do the disk write.
//
// log_write() replaces bwrite(); a typical use is:
//   bp = bread(...)
//   modify bp->data[]
//   log_write(bp)
//   brelse(bp)
void
log_write(struct buf *b)
{
/*
 * AUTOCOMMENT-FUNC-DEF: log_write
 * Explicacion facil:
 *   Registra en el log un bloque modificado por la transaccion actual.
 *   Evita duplicados del mismo bloque para no desperdiciar espacio de journal.
 */
  int i;

  acquire(&log.lock);
  if (log.lh.n >= LOGBLOCKS)
    panic("too big a transaction");
  if (log.outstanding < 1)
    panic("log_write outside of trans");

  for (i = 0; i < log.lh.n; i++) {
    if (log.lh.block[i] == b->blockno) // log absorption
      break;
  }
  log.lh.block[i] = b->blockno;
  if (i == log.lh.n) { // Add new block to log?
    bpin(b);
    log.lh.n++;
  }
  release(&log.lock);
}
