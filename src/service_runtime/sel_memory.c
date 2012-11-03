/*
 * Copyright (c) 2011 The Native Client Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

/*
 * NaCl Service Runtime memory allocation code
 * todo(d'b): since zvm only works on linux x86 64 the wrappers bellow
 * useless. remove whole class.
 */
#include <errno.h>
#include <sys/mman.h>
#include <stdio.h>
#include "src/service_runtime/zlog.h"

void NaCl_page_free(void *p, size_t size)
{
  if(p == 0 || size == 0) return;
  ZLOGFAIL(munmap(p, size) == -1, errno, "NaCl_page_free: munmap() failed");
}

/*
 * NaCl_page_alloc_intern_flags
 * d'b: made global function
 */
int NaCl_page_alloc_intern_flags(void **p, size_t size, int map_flags)
{
  void *addr;

  map_flags |= MAP_PRIVATE | MAP_ANONYMOUS;

  ZLOGS(LOG_DEBUG, "mmap(%p, %lX, %#x, %#x, %d, %d)", *p, size, PROT_NONE, map_flags, -1, 0);

  addr = mmap(*p, size, PROT_NONE, map_flags, -1, (off_t)0);
  if(MAP_FAILED == addr) addr = NULL;
  if(NULL != addr) *p = addr;
  return (NULL == addr) ? -ENOMEM : 0;
}

/*
 * This is critical to make the text region non-writable, and the data
 * region read/write but no exec.  Of course, some kernels do not
 * respect the lack of PROT_EXEC.
 */
int NaCl_mprotect(void *addr, size_t len, int prot)
{
  int ret = mprotect(addr, len, prot);

  return ret == -1 ? -errno : ret;
}

int NaCl_madvise(void *start, size_t length, int advice)
{
  int ret = madvise(start, length, advice);

  /* MADV_DONTNEED and MADV_NORMAL are needed */
  return ret == -1 ? -errno : ret;
}
