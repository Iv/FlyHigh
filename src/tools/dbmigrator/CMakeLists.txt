include_directories(${CMAKE_SOURCE_DIR}/src/database
                    ${CMAKE_SOURCE_DIR}/src/database/sql
                    ${CMAKE_SOURCE_DIR}/src/settings/flyhigh
                    ${QT_INCLUDES})

set(dbmigrator_SRCS CredentialsDlg.cpp
                    DatabaseWidget.cpp
                    MigrationDlg.cpp
                    MigratorThread.cpp)

set(dbmigrator_HDRS DatabaseWidget.h
                    MigrationDlg.h
                    MigratorThread.h)

QT4_WRAP_CPP(dbmigrator_SRCS ${dbmigrator_HDRS})

add_library(dbmigrator ${dbmigrator_SRCS} ${dbmigrator_HDRS})
