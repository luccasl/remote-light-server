# versão cmake
cmake_minimum_required(VERSION 3.10.2)

# nome do projeto
project(remote-light-server)

# nome do executável
set(EXEC server)

# C++ 14
set(CMAKE_CXX_STANDARD 14)

# diretório com arquivos de definição(.h)
include_directories("../include")

# diretório com arquivos de implementação(.cpp)
file(GLOB SOURCES "../src/*.cpp")

# compilar arquivos de implementação e gerar executável
add_executable("${EXEC}" "${SOURCES}")

# definir linguagem do linker
set_target_properties(server PROPERTIES LINKER_LANGUAGE C)

# adicionar library pthread
target_link_libraries(server pthread)