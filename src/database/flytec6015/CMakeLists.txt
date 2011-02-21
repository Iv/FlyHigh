include(${QT_USE_FILE})

include_directories(${CMAKE_SOURCE_DIR}/src/common/container
                    ${CMAKE_SOURCE_DIR}/src/common/qextserialport
                    ${CMAKE_SOURCE_DIR}/src/common/utils
                    ${CMAKE_SOURCE_DIR}/src/database
                    ${QT_INCLUDES} )

set(dbflytec6015_SRCS Device6015.cpp
                      Flytec6015.cpp
                      Protocol6015.cpp)

add_library(dbflytec6015 ${dbflytec6015_SRCS})
