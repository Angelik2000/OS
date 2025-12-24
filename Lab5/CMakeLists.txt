cmake_minimum_required(VERSION 3.10)

# Имя проекта
project(Lab5_NamedPipes)

# Стандарт C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Исходные файлы
set(SOURCES
    main.cpp
    FileController.cpp
    SyncManager.cpp
    ServerProcess.cpp
    ClientProcess.cpp
)

set(HEADERS
    Employee.h
    Utils.h
    FileController.h
    SyncManager.h
    ServerProcess.h
    ClientProcess.h
)

# Создание исполняемого файла
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Для корректной работы в Visual Studio (группировка файлов в обозревателе)
source_group("Source Files" FILES ${SOURCES})
source_group("Header Files" FILES ${HEADERS})