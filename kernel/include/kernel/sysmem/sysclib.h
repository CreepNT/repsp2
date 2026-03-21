
#ifndef _SCE_KERNEL_SYSMEM_SYSCLIB_H_
#define _SCE_KERNEL_SYSMEM_SYSCLIB_H_

#include <stddef.h>	// size_t
#include <stdarg.h>	// va_list
#include <scetypes.h>

#pragma region C standard library functions
	// ctype.h
/** @export{SceSysclibForDriver,0021DAF9} */
int tolower(int ch);

/** @export{SceSysclibForDriver,A685DCB1} */
int toupper(int ch);

#define CTYPE_UPPER 		0x01 /* uppercase letter */
#define CTYPE_LOWER 		0x02 /* lowercase letter */
#define CTYPE_DIGIT		0x04 /* digit */
#define CTYPE_SPACE		0x08 /* space */
#define CTYPE_PUNCT		0x10 /* punctuation */
#define CTYPE_CONTROL		0x20 /* control codes (incl. DEL) */
#define CTYPE_HEXDIGIT		0x40 /* hexadecimal digit (A~F/a~f)*/
#define CTYPE_UNK_0x80		0x80 /* never used */

/**
 * Name is guessed from PSP and should be official.
 *
 * @returns bitmask of CTYPE_* flags for the given character
 * @export{SceSysclibForDriver,CDF7F155}
 */
unsigned look_ctype_table(int ch);

//THESE SHOULD BE DOUBLE-CHECKED (need to find more code using them...)
//In old FW, there was NID 0x0614B013 "get_ctype_table()" instead - might be useful
#define __isxx(ch, msk)		((look_ctype_table(ch) & (msk)) != 0)
#define iscntrl(ch)		__isxx(ch, CTYPE_CONTROL)
#define isprint(ch)		__isxx(ch, CTYPE_UNK_0x80 | CTYPE_PUNCT | CTYPE_DIGIT | CTYPE_LOWER | CTYPE_UPPER)
#define isspace(ch)		__isxx(ch, CTYPE_SPACE)
//#define isblank(ch) -- not provided?
//#define isgraph(ch) -- not provided?
#define ispunct(ch)		__isxx(ch, CTYPE_PUNCT) //SHOULD NOT INCLUDE SPACE!
#define isalnum(ch)		__isxx(ch, CTYPE_DIGIT | CTYPE_LOWER | CTYPE_UPPER)
#define isalpha(ch)		__isxx(ch, CTYPE_LOWER | CTYPE_UPPER)
#define isupper(ch)		__isxx(ch, CTYPE_UPPER)
#define islower(ch)		__isxx(ch, CTYPE_LOWER)
#define isdigit(ch)		__isxx(ch, CTYPE_DIGIT)
#define isxdigit(ch)		__isxx(ch, CTYPE_HEXDIGIT | CTYPE_DIGIT)

	//string.h
/** @export{SceSysclibForDriver,0AB9BF5C} */
void *memset(void *dest, int ch, size_t count);

/** @export{SceSysclibForDriver,0B33BC43} */
int strcmp(const char *lhs, const char *rhs);

/** @export{SceSysclibForDriver,12CEE649} */
int memcmp(const void *lhs, const void *rhs, size_t count);

/** @export{SceSysclibForDriver,1304A69D} */
char *strstr(const char *str, const char *substr);

/** @export{SceSysclibForDriver,38463759} */
char* strchr(const char* str, int ch);

/** @export{SceSysclibForDriver,40C88316} */
void *memcpy(void *dest, const void *src, size_t count);

/** @export{SceSysclibForDriver,60DAEA30} */
void *memchr(const void* ptr, int ch, size_t count);

/** @export{SceSysclibForDriver,6CC9C1A1} */
void *memmove(void *dest, const void *src, size_t size);

/** @export{SceSysclibForDriver,6D286146} */
char *strncpy(char *dest, const char *src, size_t count);

/** @export{SceSysclibForDriver,7F0E0835} */
char *strrchr(const char *str, int ch);

/** @export{SceSysclibForDriver,A1D1C32C} */
char *strncat(char *dest, const char *src, size_t count);

/** @export{SceSysclibForDriver,CD4BD884} */
int strnlen(const char *s, size_t maxlen);

/** @export{SceSysclibForDriver,CFC6A9AC} */
int strlen(const char *s);

/** @export{SceSysclibForDriver,F939E83D} */
int memcmp(const void *lhs, const void *rhs, size_t count);

	// stdio.h
/** @export{SceSysclibForDriver,3DDBE2E1} */
int vsnprintf(char *s, size_t n, const char *fmt, va_list arg);

/** @export{SceSysclibForDriver,AE7A8981} */
int snprintf(char *buffer, size_t bufsz, const char *format, ...);

	// stdlib.h
/** @export{SceSysclibForDriver,4E5042DA} */
unsigned long strtoul(const char *str, char **str_end, int base);

/** @export{SceSysclibForDriver,87AAAFA2} */
long long strtoll(const char *str, char **str_end, int base);

/** @export{SceSysclibForDriver,AB77C5AA} */
long strtol(const char *str, char **str_end, int base);
#pragma endregion

#pragma region C library extensions
/** @export{SceSysclibForDriver,12504E09} */
int strlcat(char *dst, const char *src, size_t dstsize);

/** @export{SceSysclibForDriver,7FB4EBEC} */
size_t strlcpy(char *dst, const char *src, size_t dstsize);

/** @export{SceSysclibForDriver,B5A4D745} */
int timingsafe_bcmp(const void *b1, const void *b2, size_t len);
#pragma endregion

