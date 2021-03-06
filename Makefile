######################################################
# File: Makefile.in
# Desc: Makefile for boomerang
#       Makes and tests all object files all directories
#
######################################################

# $Revision: 1.131 $
# 22 Aug 06 - Mike Frysinger: respect values set up by ./configure script
# 12 Apr 06 - Gerard: Restructured, added conditional rules, added generated dependencies
# 13 Mar 06 - Mike: Prevent going to the loader directory 5 or 6 times
# 05 Jul 05 - Mike: Big merge with boomerang.pole branch
# 03 Feb 05 - Mike: Removed analysis/ related files
# 23 Nov 04 - Mike: Added PPC files
# 20 Jul 04 - Mike: Removed BOOMDIR
# 21 Aug 02 - Icer: added codegen/chllcode.cpp to the build process by adding it in STATIC_OBJ
# 20 Aug 02 - Icer: removed test from all, test code is broken. Added more to distclean
# 19 Aug 02 - Icer: changed make distclean delete toplevel makefile; changed all: to read lib static test
# 31 May 02 - Mike: Make lib directory if needed
# 20 May 02 - Trent: Created
# 15 Jul 01 - Mike: Added static target

###############
# Introduction
#
# This Makefile controls the whole build process of the Boomerang decompiler, except for the loaders.
# The loaders have their own makefile (loaders/Makefile).
# The gui's also have to take care of themselves.
#
# Index of this makefile: 
#
# - Variables detected by ./configure
# - Detection of OS specific settings
# - Compiler settings
# - Main rules
# - Loaders
# - Detection of machine types and loaders
# - Lots of object files
# - Conditional rules
# - Rules for compiling
# - Unit testing (broken?)
# - Cleaning up
# - Dependencies
#

####################################
# Variables detected by ./configure
#

top_srcdir = .

# HOST_GNU_LD is "yes" if the linker on this host machine is GNU
HOST_GNU_LD = yes

# HOST_WINDOWS is true if the host OS is CygWin or MinGW
HOST_WINDOWS = no

# HOST_OSX is true if the host OS is Mac OS X
HOST_OSX = no

# HOST_OSX_10_2 is true if the hostOS is OS X version 10.2.x (*-apple-darwin6*)
HOST_OSX = no

# TKML is set to the path to the New Jersey Machine Code Toolkit (ML version) if found
TKML = 

# The same for these, empty if not found
MAKEDEPEND=
BISONPP=
FLEXPP=
M4=/usr/bin/m4

####################################
# Detection of OS specific settings
#

# LDL is -ldl if needed (Windows does not)
# EXEEXT is .exe for Windows
# DLLEXT is .dll for Windows and .so otherwise 
ifeq ($(HOST_WINDOWS), yes)
 LDL=
 EXEEXT=.exe
 DLLEXT=.dll
 WININCLUDE=-Iwin32make/include
 # RUNPATH is not needed on Windows 
 RUNPATH=
 COPYDLL=libBinaryFile$(DLLEXT)
else # Non-Windows
 LDL=-ldl
 EXEEXT=
 DLLEXT=.so
 WININCLUDE=
 ifeq ($(HOST_GNU_LD), yes)
  RUNPATH=-Wl,-rpath -Wl,"lib"
 else
  ifeq ($(HOST_OSX), yes)
   RUNPATH=
  else		# Assume Solaris
   RUNPATH=-R"lib"
  endif
 endif
 COPYDLL=
endif

####################
# Compiler settings
#

CC = gcc
CXX = g++
CFLAGS = -m32 -Wall
CXXFLAGS = -m32 -Wall
CPPFLAGS = -I$(top_srcdir)/include 
LDFLAGS = -m32

# NOTE: "win32make/include" should be after "include" because if there is a generated include/config.h then we prefer to
# use that
# comment out the next two lines and uncomment the two after to disable the garbage collector
LINKGC=-lgc
CPPFLAGS += $(WININCLUDE)
#LINKGC=
#CPPFLAGS += $(WININCLUDE) -DNO_GARBAGE_COLLECTOR


#############
# Main rules
#

.PHONY: test clean all static distclean mostclean depend depend.in loaders

#19 Aug 02 icer: should read lib static test
#20 Aug 02 icer: should read lib static, the test code is broken
all: lib loaders static

static: boomerang$(EXEEXT)

##########
# Loaders
#

# Make the lib directory if needed
lib:
	mkdir lib
	
loaders: lib
	$(MAKE) -C loader

#########################################
# Detection of machine types and loaders
#

# Front end
MACHS     = sparc pentium ppc st20 mips _8051
MACH_LIBS = $(patsubst %, lib/libfront%.so, $(MACHS))
MACH_OBJS = $(patsubst %, frontend/%decoder.o, $(MACHS))
MACH_SRC  = $(patsubst %, frontend/%decoder.cpp, $(MACHS))
MACH_FE_SRC=$(patsubst %, frontend/%frontend.cpp, $(MACHS))
MACH_FE_OBJ=$(patsubst %, frontend/%frontend.o, $(MACHS))

# The loaders are properly dynamically linked now, so no need for individual -l<Arch>BinaryFile any more
LOADERS=BinaryFile # ElfBinaryFile Win32BinaryFile ExeBinaryFile HpSomBinaryFile PalmBinaryFile BinaryFile

# LOADERDLLS is the set of DLL files
LOADERDLLS=$(patsubst %,lib/lib%$(DLLEXT),$(LOADERS)) $(COPYDLL)

# LOADERLIBS is the set of "-l" flags passed when linking executables against the DLLs
LOADERLIBS=$(patsubst %,-l%,$(LOADERS))
	
##############
# Make remote
#
	
# In case you don't have bison++ and flex++ and don't need to mess with the parsers
# This is automatic now, though you can still force it if desired
remote:
	touch db/sslparser.cpp db/sslparser.h \
        c/ansi-c-parser.cpp c/ansi-c-scanner.cpp  \
        c/ansi-c-parser.h   c/ansi-c-scanner.h \
        db/sslscanner.cpp   db/sslscanner.h \
        transform/transformation-parser.cpp transform/transformation-scanner.cpp \
        transform/transformation-parser.h transform/transformation-scanner.h
