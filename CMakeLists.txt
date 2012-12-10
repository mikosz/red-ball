cmake_minimum_required(VERSION 2.8)

project(red-ball)

include_directories(red-ball-core/src/main/c++)
include_directories(red-ball-game/src/main/c++)
include_directories(red-ball-graphics/src/main/c++)
include_directories(red-ball-ui/src/main/c++)
include_directories(red-ball-utils/src/main/c++)

file(GLOB_RECURSE SOURCES
	*.cpp
	)

set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_LIBS ON)
set(BOOST_ROOT "C:/boost/lib")
find_package(Boost 1.48 COMPONENTS filesystem system REQUIRED)

add_executable(red-ball ${SOURCES})

include_directories(
	"${Boost_INCLUDE_DIRS}"
	"${CMAKE_SOURCE_DIR}/red-ball-core/src/main/c++"
	"${CMAKE_SOURCE_DIR}/red-ball-graphics/src/main/c++"
	"${CMAKE_SOURCE_DIR}/red-ball-utils/src/main/c++"
	"${CMAKE_SOURCE_DIR}/red-ball-ui/src/main/c++"
	"${CMAKE_SOURCE_DIR}/red-ball-game/src/main/c++"
	"C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include"
	"C:/Program Files (x86)/Microsoft SDKs/Windows/v7.0A/Include"
	"C:/Program Files (x86)/Microsoft Visual Studio 10.0/VC/include"
	)

# TODO: including libraries from x86 libraries, because I have problems with compiling the x64 version, but this should be changed

link_directories(
	"${Boost_LIBRARY_DIRS}"
	"C:/Program Files (x86)/Microsoft Visual Studio 10.0/VC/lib"
	"C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Lib/x86"
	"C:/Program Files (x86)/Microsoft SDKs/Windows/v7.0A/lib"
	)

target_link_libraries(red-ball
	user32.lib
	gdi32.lib
	winspool.lib
	comdlg32.lib
	advapi32.lib
	shell32.lib
	ole32.lib
	oleaut32.lib
	uuid.lib
	odbc32.lib
	odbccp32.lib
	d3d11.lib
	d3dx11.lib
	d3dx10.lib
	dxerr.lib
	dxgi.lib
	${Boost_LIBRARIES}
	)
	