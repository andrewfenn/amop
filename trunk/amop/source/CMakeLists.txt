set(AMOP_SRCS CheckOffsetFunc.cpp DynamicFunction.cpp DynamicObject.cpp MockFunctionImpl.cpp MockObjectBase.cpp VirtualTable.cpp)

include_directories(../include)

add_library(amop STATIC ${AMOP_SRCS})
