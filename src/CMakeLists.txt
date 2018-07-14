find_package(GLM REQUIRED)
include_directories(${GLM_INCLUDE_DIRS})

find_package(OpenGL REQUIRED)

file(GLOB_RECURSE LIB_SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HEADER_SOURCES ${CMAKE_SOURCE_DIR}/src/*.hpp)
list(REMOVE_ITEM LIB_SOURCES ${CMAKE_SOURCE_DIR}/src/app.cpp)

# Build all sources except src/app.cpp as a library.
add_library(${polybobin_lib} ${LIB_SOURCES} ${HEADER_SOURCES})

foreach(FILE ${LIB_SOURCES} ${HEADER_SOURCES}) 
    # Get the directory of the source file
    get_filename_component(PARENT_DIR "${FILE}" DIRECTORY)

    # Remove common directory prefix to make the group
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" GROUP "${PARENT_DIR}")

    # Make sure we are using windows slashes
    string(REPLACE "/" "\\" GROUP "${GROUP}")

    # Group into "Source Files" and "Header Files"
    if ("${FILE}" MATCHES ".*\\.cpp")
       set(GROUP "Source Files${GROUP}")
    elseif("${FILE}" MATCHES ".*\\.h")
       set(GROUP "Header Files${GROUP}")
    endif()

    source_group("${GROUP}" FILES "${FILE}")
endforeach()


target_link_libraries(
    ${polybobin_lib}
    ${wxWidgets_LIBRARIES}
    ${OPENGL_LIBRARIES}
    )

# Build src/app.cpp and link above library.
add_executable(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/src/app.cpp)
target_link_libraries(
    ${PROJECT_NAME}
    ${polybobin_lib}
    )

if(WIN32 AND MSVC)
    # Suppress an error due to wrong linker settings on Windows
    set_target_properties(
        ${PROJECT_NAME}
        PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:WINDOWS"
        LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS"
        )
endif()

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_SOURCE_DIR}/bin)
