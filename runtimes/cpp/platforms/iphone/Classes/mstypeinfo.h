/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#if 0
#include <base/Syscall.h>
#else
#define memset __memset
#define memcpy __memcpy
#define strcpy __strcpy
#define strcmp __strcmp
#define SYSCALL(type, nargs) type nargs __attribute__((visibility("default")))
#define __IPHONE__
#include <helpers/cpp_defs.h>
#include <helpers/cpp_maapi.h>
#endif

#undef SYSCALL

#define SYSCALL_IMPL(x) ::x
//#define SYSCALL_IMPL(x) x

#define RINT(xx) 		*(int*)(mem_ds + (xx))
#define RSHORT(xx) 		*(unsigned short*)(mem_ds + (xx))
#define RBYTE(xx) 		*(mem_ds + (xx))

#define WINT(xx,yy)		RINT(xx) = yy
#define WSHORT(xx,yy)	RSHORT(xx) = yy
#define WBYTE(xx,yy)	RBYTE(xx) = yy

//#define SXSHORT(xx) (int)((short)(xx))
//#define SXBYTE(xx) (int)((char)(xx))
#define SXSHORT(xx) ((((xx) & 0x8000) == 0) ? ((xx) & 0xFFFF) : ((xx) | ~0xFFFF))
#define SXBYTE(xx) ((((xx) & 0x80) == 0) ? ((xx) & 0xFF) : ((xx) | ~0xFF))

#define SYSCALL(name)	wrap_##name

void MoSyncDiv0();

extern int sp;
int __dbl_high;

extern unsigned char* mem_ds;

#include "syscall_static_cpp.h"

unsigned char* CppInitReadData(const char* file, int fileSize, int mallocSize);

//#define byte char