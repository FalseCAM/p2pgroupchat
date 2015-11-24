# - Find Crypto++

if(Crypto++_INCLUDE_DIR AND Crypto++_LIBRARIES)
   set(CRYPTO++_FOUND TRUE)

else(Crypto++_INCLUDE_DIR AND Crypto++_LIBRARIES)
  find_path(Crypto++_INCLUDE_DIR cryptlib.h
      /usr/include/crypto++
      /usr/include/cryptopp
      /usr/local/include/crypto++
      /usr/local/include/cryptopp
      /opt/local/include/crypto++
      /opt/local/include/cryptopp
      $ENV{SystemDrive}/Crypto++/include
      )

  find_library(Crypto++_LIBRARIES NAMES cryptopp
      PATHS
      /usr/lib
      /usr/local/lib
      /opt/local/lib
      $ENV{SystemDrive}/Crypto++/lib
      )

  if(CRYPTO++_INCLUDE_DIR AND CRYPTO++_LIBRARIES)
    set(CRYPTO++_FOUND TRUE)
    message(STATUS "Found Crypto++: ${CRYPTO++_INCLUDE_DIR}, ${CRYPTO++_LIBRARIES}")
  else(CRYPTO++_INCLUDE_DIR AND CRYPTO++_LIBRARIES)
    set(CRYPTO++_FOUND FALSE)
    message(STATUS "Crypto++ not found.")
  endif(Crypto++_INCLUDE_DIR AND Crypto++_LIBRARIES)

  mark_as_advanced(Crypto++_INCLUDE_DIR Crypto++_LIBRARIES)

endif(Crypto++_INCLUDE_DIR AND Crypto++_LIBRARIES)
