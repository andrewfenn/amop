CMAKE_MINIMUM_REQUIRED(VERSION 2.4)

# These are all of the important sources
FILE(GLOB AMOP_SRCS *.cpp)

include_directories(../include)

if (MSVC_IDE)
    # These are the header files. Listed to make a group in Visual Studio IDE
    FILE(GLOB AMOP_HDRS ../include/*.h)
    SOURCE_GROUP("Header Files" FILES ${AMOP_HDRS})
    
    LIST(APPEND AMOP_SRCS ${AMOP_HDRS})
endif(MSVC_IDE)

add_library(Amop STATIC ${AMOP_SRCS})

