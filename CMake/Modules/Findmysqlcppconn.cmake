# - Try to find Mysql-Connector-C++
# Once done, this will define
#
#  MYSQLCPPCONN_FOUND - system has Mysql-Connector-C++ installed
#  MYSQLCPPCONN_INCLUDE_DIRS - the Mysql-Connector-C++ include directories
#  MYSQLCPPCONN_LIBRARIES - link these to use Mysql-Connector-C++
#
# The user may wish to set, in the CMake GUI or otherwise, this variable:
#  MYSQLCPPCONN_ROOT_DIR - path to start searching for the module

set(MYSQLCPPCONN_ROOT_DIR
	"${MYSQLCPPCONN_ROOT_DIR}"
	CACHE
	PATH
	"Where to start looking for this component.")

if(WIN32)
	find_path(MYSQLCPPCONN_INCLUDE_DIR
		NAMES
		mysql_connection.h
		PATHS
		"C:\\Program Files"
		HINTS
		${MYSQLCPPCONN_ROOT_DIR}
		PATH_SUFFIXES
		include)
		
	find_library(MYSQLCPPCONN_LIBRARY
		NAMES
		mysqlcppconn
		mysqlcppconn-static
		HINTS
		${MYSQLCPPCONN_ROOT_DIR}
		PATH_SUFFIXES
		lib)
		
else()
	find_path(MYSQLCPPCONN_INCLUDE_DIR
		mysql_connection.h
		HINTS
		${MYSQLCPPCONN_ROOT_DIR}
		PATH_SUFFIXES
		include)
		
	find_library(MYSQLCPPCONN_LIBRARY
		NAMES
		mysqlcppconn
		mysqlcppconn-static
		HINTS
		${MYSQLCPPCONN_ROOT_DIR}
		PATH_SUFFIXES
		lib64
		lib)
endif()

mark_as_advanced(MYSQLCPPCONN_INCLUDE_DIR MYSQLCPPCONN_LIBRARY)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(MYSQLCPPCONN
	DEFAULT_MSG
	MYSQLCPPCONN_INCLUDE_DIR
	MYSQLCPPCONN_LIBRARY)

if(MYSQLCPPCONN_FOUND)
	set(MYSQLCPPCONN_INCLUDE_DIRS "${MYSQLCPPCONN_INCLUDE_DIR}") # Add any dependencies here
	set(MYSQLCPPCONN_LIBRARIES "${MYSQLCPPCONN_LIBRARY}") # Add any dependencies here
	mark_as_advanced(MYSQLCPPCONN_ROOT_DIR)
endif()