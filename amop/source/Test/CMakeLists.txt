CMAKE_MINIMUM_REQUIRED(VERSION 2.4)

if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)


# These are all of the important sources
FILE(GLOB AMOP_TEST_SRCS *.cpp)

# Paths to the UnitTest++ libraries
find_path(UNITTEST_INCLUDE_DIR UnitTest++.h)
find_library(UNITTEST_LIB_DIR UnitTest++)


include_directories(../../include ${UNITTEST_INCLUDE_DIR})
link_directories(${UNITTEST_LIB_DIR})

if (MSVC_IDE)
    # These are the header files. Listed to make a group in Visual Studio IDE
    FILE(GLOB AMOP_TEST_HDRS *.h)
    SOURCE_GROUP("Header Files" FILES ${AMOP_TEST_HDRS})
    
    LIST(APPEND AMOP_TEST_SRCS ${AMOP_TEST_HDRS})
endif(MSVC_IDE)

add_executable(UnitTestAmop ${AMOP_TEST_SRCS}) 
target_link_libraries(UnitTestAmop Amop UnitTest++)



#Add a target called  test  that will run the unit test
# the ALL adds this to the 'ALL' choices. The DEPENDS
# does not seem to be working
get_target_property(unit_test_exe UnitTestAmop LOCATION)
add_custom_target (
  test ALL
  COMMAND ${unit_test_exe}
)

ADD_DEPENDENCIES(test UnitTestAmop)
