TOP=../..

include $(TOP)/configure/CONFIG

PROD_LIBS += nt pvData Com

PROD_HOST += ntfieldTest
ntfieldTest_SRCS += ntfieldTest.cpp
ntfieldTest_LIBS += nt pvData Com

PROD_HOST += ntnameValueTest
ntnameValueTest_SRCS += ntnameValueTest.cpp
ntnameValueTest_LIBS += nt pvData Com

PROD_HOST += ntmultiChannelTest
ntmultiChannelTest_SRCS += ntmultiChannelTest.cpp
ntmultiChannelTest_LIBS += nt pvData Com

TESTPROD_HOST += nttableTest
nttableTest_SRCS = nttableTest.cpp
TESTS += nttableTest

TESTPROD_HOST += ntmultiChannelTest
ntmultiChannelTest_SRCS = ntmultiChannelTest.cpp
TESTS += ntmultiChannelTest

TESTSCRIPTS_HOST += $(TESTS:%=%.t)

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

