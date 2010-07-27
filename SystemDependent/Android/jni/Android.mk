# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := urMus

LOCAL_SRC_FILES := urMus.c \
#	mongoose.c \
#	httpServer.c \
#	urImage.cpp \
#	urTexture.cpp \
#	urFont.cpp \

LOCAL_LDLIBS	:= -llog -ldl -lGLESv1_CM

# Lua files
LOCAL_SRC_FILES += \
	../../../Classes/lua-5.1.4/src/lapi.c \
	../../../Classes/lua-5.1.4/src/lauxlib.c \
	../../../Classes/lua-5.1.4/src/lbaselib.c \
	../../../Classes/lua-5.1.4/src/lcode.c \
	../../../Classes/lua-5.1.4/src/ldblib.c \
	../../../Classes/lua-5.1.4/src/ldebug.c \
	../../../Classes/lua-5.1.4/src/ldo.c \
	../../../Classes/lua-5.1.4/src/ldump.c \
	../../../Classes/lua-5.1.4/src/lfs.c \
	../../../Classes/lua-5.1.4/src/lfunc.c \
	../../../Classes/lua-5.1.4/src/lgc.c \
	../../../Classes/lua-5.1.4/src/linit.c \
	../../../Classes/lua-5.1.4/src/liolib.c \
	../../../Classes/lua-5.1.4/src/llex.c \
	../../../Classes/lua-5.1.4/src/lmathlib.c \
	../../../Classes/lua-5.1.4/src/lmem.c \
	../../../Classes/lua-5.1.4/src/loadlib.c \
	../../../Classes/lua-5.1.4/src/lobject.c \
	../../../Classes/lua-5.1.4/src/lopcodes.c \
	../../../Classes/lua-5.1.4/src/loslib.c \
	../../../Classes/lua-5.1.4/src/lparser.c \
	../../../Classes/lua-5.1.4/src/lstate.c \
	../../../Classes/lua-5.1.4/src/lstring.c \
	../../../Classes/lua-5.1.4/src/lstrlib.c \
	../../../Classes/lua-5.1.4/src/ltable.c \
	../../../Classes/lua-5.1.4/src/ltablib.c \
	../../../Classes/lua-5.1.4/src/ltm.c \
	../../../Classes/lua-5.1.4/src/lundump.c \
	../../../Classes/lua-5.1.4/src/lvm.c \
	../../../Classes/lua-5.1.4/src/lzio.c 
	
LOCAL_CFLAGS += -I../../../Classes/lua-5.1.4/src/
	
# libpng files
LOCAL_SRC_FILES += \
	libpng/png.c \
	libpng/pngerror.c \
	libpng/pngget.c \
	libpng/pngmem.c \
	libpng/pngpread.c \
	libpng/pngread.c \
	libpng/pngrio.c \
	libpng/pngrtran.c \
	libpng/pngrutil.c \
	libpng/pngset.c \
	libpng/pngtrans.c \
	libpng/pngwio.c \
	libpng/pngwrite.c \
	libpng/pngwtran.c \
	libpng/pngwutil.c 
	
LOCAL_CFLAGS += -Ilibpng/
	
# zlib files
LOCAL_SRC_FILES += \
	zlib/adler32.c \
	zlib/compress.c \
	zlib/crc32.c \
	zlib/deflate.c \
	zlib/gzclose.c \
	zlib/gzlib.c \
	zlib/gzread.c \
	zlib/gzwrite.c \
	zlib/infback.c \
	zlib/inffast.c \
	zlib/inflate.c \
	zlib/inftrees.c \
	zlib/trees.c \
	zlib/uncompr.c \
	zlib/zutil.c 
	
