include vars.mk

EXE     =me7sum$(EXE_EXT)
LIBS    =ini
SUBDIRS =inifile
LDFLAGS=-Linifile

include makefile.common

me7sum.o: me7sum.c crc32.h utils.h inifile_prop.h inifile/inifile.h
inifile_prop.o: inifile_prop.h inifile/inifile.h
crc32.o: crc32.h
utils.o: utils.h
