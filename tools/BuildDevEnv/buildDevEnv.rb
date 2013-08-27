# Copyright 2013 David Axmark
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# 	http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

require 'fileutils'
require File.expand_path('../../rules/util.rb')
require File.expand_path('../../rules/mosync_util.rb')

def sh2(dir, exec)
	prev = Dir.getwd
	Dir.chdir(dir)
	#puts(Dir.getwd)
	sh(exec)
	Dir.chdir(prev)
end

def copy_if_not_exist(from, to)
	if(!File.exist?(to))
		FileUtils.copy_file(from, to, true)
	end
end

Dir.chdir("../../")

root = Dir.getwd

copy_if_not_exist("intlibs/bluetooth/config_bluetooth.h.example", "intlibs/bluetooth/config_bluetooth.h")
copy_if_not_exist("runtimes/cpp/platforms/sdl/config_platform.h.example", "runtimes/cpp/platforms/sdl/config_platform.h")
copy_if_not_exist("runtimes/cpp/platforms/winmobile/config_platform.h.example", "runtimes/cpp/platforms/winmobile/config_platform.h")
copy_if_not_exist("runtimes/cpp/platforms/symbian/inc/config_platform.h.example", "runtimes/cpp/platforms/symbian/inc/config_platform.h")
copy_if_not_exist("runtimes/java/platforms/android/src/config_platform.h.example", "runtimes/java/platforms/android/src/config_platform.h")
copy_if_not_exist("runtimes/java/platforms/javaME/src/config_platform.h.example", "runtimes/java/platforms/javaME/src/config_platform.h")

#sh2("tools/GLWrapperGenerator", "build.bat")

sh2("intlibs/filelist", "vcbuild filelist.vcproj \"Release|Win32\"")
sh2("intlibs/demangle", "vcbuild demangle.vcproj \"Release|Win32\"")
sh2("intlibs/idl-common", "vcbuild idl-common.vcproj \"Release|Win32\"")
sh2("tools/idl2", "vcbuild idl2.vcproj \"Release|Win32\"")

sh2("tools/idl2/", "Release/idl2.exe");

sh2("libs/", "copyHeaders.bat")

sh2("intlibs/helpers/platforms/stdout", "vcbuild stdout.vcproj \"Release|Win32\"")
sh2("intlibs/helpers/platforms/windows", "vcbuild windows.vcproj \"Release|Win32\"")
sh2("intlibs/stabs", "ruby typeGen.rb")
sh2("intlibs/stabs", "vcbuild stabs.vcproj \"Release|Win32\"")
sh2("intlibs/bluetooth", "vcbuild bluetooth.vcproj \"Release|Win32\"")
sh2("intlibs/net", "vcbuild net.vcproj \"Release|Win32\"")
sh2("intlibs/gsm_amr", "vcbuild gsm_amr.vcproj \"Release|Win32\"")
#sh("xcopy intlibs/gsm_amr/release/gsm_amr.dll \"%MOSYNCDIR%/bin/gsm_amr.dll\" /e /y")
copy_if_not_exist("intlibs/gsm_amr/release/gsm_amr.dll", "#{mosyncdir}/bin/gsm_amr.dll")
sh2("tools/mobex", "vcbuild mobex.vcproj \"Release|Win32\"")

sh2("libs", "ruby workfile.rb")
sh2("libs", "ruby workfile.rb CONFIG=\"\"")
sh2("libs", "ruby workfile.rb USE_NEWLIB=\"\"")
sh2("libs", "ruby workfile.rb USE_NEWLIB=\"\" CONFIG=\"\"")

sh2("runtimes/cpp/platforms/sdl", "vcbuild sdl.vcproj \"Release|Win32\"")
sh2("runtimes/cpp/platforms/sdl/mosynclib", "vcbuild mosynclib.vcproj \"Release|Win32\"")
sh2("runtimes/cpp/platforms/sdl/more", "vcbuild more.vcproj \"Release|Win32\"")
