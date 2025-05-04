cmake_minimum_required(VERSION 3.19)
project(SimInter LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Charts)

qt_standard_project_setup()

qt_add_executable(SimInter
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    Processor/headers/CacheMemory.h
    Processor/headers/Instruction.h
    Processor/headers/InstructionList.h
    Processor/headers/InstructionMemory.h
    Processor/headers/Interconnect.h
    Processor/headers/List.h
    Processor/headers/ListNode.h
    Processor/headers/Processor.h
    Processor/headers/ProcessorCache.h
    Processor/headers/ProcessorController.h
    Processor/headers/ProcessorRead.h
    Processor/headers/ProcessorWrite.h
    Processor/headers/SharedMemory.h
    Processor/sources/CacheMemory.cpp
    Processor/sources/Instruction.cpp
    Processor/sources/InstructionList.cpp
    Processor/sources/InstructionMemory.cpp
    Processor/sources/Interconnect.cpp
    Processor/sources/List.cpp
    Processor/sources/ListNode.cpp
    Processor/sources/Processor.cpp
    Processor/sources/ProcessorCache.cpp
    Processor/sources/ProcessorController.cpp
    Processor/sources/ProcessorRead.cpp
    Processor/sources/ProcessorWrite.cpp
    Processor/sources/SharedMemory.cpp
    slotinvalidated.h slotinvalidated.cpp
    slotslist.h slotslist.cpp
    stepchanges.h stepchanges.cpp
    memorychange.h memorychange.cpp


)

target_link_libraries(SimInter
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::Charts
)

include(GNUInstallDirs)

install(TARGETS SimInter
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET SimInter
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