# Freetype files
LOCAL_SRC_FILES += \
	Freetype/autofit/autofit.c \
	Freetype/base/ftbase.c \
	Freetype/bdf/bdf.c \
	Freetype/cache/ftcache.c \
	Freetype/cff/cff.c \
	Freetype/cid/type1cid.c \
	Freetype/gxvalid/gxvalid.c \
	Freetype/gzip/ftgzip.c \
	Freetype/lzw/ftlzw.c \
	Freetype/otvalid/otvalid.c \
	Freetype/pcf/pcf.c \
	Freetype/pfr/pfr.c \
	Freetype/psaux/psaux.c \
	Freetype/pshinter/pshinter.c \
	Freetype/psnames/psnames.c \
	Freetype/raster/raster.c \
	Freetype/sfnt/sfnt.c \
	Freetype/smooth/smooth.c \
	Freetype/truetype/truetype.c \
	Freetype/type1/type1.c \
	Freetype/type42/type42.c \
	Freetype/winfonts/winfnt.c
	
LOCAL_CFLAGS += -IFreetype/include/


# STK files

LOCAL_SRC_FILES += \
	../../../Classes/stk-4.4.1/src/ADSR.cpp \
	../../../Classes/stk-4.4.1/src/Asymp.cpp \
	../../../Classes/stk-4.4.1/src/BandedWG.cpp \
	../../../Classes/stk-4.4.1/src/BeeThree.cpp \
	../../../Classes/stk-4.4.1/src/BiQuad.cpp \
	../../../Classes/stk-4.4.1/src/Blit.cpp \
	../../../Classes/stk-4.4.1/src/BlitSaw.cpp \
	../../../Classes/stk-4.4.1/src/BlitSquare.cpp \
	../../../Classes/stk-4.4.1/src/BlowBotl.cpp \
	../../../Classes/stk-4.4.1/src/BlowHole.cpp \
	../../../Classes/stk-4.4.1/src/Bowed.cpp \
	../../../Classes/stk-4.4.1/src/Brass.cpp \
	../../../Classes/stk-4.4.1/src/Chorus.cpp \
	../../../Classes/stk-4.4.1/src/Clarinet.cpp \
	../../../Classes/stk-4.4.1/src/Delay.cpp \
	../../../Classes/stk-4.4.1/src/DelayA.cpp \
	../../../Classes/stk-4.4.1/src/DelayL.cpp \
	../../../Classes/stk-4.4.1/src/Drummer.cpp \
	../../../Classes/stk-4.4.1/src/Echo.cpp \
	../../../Classes/stk-4.4.1/src/Envelope.cpp \
	../../../Classes/stk-4.4.1/src/FileLoop.cpp \
	../../../Classes/stk-4.4.1/src/FileRead.cpp \
	../../../Classes/stk-4.4.1/src/FileWrite.cpp \
	../../../Classes/stk-4.4.1/src/FileWvIn.cpp \
	../../../Classes/stk-4.4.1/src/FileWvOut.cpp \
	../../../Classes/stk-4.4.1/src/Fir.cpp \
	../../../Classes/stk-4.4.1/src/Flute.cpp \
	../../../Classes/stk-4.4.1/src/FM.cpp \
	../../../Classes/stk-4.4.1/src/FMVoices.cpp \
	../../../Classes/stk-4.4.1/src/FormSwep.cpp \
	../../../Classes/stk-4.4.1/src/Granulate.cpp \
	../../../Classes/stk-4.4.1/src/HevyMetl.cpp \
	../../../Classes/stk-4.4.1/src/Iir.cpp \
	../../../Classes/stk-4.4.1/src/JCRev.cpp \
	../../../Classes/stk-4.4.1/src/Mandolin.cpp \
	../../../Classes/stk-4.4.1/src/Mesh2D.cpp \
	../../../Classes/stk-4.4.1/src/Modal.cpp \
	../../../Classes/stk-4.4.1/src/ModalBar.cpp \
	../../../Classes/stk-4.4.1/src/Modulate.cpp \
	../../../Classes/stk-4.4.1/src/Moog.cpp \
	../../../Classes/stk-4.4.1/src/Noise.cpp \
	../../../Classes/stk-4.4.1/src/NRev.cpp \
	../../../Classes/stk-4.4.1/src/OnePole.cpp \
	../../../Classes/stk-4.4.1/src/OneZero.cpp \
	../../../Classes/stk-4.4.1/src/PercFlut.cpp \
	../../../Classes/stk-4.4.1/src/Phonemes.cpp \
	../../../Classes/stk-4.4.1/src/PitShift.cpp \
	../../../Classes/stk-4.4.1/src/Plucked.cpp \
	../../../Classes/stk-4.4.1/src/PluckTwo.cpp \
	../../../Classes/stk-4.4.1/src/PoleZero.cpp \
	../../../Classes/stk-4.4.1/src/PRCRev.cpp \
	../../../Classes/stk-4.4.1/src/Resonate.cpp \
	../../../Classes/stk-4.4.1/src/Rhodey.cpp \
	../../../Classes/stk-4.4.1/src/Sampler.cpp \
	../../../Classes/stk-4.4.1/src/Saxofony.cpp \
	../../../Classes/stk-4.4.1/src/Shakers.cpp \
	../../../Classes/stk-4.4.1/src/Simple.cpp \
	../../../Classes/stk-4.4.1/src/SineWave.cpp \
	../../../Classes/stk-4.4.1/src/SingWave.cpp \
	../../../Classes/stk-4.4.1/src/Sitar.cpp \
	../../../Classes/stk-4.4.1/src/Sphere.cpp \
	../../../Classes/stk-4.4.1/src/StifKarp.cpp \
	../../../Classes/stk-4.4.1/src/Stk.cpp \
	../../../Classes/stk-4.4.1/src/TapDelay.cpp \
	../../../Classes/stk-4.4.1/src/TubeBell.cpp \
	../../../Classes/stk-4.4.1/src/TwoPole.cpp \
	../../../Classes/stk-4.4.1/src/TwoZero.cpp \
	../../../Classes/stk-4.4.1/src/Voicer.cpp \
	../../../Classes/stk-4.4.1/src/VoicForm.cpp \
	../../../Classes/stk-4.4.1/src/Whistle.cpp \
	../../../Classes/stk-4.4.1/src/Wurley.cpp

