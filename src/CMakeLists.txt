cmake_minimum_required(VERSION 3.20)
project(kot LANGUAGES CXX)


set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
set(CMAKE_POLICY_VERSION_MAXIMUM 3.29)

# ✅ Ustaw kompilator MinGW (dostosowany do twojej ścieżki)
set(CMAKE_C_COMPILER "D:/gcc14/mingw64/bin/gcc.exe")
set(CMAKE_CXX_COMPILER "D:/gcc14/mingw64/bin/g++.exe")

# ✅ Wymagany standard C++
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# 🔹 Wyłącz niepotrzebne ostrzeżenia CMake dla MinGW
if (MINGW)
    set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
endif()

# ✅ Główne źródła projektu
add_executable(${PROJECT_NAME}
    main.cpp grid/Grid.cpp grid/Grid.hpp grid/GridGenerator.cpp grid/GridRenderer.cpp grid/GridSelector.cpp
     World.hpp World.cpp components/Component.cpp components/Component.hpp
    components/ComponentManager.cpp components/ComponentManager.hpp
)

# 🔹 Ścieżki nagłówków
target_include_directories(${PROJECT_NAME} PRIVATE include)

# ✅ Znajdź i dołącz bibliotekę SFML 3
# Jeśli masz SFML 3 zainstalowane lokalnie, podaj ścieżkę do jej folderu.
# Przykład:
#   D:/libs/SFML-3.0.0
#
# Wtedy użyj poniższej linii:
set(SFML_DIR "D:\\SFML\\SFML-3.0.2\\lib\\cmake\\SFML")

find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System)

target_include_directories(${PROJECT_NAME} PRIVATE ${SFML_INCLUDE_DIRS})

# 🔗 połączenie projektu z SFML
target_link_libraries(${PROJECT_NAME} PRIVATE SFML::Graphics SFML::System SFML::Window)

# ✅ Ustaw folder wyjściowy na /build/bin
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)