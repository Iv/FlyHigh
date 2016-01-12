include_directories(${CMAKE_SOURCE_DIR}/src/database
                    ${CMAKE_SOURCE_DIR}/src/database/sql
                    ${CMAKE_SOURCE_DIR}/src/settings/flyhigh)

set(dbmigrator_SRCS CredentialsDlg.cpp
                    DatabaseWidget.cpp
                    MigrationDlg.cpp
                    MigratorThread.cpp)

add_library(dbmigrator ${dbmigrator_SRCS})
