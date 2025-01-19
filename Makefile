PROJECTROOT   = ..
CSHAREDDIR    = .
PROJECT       = cshared
DEBUG         = yes
testdir       = tests

alibs        = $(PROJECT)
solibs       = $(PROJECT)

sources       := copts.c cserialize.c cstr.c cring.c e4c_lite.c casn_oer.c cmsgq.c ctree.c clog.c citstime.c
sources-WIN32 := cdir_win.c mkgmtime.c
headers       := copts.h cserialize.h cstr.h cdir.h cring.h e4c_lite.h cmsgq.h cbyteswap.h clog.h mkgmtime.h
tests         := test_copts.c test_cmsgq.c test_ctree.c test_coer.c
packages      := thread
include common.mk
