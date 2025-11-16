
# REpsp2

PSP2 OS reverse engineering

# Repository organization

```
common/include/					-- Shared definitions
			  hw/					-- Hardware description headers

docs/							-- Doxygen configuration/definitions
	assets/							-- Documentation assets: images/styling/...
	groups_doc/						-- Topic-specific documentation-only content

kernel/							-- Kernel modules
	  include/						-- Kernel-only headers
	  <module name>/				-- Source code of corresponding kernel module

mep/							-- CMeP binaries
   include/							-- CMeP-only headers
   sl/								-- Secure Loader (second_loader) source
   sk/								-- Secure Kernel (secure_kernel) source
   sm/								-- Secure modules
	 <SM name>/							-- Source code of corresponding secure module
```

# Contribution guidelines

## General

Reverse engineering should be based on firmware 4.00 for the most part. Due to
optimizations, retail 3.65 (CEX/TOOL) may sometimes be easier to understand,
so it may be compared to 4.00 and used as part of RE too.

Features present only in older firmwares should be reimplemented based on their
implementation in the lastest publicly available firmware they are present in.

Documenting older behavior for functions is allowed, but only as a `@note`.

## Guessed names

All names are assumed to be guessed, unless documented otherwise thanks to the custom
Doxygen commands listed below. When the source of a name is not obvious, for example
if it comes from a caller module rather than a `kmc`, the source should be indicated
in the PR or commit message.

## Forward declarations

Avoid forward declarations unless necessary. Instead, all structures should be defined and `typedef`'ed at the same time, as in:

```c
typedef struct {
	/* ... */
} StructureName;
```

When structures refer to themselves through pointers, forward declare the structure as a `typedef` with identical tag and type name, then define the structure:

```c
typedef struct SceXXX SceXXX;
struct SceXXX {
	SceXXX *next;
	/* ... */
};
```

## Magic numbers

When magic numbers are used part of structures, declare the magic number as a `#define` right under the field itself:
```c
typedef struct {
	/* ... */
	uint32_t magic;
#	define SCE_XXX_MAGIC (0xAAAAAAAA)
} SceXXX;
```

## Custom commands/annotations

The following custom Doxygen commands are used in this project:

* `@wip`: function prototype/implementation is not complete

* `@returns_scerr`: generic boilerplate indicating the function returns
  `SCE_OK` on success, and a negative error code otherwise

* Export indicators
	* `@export{<library name>,<NID>}` / `@export{<library name>,<old NID>,<new NID>}`:
	  indicates function/variable is exported by library `library name`
	  * When two arguments are provided, the NID is the same in all firmware versions
	  * When three arguments are provided, `old NID` is used in pre-3.63 firmwares and
	    `new NID` is used in 3.63+ firmwares
	  * `NID` should not have `0x` prefix
	* `@additionalExport{<library name>,<NID>}`/`@additionalExport{<library name>,<old NID>,<new NID>}`:
	  **following an `@export` directive**, this annotation can be added to function/variables exported
	  by different libraries. Usage is identical to `@export`.

* Official names indicators
	* `@varok`: indicates variable name is official
		* This applies mainly to static/global variables, but can be added
			to local variables in `.c` if desired

	* `@sfnok`: indicates structure field name is official

	* `@fnnok`: indicates function name is official
		* This also applies to function-like macros

	* `@fnaok{<argument index>[, <arg index>...]}`: indicates the name of function argument(s) with specified index(es) is/are official
		* Index of the first argument is 1
		* This also applies to function-like macros
		* *Implementation detail*: command supports up to 5 indexes
			* *This can be extended if need arises*

# TODO

* should `docs/groups_doc` (and `docs/groups.dox`) be moved to header files?

* How to handle Kernel Boot Loader?
  * It uses the same module source code with different preprocessor defines which control `#ifdef` blocks + some glue logic

* How to handle userland

* Proper `@export{}` annotations
	* This should generate one page per library listing all exports of that library
	* How to handle exports that moved from one library to another?

* Linked list helpers (à la BSD)
  * Mainly used in threadmgr
