include_directories(${CMAKE_SOURCE_DIR}/src/common/container
                    ${CMAKE_SOURCE_DIR}/src/common/utils
                    ${CMAKE_SOURCE_DIR}/src/database)

set(dbflytec_SRCS flytec_al.c
                  flytec_ll.c
                  flytec_pl_rs232.c
                  FlytecDevice.cpp
                  Flytec5020.cpp
                  Protocol5020.cpp
                  Device5020.cpp
                  )

add_library(dbflytec ${dbflytec_SRCS})
