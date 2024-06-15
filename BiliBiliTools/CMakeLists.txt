add_executable(${PROJECT_NAME} main.cc)

if (CMAKE_CXX_STANDARD LESS 17)
    message(FATAL_ERROR "c++17 or higher is required")
elseif (CMAKE_CXX_STANDARD LESS 20)
    message(STATUS "use c++17")
else ()
    message(STATUS "use c++20")
endif ()

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/controllers CTL_SRC)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/filters FILTER_SRC)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/plugins PLUGIN_SRC)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/models MODEL_SRC)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/Core CORE_SRC)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/Tools TOOLS_SRC)

# ##############################################################################
# If you include the drogon source code locally in your project, use this method
# to add drogon 
# add_subdirectory(drogon) 
# target_link_libraries(${PROJECT_NAME} PRIVATE drogon)
#
# and comment out the following lines
find_package(Drogon CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Drogon::Drogon)

drogon_create_views(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/views
                    ${CMAKE_CURRENT_BINARY_DIR})
# use the following line to create views with namespaces.
# drogon_create_views(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/views
#                     ${CMAKE_CURRENT_BINARY_DIR} TRUE)
# use the following line to create views with namespace CHANGE_ME prefixed
# and path namespaces.
# drogon_create_views(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/views
#                     ${CMAKE_CURRENT_BINARY_DIR} TRUE CHANGE_ME)

target_include_directories(${PROJECT_NAME}
                           PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
                                   ${CMAKE_CURRENT_SOURCE_DIR}/models
                                   ${CMAKE_CURRENT_SOURCE_DIR}/Common
                                   ${CMAKE_CURRENT_SOURCE_DIR}/Core
                                   ${CMAKE_CURRENT_SOURCE_DIR}/Tools
                                   )
target_sources(${PROJECT_NAME}
               PRIVATE
               ${SRC_DIR}
               ${CTL_SRC}
               ${FILTER_SRC}
               ${PLUGIN_SRC}
               ${MODEL_SRC}
               ${TOOLS_SRC}
               ${CORE_SRC})
# ##############################################################################
# uncomment the following line for dynamically loading views 
# set_property(TARGET ${PROJECT_NAME} PROPERTY ENABLE_EXPORTS ON)

# ##############################################################################
