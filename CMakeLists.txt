# Copyright (C) 2021 Luca Gasperini
#
# This file is part of Master Tracker.
#
# Master Tracker is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 2 of the License, or
# (at your option) any later version.
#
# Master Tracker is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Master Tracker.  If not, see <http://www.gnu.org/licenses/>.

project(master_tracker)

cmake_minimum_required(VERSION 3.9)

include(GNUInstallDirs)


# Tell CMake to compile with C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Search on the system Doxygen
find_package(Doxygen)

if (DOXYGEN_FOUND)

    set(DOXYGEN_EXCLUDE_PATTERNS "*/Bin/*")

    doxygen_add_docs(
        doc_master_tracker
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}

        COMMENT "Generating API documentation with Doxygen"
    )
else (DOXYGEN_FOUND)
  message("Doxygen need to be installed to generate the doxygen documentation")
endif (DOXYGEN_FOUND)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    enable_testing()
    add_definitions(-DMTK_DEBUG=1)

    # TODO: For windows?
#if (UNIX)
#    set(CAMOMILLA_PYTHON "/usr/bin/python3 ${CMAKE_SOURCE_DIR}/src/scripts/camomilla/camomilla")

#    set(CMAKE_CXX_COMPILE_OBJECT "${CMAKE_CXX_COMPILE_OBJECT} 2>&1 | ${CAMOMILLA_PYTHON} -r -d1")
#    set(CMAKE_C_COMPILE_OBJECT "${CMAKE_CXX_COMPILE_OBJECT} 2>&1 | ${CAMOMILLA_PYTHON} -r -d1")
#endif()
endif()

add_compile_options(-Wall) # Enable all compiler warning
add_compile_options(-pipe) # Use pipes rather than temporary files for communication between the various stages of compilation.


include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/core
)

set (CPACK_PACKAGE_NAME "mtk")
set (CPACK_GENERATOR "DEB")
set (CPACK_DEBIAN_PACKAGE_MAINTAINER "Luca Gasperini")
set (CPACK_PACKAGE_VENDOR "XSoftware")
set (CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
set (CPACK_DEB_COMPONENT_INSTALL ON)
set (CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}")
set (CPACK_PACKAGE_HOMEPAGE_URL "https://xsoftware.it/")
include (CPack)

function (add_package TARGET_NAME HEADER_SUBPATH DESCR)

    string(REPLACE "master_tracker_" "" COMPONENT_NAME "${TARGET_NAME}")
    string(REPLACE "_app" "" COMPONENT_NAME "${COMPONENT_NAME}")

    install(TARGETS "${TARGET_NAME}"
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
        COMPONENT "${COMPONENT_NAME}-app"
    )
    install(TARGETS "${TARGET_NAME}"
        LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
         COMPONENT "${COMPONENT_NAME}-lib"
    )
    install(TARGETS "${TARGET_NAME}"
        PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${HEADER_SUBPATH}"
        COMPONENT "${COMPONENT_NAME}-dev"
    )

    cpack_add_component("${COMPONENT_NAME}-app"
                        DISPLAY_NAME "${TARGET_NAME}-app"
                        DESCRIPTION "${DESCR}"
    )

    cpack_add_component("${COMPONENT_NAME}-lib"
                        DISPLAY_NAME "${TARGET_NAME}-lib"
                        DESCRIPTION "${DESCR}"
    )

    cpack_add_component("${COMPONENT_NAME}-dev"
                        DISPLAY_NAME "${TARGET_NAME}-dev"
                        DESCRIPTION "${DESCR}"
    )
endfunction ()

add_subdirectory(src)
