export LANG=en_US
ALL_CONFIGURATIONS := POSIX WIN32
.PHONY: all clean tests docs cleandocs distr DUMMY echo echo_includes echo_defines echo_libs echo_cflags echo_ldflags

BUILDROOT ?= $(PROJECTROOT)/build
CSHAREDDIR ?= $(PROJECTROOT)/cshared 
CXMLDIR ?= $(PROJECTROOT)/cxml

HOSTARCH = $(shell gcc -dumpmachine)
ifeq ($(ARCH),)
 ARCH = $(HOSTARCH)
 GCC := gcc
 STRIP := strip
 AR := ar
else ifeq ($(ARCH),$(HOSTARCH))
 GCC := gcc
 STRIP := strip
 AR := ar
else
 GCC   := $(addprefix $(addsuffix -,$(ARCH)), gcc)
 STRIP := $(addprefix $(addsuffix -,$(ARCH)), strip)
 AR    := $(addprefix $(addsuffix -,$(ARCH)), ar)
endif

ifneq (,$(findstring cygwin,$(ARCH)))
 STRIP := strip
endif

-include $(CSHAREDDIR)/mk/$(ARCH).mk

ifeq ($(DEBUG),)
 DEBUG=no
endif

ifeq ($(DEBUG),yes)
  cflags  += -g -O0
  defines += DEBUG
  dsuffix = -d
else
  defines += NDEBUG
  cflags  += -O2
endif

outdir     := $(BUILDROOT)/$(ARCH)$(dsuffix)
objdir     := $(outdir)/o-$(PROJECT)

ifneq ($(findstring w32,$(ARCH)),)
 packages := $(filter-out readline threads, $(packages))
 CFG  += WIN32
else 
 CFG  += POSIX
endif

ifneq ($(findstring cygwin,$(ARCH)),)
 CFG += CYGWIN
endif

ifneq ($(findstring openwrt,$(ARCH)),)
 CFG += OPENWRT
endif

cflags   += -fPIC -Wall

define IncludePackage
 ifeq (,$$(pkg_$(1)_disable))
  #$$(info include $$(CSHAREDDIR)/mk/pkg-$(1).mk)
  include $$(CSHAREDDIR)/mk/pkg-$(1).mk
  ifeq (,$$(pkg_$(1)_disable))
   sources  += $$(sources_$(1))
   includes += $$(includes_$(1))
   defines  += $$(defines_$(1))
   cflags   += $$(cflags_$(1))
   libs     += $$(libs_$(1))
  endif
 endif
endef

$(foreach pkg,$(packages),$(eval $(call IncludePackage,$(pkg))))

ifeq ($(testdir), )
  testdir := tests
endif

all_includes  := $(includes)  $(foreach cfg,$(CFG),$(includes-$(cfg)))
all_defines   := $(defines)   $(foreach cfg,$(CFG),$(defines-$(cfg)))
all_cflags    := $(cflags)    $(foreach cfg,$(CFG),$(cflags-$(cfg)))
all_ldflags   := $(ldflags)   $(foreach cfg,$(CFG),$(ldflags-$(cfg)))
all_libs      := $(libs)      $(foreach cfg,$(CFG),$(libs-$(cfg)))
all_libdirs   := $(libdirs)   $(foreach cfg,$(CFG),$(libdirs-$(cfg)))
all_sources   := $(sources)   $(foreach cfg,$(CFG),$(sources-$(cfg))) 
all_headers   := $(headers)   $(foreach cfg,$(CFG),$(headers-$(cfg)))
all_tests     := $(tests)     $(foreach cfg,$(CFG),$(tests-$(cfg)))
all_distfiles := $(distfiles) $(foreach cfg,$(CFG),$(distfiles-$(cfg)))
all_predirs   := $(predirs)   $(foreach cfg,$(CFG),$(predirs-$(cfg)))
all_postdirs  := $(postdirs)  $(foreach cfg,$(CFG),$(postdirs-$(cfg)))

define IncludeModule
undefine sources
undefine modules
undefine includes
undefine defines
undefine distfiles
undefine libdirs
include $(1)/module.mk
$$(foreach V, $$(sources),   $$(eval all_sources  +=  $$(if $$(filter /%, $$(V)), $$(TOPDIR)$$(V), $(1)/$$(V))))
$$(foreach V, $$(includes),  $$(eval all_includes += $$(if $$(filter /%, $$(V)), $$(TOPDIR)$$(V), $(1)/$$(V))))
$$(foreach V, $$(libdurs),   $$(eval all_libdirs  += $$(if $$(filter /%, $$(V)), $$(TOPDIR)$$(V), $(1)/$$(V))))
$$(foreach V, $$(modules),   $$(eval $$(call IncludeModule, $$(if $$(filter /%, $$(V)), $$(TOPDIR)$$(V), $(1)/$$(V)))))
all_defines += $$(defines)
$$(foreach V, $$(distfiles), $$(eval all_distfiles += $$(if $$(filter /%, $$(V)), $$(TOPDIR)$$(V), $(1)/$$(V))))
endef

$(foreach V, $(modules), $(eval $(call IncludeModule, $(V))))

tests     := $(addprefix $(addsuffix /,$(testdir)),$(tests))
sources   := $(addprefix $(addsuffix /,$(srcdir)),$(all_sources))
headers   := $(addprefix $(addsuffix /,$(incdir)),$(all_headers))
distfiles := $(all_distfiles)
cflags    := $(all_cflags) $(addprefix -I, $(all_includes)) $(addprefix -D, $(all_defines))
ldflags   := $(all_ldflags) $(addprefix -L,$(outdir) $(all_libdirs))
libs      := $(all_libs)