# Also touch the decoders, since the user won't have the NJMC toolkit
	touch frontend/pentiumdecoder.cpp frontend/sparcdecoder.cpp frontend/ppcdecoder.cpp frontend/_8051decoder.cpp frontend/st20decoder.cpp frontend/mipsdecoder.cpp

#######################
# Lots of object files
#

TEST_OBJS = util/UtilTest.o db/ExpTest.o db/ParserTest.o db/ProcTest.o db/ProgTest.o db/RtlTest.o db/StatementTest.o \
	db/CfgTest.o db/DfaTest.o frontend/FrontSparcTest.o frontend/FrontPentTest.o loader/BinaryFileStub.o c/CTest.o \
	type/TypeTest.o

UTIL_OBJS = util/util.o
DB_OBJS = db/basicblock.o db/proc.o db/sslscanner.o db/cfg.o db/prog.o db/table.o db/statement.o db/register.o \
	db/sslparser.o db/exp.o db/rtl.o db/sslinst.o db/insnameelem.o db/signature.o db/managed.o c/ansi-c-parser.o \
	c/ansi-c-scanner.o boomerang.o log.o db/visitor.o db/dataflow.o db/xmlprogparser.o 
TRANSFORM_OBJS = transform/rdi.o transform/transformer.o transform/generic.o transform/transformation-parser.o \
	transform/transformation-scanner.o
FRONT_OBJS = frontend/frontend.o frontend/njmcDecoder.o frontend/sparcdecoder.o frontend/pentiumdecoder.o \
	frontend/sparcfrontend.o frontend/pentiumfrontend.o frontend/ppcdecoder.o frontend/ppcfrontend.o \
	frontend/st20decoder.o frontend/st20frontend.o frontend/mipsdecoder.o frontend/mipsfrontend.o frontend/_8051frontend.o frontend/_8051decoder.o
CODEGEN = codegen/chllcode.o codegen/syntax.o
TYPEOBJS = type/constraint.o type/type.o type/dfa.o
LOADER_OBJS = loader/BinaryFileFactory.o
ASSEMBLY_OBJS = db/AssHandler.o db/AssScanner.o  
STATIC_OBJS = $(CODEGEN) $(UTIL_OBJS) $(DB_OBJS) $(FRONT_OBJS) $(TYPEOBJS) $(LOADER_OBJS) $(TRANSFORM_OBJS) $(ASSEMBLY_OBJS)

####################
# Conditional rules
#

# This SSL file is generated (m4 macro expanded) from a .m file
ifdef M4
 GENSSL = frontend/machine/st20/st20.ssl

 frontend/machine/st20/st20.ssl: frontend/machine/st20/st20.ssl.m
	$(M4) < frontend/machine/st20/st20.ssl.m > frontend/machine/st20/st20.ssl
	
endif # ifdef M4

