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

//*********************************************************************************************
//				  			  	   Peephole System
// 						   		Written by A.R.Hartley
//*********************************************************************************************

#include "compile.h"


//****************************************
//		  Peephole Optimizer
//****************************************

#if 0

void Peeper(uchar *ip, uchar *ip_end)
{
	OpcodeInfo op[4];
	uchar *start_ip = ip;
	int n;
	int op_count = 0;
	
	for (n=0;n<4;n++)
	{
		if (ip > ip_end)
			break;

		ip = DecodeOpcode(&op[n], ip);
		op_count++;
	}

	if (op_count != 4)
		return;


	// Double optimizer

	while(1)
	{
/*
	ld r0,r14	
	ld r1,r15	
	ld r14,r0	
	ld r15,r1	
*/
		// All are ldr
		
		if (op[0].op != _LDR)
			break;
		if (op[1].op != _LDR)
			break;
		if (op[2].op != _LDR)
			break;
		if (op[3].op != _LDR)
			break;

		// check r0's
		if (op[0].rd != op[2].rs)
			break;

		// check r1's
		if (op[1].rd != op[3].rs)
			break;

		// check r14's
		if (op[0].rs != op[2].rd)
			break;

		// check r15's
		if (op[1].rs != op[3].rd)
			break;

		*start_ip++ = 0;
		*start_ip++ = 0;
		*start_ip++ = 0;

		*start_ip++ = 0;
		*start_ip++ = 0;
		*start_ip++ = 0;

		*start_ip++ = 0;
		*start_ip++ = 0;
		*start_ip++ = 0;

		*start_ip++ = 0;
		*start_ip++ = 0;
		*start_ip++ = 0;
		return;
	}
	
	
	// Register to register switches
	
	while(1)
	{
/*
		ld r0,r1	
		ld r1,r0	
*/
		// All are ldr

		if (op[0].op != _LDR)
			break;
		if (op[1].op != _LDR)
			break;

		// check r0's
		if (op[0].rd != op[1].rs)
			break;

		// check r1's
		if (op[1].rd != op[0].rs)
			break;

		*start_ip++ = 0;
		*start_ip++ = 0;
		*start_ip++ = 0;

		*start_ip++ = 0;
		*start_ip++ = 0;
		*start_ip++ = 0;
		return;
	}
	
	
	//memset(start_ip, 0, start_ip - ip);
}

#endif



#define INTPART(addr) ((addr) >> 2)
#define BYTEPART(addr) ((addr) & 0x3)

#define RBYTE(a, ret) { \
	int addr = (a);\
	ret = (byte)((mem_ds[INTPART(addr)] >> (BYTEPART(addr) << 3)) & 0x0ff); \
}

#define WBYTE(a, b) {\
	int addr = (a);\
	int shift = BYTEPART(addr) << 3;\
	mem_ds[INTPART(addr)] = (mem_ds[INTPART(addr)] & ~(0x000000ff << shift)) |\
	((((int)b) & 0x0ff) << shift);\
}

#define RINT(addr) mem_ds[INTPART(addr)]
#define WINT(addr, i) mem_ds[INTPART(addr)] = i;
#define RSHORT(a, ret) {\
	int addr = (a);\
	ret = (short)((mem_ds[INTPART(addr)] >> (BYTEPART(addr) << 3)) & (0xffff));\
		}

#define WSHORT(a, s) {\
	int addr = (a);\
	int shift = BYTEPART(addr) << 3;\
	CORE.mem_ds[INTPART(addr)] = (mem_ds[INTPART(addr)] & ~(0x0000ffff << shift)) |\
	((((int)s) & (0xffff)) << shift);\
	}

//#define byte uchar

uchar mem_ds[100];

void test_1(int rs,int rd,int imm)
{
//	RBYTE(rs + imm, rd);
//	x = RSHORT(rs + imm, rd);
//	x = RINT(rs + imm);

	WBYTE(rd + imm, (byte)rs);
//	WSHORT(rd + imm, (short)rs);
//	WINT(rd + imm, RS);
}

/*

ReadByte
{
	return (mem_ds[i0 >> 2] >> ((i0 & 3) << 3)) & 0x0f;
	return rd & 0xff;
}

public static int RBYTE(int addr)
{
	return ((mem_ds[addr>>2] >> ((addr & 3) << 3)) & 0x0ff);
}

public static void WBYTE(int addr, int b)
{
	int shift = (addr & 3) << 3;
	mem_ds[addr >> 2] = (mem_ds[addr >> 2] & ~(0x000000ff << shift)) | ((b & 0x0ff) << shift);
}


*/