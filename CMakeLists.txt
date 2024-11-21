cmake_minimum_required(VERSION 3.25...3.29)
project(dourgaur C)

# Include your module directory
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)

find_package(Libgcrypt REQUIRED)

set(CMAKE_C_STANDARD 11)

add_executable(dourgaur main.c dg_init.c dg_pkcrypt.c dg_gcry.c dg_get_configfile.c config.c dg_logger.c)

target_link_libraries(dourgaur ${LIBGCRYPT_LIBRARIES})
