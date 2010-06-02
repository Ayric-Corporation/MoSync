/* Copyright (C) 2010 Mobile Sorcery AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
*/

class Syscall {

private:
	bool loadImage(int resourceIndex, int pos, int length);
	Image* loadSprite(void* surface, ushort left, ushort top, ushort width, ushort height, ushort cx, ushort cy);
	char* loadBinary(int resourceIndex, int size);
	
public:

	Syscall();	
	void postEvent(MAEvent event);
	void setJNIEnvironment(JNIEnv* je, jobject jthis);
