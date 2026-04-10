cmake_minimum_required(VERSION 3.10)
project(type_info_extractor VERSION 1.0.0)

# Флаги компилятора
set(BASE_CXX_FLAGS -pipe -g -Wall -W -fPIC)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/pod_extractor
    ${CMAKE_CURRENT_SOURCE_DIR}/pod_extractor/tests
)

set(SOURCES
    pod_extractor/main.cpp
    pod_extractor/tests/pod_ops.cpp
    pod_extractor/tests/tests.cpp
)

# Список стандартов для сборки
set(CPP_STANDARDS 11 14 17 20)

foreach(STD ${CPP_STANDARDS})
    # Имя цели с суффиксом
    set(TARGET_NAME ${PROJECT_NAME}_cpp${STD})
    
    # Создаём цель
    add_executable(${TARGET_NAME} ${SOURCES})
    
    # Устанавливаем стандарт для этой цели
    set_target_properties(${TARGET_NAME} PROPERTIES
        CXX_STANDARD ${STD}
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
    )
    
    # Флаги для этой цели
    target_compile_options(${TARGET_NAME} PRIVATE ${BASE_CXX_FLAGS})
    
    # Определения для конкретного стандарта
    target_compile_definitions(${TARGET_NAME} PRIVATE 
        IGNORE_DIAG
        USE_CPP_11_WAY
        CPP_14
    )
    
    message(STATUS "Added target ${TARGET_NAME} with C++${STD}")
endforeach()

# Дополнительная цель для сборки всех версий
add_custom_target(all_standards ALL
    DEPENDS # ${PROJECT_NAME}_cpp11 
            ${PROJECT_NAME}_cpp14 
            ${PROJECT_NAME}_cpp17 
            ${PROJECT_NAME}_cpp20
)

message(STATUS "Run 'make all_standards' to build all C++ versions")
