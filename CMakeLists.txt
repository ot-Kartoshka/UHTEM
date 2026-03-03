# CMakeList.txt: проект CMake для Lab1; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.10)

# Включение горячей перезагрузки для компиляторов MSVC, если поддерживается.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("Lab1")

# Добавьте источник в исполняемый файл этого проекта.
add_executable (Lab1 "main.cpp" "include/Errors.h" "include/Text_filter.h" "src/Text_filter.cpp" "include/Frequency.h" "src/Frequency.cpp" "include/Entropy.h" "src/Entropy.cpp" "include/Display.h" "src/Display.cpp" "include/Guessing_game.h" "src/Guessing_game.cpp" "src/Alphabet.cpp" "src/Errors.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Lab1 PROPERTY CXX_STANDARD 23)
endif()


if(EXISTS "${CMAKE_SOURCE_DIR}/TEXT.bin")
  configure_file("${CMAKE_SOURCE_DIR}/TEXT.bin" "${CMAKE_BINARY_DIR}/TEXT.bin" COPYONLY)
else()
  message(WARNING "TEXT.bin not found in source directory; skipping copy to binary dir.")
endif()

# TODO: Добавьте тесты и целевые объекты, если это необходимо.