#pragma region Checked C library functions
/** @export{SceSysclibForDriver,1A30BB28} */
void *__memset_chk(void *dest, int c, size_t len, size_t destlen);

/** @export{SceSysclibForDriver,224BE33F} */
void *__strlcat_chk(char *dst, const char *src, size_t dstsize, size_t dstlen);

/** @export{SceSysclibForDriver,33EE298B} */
char *__strncat_chk(char *dest, const char *src, size_t count, size_t dstlen);

/** @export{SceSysclibForDriver,35DBB110} */
void *__memmove_chk(void *dest, const void *src, size_t len, size_t destlen);

/** @export{SceSysclibForDriver,545DA5FD} */
char *__strcpy_chk(char *dest, const char *src, size_t destlen);

/** @export{SceSysclibForDriver,7DBE7007} */
int __snprintf_chk(char *str, size_t maxlen, int flag, size_t strlen, const char *format, ...);

/** @export{SceSysclibForDriver,8A0B0815} */
void *__memcpy_chk(void *dest, const void *src, size_t len, size_t destlen);

/** @export{SceSysclibForDriver,96268C53} */
char *__strncpy_chk(char *s1, const char *s2, size_t n, size_t s1len);

/** @export{SceSysclibForDriver,CF86EA38} */
char *__strlcpy_chk(char *s1, const char *s2, size_t n, size_t s1len);

/** @export{SceSysclibForDriver,DE4666F0} */
char *__strcat_chk(char *dest, const char *src, size_t destlen);
#pragma endregion

/**
 * @brief Generic formatted output processing function
 *
 * This functions generates `printf()`-like formatted output from @p fmt + @p ap
 * and invokes the user callback @p cb for each character of the output stream.
 *
 * Name comes from PSP (according to HENkaku wiki) - not confirmed official.
 *
 * @p fmt may contain the following format characters:
 * - TBD
 *
 * @p cb is called two times in addition to each character of the output stream:
 * - with `ch=0x200` before any character of the output stream (start marker)
 * - with `ch=0x201` after all characters of the output stream (end marker)
 *
 * @param cb `putchar()-like` callback invoked for each character of the output stream
 * (+ two additional times with `ch=0x200` and `ch=0x201` - see above)
 * @param argp user argument passed verbatim to @p cb
 * @param fmt format string
 * @param ap argument list
 * @returns number of characters generated in the output stream
 * (not including special calls to @p cb with `ch=0x200` and `ch=0x201`)
 *
 * @note @ref snprintf and @ref vsnprintf are implemented internally on top of
 * of this function so all remarks here also apply to these functions.
 *
 * @export{SceSysclibForDriver,E38E7605}
 */
int __prnt(void (*cb)(void *argp, int ch), void *argp, const char *fmt, va_list ap);

/**
 * Similar to strlcpy(): copies up to `dstsize - 1` characters from `src` to `dst`
 * then NUL-terminates `dst` (except if `dstsize == 0`).
 *
 * The return value is same as strncpy() (i.e., equal to `dst`).
 *
 * @export{SceSysclibForDriver,FE39AEAC}
 */
char *SceSysclibForDriver_FE39AEAC(char *dst, const char *src, size_t dstsize);

#pragma region Stack canary
/** @export{SceSysclibForDriver,99EEBD1F} */
extern unsigned int __stack_chk_guard;

/** @export{SceSysclibForDriver,B997493D} */
noreturn void __stack_chk_fail(void);
#pragma endregion

#pragma region ARM AEABI functions
typedef struct {
	int quot;
	int rem;
} idiv_return;

typedef struct {
	unsigned quot;
	unsigned rem;
} udiv_return;

/* Should come from C stdlib headers */
typedef struct lldiv_t {
	long long quot;
	long long rem;
} lldiv_t;

typedef struct ulldiv_t {
	unsigned long long quot;
	unsigned long long rem;
} ulldiv_t;

/** @export{SceSysclibForDriver,FEE5E751} */
long long __aeabi_lmul(long long, long long);

/** @export{SceSysclibForDriver,7554AB04} */
lldiv_t __aeabi_ldivmod(long long n, long long d);

/** @export{SceSysclibForDriver,9D148CDE} */
ulldiv_t __aeabi_uldivmod(unsigned long long n, unsigned long long d);

/** @export{SceSysclibForDriver,72D31F9D} */
long long __aeabi_llsl(long long, int);

/** @export{SceSysclibForDriver,E46C47E6} */
long long __aeabi_llsr(long long, int);

/** @export{SceSysclibForDriver,1D89F6C0} */
long long __aeabi_lasr(long long, int);

/** @export{SceSysclibForDriver,709077A1} */
int __aeabi_lcmp(long long, long long);

/** @export{SceSysclibForDriver,FE900DE8} */
int __aeabi_ulcmp(unsigned long long, unsigned long long);

/** @export{SceSysclibForDriver,2518CD9E} */
int __aeabi_idiv(int numerator, int denominator);

/** @export{SceSysclibForDriver,A9FF1205} */
unsigned __aeabi_uidiv(unsigned numerator, unsigned denominator);

/** @export{SceSysclibForDriver,AC86B4BA} */
idiv_return __aeabi_idivmod(int numerator, int denominator);

/** @export{SceSysclibForDriver,A46CB7DE} */
udiv_return __aeabi_uidivmod(unsigned numerator, unsigned denominator);
#pragma endregion

#endif /* _SCE_KERNEL_SYSMEM_SYSCLIB_H_ */
