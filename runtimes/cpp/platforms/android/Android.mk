# Copyright (C) 2010 Mobile Sorcery AB
# 
# This program is free software; you can redistribute it and/or modify it under
# the terms of the GNU General Public License, version 2, as published by
# the Free Software Foundation.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
# or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
# for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; see the file COPYING.  If not, write to the Free
# Software Foundation, 59 Temple Place - Suite 330, Boston, MA
# 02111-1307, USA.

LOCAL_PATH:= /cygdrive/d/android-ndk-1.5_r1/sources/mosync

include $(CLEAR_VARS)

LOCAL_MODULE    := mosync
LOCAL_ARM_MODE	:= arm
LOCAL_SRC_FILES := Core.cpp FileImpl.cpp FileStream.cpp MemStream.cpp Stream.cpp Image.cpp hashmap/hashmap.cpp AndroidHelpers.cpp Syscall.cpp SyscallImpl.cpp disassembler.cpp Recompiler/ArmAssembler.cpp Recompiler/ArmRecompiler.cpp MoSyncBridge.cpp helpers/platforms/linux/log.cpp
LOCAL_LDLIBS := -L$(SYSROOT)/usr/lib -llog

LOCAL_CFLAGS := -DFPM_ARM 

include $(BUILD_SHARED_LIBRARY)
