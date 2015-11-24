set(OpenP2P_HINTS
	../openp2p/build
	../../openp2p/build
	/usr/local
	C:/Dev
	${OpenP2P_DIR} 
	$ENV{OpenP2P_DIR}
)

if(NOT OpenP2P_ROOT_DIR)
	# look for the root directory, first for the source-tree variant
	find_path(OpenP2P_ROOT_DIR 
		NAMES include/p2p/Root.hpp
		HINTS ${OpenP2P_HINTS}
	)
	if(NOT OpenP2P_ROOT_DIR)
		# this means OpenP2P may have a different directory structure, maybe it was installed, let's check for that
		message(STATUS "Looking for OpenP2P install directory structure.")
		find_path(OpenP2P_ROOT_DIR 
			NAMES include/p2p/Root.hpp
			HINTS ${OpenP2P_HINTS}
		)
		if(NOT OpenP2P_ROOT_DIR) 
			# OpenP2P was still not found -> Fail
			if(OpenP2P_FIND_REQUIRED)
				message(FATAL_ERROR "OpenP2P: Could not find OpenP2P install directory")
			endif()
			if(NOT OpenP2P_FIND_QUIETLY)
				message(STATUS "OpenP2P: Could not find OpenP2P install directory")
			endif()
			return()
		else()
			# OpenP2P was found with the make install directory structure
			message(STATUS "Assuming OpenP2P install directory structure at ${OpenP2P_ROOT_DIR}.")
			set(OpenP2P_INSTALLED true)
		endif()
	endif()
endif()

# add dynamic library directory
if(WIN32)
	find_path(OpenP2P_RUNTIME_LIBRARY_DIRS
		NAMES openp2p-root.dll
		HINTS ${OpenP2P_ROOT_DIR}
		PATH_SUFFIXES 
			bin
			lib
	)
endif()

# if installed directory structure, set full include dir
if(OpenP2P_INSTALLED)
	set(OpenP2P_INCLUDE_DIRS ${OpenP2P_ROOT_DIR}/include/ CACHE PATH "The global include path for OpenP2P")
endif()

# append the default minimum components to the list to find
list(APPEND components 
	${OpenP2P_FIND_COMPONENTS} 
	# default components:
	"Root"
)
list(REMOVE_DUPLICATES components) # remove duplicate defaults


foreach( component ${components} )
	string( TOLOWER "${component}" component_lower )	
	# include directory for the component
	if(NOT OpenP2P_${component}_INCLUDE_DIR)

		find_path(OpenP2P_${component}_INCLUDE_DIR
			NAMES 
				p2p/${component}.hpp
				p2p/${component}/${component}.hpp
			HINTS
				${OpenP2P_ROOT_DIR}
			PATH_SUFFIXES
				include
				${component_lower}/include
		)
	endif()
	if(NOT OpenP2P_${component}_INCLUDE_DIR)
		message(FATAL_ERROR "OpenP2P_${component}_INCLUDE_DIR NOT FOUND")
	else()
		list(APPEND OpenP2P_INCLUDE_DIRS ${OpenP2P_${component}_INCLUDE_DIR})
	endif()

	# release library
	if(NOT OpenP2P_${component}_LIBRARY)
		find_library(
			OpenP2P_${component}_LIBRARY 
			NAMES openp2p-${component_lower} 
			HINTS ${OpenP2P_ROOT_DIR}
			PATH_SUFFIXES
				lib
				bin
		)
		if(OpenP2P_${component}_LIBRARY)
			message(STATUS "Found OpenP2P ${component}: ${OpenP2P_${component}_LIBRARY}")
		endif()
	endif()
	if(OpenP2P_${component}_LIBRARY)
		list(APPEND OpenP2P_LIBRARIES "optimized" ${OpenP2P_${component}_LIBRARY} )
		mark_as_advanced(OpenP2P_${component}_LIBRARY)
	endif()

	# debug library
	if(NOT OpenP2P_${component}_LIBRARY_DEBUG)
		find_library(
			OpenP2P_${component}_LIBRARY_DEBUG
			Names openp2p-${component_lower}d
			HINTS ${OpenP2P_ROOT_DIR}
			PATH_SUFFIXES
				lib
				bin
		)
		if(OpenP2P_${component}_LIBRARY_DEBUG)
			message(STATUS "Found OpenP2P ${component} (debug): ${OpenP2P_${component}_LIBRARY_DEBUG}")
		endif()
	endif(NOT OpenP2P_${component}_LIBRARY_DEBUG)
	if(OpenP2P_${component}_LIBRARY_DEBUG)
		list(APPEND OpenP2P_LIBRARIES "debug" ${OpenP2P_${component}_LIBRARY_DEBUG})
		mark_as_advanced(OpenP2P_${component}_LIBRARY_DEBUG)
	endif()

	# mark component as found or handle not finding it
	if(OpenP2P_${component}_LIBRARY_DEBUG OR OpenP2P_${component}_LIBRARY)
		set(OpenP2P_${component}_FOUND TRUE)
	elseif(NOT OpenP2P_FIND_QUIETLY)
		message(FATAL_ERROR "Could not find OpenP2P component ${component}!")
	endif()
endforeach()

if(DEFINED OpenP2P_LIBRARIES)
	set(OpenP2P_FOUND true)
endif()

if(${OpenP2P_OSP_FOUND})
	# find the osp bundle program
	find_program(
		OpenP2P_OSP_Bundle_EXECUTABLE 
		NAMES bundle
		HINTS 
			${OpenP2P_RUNTIME_LIBRARY_DIRS}
			${OpenP2P_ROOT_DIR}
		PATH_SUFFIXES
			bin
			OSP/BundleCreator/bin/Darwin/x86_64
			OSP/BundleCreator/bin/Darwin/i386
		DOC "The executable that bundles OSP packages according to a .bndlspec specification."
	)
	if(OpenP2P_OSP_Bundle_EXECUTABLE)
		set(OpenP2P_OSP_Bundle_EXECUTABLE_FOUND true)
	endif()
	# include bundle script file
	find_file(OpenP2P_OSP_Bundles_file NAMES OpenP2PBundles.cmake HINTS ${CMAKE_MODULE_PATH})
	if(${OpenP2P_OSP_Bundles_file})
		include(${OpenP2P_OSP_Bundles_file})
	endif()
endif()

message(STATUS "Found OpenP2P: ${OpenP2P_LIBRARIES}")


