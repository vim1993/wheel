PROJECT(WHEEL_1_0)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.7)

SET(TEST_ELF "YES")
SET(LX_DEBUG_ENABLE "NO")

ADD_DEFINITIONS(-DUNIT_TEST)
ADD_DEFINITIONS(-DARCH_LINUX_X86)

IF(LX_DEBUG_ENABLE STREQUAL "YES")
ADD_DEFINITIONS(-DLX_DEBUG)
ENDIF()

LINK_DIRECTORIES(/usr/lib)
LINK_DIRECTORIES(${CMAKE_HOME_DIRECTORY}/libs)

INCLUDE_DIRECTORIES(./include)
AUX_SOURCE_DIRECTORY(./src SRC_CODE)

ADD_LIBRARY(${PROJECT_NAME} SHARED ${SRC_CODE})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} pthread crypto)

IF(TEST_ELF STREQUAL "YES")
AUX_SOURCE_DIRECTORY(./ TEST_MAIN)
ADD_EXECUTABLE(test.elf ${TEST_MAIN})
TARGET_LINK_LIBRARIES(test.elf ${PROJECT_NAME})
ENDIF()





