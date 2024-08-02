  ifneq (,$(GPSD_SDK))
   includes += "$(GPSD_SDK)/include"
   libdirs  += "$(GPSD_SDK)"
   defines  += USE_LIBGPS
   libs     += -lgps -lm
  else ifneq (,$(wildcard /usr/include/gps.h))
  #check include file
   defines  += USE_LIBGPS
   libs     += -lgps -lm
  else
   $(warning libgps not found. Set GPSD_SDK variable)
  endif
 
