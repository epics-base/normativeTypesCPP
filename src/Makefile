TOP = ..
include $(TOP)/configure/CONFIG

SRC = $(TOP)/src/

SRC_DIRS += $(SRC)/nt

INC += nt.h
INC += ntfield.h
INC += ntnameValue.h
INC += nttable.h
INC += ntmultiChannel.h

LIBSRCS += ntfield.cpp
LIBSRCS += ntnameValue.cpp
LIBSRCS += nttable.cpp
LIBSRCS += ntmultiChannel.cpp

LIBRARY=nt

nt_LIBS += pvData Com

include $(TOP)/configure/RULES