# Automatic generation of dependencies
ifdef MAKEDEPEND
 CPPFILES=*.cpp c/*.cpp codegen/*.cpp db/*.cpp frontend/*.cpp transform/*.cpp type/*.cpp util/*.cpp

 # automatic is better!
 depend: $(MAKEDEPEND) lib
	-mv include/gc*.h lib
	$(MAKEDEPEND) -w120 -Y -Iinclude -I. -Ic -Icodegen -Ifrontend $(CPPFILES) > /dev/null 2>&1
	$(MAKE) -C loader depend
	-mv lib/gc*.h include

 depend.in: $(MAKEDEPEND) lib
	-mv include/gc*.h lib
	$(MAKEDEPEND) -fMakefile.in -w120 -Y -Iinclude -I. -Ic -Icodegen -Ifrontend $(CPPFILES) > /dev/null 2>&1
	$(MAKE) -C loader depend
	-mv lib/gc*.h include
	
else
 depend:
	# You need the (X11) makedepend tool to generate dependencies
 depend.in:
	# You need the (X11) makedepend tool to generate dependencies
endif # ifdef MAKEDEPEND


# These rules describe the generation of the parsers using Bison++ and yacc++
# If one of those tools is not installed, generation is not possible
ifdef BISONPP
 ifdef FLEXPP
 
  db/sslparser.cpp db/sslparser.h: db/sslparser.y db/sslscanner.l
	cd db && $(BISONPP) -d -v -osslparser.cpp -hsslparser.h sslparser.y
	cd db && sed -e "s/int __alloca_free_ptr/static int __alloca_free_ptr/" < sslparser.cpp > sslparser.cpp1
	mv db/sslparser.cpp1 db/sslparser.cpp

  c/ansi-c-parser.cpp c/ansi-c-parser.h: c/ansi-c.y c/ansi-c.l
	cd c && $(BISONPP) -d -v -oansi-c-parser.cpp -hansi-c-parser.h ansi-c.y
	
  transform/transformation-parser.cpp transform/transformation-parser.h: transform/transformation.y transform/transformation.l
	cd transform && $(BISONPP) -d -v -otransformation-parser.cpp -htransformation-parser.h transformation.y

  db/sslscanner.cpp db/sslscanner.h: db/sslscanner.l db/sslparser.y
	cd db && $(FLEXPP) -osslscanner.cpp1 -hsslscanner.h sslscanner.l
	sed -f db/sslscanner.sed db/sslscanner.cpp1 > db/sslscanner.cpp		# Fix warnings re array subscript has type char

  c/ansi-c-scanner.cpp c/ansi-c-scanner.h: c/ansi-c.l c/ansi-c.y
	cd c && $(FLEXPP) -oansi-c-scanner.cpp1 -hansi-c-scanner.h ansi-c.l
	sed -f db/sslscanner.sed c/ansi-c-scanner.cpp1 > c/ansi-c-scanner.cpp	# Fix warnings with same sed file

  transform/transformation-scanner.cpp transform/transformation-scanner.h: transform/transformation.l transform/transformation.y
	cd transform && $(FLEXPP) -otransformation-scanner.cpp1 -htransformation-scanner.h transformation.l
	# Fix warnings with same sed file
	sed -f db/sslscanner.sed transform/transformation-scanner.cpp1 > transform/transformation-scanner.cpp

 endif # ifdef FLEXPP
endif # ifdef BISONPP

# Generate the decoders using the New Jersey Machine Toolkit (ML version)
ifdef TKML
 # The .cpp's depend on the .m's
 $(MACH_SRC): frontend/%decoder.cpp : frontend/machine/%/core.spec \
                                   frontend/machine/%/synth.spec \
                                   frontend/machine/%/dis.spec \
                                   frontend/machine/%/decoder.m
	frontend/mltk.sh -o $@ $^ 2> mltk.err
endif

######################
# Rules for compiling
#

# Extra flags
db/prog.o:                 	EXTRA = -Icodegen -Ic
db/proc.o:                 	EXTRA = -fno-strict-aliasing
db/exp.o:                 	EXTRA = -fno-strict-aliasing
frontend/frontend.o:       	EXTRA = -Ic
db/ProgTest.o:             	EXTRA = -Ifrontend
db/ProcTest.o:             	EXTRA = -Ifrontend
db/RtlTest.o:              	EXTRA = -Ifrontend
db/StatementTest.o:        	EXTRA = -Ifrontend
db/CfgTest.o:              	EXTRA = -Ifrontend
type/dfa.o:                	EXTRA = -fno-strict-aliasing
type/TypeTest.o:           	EXTRA = -Ifrontend
frontend/pentiumdecoder.o: 	EXTRA = -fno-exceptions
db/AssScanner.o:            EXTRA = -lfl  
db/AssHandler.o:			EXTRA = -lfl  
boomerang$(EXEEXT): driver.o $(STATIC_OBJS) $(GENSSL)
	$(CXX) $(CXXFLAGS) -o $@ driver.o $(STATIC_OBJS)  $(RUNPATH) -lfl -Llib $(LINKGC) $(LDL) $(LDFLAGS) $(LOADERLIBS) -lexpat

bffDump$(EXEEXT): loader/bffDump.o
	$(CXX) $(CXXFLAGS) -o $@ loader/bffDump.o loader/BinaryFileFactory.o -Llib -lgc $(LDL) $(LOADERLIBS) \
		$(RUNPATH) $(LDFLAGS) -lexpat

# Compile ordinary files
$(STATIC_OBJS): %.o : %.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $(EXTRA) $<
# driver.o is special because it contains main() and is not wanted for bigtest (the unit test program)
driver.o: driver.cpp include/boomerang.h
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $(EXTRA) $<

###############
# Unit testing
#

testDyn: lib
	$(MAKE) -C util     test
	$(MAKE) -C loader   test
	$(MAKE) -C db       test
	$(MAKE) -C frontend test

check test: lib loaders bigtest$(EXEEXT)
	$(MAKE) -C loader test
	./bigtest$(EXEEXT)

# Needed because the test header files are in all these different directories
INCLUDEALL = -Idb -Ifrontend -Ic -Itype

testAll.o: testAll.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $(EXTRA) $< $(INCLUDEALL)

bigtest$(EXEEXT): testAll.o $(STATIC_OBJS) $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ testAll.o $(STATIC_OBJS) $(TEST_OBJS) $(LOADERLIBS) \
	    $(RUNPATH) -Llib -lcppunit -lgc $(LDL) $(LDFLAGS) -lexpat

$(TEST_OBJS): %.o : %.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $(EXTRA) $<

##############
# Cleaning up
#

clean:
	rm -f driver.o $(STATIC_OBJS) boomerang$(EXEEXT) bigtest$(EXEEXT) testAll.o
	rm -f type/TypeTest.o util/UtilTest.o c/CTest.o \
		frontend/FrontPentTest.o frontend/FrontSparcTest.o
	$(MAKE) -C loader clean
	$(MAKE) -C db     clean

distclean: clean
	rm -f Makefile util/Makefile loader/Makefile db/Makefile frontend/Makefile transform/Makefile include/config.h \
		config.log config.status frontend/mltk.sh
	rm -f include/gc.h include/gc_config_macros.h lib/libcppunit.a
	rm -f functest.c functest.exe functest.out functest.res functest.tmp functests.out output/log
	rm -rf functest unit_test lib

# target mostclean (clean also the autoconf output)
mostclean: distclean
	rm -rf autom4te.cache/ configure

###############
# Dependencies
# Automatically generated by "make depend"
# DO NOT DELETE THIS LINE - It is used by makedepend

boomerang.o: include/prog.h include/BinaryFile.h include/types.h include/frontend.h include/sigenum.h include/type.h
boomerang.o: include/memo.h include/cluster.h include/proc.h include/exp.h include/operator.h include/exphelp.h
boomerang.o: include/cfg.h include/basicblock.h include/managed.h include/dataflow.h include/hllcode.h
boomerang.o: include/statement.h codegen/chllcode.h include/transformer.h include/boomerang.h include/xmlprogparser.h
boomerang.o: include/log.h
driver.o: include/boomerang.h include/types.h
log.o: include/log.h include/types.h include/statement.h include/memo.h include/exphelp.h include/managed.h
log.o: include/dataflow.h include/rtl.h include/exp.h include/operator.h include/type.h include/register.h
testAll.o: include/exp.h include/operator.h include/types.h include/type.h include/memo.h include/exphelp.h
testAll.o: frontend/FrontSparcTest.h include/BinaryFile.h frontend/FrontPentTest.h include/decoder.h include/rtl.h
testAll.o: include/register.h c/CTest.h include/util.h c/ansi-c-parser.h include/cfg.h include/basicblock.h
testAll.o: include/managed.h include/dataflow.h include/proc.h include/hllcode.h include/statement.h
testAll.o: include/signature.h include/sigenum.h include/prog.h include/frontend.h include/cluster.h
windows.o: resource.h include/prog.h include/BinaryFile.h include/types.h include/frontend.h include/sigenum.h
windows.o: include/type.h include/memo.h include/cluster.h include/proc.h include/exp.h include/operator.h
windows.o: include/exphelp.h include/cfg.h include/basicblock.h include/managed.h include/dataflow.h include/hllcode.h
windows.o: include/statement.h include/signature.h include/boomerang.h include/log.h
c/CTest.o: c/CTest.h include/util.h include/type.h include/memo.h include/types.h c/ansi-c-parser.h include/exp.h
c/CTest.o: include/operator.h include/exphelp.h include/cfg.h include/basicblock.h include/managed.h include/dataflow.h
c/CTest.o: include/proc.h include/hllcode.h include/statement.h include/signature.h include/sigenum.h
c/ansi-c-parser.o: include/exp.h include/operator.h include/types.h include/type.h include/memo.h include/exphelp.h
c/ansi-c-parser.o: include/cfg.h include/basicblock.h include/managed.h include/dataflow.h include/proc.h
c/ansi-c-parser.o: include/hllcode.h include/statement.h include/signature.h include/sigenum.h c/ansi-c-scanner.h
c/ansi-c-parser.o: c/ansi-c-parser.h
c/ansi-c-scanner.o: c/ansi-c-scanner.h c/ansi-c-parser.h include/exp.h include/operator.h include/types.h
c/ansi-c-scanner.o: include/type.h include/memo.h include/exphelp.h include/cfg.h include/basicblock.h
c/ansi-c-scanner.o: include/managed.h include/dataflow.h include/proc.h include/hllcode.h include/statement.h
c/ansi-c-scanner.o: include/signature.h include/sigenum.h
codegen/chllcode.o: include/cfg.h include/types.h include/exphelp.h include/basicblock.h include/managed.h
codegen/chllcode.o: include/dataflow.h include/statement.h include/memo.h include/exp.h include/operator.h
codegen/chllcode.o: include/type.h include/proc.h include/hllcode.h include/prog.h include/BinaryFile.h
codegen/chllcode.o: include/frontend.h include/sigenum.h include/cluster.h codegen/chllcode.h include/signature.h
codegen/chllcode.o: include/boomerang.h include/util.h include/log.h
codegen/syntax.o: include/prog.h include/BinaryFile.h include/types.h include/frontend.h include/sigenum.h
codegen/syntax.o: include/type.h include/memo.h include/cluster.h include/exp.h include/operator.h include/exphelp.h
codegen/syntax.o: include/hllcode.h include/statement.h include/managed.h include/dataflow.h include/cfg.h
codegen/syntax.o: include/basicblock.h
db/CfgTest.o: db/CfgTest.h include/cfg.h include/types.h include/exphelp.h include/basicblock.h include/managed.h
db/CfgTest.o: include/dataflow.h include/BinaryFile.h include/frontend.h include/sigenum.h include/proc.h include/exp.h
db/CfgTest.o: include/operator.h include/type.h include/memo.h include/hllcode.h include/statement.h include/prog.h
db/CfgTest.o: include/cluster.h frontend/pentiumfrontend.h
db/AssHandler.o: include/AssemblyInfo.h db/AssParser.cpp
db/AssScanner.o: db/AssParser.h
db/DfaTest.o: db/DfaTest.h include/type.h include/memo.h include/types.h include/log.h include/boomerang.h
db/ExpTest.o: db/ExpTest.h include/exp.h include/operator.h include/types.h include/type.h include/memo.h
db/ExpTest.o: include/exphelp.h include/statement.h include/managed.h include/dataflow.h include/visitor.h
db/ParserTest.o: db/ParserTest.h include/prog.h include/BinaryFile.h include/types.h include/frontend.h
db/ParserTest.o: include/sigenum.h include/type.h include/memo.h include/cluster.h db/sslparser.h include/rtl.h
db/ParserTest.o: include/exp.h include/operator.h include/exphelp.h include/register.h db/table.h db/insnameelem.h
db/ParserTest.o: include/util.h include/statement.h include/managed.h include/dataflow.h
db/ProcTest.o: db/ProcTest.h include/proc.h include/exp.h include/operator.h include/types.h include/type.h
db/ProcTest.o: include/memo.h include/exphelp.h include/cfg.h include/basicblock.h include/managed.h include/dataflow.h
db/ProcTest.o: include/hllcode.h include/statement.h include/prog.h include/BinaryFile.h include/frontend.h
db/ProcTest.o: include/sigenum.h include/cluster.h include/BinaryFileStub.h frontend/pentiumfrontend.h
db/ProgTest.o: db/ProgTest.h include/prog.h include/BinaryFile.h include/types.h include/frontend.h include/sigenum.h
db/ProgTest.o: include/type.h include/memo.h include/cluster.h frontend/pentiumfrontend.h
db/RtlTest.o: db/RtlTest.h include/rtl.h include/exp.h include/operator.h include/types.h include/type.h include/memo.h
db/RtlTest.o: include/exphelp.h include/register.h include/statement.h include/managed.h include/dataflow.h
db/RtlTest.o: include/BinaryFile.h include/frontend.h include/sigenum.h frontend/sparcfrontend.h include/decoder.h
db/RtlTest.o: frontend/pentiumfrontend.h include/proc.h include/cfg.h include/basicblock.h include/hllcode.h
db/RtlTest.o: include/prog.h include/cluster.h include/visitor.h
db/StatementTest.o: db/StatementTest.h include/proc.h include/exp.h include/operator.h include/types.h include/type.h
db/StatementTest.o: include/memo.h include/exphelp.h include/cfg.h include/basicblock.h include/managed.h
db/StatementTest.o: include/dataflow.h include/hllcode.h include/statement.h include/prog.h include/BinaryFile.h
db/StatementTest.o: include/frontend.h include/sigenum.h include/cluster.h include/rtl.h include/register.h
db/StatementTest.o: frontend/pentiumfrontend.h include/boomerang.h include/log.h include/signature.h
db/basicblock.o: include/types.h include/statement.h include/memo.h include/exphelp.h include/managed.h
db/basicblock.o: include/dataflow.h include/exp.h include/operator.h include/type.h include/cfg.h include/basicblock.h
db/basicblock.o: include/register.h include/rtl.h include/hllcode.h include/proc.h include/prog.h include/BinaryFile.h
db/basicblock.o: include/frontend.h include/sigenum.h include/cluster.h include/util.h include/boomerang.h
db/basicblock.o: include/log.h include/visitor.h
db/cfg.o: include/types.h include/statement.h include/memo.h include/exphelp.h include/managed.h include/dataflow.h
db/cfg.o: include/signature.h include/exp.h include/operator.h include/type.h include/sigenum.h include/cfg.h
db/cfg.o: include/basicblock.h include/register.h include/rtl.h include/proc.h include/hllcode.h include/prog.h
db/cfg.o: include/BinaryFile.h include/frontend.h include/cluster.h include/util.h include/boomerang.h include/log.h
db/checkstrings.o: db/operstrings.h include/operator.h
db/dataflow.o: include/dataflow.h include/exphelp.h include/managed.h include/cfg.h include/types.h
db/dataflow.o: include/basicblock.h include/proc.h include/exp.h include/operator.h include/type.h include/memo.h
db/dataflow.o: include/hllcode.h include/statement.h include/boomerang.h include/visitor.h include/log.h
db/exp.o: include/types.h include/statement.h include/memo.h include/exphelp.h include/managed.h include/dataflow.h
db/exp.o: include/cfg.h include/basicblock.h include/exp.h include/operator.h include/type.h include/register.h
db/exp.o: include/rtl.h include/proc.h include/hllcode.h include/signature.h include/sigenum.h include/prog.h
db/exp.o: include/BinaryFile.h include/frontend.h include/cluster.h db/operstrings.h include/util.h include/boomerang.h
db/exp.o: include/transformer.h include/visitor.h include/log.h
db/insnameelem.o: include/types.h db/insnameelem.h db/table.h
db/managed.o: include/types.h include/managed.h include/exphelp.h include/statement.h include/memo.h include/dataflow.h
db/managed.o: include/exp.h include/operator.h include/type.h include/log.h include/boomerang.h
db/proc.o: include/proc.h include/exp.h include/operator.h include/types.h include/type.h include/memo.h
db/proc.o: include/exphelp.h include/cfg.h include/basicblock.h include/managed.h include/dataflow.h include/hllcode.h
db/proc.o: include/statement.h include/cluster.h include/register.h include/rtl.h include/prog.h include/BinaryFile.h
db/proc.o: include/frontend.h include/sigenum.h include/util.h include/signature.h include/boomerang.h
db/proc.o: include/constraint.h include/visitor.h include/log.h
db/prog.o: include/type.h include/memo.h include/types.h include/cluster.h include/statement.h include/exphelp.h
db/prog.o: include/managed.h include/dataflow.h include/exp.h include/operator.h include/cfg.h include/basicblock.h
db/prog.o: include/proc.h include/hllcode.h include/util.h include/register.h include/rtl.h include/BinaryFile.h
db/prog.o: include/frontend.h include/sigenum.h include/prog.h include/signature.h include/boomerang.h
db/prog.o: c/ansi-c-parser.h include/config.h include/log.h
db/register.o: include/register.h include/type.h include/memo.h include/types.h
db/rtl.o: include/types.h include/statement.h include/memo.h include/exphelp.h include/managed.h include/dataflow.h
db/rtl.o: include/exp.h include/operator.h include/type.h include/register.h include/cfg.h include/basicblock.h
db/rtl.o: include/proc.h include/hllcode.h include/rtl.h include/prog.h include/BinaryFile.h include/frontend.h
db/rtl.o: include/sigenum.h include/cluster.h include/util.h include/boomerang.h include/visitor.h include/log.h
db/signature.o: include/type.h include/memo.h include/types.h include/signature.h include/exp.h include/operator.h
db/signature.o: include/exphelp.h include/sigenum.h include/statement.h include/managed.h include/dataflow.h
db/signature.o: include/prog.h include/BinaryFile.h include/frontend.h include/cluster.h include/util.h include/cfg.h
db/signature.o: include/basicblock.h include/proc.h include/hllcode.h include/boomerang.h include/log.h
db/signatureStubs.o: include/signature.h include/exp.h include/operator.h include/types.h include/type.h include/memo.h
db/signatureStubs.o: include/exphelp.h include/sigenum.h include/statement.h include/managed.h include/dataflow.h
db/sslinst.o: include/types.h include/statement.h include/memo.h include/exphelp.h include/managed.h include/dataflow.h
db/sslinst.o: include/exp.h include/operator.h include/type.h include/register.h include/rtl.h include/cfg.h
db/sslinst.o: include/basicblock.h include/proc.h include/hllcode.h include/prog.h include/BinaryFile.h
db/sslinst.o: include/frontend.h include/sigenum.h include/cluster.h db/sslparser.h db/table.h db/insnameelem.h
db/sslinst.o: include/util.h include/boomerang.h
db/sslparser.o: include/types.h include/rtl.h include/exp.h include/operator.h include/type.h include/memo.h
db/sslparser.o: include/exphelp.h include/register.h db/table.h db/insnameelem.h include/util.h include/statement.h
db/sslparser.o: include/managed.h include/dataflow.h db/sslscanner.h db/sslparser.h
db/sslscanner.o: db/sslscanner.h db/sslparser.h include/types.h include/rtl.h include/exp.h include/operator.h
db/sslscanner.o: include/type.h include/memo.h include/exphelp.h include/register.h db/table.h db/insnameelem.h
db/sslscanner.o: include/util.h include/statement.h include/managed.h include/dataflow.h
db/statement.o: include/statement.h include/memo.h include/exphelp.h include/types.h include/managed.h
db/statement.o: include/dataflow.h include/exp.h include/operator.h include/type.h include/cfg.h include/basicblock.h
db/statement.o: include/proc.h include/hllcode.h include/prog.h include/BinaryFile.h include/frontend.h
db/statement.o: include/sigenum.h include/cluster.h include/boomerang.h include/rtl.h include/register.h include/util.h
db/statement.o: include/signature.h include/visitor.h include/log.h
db/table.o: include/types.h db/table.h include/statement.h include/memo.h include/exphelp.h include/managed.h
db/table.o: include/dataflow.h include/exp.h include/operator.h include/type.h
db/testDbase.o: include/exp.h include/operator.h include/types.h include/type.h include/memo.h include/exphelp.h
db/testDbase.o: db/ExpTest.h db/ProgTest.h include/prog.h include/BinaryFile.h include/frontend.h include/sigenum.h
db/testDbase.o: include/cluster.h db/ProcTest.h include/proc.h include/cfg.h include/basicblock.h include/managed.h
db/testDbase.o: include/dataflow.h include/hllcode.h include/statement.h db/RtlTest.h include/rtl.h include/register.h
db/testDbase.o: db/ParserTest.h
db/testExp.o: db/ExpTest.h include/exp.h include/operator.h include/types.h include/type.h include/memo.h
db/testExp.o: include/exphelp.h
db/testExpStubs.o: include/type.h include/memo.h include/types.h include/cfg.h include/exphelp.h include/basicblock.h
db/testExpStubs.o: include/managed.h include/dataflow.h include/proc.h include/exp.h include/operator.h
db/testExpStubs.o: include/hllcode.h include/statement.h include/prog.h include/BinaryFile.h include/frontend.h
db/testExpStubs.o: include/sigenum.h include/cluster.h include/signature.h include/boomerang.h db/utilStubs.cpp
db/testExpStubs.o: include/util.h db/typeStubs.cpp
db/testRtl.o: db/RtlTest.h include/rtl.h include/exp.h include/operator.h include/types.h include/type.h include/memo.h
db/testRtl.o: include/exphelp.h include/register.h
db/testRtlStubs.o: include/types.h include/cfg.h include/exphelp.h include/basicblock.h include/managed.h
db/testRtlStubs.o: include/dataflow.h include/proc.h include/exp.h include/operator.h include/type.h include/memo.h
db/testRtlStubs.o: include/hllcode.h include/statement.h include/prog.h include/BinaryFile.h include/frontend.h
db/testRtlStubs.o: include/sigenum.h include/cluster.h include/boomerang.h db/typeStubs.cpp db/signatureStubs.cpp
db/testRtlStubs.o: include/signature.h
db/testStmt.o: db/StatementTest.h include/proc.h include/exp.h include/operator.h include/types.h include/type.h
db/testStmt.o: include/memo.h include/exphelp.h include/cfg.h include/basicblock.h include/managed.h include/dataflow.h
db/testStmt.o: include/hllcode.h include/statement.h include/prog.h include/BinaryFile.h include/frontend.h
db/testStmt.o: include/sigenum.h include/cluster.h
db/testStmtStubs.o: include/type.h include/memo.h include/types.h include/cfg.h include/exphelp.h include/basicblock.h
db/testStmtStubs.o: include/managed.h include/dataflow.h include/proc.h include/exp.h include/operator.h
db/testStmtStubs.o: include/hllcode.h include/statement.h include/prog.h include/BinaryFile.h include/frontend.h
db/testStmtStubs.o: include/sigenum.h include/cluster.h include/signature.h include/boomerang.h
db/testStmtStubs.o: frontend/pentiumfrontend.h db/utilStubs.cpp include/util.h db/typeStubs.cpp
db/typeStubs.o: include/type.h include/memo.h include/types.h
db/utilStubs.o: include/util.h
db/visitor.o: include/visitor.h include/exp.h include/operator.h include/types.h include/type.h include/memo.h
db/visitor.o: include/exphelp.h include/statement.h include/managed.h include/dataflow.h include/log.h
db/visitor.o: include/boomerang.h include/proc.h include/cfg.h include/basicblock.h include/hllcode.h
db/visitor.o: include/signature.h include/sigenum.h include/prog.h include/BinaryFile.h include/frontend.h
db/visitor.o: include/cluster.h
db/visitor.old.o: include/visitor.h include/exp.h include/operator.h include/types.h include/type.h include/memo.h
db/visitor.old.o: include/exphelp.h include/statement.h include/managed.h include/dataflow.h include/log.h
db/visitor.old.o: include/boomerang.h include/proc.h include/cfg.h include/basicblock.h include/hllcode.h
db/visitor.old.o: include/signature.h include/sigenum.h include/prog.h include/BinaryFile.h include/frontend.h
db/visitor.old.o: include/cluster.h
db/xmlprogparser.o: include/type.h include/memo.h include/types.h include/cluster.h include/prog.h include/BinaryFile.h
db/xmlprogparser.o: include/frontend.h include/sigenum.h include/proc.h include/exp.h include/operator.h
db/xmlprogparser.o: include/exphelp.h include/cfg.h include/basicblock.h include/managed.h include/dataflow.h
db/xmlprogparser.o: include/hllcode.h include/statement.h include/rtl.h include/register.h include/signature.h
db/xmlprogparser.o: include/xmlprogparser.h include/boomerang.h include/log.h
frontend/FrontPentTest.o: include/types.h include/rtl.h include/exp.h include/operator.h include/type.h include/memo.h
frontend/FrontPentTest.o: include/exphelp.h include/register.h frontend/FrontPentTest.h include/BinaryFile.h
frontend/FrontPentTest.o: include/decoder.h include/AssemblyInfo.h include/prog.h include/frontend.h include/sigenum.h include/cluster.h
frontend/FrontPentTest.o: frontend/pentiumfrontend.h include/BinaryFileStub.h
frontend/FrontSparcTest.o: include/types.h frontend/FrontSparcTest.h include/BinaryFile.h include/proc.h include/exp.h
frontend/FrontSparcTest.o: include/operator.h include/type.h include/memo.h include/exphelp.h include/cfg.h
frontend/FrontSparcTest.o: include/basicblock.h include/managed.h include/dataflow.h include/hllcode.h
frontend/FrontSparcTest.o: include/statement.h include/prog.h include/frontend.h include/sigenum.h include/cluster.h
frontend/FrontSparcTest.o: frontend/sparcfrontend.h include/decoder.h include/AssemblyInfo.h include/rtl.h include/register.h
frontend/FrontSparcTest.o: include/BinaryFileStub.h
frontend/FrontendTest.o: frontend/FrontendTest.h include/BinaryFile.h include/types.h include/prog.h include/frontend.h
frontend/FrontendTest.o: include/sigenum.h include/type.h include/memo.h include/cluster.h
frontend/frontend.o: include/frontend.h include/types.h include/sigenum.h include/BinaryFile.h include/exp.h 
frontend/frontend.o: include/operator.h include/type.h include/memo.h include/exphelp.h include/cfg.h
frontend/frontend.o: include/basicblock.h include/managed.h include/dataflow.h include/proc.h include/hllcode.h
frontend/frontend.o: include/statement.h include/register.h include/rtl.h include/decoder.h include/AssemblyInfo.h frontend/sparcfrontend.h
frontend/frontend.o: frontend/pentiumfrontend.h frontend/ppcfrontend.h frontend/_8051frontend.h frontend/st20frontend.h frontend/mipsfrontend.h include/prog.h
frontend/frontend.o: include/cluster.h include/signature.h include/boomerang.h include/log.h c/ansi-c-parser.h
frontend/njmcDecoder.o: include/rtl.h include/exp.h include/operator.h include/types.h include/type.h include/memo.h
frontend/njmcDecoder.o: include/exphelp.h include/register.h include/decoder.h include/AssemblyInfo.h include/cfg.h include/basicblock.h
frontend/njmcDecoder.o: include/managed.h include/dataflow.h include/proc.h include/hllcode.h include/statement.h
frontend/njmcDecoder.o: include/prog.h include/BinaryFile.h include/frontend.h include/sigenum.h include/cluster.h
frontend/njmcDecoder.o: include/boomerang.h
frontend/pentiumdecoder.o: include/rtl.h include/exp.h include/operator.h include/types.h include/type.h include/memo.h
frontend/pentiumdecoder.o: include/exphelp.h include/register.h include/decoder.h include/AssemblyInfo.h frontend/pentiumdecoder.h
frontend/pentiumdecoder.o: include/prog.h include/BinaryFile.h include/frontend.h include/sigenum.h include/cluster.h
frontend/pentiumdecoder.o: include/proc.h include/cfg.h include/basicblock.h include/managed.h include/dataflow.h
frontend/pentiumdecoder.o: include/hllcode.h include/statement.h include/boomerang.h
frontend/pentiumfrontend.o: include/types.h include/BinaryFile.h include/frontend.h include/sigenum.h
frontend/pentiumfrontend.o: frontend/pentiumfrontend.h include/rtl.h include/exp.h include/operator.h include/type.h
frontend/pentiumfrontend.o: include/memo.h include/exphelp.h include/register.h include/decoder.h include/AssemblyInfo.h
frontend/pentiumfrontend.o: frontend/pentiumdecoder.h include/cfg.h include/basicblock.h include/managed.h
frontend/pentiumfrontend.o: include/dataflow.h include/proc.h include/hllcode.h include/statement.h include/signature.h
frontend/pentiumfrontend.o: include/prog.h include/cluster.h include/boomerang.h include/log.h 
frontend/ppcdecoder.o: include/exp.h include/operator.h include/types.h include/type.h include/memo.h include/exphelp.h
frontend/ppcdecoder.o: include/prog.h include/BinaryFile.h include/frontend.h include/sigenum.h include/cluster.h
frontend/ppcdecoder.o: include/proc.h include/cfg.h include/basicblock.h include/managed.h include/dataflow.h
frontend/ppcdecoder.o: include/hllcode.h include/statement.h include/decoder.h include/AssemblyInfo.h include/rtl.h include/register.h
frontend/ppcdecoder.o: frontend/ppcdecoder.h include/boomerang.h
frontend/_8051decoder.o: include/exp.h include/operator.h include/types.h include/type.h include/memo.h include/exphelp.h
frontend/_8051decoder.o: include/prog.h include/BinaryFile.h include/frontend.h include/sigenum.h include/cluster.h
frontend/_8051decoder.o: include/proc.h include/cfg.h include/basicblock.h include/managed.h include/dataflow.h
frontend/_8051decoder.o: include/hllcode.h include/statement.h include/decoder.h include/AssemblyInfo.h include/rtl.h include/register.h
frontend/_8051decoder.o: frontend/_8051decoder.h include/boomerang.h
frontend/mipsdecoder.o: include/exp.h include/operator.h include/types.h include/type.h include/memo.h include/exphelp.h
frontend/mipsdecoder.o: include/prog.h include/BinaryFile.h include/frontend.h include/sigenum.h include/cluster.h
frontend/mipsdecoder.o: include/proc.h include/cfg.h include/basicblock.h include/managed.h include/dataflow.h
frontend/mipsdecoder.o: include/hllcode.h include/statement.h include/decoder.h  include/AssemblyInfo.h include/rtl.h include/register.h
frontend/mipsdecoder.o: frontend/mipsdecoder.h include/boomerang.h
frontend/mipsfrontend.o: include/exp.h include/operator.h include/types.h include/type.h include/memo.h
frontend/mipsfrontend.o: include/exphelp.h include/register.h include/rtl.h include/cfg.h include/basicblock.h
frontend/mipsfrontend.o: include/managed.h include/dataflow.h include/proc.h include/hllcode.h include/statement.h
frontend/mipsfrontend.o: include/prog.h include/BinaryFile.h include/frontend.h include/sigenum.h include/cluster.h
frontend/mipsfrontend.o: include/decoder.h include/AssemblyInfo.h frontend/mipsdecoder.h frontend/mipsfrontend.h include/boomerang.h
frontend/mipsfrontend.o: include/signature.h
frontend/ppcfrontend.o: include/exp.h include/operator.h include/types.h include/type.h include/memo.h
frontend/ppcfrontend.o: include/exphelp.h include/register.h include/rtl.h include/cfg.h include/basicblock.h
frontend/ppcfrontend.o: include/managed.h include/dataflow.h include/proc.h include/hllcode.h include/statement.h
frontend/ppcfrontend.o: include/prog.h include/BinaryFile.h include/frontend.h include/sigenum.h include/cluster.h
frontend/ppcfrontend.o: include/decoder.h include/AssemblyInfo.h frontend/ppcdecoder.h frontend/ppcfrontend.h include/boomerang.h
frontend/ppcfrontend.o: include/signature.h
frontend/_8051frontend.o: include/exp.h include/operator.h include/types.h include/type.h include/memo.h
frontend/_8051frontend.o: include/exphelp.h include/register.h include/rtl.h include/cfg.h include/basicblock.h
frontend/_8051frontend.o: include/managed.h include/dataflow.h include/proc.h include/hllcode.h include/statement.h
frontend/_8051frontend.o: include/prog.h include/BinaryFile.h include/frontend.h include/sigenum.h include/cluster.h
frontend/_8051frontend.o: include/decoder.h include/AssemblyInfo.h frontend/_8051decoder.h frontend/_8051frontend.h include/boomerang.h
frontend/_8051frontend.o: include/signature.h 
frontend/sparcdecoder.o: include/decoder.h include/AssemblyInfo.h include/types.h include/rtl.h include/exp.h include/operator.h
frontend/sparcdecoder.o: include/type.h include/memo.h include/exphelp.h include/register.h include/prog.h
frontend/sparcdecoder.o: include/BinaryFile.h include/frontend.h include/sigenum.h include/cluster.h include/proc.h
frontend/sparcdecoder.o: include/cfg.h include/basicblock.h include/managed.h include/dataflow.h include/hllcode.h
frontend/sparcdecoder.o: include/statement.h frontend/sparcdecoder.h include/boomerang.h
frontend/sparcfrontend.o: include/exp.h include/operator.h include/types.h include/type.h include/memo.h
frontend/sparcfrontend.o: include/exphelp.h include/register.h include/rtl.h include/cfg.h include/basicblock.h
frontend/sparcfrontend.o: include/managed.h include/dataflow.h include/proc.h include/hllcode.h include/statement.h
frontend/sparcfrontend.o: include/prog.h include/BinaryFile.h include/frontend.h include/sigenum.h include/cluster.h
frontend/sparcfrontend.o: include/decoder.h include/AssemblyInfo.h frontend/sparcdecoder.h frontend/sparcfrontend.h include/boomerang.h
frontend/sparcfrontend.o: include/signature.h include/log.h
frontend/st20decoder.o: include/rtl.h include/exp.h include/operator.h include/types.h include/type.h include/memo.h
frontend/st20decoder.o: include/exphelp.h include/register.h include/decoder.h include/AssemblyInfo.h frontend/st20decoder.h include/proc.h
frontend/st20decoder.o: include/cfg.h include/basicblock.h include/managed.h include/dataflow.h include/hllcode.h
frontend/st20decoder.o: include/statement.h include/boomerang.h
frontend/st20frontend.o: include/exp.h include/operator.h include/types.h include/type.h include/memo.h
frontend/st20frontend.o: include/exphelp.h include/register.h include/rtl.h include/cfg.h include/basicblock.h
frontend/st20frontend.o: include/managed.h include/dataflow.h include/proc.h include/hllcode.h include/statement.h
frontend/st20frontend.o: include/prog.h include/BinaryFile.h include/frontend.h include/sigenum.h include/cluster.h
frontend/st20frontend.o: include/decoder.h include/AssemblyInfo.h frontend/st20decoder.h frontend/st20frontend.h include/boomerang.h
frontend/st20frontend.o: include/signature.h
frontend/testFront.o: frontend/FrontSparcTest.h include/BinaryFile.h include/types.h frontend/FrontPentTest.h
frontend/testFront.o: include/decoder.h include/AssemblyInfo.h include/rtl.h include/exp.h include/operator.h include/type.h include/memo.h
frontend/testFront.o: include/exphelp.h include/register.h include/prog.h include/frontend.h include/sigenum.h
frontend/testFront.o: include/cluster.h
transform/generic.o: include/types.h include/statement.h include/memo.h include/exphelp.h include/managed.h
transform/generic.o: include/dataflow.h include/cfg.h include/basicblock.h include/exp.h include/operator.h
transform/generic.o: include/type.h include/register.h include/rtl.h include/proc.h include/hllcode.h
transform/generic.o: include/boomerang.h include/transformer.h transform/generic.h include/log.h
transform/rdi.o: include/types.h include/statement.h include/memo.h include/exphelp.h include/managed.h
transform/rdi.o: include/dataflow.h include/cfg.h include/basicblock.h include/exp.h include/operator.h include/type.h
transform/rdi.o: include/register.h include/rtl.h include/proc.h include/hllcode.h include/transformer.h
transform/rdi.o: transform/rdi.h
transform/transformation-parser.o: include/exp.h include/operator.h include/types.h include/type.h include/memo.h
transform/transformation-parser.o: include/exphelp.h include/cfg.h include/basicblock.h include/managed.h
transform/transformation-parser.o: include/dataflow.h include/proc.h include/hllcode.h include/statement.h
transform/transformation-parser.o: include/signature.h include/sigenum.h include/transformer.h transform/generic.h
transform/transformation-parser.o: transform/transformation-scanner.h transform/transformation-parser.h
transform/transformation-scanner.o: transform/transformation-scanner.h transform/transformation-parser.h
transform/transformer.o: include/types.h include/statement.h include/memo.h include/exphelp.h include/managed.h
transform/transformer.o: include/dataflow.h include/cfg.h include/basicblock.h include/exp.h include/operator.h
transform/transformer.o: include/type.h include/register.h include/rtl.h include/proc.h include/hllcode.h
transform/transformer.o: include/boomerang.h include/transformer.h transform/rdi.h include/log.h
transform/transformer.o: transform/transformation-parser.h
type/TypeTest.o: type/TypeTest.h include/util.h include/type.h include/memo.h include/types.h include/BinaryFile.h
type/TypeTest.o: frontend/pentiumfrontend.h include/frontend.h include/sigenum.h include/signature.h include/exp.h
type/TypeTest.o: include/operator.h include/exphelp.h include/statement.h include/managed.h include/dataflow.h
type/TypeTest.o: include/boomerang.h include/log.h include/prog.h include/cluster.h
type/constraint.o: include/constraint.h include/statement.h include/memo.h include/exphelp.h include/types.h
type/constraint.o: include/managed.h include/dataflow.h include/exp.h include/operator.h include/type.h
type/constraint.o: include/boomerang.h include/log.h
type/dfa.o: include/type.h include/memo.h include/types.h include/boomerang.h include/signature.h include/exp.h
type/dfa.o: include/operator.h include/exphelp.h include/sigenum.h include/statement.h include/managed.h
type/dfa.o: include/dataflow.h include/prog.h include/BinaryFile.h include/frontend.h include/cluster.h include/util.h
type/dfa.o: include/visitor.h include/log.h include/proc.h include/cfg.h include/basicblock.h include/hllcode.h
type/type.o: include/types.h include/type.h include/memo.h include/util.h include/exp.h include/operator.h
type/type.o: include/exphelp.h include/cfg.h include/basicblock.h include/managed.h include/dataflow.h include/proc.h
type/type.o: include/hllcode.h include/statement.h include/signature.h include/sigenum.h include/boomerang.h
type/type.o: include/log.h
util/UtilTest.o: util/UtilTest.h include/util.h include/type.h include/memo.h include/types.h
util/testUtil.o: util/UtilTest.h include/util.h include/type.h include/memo.h include/types.h
util/util.o: include/util.h
