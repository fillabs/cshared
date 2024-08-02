ifeq (,$(CSHAREDDIR))
 $(error Variable CSHAREDDIR must be set to use package cxml)
endif

predirs  += $(CSHAREDDIR)
includes += $(CSHAREDDIR)
deps     += $(outdir)/libcshared.a
libs     += $(outdir)/libcshared.a
