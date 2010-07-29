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
	../../../src/mongoose.c \
	../../../src/httpServer.c \
	../../../src/urImage.cpp \
	../../../src/urTexture.cpp \
	../../../src/urFont.cpp \

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
	../../../src/lib/libpng/png.c \
	../../../src/lib/libpng/pngerror.c \
	../../../src/lib/libpng/pngget.c \
	../../../src/lib/libpng/pngmem.c \
	../../../src/lib/libpng/pngpread.c \
	../../../src/lib/libpng/pngread.c \
	../../../src/lib/libpng/pngrio.c \
	../../../src/lib/libpng/pngrtran.c \
	../../../src/lib/libpng/pngrutil.c \
	../../../src/lib/libpng/pngset.c \
	../../../src/lib/libpng/pngtrans.c \
	../../../src/lib/libpng/pngwio.c \
	../../../src/lib/libpng/pngwrite.c \
	../../../src/lib/libpng/pngwtran.c \
	../../../src/lib/libpng/pngwutil.c 
	
LOCAL_CFLAGS += -I../../../src/lib/libpng/
	
# zlib files
LOCAL_SRC_FILES += \
	../../../src/lib/zlib/adler32.c \
	../../../src/lib/zlib/compress.c \
	../../../src/lib/zlib/crc32.c \
	../../../src/lib/zlib/deflate.c \
	../../../src/lib/zlib/gzclose.c \
	../../../src/lib/zlib/gzlib.c \
	../../../src/lib/zlib/gzread.c \
	../../../src/lib/zlib/gzwrite.c \
	../../../src/lib/zlib/infback.c \
	../../../src/lib/zlib/inffast.c \
	../../../src/lib/zlib/inflate.c \
	../../../src/lib/zlib/inftrees.c \
	../../../src/lib/zlib/trees.c \
	../../../src/lib/zlib/uncompr.c \
	../../../src/lib/zlib/zutil.c 
	
# Freetype files
LOCAL_SRC_FILES += \
	../../../src/lib/Freetype/autofit/autofit.c \
	../../../src/lib/Freetype/base/ftbase.c \
	../../../src/lib/Freetype/bdf/bdf.c \
	../../../src/lib/Freetype/cache/ftcache.c \
	../../../src/lib/Freetype/cff/cff.c \
	../../../src/lib/Freetype/cid/type1cid.c \
	../../../src/lib/Freetype/gxvalid/gxvalid.c \
	../../../src/lib/Freetype/gzip/ftgzip.c \
	../../../src/lib/Freetype/lzw/ftlzw.c \
	../../../src/lib/Freetype/otvalid/otvalid.c \
	../../../src/lib/Freetype/pcf/pcf.c \
	../../../src/lib/Freetype/pfr/pfr.c \
	../../../src/lib/Freetype/psaux/psaux.c \
	../../../src/lib/Freetype/pshinter/pshinter.c \
	../../../src/lib/Freetype/psnames/psnames.c \
	../../../src/lib/Freetype/raster/raster.c \
	../../../src/lib/Freetype/sfnt/sfnt.c \
	../../../src/lib/Freetype/smooth/smooth.c \
	../../../src/lib/Freetype/truetype/truetype.c \
	../../../src/lib/Freetype/type1/type1.c \
	../../../src/lib/Freetype/type42/type42.c \
	../../../src/lib/Freetype/winfonts/winfnt.c
	
LOCAL_CFLAGS += -I../../../src/lib/Freetype/include/


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
	../../../src/lib/STLport/src/allocators.cpp \
	../../../src/lib/STLport/src/bitset.cpp \
	../../../src/lib/STLport/src/c_locale.c \
	../../../src/lib/STLport/src/codecvt.cpp \
	../../../src/lib/STLport/src/collate.cpp \
	../../../src/lib/STLport/src/complex.cpp \
	../../../src/lib/STLport/src/complex_io.cpp \
	../../../src/lib/STLport/src/complex_trig.cpp \
	../../../src/lib/STLport/src/ctype.cpp \
	../../../src/lib/STLport/src/cxa.c \
	../../../src/lib/STLport/src/dll_main.cpp \
	../../../src/lib/STLport/src/facets_byname.cpp \
	../../../src/lib/STLport/src/fstream.cpp \
	../../../src/lib/STLport/src/ios.cpp \
	../../../src/lib/STLport/src/iostream.cpp \
	../../../src/lib/STLport/src/istream.cpp \
	../../../src/lib/STLport/src/locale_catalog.cpp \
	../../../src/lib/STLport/src/locale_impl.cpp \
	../../../src/lib/STLport/src/messages.cpp \
	../../../src/lib/STLport/src/monetary.cpp \
	../../../src/lib/STLport/src/num_get.cpp \
	../../../src/lib/STLport/src/num_get_float.cpp \
	../../../src/lib/STLport/src/num_put.cpp \
	../../../src/lib/STLport/src/num_put_float.cpp \
	../../../src/lib/STLport/src/numpunct.cpp \
	../../../src/lib/STLport/src/ostream.cpp \
	../../../src/lib/STLport/src/sstream.cpp \
	../../../src/lib/STLport/src/stdio_streambuf.cpp \
	../../../src/lib/STLport/src/string.cpp \
	../../../src/lib/STLport/src/strstream.cpp \
	../../../src/lib/STLport/src/time_facets.cpp 

LOCAL_CFLAGS += -I../../../src/lib/STLport/stlport/ \
	-DANDROID \
	-DOS_ANDROID \
	-D__LITTLE_ENDIAN__ 

# end of source file lists

include $(BUILD_SHARED_LIBRARY)
