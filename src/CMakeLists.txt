cmake_minimum_required(VERSION 3.2)
project(HubeiExin C)
set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR i686)
#set(CMAKE_C_COMPILER "/usr/bin/clang")
#set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")

set(SOURCE_FILES hubeiexin.h hubeiexin.c utils.h utils.c)

if("Linux" STREQUAL CMAKE_SYSTEM_NAME)
    set(PLATFORM_FILES platform_linux.c platform.h)

    # As linux build requires openssl
    find_package(PkgConfig REQUIRED)
    pkg_search_module(OPENSSL REQUIRED openssl)
    include_directories(${OPENSSL_INCLUDE_DIRS})
endif("Linux" STREQUAL CMAKE_SYSTEM_NAME)

if("Windows" STREQUAL CMAKE_SYSTEM_NAME)
    set(PLATFORM_FILES platform_win32.c platform.h)
    if("Linux" STREQUAL CMAKE_HOST_SYSTEM_NAME)
        if("x86_64" STREQUAL CMAKE_SYSTEM_PROCESSOR)
            set(CMAKE_C_COMPILER "x86_64-w64-mingw32-gcc")
            set(CMAKE_CXX_COMPILER "x86_64-w64-mingw32-g++")
            set(CMAKE_LINKER "x86_64-w64-mingw32-ld")
        else()
            set(CMAKE_C_COMPILER "i686-w64-mingw32-gcc")
            set(CMAKE_CXX_COMPILER "i686-w64-mingw32-g++")
            set(CMAKE_LINKER "i686-w64-mingw32-ld")
        endif()
        set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")  # otherwise error by unsupported linker option "-rdynamic"
        set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")  # otherwise error by unsupported linker option "-rdynamic"
    endif("Linux" STREQUAL CMAKE_HOST_SYSTEM_NAME)
endif("Windows" STREQUAL CMAKE_SYSTEM_NAME)

set(EXAM_FILES exam.h exam.c)
add_executable(Exam ${SOURCE_FILES} ${EXAM_FILES} ${PLATFORM_FILES})
if("Linux" STREQUAL CMAKE_SYSTEM_NAME)
    target_link_libraries(Exam ${OPENSSL_LIBRARIES})
endif("Linux" STREQUAL CMAKE_SYSTEM_NAME)


if("x86_64" STREQUAL CMAKE_SYSTEM_PROCESSOR)
    set(CROSSEXAM_FILES crossexam.h crossexam.c)
    set(ASM64_FILES asm.c asm.h)

    add_executable(CrossExam ${SOURCE_FILES} ${ASM64_FILES} ${CROSSEXAM_FILES} ${PLATFORM_FILES})
    if("Linux" STREQUAL CMAKE_SYSTEM_NAME)
        target_link_libraries(CrossExam ${OPENSSL_LIBRARIES})
    endif("Linux" STREQUAL CMAKE_SYSTEM_NAME)

    add_test(NAME CrossExam
             COMMAND CrossExam)
endif("x86_64" STREQUAL CMAKE_SYSTEM_PROCESSOR)

if("Linux" STREQUAL CMAKE_SYSTEM_NAME)
    set(PLUGIN_FILES ppp_plugin.c ppp_plugin.h)
    set(PPP_VERSION_FILES ppp_version.c)
    set(SETUP_FILES ppp_setup.py)

    add_library(HubeiExin4PPP SHARED ${SOURCE_FILES} ${PLUGIN_FILES} ${PLATFORM_FILES})
    target_link_libraries(HubeiExin4PPP ${OPENSSL_LIBRARIES})

    add_executable(PrintPPPVersion ${PPP_VERSION_FILES})

    # Installations
    execute_process(COMMAND ./PrintPPPVersion
                    OUTPUT_VARIABLE PPP_VERSION
                    OUTPUT_STRIP_TRAILING_WHITESPACE)

    install(TARGETS HubeiExin4PPP
            LIBRARY
            DESTINATION "/usr/lib/pppd/${PPP_VERSION}")

    install(PROGRAMS ${SETUP_FILES}
            DESTINATION "/usr/bin"
            RENAME "hubei_exin_setup")

    install(PROGRAMS ppp_hubeiexin.ip-up
            DESTINATION "/etc/ppp/ip-up.d/"
            RENAME "01-hubeiexin.sh")
endif("Linux" STREQUAL CMAKE_SYSTEM_NAME)

if("Windows" STREQUAL CMAKE_SYSTEM_NAME)
    set(RASDIALER_FILES ras_dialer.c ras_dialer.h)
    add_executable(RasDialer ${RASDIALER_FILES} ${SOURCE_FILES} ${PLATFORM_FILES})
    target_link_libraries(RasDialer rasapi32 rasdlg)
endif("Windows" STREQUAL CMAKE_SYSTEM_NAME)

enable_testing()
add_test(NAME Exam
         COMMAND Exam)
