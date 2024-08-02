 ifneq ($(findstring cygwin,$(ARCH)),)
  ifeq (,$(NPCAP_SDK))
   $(error Variable NPCAP_SDK must be set to use package pcap)
  endif
  override NPCAP_SDK := $(shell cygpath -d "$(NPCAP_SDK)")
  override NPCAP_SDK := $(shell cygpath -u "$(NPCAP_SDK)")

  includes += "$(NPCAP_SDK)/Include"
  libs     += "/cygdrive/c/Windows/System32/Npcap/wpcap.dll"
 else
  libs    += -lpcap
 endif