LOCAL_CFLAGS += -I../../../Classes/stk-4.4.1/include/

# STLport files

LOCAL_SRC_FILES += \
	STLport/src/allocators.cpp \
	STLport/src/bitset.cpp \
	STLport/src/c_locale.c \
	STLport/src/codecvt.cpp \
	STLport/src/collate.cpp \
	STLport/src/complex.cpp \
	STLport/src/complex_io.cpp \
	STLport/src/complex_trig.cpp \
	STLport/src/ctype.cpp \
	STLport/src/cxa.c \
	STLport/src/dll_main.cpp \
	STLport/src/facets_byname.cpp \
	STLport/src/fstream.cpp \
	STLport/src/ios.cpp \
	STLport/src/iostream.cpp \
	STLport/src/istream.cpp \
	STLport/src/locale_catalog.cpp \
	STLport/src/locale_impl.cpp \
	STLport/src/messages.cpp \
	STLport/src/monetary.cpp \
	STLport/src/num_get.cpp \
	STLport/src/num_get_float.cpp \
	STLport/src/num_put.cpp \
	STLport/src/num_put_float.cpp \
	STLport/src/numpunct.cpp \
	STLport/src/ostream.cpp \
	STLport/src/sstream.cpp \
	STLport/src/stdio_streambuf.cpp \
	STLport/src/string.cpp \
	STLport/src/strstream.cpp \
	STLport/src/time_facets.cpp 

LOCAL_CFLAGS += -ISTLport/stlport/ \
	-DANDROID \
	-DOS_ANDROID \
	-D__LITTLE_ENDIAN__ 

# end of source file lists

include $(BUILD_SHARED_LIBRARY)
