CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

# These are all of the important sources
FILE(GLOB AMOP_SRCS *.cpp)

include_directories(../include)

if (MSVC_IDE)
    # These are the header files. Listed to make a group in Visual Studio IDE
    FILE(GLOB folder_header ../include/*.h)
    SOURCE_GROUP("Header Files" FILES ${folder_header})
    
    LIST(APPEND AMOP_SRCS ${folder_header})
endif(MSVC_IDE)

add_library(amop STATIC ${AMOP_SRCS})

