TOP = ..
include $(TOP)/configure/CONFIG

SRC = $(TOP)/src

SRC_DIRS += $(SRC)/nt

INC += nt.h
INC += ntutils.h
INC += ntfield.h
INC += ntscalar.h
INC += ntscalarArray.h
INC += ntnameValue.h
INC += nttable.h
INC += ntmultiChannel.h
INC += ntscalarMultiChannel.h
INC += ntndarray.h
INC += ntmatrix.h
INC += ntenum.h
INC += ntunion.h
INC += ntaggregate.h
INC += ntattribute.h
INC += ntcontinuum.h

LIBSRCS += ntutils.cpp
LIBSRCS += ntfield.cpp
LIBSRCS += ntscalar.cpp
LIBSRCS += ntscalarArray.cpp
LIBSRCS += ntnameValue.cpp
LIBSRCS += nttable.cpp
LIBSRCS += ntmultiChannel.cpp
LIBSRCS += ntscalarMultiChannel.cpp
LIBSRCS += ntndarray.cpp
LIBSRCS += ntmatrix.cpp
LIBSRCS += ntenum.cpp
LIBSRCS += ntunion.cpp
LIBSRCS += ntaggregate.cpp
LIBSRCS += ntattribute.cpp
LIBSRCS += ntcontinuum.cpp

LIBRARY = nt

nt_LIBS += pvData Com

include $(TOP)/configure/RULES

