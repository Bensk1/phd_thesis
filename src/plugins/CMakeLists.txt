function(add_plugin)
    cmake_parse_arguments(
        PARSED_ARGS
        ""
        "NAME"
        "SRCS;DEPS"
        ${ARGN}
    )
    if(NOT PARSED_ARGS_NAME)
        message(FATAL_ERROR "You must provide a name for the plugin")
    endif(NOT PARSED_ARGS_NAME)

    add_library(${PARSED_ARGS_NAME} SHARED ${PARSED_ARGS_SRCS})

    add_dependencies(${PARSED_ARGS_NAME} hyrise)

    include_directories(${CMAKE_BINARY_DIR})  # For provider.hpp

    foreach(dep ${PARSED_ARGS_DEPS})
        add_dependencies(${PARSED_ARGS_NAME} ${dep})
    endforeach(dep)

    # Prevent the linker under macOS from complaining about undefined methods
    if (APPLE)
        if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
            target_link_libraries(${PARSED_ARGS_NAME} "-undefined dynamic_lookup")
        endif()
    endif()
endfunction(add_plugin)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    # ${PROJECT_SOURCE_DIR}/third_party/compact_vector/include
)

#add_subdirectory(../third_party/robin-map)

include_directories(
    SYSTEM
    ${PROJECT_SOURCE_DIR}/third_party/compact_vector/include
    ${PROJECT_SOURCE_DIR}/third_party/robin-map/include
    ${PROJECT_SOURCE_DIR}/third_party/uninitialized_vector/
    ${PROJECT_SOURCE_DIR}/third_party/nlohmann_json/single_include
)


add_plugin(NAME hyriseMvccDeletePlugin SRCS mvcc_delete_plugin.cpp mvcc_delete_plugin.hpp)
add_plugin(NAME hyriseDependencyMiningPlugin SRCS
    dependency_mining_plugin.cpp
    dependency_mining_plugin.hpp
    dependency_mining/dependency_validator.cpp
    dependency_mining/dependency_validator.hpp
    dependency_mining/candidate_strategy/abstract_dependency_candidate_rule.cpp
    dependency_mining/candidate_strategy/abstract_dependency_candidate_rule.hpp
    dependency_mining/candidate_strategy/dependent_group_by_candidate_rule.cpp
    dependency_mining/candidate_strategy/dependent_group_by_candidate_rule.hpp
    dependency_mining/candidate_strategy/join_elimination_candidate_rule.cpp
    dependency_mining/candidate_strategy/join_elimination_candidate_rule.hpp
    dependency_mining/candidate_strategy/join_to_predicate_candidate_rule.cpp
    dependency_mining/candidate_strategy/join_to_predicate_candidate_rule.hpp
    dependency_mining/candidate_strategy/join_to_semi_candidate_rule.cpp
    dependency_mining/candidate_strategy/join_to_semi_candidate_rule.hpp
    dependency_mining/pqp_analyzer.cpp
    dependency_mining/pqp_analyzer.hpp
    dependency_mining/util.cpp
    dependency_mining/util.hpp
    dependency_mining/validation_state.cpp
    dependency_mining/validation_state.hpp
    dependency_mining/validation_strategy/abstract_dependency_validation_rule.cpp
    dependency_mining/validation_strategy/abstract_dependency_validation_rule.hpp
    dependency_mining/validation_strategy/fd_validation_rule.cpp
    dependency_mining/validation_strategy/fd_validation_rule.hpp
    dependency_mining/validation_strategy/ind_validation_rule.cpp
    dependency_mining/validation_strategy/ind_validation_rule.hpp
    dependency_mining/validation_strategy/ind_validation_rule_set.cpp
    dependency_mining/validation_strategy/ind_validation_rule_set.hpp
    dependency_mining/validation_strategy/od_validation_rule.cpp
    dependency_mining/validation_strategy/od_validation_rule.hpp
    dependency_mining/validation_strategy/ucc_validation_rule.cpp
    dependency_mining/validation_strategy/ucc_validation_rule.hpp
)
add_plugin(NAME hyriseTestPlugin SRCS test_plugin.cpp test_plugin.hpp)
add_plugin(NAME hyriseTestNonInstantiablePlugin SRCS non_instantiable_plugin.cpp)


# We define TEST_PLUGIN_DIR to always load plugins from the correct directory for testing purposes
add_definitions(-DTEST_PLUGIN_DIR="${CMAKE_BINARY_DIR}/lib/")
