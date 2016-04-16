# - Find P2PGroupChat

if(P2PGroupChat_INCLUDE_DIR AND P2PGroupChat_LIBRARIES)
   set(P2PGroupChat_FOUND TRUE)

else(P2PGroupChat_INCLUDE_DIR AND P2PGroupChat_LIBRARIES)

  set(P2PGroupChat_INCLUDE_HINTS
        /usr/include/P2PGroupChat
        /usr/include/p2pgroupchat
        /usr/local/include/P2PGroupChat
        /usr/local/include/p2pgroupchat
        /opt/local/include/P2PGroupChat
        /opt/local/include/p2pgroupchat
        $ENV{SystemDrive}/p2pgroupchat/src
        ../p2pgroupchat/src/
        ../../p2pgroupchat/src/
        C:/Dev/p2pgrouchat/src/
        ${P2PGroupChat_DIR}/src
        $ENV{P2PGroupChat_DIR}/src
  )

  set(P2PGroupChat_LIBRARIES_HINTS
        /usr/include/P2PGroupChat/lib
        /usr/include/p2pgroupchat/lib
        /usr/local/include/P2PGroupChat/lib
        /usr/local/include/p2pgroupchat/lib
        /opt/local/include/P2PGroupChat/lib
        /opt/local/include/p2pgroupchat/lib
        $ENV{SystemDrive}/p2pgroupchat/lib
        ../p2pgroupchat/build/lib
        ../../p2pgroupchat/build/lib
        C:/Dev/p2pgroupchat/lib
        ${P2PGroupChat_DIR}/lib
        $ENV{P2PGroupChat_DIR}/lib
  )

  find_path(P2PGroupChat_INCLUDE_DIR p2pgroupchat.h
        ${P2PGroupChat_INCLUDE_HINTS}
      )

  find_library(P2PGroupChat_LIBRARIES NAMES p2pgroupchat
      PATHS ${P2PGroupChat_LIBRARIES_HINTS}
      )

  if(P2PGroupChat_INCLUDE_DIR AND P2PGroupChat_LIBRARIES)
    set(P2PGroupChat_FOUND TRUE)
    message(STATUS "Found P2PGroupChat: ${P2PGroupChat_INCLUDE_DIR}, ${P2PGroupChat_LIBRARIES}")
  else(P2PGroupChat_INCLUDE_DIR AND P2PGroupChat_LIBRARIES)
    set(P2PGroupChat_FOUND FALSE)
    message(STATUS "P2PGroupChat not found.")
  endif(P2PGroupChat_INCLUDE_DIR AND P2PGroupChat_LIBRARIES)

  mark_as_advanced(P2PGroupChat_INCLUDE_DIR P2PGroupChat_LIBRARIES)

endif(P2PGroupChat_INCLUDE_DIR AND P2PGroupChat_LIBRARIES)
