# FindLibgcrypt.cmake

find_path(LIBGCRYPT_INCLUDE_DIR gcrypt.h)

find_library(LIBGCRYPT_LIBRARY NAMES gcrypt libgcrypt)

set(LIBGCRYPT_LIBRARIES ${LIBGCRYPT_LIBRARY})
set(LIBGCRYPT_INCLUDE_DIRS ${LIBGCRYPT_INCLUDE_DIR})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Libgcrypt DEFAULT_MSG
        LIBGCRYPT_LIBRARY LIBGCRYPT_INCLUDE_DIR)

mark_as_advanced(LIBGCRYPT_INCLUDE_DIR LIBGCRYPT_LIBRARY)