ifeq (,$(CXMLDIR))
 $(error Variable CXMLDIR must be set to use package cxml)
endif

predirs  += $(CXMLDIR)
includes += $(CXMLDIR)
deps     += $(outdir)/libcxml.a
libs     += $(outdir)/libcxml.a
