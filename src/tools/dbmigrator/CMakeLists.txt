include_directories(${CMAKE_SOURCE_DIR}/src/database
                    ${CMAKE_SOURCE_DIR}/src/database/sql
                    ${QT_INCLUDES})

set(dbmigrator_SRCS MigrationDlg.cpp)

set(dbmigrator_HDRS MigrationDlg.h)

QT4_WRAP_CPP(dbmigrator_SRCS ${dbmigrator_HDRS})

add_library(dbmigrator ${dbmigrator_SRCS} ${dbmigrator_HDRS})
