include_directories(${CMAKE_SOURCE_DIR}/src/common/container
                    ${CMAKE_SOURCE_DIR}/src/common/qextserialport
                    ${CMAKE_SOURCE_DIR}/src/common/utils
                    ${CMAKE_SOURCE_DIR}/src/database
                    ${QT_INCLUDES})

set(dbflytec_SRCS flytec_al.c
                  flytec_ll.c
                  flytec_pl_rs232.c
                  Flytec5020.cpp
                  Protocol5020.cpp
                  Device5020.cpp)

add_library(dbflytec ${dbflytec_SRCS})