objects    := $(patsubst %.c, $(objdir)/%.o, $(sources))
$(foreach b,$(bins),$(eval objects-$(b)=$$(patsubst %.c,$$(objdir)/%.o,$$(sources-$(b)))))

binobjects := $(foreach b,$(bins),$(objects-$(b))) 
testbins   := $(patsubst %.c, $(outdir)/%, $(tests))
dirs       := $(objdir) $(outdir)/tests

alibnames  := $(patsubst %, $(outdir)/lib%.a,  $(alibs))
solibnames := $(patsubst %, $(outdir)/lib%.so, $(solibs))
binnames   := $(patsubst %, $(outdir)/%, $(bins))

all: $(dirs) $(pre) $(predirs) $(alibnames) $(solibnames) $(binnames) $(postdirs) $(post)

echo: echo_includes echo_defines echo_libs
echo_includes:
	@echo includes=$(all_includes)
echo_defines:
	@echo defines=$(all_defines)
echo_libs:
	@echo libs=$(libs)
echo_cflags:
	@echo cflags=$(cflags)
echo_ldflags:
	@echo ldflags=$(ldflags)

tests: all $(testbins)

$(predirs) $(postdirs): DUMMY
	$(MAKE) -C $@ ARCH=$(ARCH) PROJECTROOT=$(realpath $(PROJECTROOT)) BUILDROOT=$(realpath $(BUILDROOT)) DEBUG=$(DEBUG)

define ALibRule
$$(outdir)/lib$(l).a: $$(outdir)/lib%.a : $$(objects-$(l)) $$(objects) $$(deps) $$(deps-$(l))
	$$(AR) rcs $$@ $$(objects-$(l)) $$(objects) 
ifeq (no,$$(DEBUG))
	$$(STRIP) --strip-unneeded $$@
endif
endef
$(foreach l, $(alibs), $(eval $(ALibRule)))

define SoLibRule
$$(outdir)/lib$(l).so: $$(outdir)/lib%.so : $$(objects-$(l)) $$(objects) $$(deps) $$(deps-$(l))
	$$(GCC) $$(cflags) -shared $$(ldflags) -o $$@ $$(objects-$(l)) $$(objects)  $$(csharedlib) $$(libs) $$(libs-$(l))
ifeq (no,$$(DEBUG))
	$$(STRIP) -s $$@
endif
endef
$(foreach l, $(solibs), $(eval $(SoLibRule)))

define BinRule
$$(outdir)/$(b): $$(outdir)/%: $$(objects-$(b)) $$(objects) $$(deps) $$(deps-$(b))
	$$(GCC) $$(cflags) $$(ldflags) -o $$@ $$(objects-$(b)) $$(objects)  $$(csharedlib) $$(libs) $$(libs-$(b)) 
ifeq (no,$$(DEBUG))
	$$(STRIP) -s $$@
endif
endef
$(foreach b, $(bins), $(eval $(BinRule)))

$(testbins): $(alibnames)
$(testbins): $(outdir)/tests/% : tests/%.c
	$(GCC) $(cflags) $(cflags-$*) -o $@ $< $(alibnames) $(libs) $(libs-$*)
ifeq (no,$(DEBUG))
	$(STRIP) -s $@
endif

$(dirs) $(INSTALLROOT):
	mkdir -p "$@"

$(objects) $(binobjects): $(objdir)/%.o: %.c
	@mkdir -p "$(dir $@)"
	$(GCC)  $(cflags) -o $@ -MMD -MF $(objdir)/$*.d -c $(abspath $<)

clean:
	rm -rf $(alibnames) $(solibnames) $(binnames) $(testbins) $(objects) $(patsubst %.o, %.d, $(objects)) $(tobeclean)

distfiles += $(wildcard Makefile $(DOXYFILE))
dist:
	-rm -rf $(PROJECT) $(PROJECT)-$(shell date -u '+%Y%m%d').tar.gz
	mkdir "$(PROJECT)"
	cp --parents $(sources) $(headers) $(distfiles) $(addprefix tests/, $(tests)) $(PROJECT)
	tar -zcvf $(PROJECT)-$(shell date -u '+%Y%m%d').tar.gz $(PROJECT)
	rm -rf $(PROJECT)
#	tar -zcvf $(PROJECT)-$(shell date -u '+%Y%m%d').tar.gz $(sources) $(headers) $(distfiles) $(addprefix tests/, $(tests))

ifneq (,$(DOXYFILE))
docs: $(DOXYFILE)
	doxygen $(DOXYFILE)

cleandocs:
	rm -rf doc/html
endif

ifneq (,$(INSTALLROOT))

install: $(INSTALLROOT) 
ifneq (,$(strip $(binnames)))
	mkdir -p "$(INSTALLROOT)/$(ARCH)$(dsuffix)/bin"
	cp -f $(binnames) "$(INSTALLROOT)/$(ARCH)$(dsuffix)/bin"
endif
#ifneq (,$(strip $(tests)))
#	mkdir -p $(INSTALLROOT)/$(ARCH)$(dsuffix)/bin
#	cp -f $(testbins) $(INSTALLROOT)/$(ARCH)$(dsuffix)/bin
#endif
ifneq (,$(strip $(alibnames) $(solibnames)))
	mkdir -p "$(INSTALLROOT)/$(ARCH)$(dsuffix)/lib"
	cp -f $(alibnames) $(solibnames) "$(INSTALLROOT)/$(ARCH)$(dsuffix)/lib"
endif
ifneq (,$(headers))
	mkdir -p "$(INSTALLROOT)/include"
	cp -rf $(headers) $(INSTALLROOT)/include
endif

endif  # INSTALLROOT

include $(wildcard $(addsuffix /*.d, $(objdir)))
