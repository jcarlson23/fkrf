#ifndef	_SYSCALLS_INTERNAL_H
#define	_SYSCALLS_INTERNAL_H

#include <sys/types.h>
#include <sys/systm.h>
#include <sys/errno.h>
#include <sys/sysproto.h>
#include <sys/sysent.h>
#include <sys/syscall.h>

#include "../config.h"

#define KRF_RNG_NEXT() (krf_rng_state = krf_mulberry32())

#define KRF_SYS_INTERNAL(sys) krf_sys_internal_##sys
#define KRF_DEFINE_INTERNAL_PROTO(sys)  __typeof(sys_##sys) KRF_SYS_INTERNAL(sys)
#define KRF_DEFINE_INTERNAL(sys) KRF_SYS_INTERNAL(sys)

/* Individual syscall files (read.c, write.c) provide these.
 */
#undef KRF_SYS_CALL
#undef KRF_SYS_PARMS
#undef KRF_SYS_PARMSX

#define FAULT(x) KRF_SYS_INTERNAL(_##KRF_SYS_CALL_##x)
#define FAULTX(x) FAULT(x)(KRF_SYS_PARMSX)
#define DEFINE_FAULT(x) static int KRF_DEFINE_INTERNAL(_##KRF_SYS_CALL_##x)(KRF_SYS_PARMS)
#define NFAULTS (sizeof(fault_table) / sizeof(fault_table[0]))

/* Cribbed from the public domain impl:
 * https://gist.github.com/tommyettinger/46a874533244883189143505d203312c
 *
 * TODO(ww): 64 bit would probably be faster; use Thrust instead?
 */
static __inline unsigned int krf_mulberry32(void) {
  unsigned int z = krf_rng_state += 0x6D2B79F5;
  z = (z ^ z >> 15) * (1 | z);
  z ^= z + (z ^ z >> 7) * (61 | z);
  return z ^ z >> 14;
}

#include "internal.gen.h"

#endif	/* !_SYSCALLS_INTERNAL_H */
