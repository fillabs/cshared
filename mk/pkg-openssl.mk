 ifneq ($(findstring mingw32,$(ARCH)),)
  ifeq ($(OPENSSL_DIR),)
    OPENSSL_DIR := C:/OpenSSL/Win32
  endif
  includes += $(OPENSSL_DIR)/include
  libdirs  += $(OPENSSL_DIR)/lib
#  libs += $(OPENSSL_DIR)/lib/MinGW/libcrypto.a $(OPENSSL_DIR)/lib/MinGW/libssl.a
 endif

 libs += -lssl -lcrypto
