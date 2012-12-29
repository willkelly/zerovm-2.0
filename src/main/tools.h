/*
 * all macros, definitions and small functions which
 * not fit to other header and often used are here
 *
 * todo(d'b): split into macro functions and zvm setup values
 *  Created on: Jun 17, 2012
 *      Author: d'b
 */

#ifndef TOOLS_H_
#define TOOLS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <glib.h>
#include <unistd.h>
#include <sys/stat.h>
#include "src/platform/nacl_compiler_annotations.h"
#include "src/main/nacl_exit.h"
#include "src/main/zlog.h"

#define MANIFEST_MAX 0x100000 /* limit for the manifest size */
#define KEYWORD_SIZE_MAX 256
#define BIG_ENOUGH_SPACE 65536 /* ..size of the biggest temporary variable */
#define BIG_ENOUGH_STRING 1024 /* ..size of the random biggest string */
#define INT32_STRLEN (11) /* enough space to place maximum int32 value + '\0' */
#define SIGNAL_STRLEN (64) /* enough space to place zerovm state message */

#undef MIN /* prevent conflict with glib.h*/
#undef MAX /* prevent conflict with glib.h*/
#undef SHOWID
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define SHOWID printf("%s: %d, %s\n", __FILE__, __LINE__, __func__)

/* safe strlen(). NULL can be used. return 0 for NULL */
static INLINE size_t my_strlen(const char *str)
{
  return (str == NULL) ? 0 : strlen(str);
}
#define STRLEN(str) my_strlen(str)

/* safe atoi(). NULL can be used. return 0 for NULL */
static INLINE int64_t my_atoi(const char *str)
{
  return (str == NULL) ? 0 : g_ascii_strtoll(str, NULL, 10);
}
#define ATOI(str) my_atoi(str)

/* safe strcpy(). NULL can be used. return NULL if dst or src is NULL */
static INLINE void *my_strcpy(char *dst, const char *src)
{
  return (src == NULL || dst == NULL) ? NULL : strcpy(dst, src);
}
#define STRCPY(dst, src) my_strcpy((dst), (src))

/* safe memcpy(). NULL can be used. return NULL if copy failed */
static INLINE void *my_memcpy(void *dst, const void *src, size_t size)
{
  return (dst == NULL || src == NULL) ? NULL : memcpy(dst, src, size);
}
#define MEMCPY(dst, src, cnt) my_memcpy((dst), (src), (cnt))

/* safe strcmp(). NULL can be used. return -1 if failed */
static INLINE int my_strcmp(const char *a, const char *b)
{
  return (a == NULL || b == NULL) ? -1 : strcmp(a, b);
}
#define STRCMP(a, b) my_strcmp((a), (b))

/* safe streq(). NULL can be used. return -1 if failed */
static INLINE int my_streq(const char *a, const char *b)
{
  return STRCMP(a, b) == 0;
}
#define STREQ(a, b) my_streq((a), (b))

/* safe strneq(). NULL can be used. return -1 if failed */
static INLINE int my_strneq(const char *a, const char *b)
{
  return !STREQ(a, b);
}
#define STRNEQ(a, b) my_strneq((a), (b))

/* return size of given file or negative error code */
static INLINE int64_t GetFileSize(const char *name)
{
  struct stat fs;
  int handle = open(name, O_RDONLY);
  return fstat(handle, &fs), close(handle) ? -1 : fs.st_size;
}

#endif /* TOOLS_H_ */