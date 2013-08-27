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

#include <fstream>
#include <vector>
#define CONFIG_H
#include "helpers/array.h"
#include "helpers/types.h"
#include "crc.h"
#include "deflate/deflate/deflate.h"

typedef std::fstream File;

#define IB (std::ios_base::in | std::ios_base::binary)
#define OB (std::ios_base::out | std::ios_base::binary)

#define KUidCompressionDeflate 0x101f7afc

static const char usage[] =
	"usage: e32hack <input-exe> <output-exe> <new-uid> [-v2]\n";

static int copyHackExe(File& inFile, File& outFile, u32 uid);
static int copyHackApp(File& inFile, File& outFile, u32 uid);

int main(int argc, const char** argv) {
	if(argc != 4 && argc != 5) {
		puts(usage);
		return 1;
	}

	//convert/validate uid
	u32 uid;
	const char* uidString = argv[3];
	for(int i=0; i<8; i++) {
		if(!isxdigit(uidString[i])) {
			printf("Bad UID: '%s'\n", uidString);
			return 1;
		}
	}
	sscanf(uidString, "%x", &uid);

	//open input
	File input(argv[1], IB);
	if(!input) {
		printf("Cannot open input '%s'\n", argv[1]);
		return 1;
	}

	//open output
	File output(argv[2], OB);
	if(!output) {
		printf("Cannot open output '%s'\n", argv[2]);
		return 1;
	}

	bool v2 = false;
	if(argc == 5) if(strcmp(argv[4], "-v2") == 0) {
		printf("S60v2 mode.\n");
		v2 = true;
	}

	//hack
	int res = (v2 ? copyHackApp : copyHackExe)(input, output, uid);
	if(res != 0) {
		output.close();
		remove(argv[2]);
	}
	return res;
}

void MoSyncErrorExit(int errorCode) {
	printf("Fatal error %i\n", errorCode);
	exit(errorCode);
}

int copyHackExe(File& inFile, File& outFile, u32 uid) {
	inFile.seekg(0, std::ios_base::end);
	if(!inFile.good())
		return inFile.rdstate();
	Array<char> buf((size_t)inFile.tellg());
	char* buffer(buf.p());
	inFile.seekg(0, std::ios_base::beg);
	inFile.read(buffer, (int)buf.size());
	if(!inFile.good())
		return inFile.rdstate();

	uint crc;
	uint oldUid;
	memcpy(&oldUid, buffer + 0x8, 4);

	//uids with crc
	memcpy(buffer + 0x8, &uid, 4);
	calcUidCrc(buffer);

	//"secure" uid
	memcpy(buffer + 0x80, &uid, 4);

	//header crc
	crc = 0xc90fdaa2u;	//magic number from Symbian's f32image.h
	memcpy(buffer + 0x14, &crc, 4);
#define HEADER_SIZE 0x9C//0x80//0x7C//
	crc = crc_itu32_calc(buffer, HEADER_SIZE, 0);	//check size. algo should be ok.
	memcpy(buffer + 0x14, &crc, 4);

	//code section: uncompress if needed
	uint* codeSection;
	u32 codeSize = *(u32*)(buffer + 0x30);
	uint csLen = codeSize / sizeof(uint);	//len of cs in uints
	//check compression state
	u32 compressionType = *(u32*)(buffer + 0x1C);
	Array<byte> inflated(0);
	u32 inflatedSize = *(u32*)(buffer + 0x7C);
	if(compressionType == KUidCompressionDeflate) {
		//inflate
		u32 compressedSize = (u32)buf.size() - HEADER_SIZE;
		inflated.resize(inflatedSize);
		printf("Reading %i compressed bytes. Unpack to %i bytes.\n", compressedSize, inflatedSize);
		TBitInput bitInput((byte*)buffer + HEADER_SIZE, 8*compressedSize);
		CInflater* inflater = CInflater::NewLC(bitInput);
		u32 n = inflater->ReadL(inflated.p(), inflatedSize);
		delete inflater;
		if(n != inflatedSize) {
			printf("Compression size mismatch. Wanted 0x%x bytes, got 0x%x.\n", inflatedSize, n);
			return 1;
		}
		//File dump("dump.bin", OB);
		//dump.write((char*)decompressed.p(), codeSize);
		codeSection = (uint*)inflated.p();
	} else if(compressionType == 0) {	//uncompressed
		codeSection = (uint*)(buffer + HEADER_SIZE);
	} else {
		printf("Unknown compression type: 0x%08x\n", compressionType);
		return 1;
	}

	//uid in code: find & change
	for(uint i=0; i<csLen; i++) {
		if(codeSection[i] == oldUid) {
			printf("found uid @ 0x%x\n", HEADER_SIZE + i*4);
			codeSection[i] = uid;
		}
	}

	if(compressionType == KUidCompressionDeflate) {
		//recompress code section
		outFile.write(buffer, HEADER_SIZE);
		u32 res = DeflateCompressCheck((char*)inflated.p(), inflatedSize, outFile);
		printf("Wrote %i compressed bytes from %i.\n", res, inflatedSize);
	} else {
		outFile.write(buffer, (int)buf.size());
	}
	if(!outFile.good())
		return outFile.rdstate();
	return 0;
}


static uint codeCheckSumV2(uint *aPtr, int aSize) {
	uint sum = 0;
	aSize /= 4;
	while (aSize-- > 0)
		sum += *aPtr++;
	return sum;
}

static int copyHackApp(File& inFile, File& outFile, uint uid) {
	inFile.seekg(0, std::ios_base::end);
	if(!inFile.good())
		return inFile.rdstate();
	Array<char> buf((size_t)inFile.tellg());
	char* buffer(buf.p());
	inFile.seekg(0, std::ios_base::beg);
	inFile.read(buffer, (int)buf.size());
	if(!inFile.good())
		return inFile.rdstate();

	uint oldUid;
	memcpy(&oldUid, buffer + 0x8, 4);

	//uids with crc
	memcpy(buffer + 0x8, &uid, 4);
	calcUidCrc(buffer);

	//uid in code: find & change
	uint* codeSection = (uint*)(buffer + 0x20);
	uint csLen = ((uint)buf.size() - 0x20) / sizeof(uint);	//len of cs in uints
	for(uint i=0; i<csLen; i++) {
		if(codeSection[i] == oldUid) {
			printf("found uid @ 0x%x\n", 0x20 + i*4);
			codeSection[i] = uid;
		}
	}

	//code section checksum
	const int codeOffset = ((int*)(buffer))[25];
	const int codeSize = ((int*)(buffer))[12];
	ASSERT(codeOffset + codeSize < (int)buf.size());
	((int*)(buffer))[6] = codeCheckSumV2((uint*)(buffer + codeOffset), codeSize);

	outFile.write(buffer, (uint)buf.size());
	if(!outFile.good())
		return outFile.rdstate();
	return 0;
}
