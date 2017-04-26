find_package(GMock REQUIRED)
include_directories(${GMOCK_INCLUDE_DIRS})

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

find_package(Threads)

include_directories(../src/)

add_executable(EventDispatcherTest ut/eventdispatcher_test.cpp)
target_link_libraries(
    EventDispatcherTest
    ${CMAKE_THREAD_LIBS_INIT}
    ${GMOCK_LIBRARIES}
    ${GTEST_LIBRARIES}
    ${polybobin_lib}
    )
add_test(
    NAME EventDispatcherTest
    COMMAND EventDispatcherTest
    )

add_executable(PolygonSelectionTest ut/polygonselection_test.cpp)
target_link_libraries(
    PolygonSelectionTest
    ${CMAKE_THREAD_LIBS_INIT}
    ${GMOCK_LIBRARIES}
    ${GTEST_LIBRARIES}
    ${polybobin_lib}
    )
add_test(
    NAME PolygonSelectionTest
    COMMAND PolygonSelectionTest
    )

add_executable(SelectionManagerTest ut/selectionmanager_tests.cpp)
target_link_libraries(
    SelectionManagerTest
    ${CMAKE_THREAD_LIBS_INIT}
    ${GMOCK_LIBRARIES}
    ${GTEST_LIBRARIES}
    ${polybobin_lib}
    )
add_test(
    NAME SelectionManagerTest
    COMMAND SelectionManagerTest
    )

add_executable(SelectionTest ut/selection_test.cpp)
target_link_libraries(
    SelectionTest
    ${CMAKE_THREAD_LIBS_INIT}
    ${GMOCK_LIBRARIES}
    ${GTEST_LIBRARIES}
    ${polybobin_lib}
    )
add_test(
    NAME SelectionTest
    COMMAND SelectionTest
    )
