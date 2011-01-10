include_directories(${CMAKE_SOURCE_DIR}/src/common/container
                    ${CMAKE_SOURCE_DIR}/src/common/utils
                    ${CMAKE_SOURCE_DIR}/src/database)

set(dbgarmin_SRCS GarminDevice.cpp
                  gpsapp.c
                  gpscom.c
                  gpsinput.c
                  gpsmath.c
                  gpsmem.c
                  gpsprot.c
                  gpsread.c
                  gpsrqst.c
                  gpssend.c
                  gpsserial.c
                  gpsutil.c
                  GarminFmt.cpp)

add_library(dbgarmin ${dbgarmin_SRCS})
