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

#include "WinmobileInjector.h"
#include "Icon.h"
#include "Shared.h"
#include <windows.h>
#include <fcntl.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include "IconFileLoader.h"

using namespace std;

/*
#pragma pack(push, 2)
typedef struct {
	WORD Reserved1;       // reserved, must be 0
	WORD ResourceType;    // type is 1 for icons
	WORD ImageCount;      // number of icons in structure (1)
	BYTE Width;           // icon width (32)
	BYTE Height;          // icon height (32)
	BYTE Colors;          // colors (0 means more than 8 bits per pixel)
	BYTE Reserved2;       // reserved, must be 0
	WORD Planes;          // color planes
	WORD BitsPerPixel;    // bit depth
	DWORD ImageSize;      // size of structure
	WORD ResourceID;      // resource ID
} GROUPICON;
#pragma pack(pop)
*/


#pragma pack( push )
#pragma pack( 2 )
typedef struct
{
   BYTE   bWidth;               // Width, in pixels, of the image
   BYTE   bHeight;              // Height, in pixels, of the image
   BYTE   bColorCount;          // Number of colors in image (0 if >=8bpp)
   BYTE   bReserved;            // Reserved
   WORD   wPlanes;              // Color Planes
   WORD   wBitCount;            // Bits per pixel
   DWORD   dwBytesInRes;         // how many bytes in this resource?
   WORD   nID;                  // the ID
} GRPICONDIRENTRY, *LPGRPICONDIRENTRY;
#pragma pack( pop )


// #pragmas are used here to insure that the structure's
// packing in memory matches the packing of the EXE or DLL.
#pragma pack( push )
#pragma pack( 2 )
typedef struct 
{
   WORD            idReserved;   // Reserved (must be 0)
   WORD            idType;       // Resource type (1 for icons)
   WORD            idCount;      // How many images?
   GRPICONDIRENTRY   idEntries[1]; // The entries for each image
} GRPICONDIR, *LPGRPICONDIR;
#pragma pack( pop )


namespace MoSync 
{
	static void ErrorExit(const char* lpszFunction) GCCATTRIB(noreturn);
	static void ErrorExit(const char* lpszFunction)
	{ 
		// Retrieve the system error message for the last-error code

		LPVOID lpMsgBuf;
		LPVOID lpDisplayBuf;
		DWORD dw = GetLastError(); 

		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			dw,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPSTR) &lpMsgBuf,
			0, NULL );

		// Display the error message and exit the process

		lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, 
			(strlen((LPCSTR)lpMsgBuf) + strlen((LPCSTR)lpszFunction) + 40) * sizeof(char)); 
		sprintf( (LPSTR)lpDisplayBuf,
		         "%s failed with error %lu: %s",
		         lpszFunction,
		         dw,
		         (char*)lpMsgBuf );
		printf( "Error: %s", (char*)lpDisplayBuf );

		LocalFree(lpMsgBuf);
		LocalFree(lpDisplayBuf);
		ExitProcess(dw); 
	}

#ifdef UNICODE
#define T L
#else
#define T
#endif

	static bool InjectMainIcon(const vector<ICON>& icons, const char *exeFileName)
	{
		HANDLE hWhere = BeginUpdateResource(exeFileName, FALSE);
		if ( hWhere == NULL )
			ErrorExit( T"MoSync::InjectMainIcon" );


		for(size_t i = 0; i < icons.size(); i++) 
		{
			BOOL res = UpdateResource(
				hWhere,                         // Handle to executable
				RT_ICON,                        // Resource type - icon
				MAKEINTRESOURCE(1+i),             // Make the id 1
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				(icons[i].data),
				// skip the first 22 bytes because this is the  icon header
				// and directory entry (if the file  contains multiple
				// images, the directory entries will be larger than 22 bytes)
				icons[i].iconDir.idEntries[0].dwBytesInRes // length of buffer
				);
				
			if ( res == FALSE )
				ErrorExit( T"MoSync::InjectMainIcon" );
		}

		// Again, we use this structure for educational purposes.
		// The icon header and directory entries can be read from the
		// file.

		GRPICONDIR *grData;
		size_t dataSize = sizeof(GRPICONDIR)+sizeof(GRPICONDIRENTRY)*icons.size();
		byte *data = new byte[dataSize];
		grData = (GRPICONDIR*)data;
		memset(grData, 0, sizeof(GRPICONDIR));
		grData->idType = 1;
		grData->idCount = (WORD)icons.size();
		
		GRPICONDIRENTRY *grDirEntry = (GRPICONDIRENTRY*)(data+sizeof(GRPICONDIR));

		for(size_t i = 0; i < icons.size(); i++) {
			grDirEntry->nID = 1;
			grDirEntry->bWidth = icons[i].iconDir.idEntries[0].bWidth;
			grDirEntry->bHeight = icons[i].iconDir.idEntries[0].bHeight;
			grDirEntry->wBitCount = icons[i].iconDir.idEntries[0].wBitCount;
			grDirEntry->bColorCount = icons[i].iconDir.idEntries[0].bColorCount;
			grDirEntry->wPlanes = icons[i].iconDir.idEntries[0].wPlanes;
			grDirEntry->dwBytesInRes = sizeof(GRPICONDIR);
			grDirEntry->bReserved = 0;
			grDirEntry++;
		}

		BOOL res = UpdateResource(
			hWhere,
			RT_GROUP_ICON,
			// RT_GROUP_ICON resources contain information about
			// stored icons
			"MAINICON",
			// MAINICON contains information about the application's
			// displayed icon
			//			  MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			data,
			// Pointer to this structure
			(DWORD) dataSize
			);

		if ( res == FALSE )
			ErrorExit( "MoSync::InjectMainIcon" );

		// Perform the update, don't discard changes
		if ( EndUpdateResource(hWhere, FALSE) == FALSE )
			ErrorExit( "MoSync::InjectMainIcon" );		

		delete data;
		return true;
	}

	void WinmobileInjector::inject(const Icon* icon, const std::map<std::string, std::string>& params) {
		//VP(largeSize);
		//VP(smallSize);
		//VP(dst);
		string largeSize = verifyParameter(params, "largeSize");
		string smallSize = verifyParameter(params, "smallSize");
		string dst = verifyParameter(params, "dst");
		const IconInstance* smallInst = icon->findBestInstance(smallSize);
		const IconInstance* largeInst = icon->findBestInstance(largeSize);
		if(!smallInst) errorExit("Couldn't find any small icon instance.");
		if(!largeInst) errorExit("Couldn't find any large icon instance.");

		if(!convertInstanceToImageFormat(largeInst, "temp_large.ico", largeSize, "ico")) errorExit("Windows mobile icon conversion failed.");
		if(!convertInstanceToImageFormat(smallInst, "temp_small.ico", smallSize, "ico")) errorExit("Windows mobile icon conversion failed.");

//		string command = string("winIconInject ") + dest + " temp.png";
//		if(system(command.c_str()) != 0) errorExit("Windows mobile injection failed.");

		vector<ICON> icons;

		IconFileLoader ifl1("temp_large.ico");
		icons.push_back(ifl1.getIconData(0));
		IconFileLoader ifl2("temp_small.ico");
		icons.push_back(ifl2.getIconData(0));

		if(InjectMainIcon(icons, dst.c_str())==false) errorExit("Windows mobile icon injection failed.");
	}
}
