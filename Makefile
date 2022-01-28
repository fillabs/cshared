PROJECTROOT  ?= ..
BUILDROOT     = $(PROJECTROOT)/build/
PROJECT       = cshared
DEBUG         = yes
testdir       = tests

alibs        = $(PROJECT)
solibs       = $(PROJECT)

sources       := copts.c cserialize.c cstr.c cring.c e4c_lite.c casn_oer.c cmsgq.c
sources-WIN32 := cdir_win.c
headers       := copts.h cserialize.h cstr.h cdir.h cring.h e4c_lite.h cmsgq.h cbyteswap.h
tests         := test_copts.c test_cmsgq.c
packages      := thread

include common.mk
