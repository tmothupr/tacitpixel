# thirdparty stuff

add_subdirectory(_thirdparty)

set(tp_name tacit)

set(tp_include_path ${CMAKE_CURRENT_SOURCE_DIR}/../../include)


include_directories(${tp_include_path}
	_thirdparty
	_thirdparty/libogg/include
	_thirdparty/libvorbis/include
	${CMAKE_BINARY_DIR}/include)

find_package(X11)
find_package(OpenGL)
find_package(OpenAL)
find_package(OpenGLES)
#find_package(Cocoa)

include(CheckIncludeFiles)

CHECK_INCLUDE_FILES("stdint.h" HAVE_STDINT_H)
CHECK_INCLUDE_FILES("malloc.h" HAVE_MALLOC_H)
CHECK_INCLUDE_FILES("unistd.h" HAVE_UNISTD_H)
CHECK_INCLUDE_FILES("sys/types.h" HAVE_SYS_TYPES_H)
CHECK_INCLUDE_FILES("sys/param.h;sys/mount.h" HAVE_SYS_MOUNT_H)
CHECK_INCLUDE_FILES("pthread.h" HAVE_PTHREAD_H)
CHECK_INCLUDE_FILES("dlfcn.h" HAVE_DLFCN_H)
CHECK_INCLUDE_FILES("time.h" HAVE_TIME_H)
CHECK_INCLUDE_FILES("sys/time.h" HAVE_SYS_TIME_H)
CHECK_INCLUDE_FILES("stdio.h" HAVE_STDIO_H)
# STL stuff
CHECK_INCLUDE_FILES("new" HAVE_STL_NEW)
CHECK_INCLUDE_FILES("cstdlib" HAVE_STL_CSTDLIB)
CHECK_INCLUDE_FILES("cassert" HAVE_STL_CSTDLIB)


include(CheckSymbolExists)
check_symbol_exists(clock_gettime sys/time.h HAVE_CLOCK_GETTIME)
check_symbol_exists(gettimeofday sys/time.h HAVE_GETTIMEOFDAY)
check_symbol_exists(pthread_yield_np pthread.h HAVE_PTHREAD_YIELD_NP)
check_symbol_exists(pthread_yield pthread.h HAVE_PTHREAD_YIELD)


set(CMAKE_SKIP_BUILD_RPATH FALSE)


find_package(SWIG)


set(tp_impl_srcs)
set(tp_impl_libs)


message(STATUS "Build ${TP_VERSION_BUILD}")


set(tp_hdrs
	${tp_include_path}/tp/allocator.h
	${tp_include_path}/tp/array.h
	${tp_include_path}/tp/arguments.h
	${tp_include_path}/tp/assert.h
	${tp_include_path}/tp/chunk.h
	${tp_include_path}/tp/dialogs.h
	${tp_include_path}/tp/event.h
	${tp_include_path}/tp/field.h
	${tp_include_path}/tp/file.h
	${tp_include_path}/tp/functoid.h
	${tp_include_path}/tp/globals.h
	${tp_include_path}/tp/egl.h
	${tp_include_path}/tp/font.h
	${tp_include_path}/tp/fixed.h
	${tp_include_path}/tp/gl.h
	${tp_include_path}/tp/gzipio.h
	${tp_include_path}/tp/hash.h
	${tp_include_path}/tp/image.h
	${tp_include_path}/tp/imagehandler.h
	${tp_include_path}/tp/imageop.h
	${tp_include_path}/tp/io.h
	${tp_include_path}/tp/ioutils.h
	${tp_include_path}/tp/log.h
	${tp_include_path}/tp/logutils.h
	${tp_include_path}/tp/list.h
	${tp_include_path}/tp/library.h
	${tp_include_path}/tp/map.h
	${tp_include_path}/tp/math.h
	${tp_include_path}/tp/matrix.h
	${tp_include_path}/tp/mat.h
	${tp_include_path}/tp/matop.h
	${tp_include_path}/tp/module.h
	${tp_include_path}/tp/mutex.h
	${tp_include_path}/tp/opengl.h
	${tp_include_path}/tp/pixelformat.h
	${tp_include_path}/tp/pair.h
	${tp_include_path}/tp/quat.h
	${tp_include_path}/tp/referenced.h
	${tp_include_path}/tp/refptr.h
	${tp_include_path}/tp/rs232.h
	${tp_include_path}/tp/rtl.h
	${tp_include_path}/tp/rtti.h
	${tp_include_path}/tp/socket.h
        ${tp_include_path}/tp/script.h
	${tp_include_path}/tp/sort.h
	${tp_include_path}/tp/sound.h
	${tp_include_path}/tp/scopeptr.h
	${tp_include_path}/tp/scopelock.h
	${tp_include_path}/tp/stack.h
	${tp_include_path}/tp/stream.h
	${tp_include_path}/tp/string.h
	${tp_include_path}/tp/stringtokenizer.h
	${tp_include_path}/tp/stringformater.h
	${tp_include_path}/tp/stringutils.h
	${tp_include_path}/tp/system.h
	${tp_include_path}/tp/thread.h
	${tp_include_path}/tp/timer.h
	${tp_include_path}/tp/types.h
	${tp_include_path}/tp/vec.h
	${tp_include_path}/tp/version.h
	${tp_include_path}/tp/viewer.h
        ${tp_include_path}/tp/widget.h
	${tp_include_path}/tp/xml.h
	)

if(OPENGL_FOUND)
	set(TP_USE_OPENGL ON)
	list(APPEND tp_impl_srcs
		_impl/renderer_gl1.cxx
		_impl/renderer_gl1.h
		_impl/renderer_gl2.cxx
		_impl/renderer_gl2.h
		_impl/glprogram.h
		_impl/glprogram.cxx
		)
	include_directories(${OPENGL_INCLUDE_DIRS})
	list(APPEND tp_impl_libs
		${OPENGL_LIBRARIES}
		)
endif()

if(ANDROID AND OPENGLES1_FOUND)
	set(TP_USE_OPENGLES1 ON)
	list(APPEND tp_impl_srcs
		_impl/renderer_gl1.cxx
		_impl/renderer_gl1.h
		)
	include_directories(${OPENGLES1_INCLUDE_DIR})
	list(APPEND tp_impl_libs
		${OPENGLES1_gl_LIBRARY}
		)
endif()

if(OPENAL_FOUND)
	set(TP_USE_OPENAL ON)
#	list(APPEND tp_impl_srcs
#		_impl/renderer_gl1.cxx
#		_impl/renderer_gl1.h
#		)
	include_directories(${OPENAL_INCLUDE_DIR})
	list(APPEND tp_impl_libs
		${OPENAL_LIBRARY}
		)
endif()


if (APPLE)

	option(TP_USE_COCOA "Use Cocoa as a rendersurface backend" OFF)

	if (TP_USE_COCOA)
		list(APPEND tp_impl_srcs
			_impl/window_cocoa.mm
			_impl/window_cocoa.h
			_impl/rendercontext_cocoa.mm
			_impl/rendercontext_cocoa.h
			_impl/dialogs_cocoa.mm
			_impl/dialogs_cocoa.h
			)

		list(APPEND tp_impl_libs
			"-framework Cocoa"
			"-framework OpenGL"
			)
	endif()
endif()

if(IOS)
	list(APPEND tp_impl_srcs
#				_impl/window_cocoa.mm
#				_impl/window_cocoa.h
				_impl/rendercontext_eagl.mm
				_impl/rendercontext_eagl.h
				_impl/renderer_gl1.cxx
				_impl/renderer_gl1.h
				_impl/glprogram.h
				_impl/glprogram.cxx
#				_impl/dialogs_cocoa.mm
#				_impl/dialogs_cocoa.h
				)
endif()

if(X11_FOUND)

	option(TP_USE_X11 "Use X11 as a rendersurface backend" OFF)

	if(TP_USE_X11)

		list(APPEND tp_impl_srcs
			_impl/window_x11.cxx
			_impl/window_x11.h
			)

		list(APPEND tp_impl_libs
			${X11_LIBRARIES}
			)

		# only now we add GLX
		if(OPENGL_FOUND)
			list(APPEND tp_impl_srcs
				_impl/renderbuffer_glx.cxx
				_impl/renderbuffer_glx.h
				_impl/rendercontext_glx.cxx
				_impl/rendercontext_glx.h
				)
		endif()

	endif()

endif()

if(WIN32)

	list(APPEND tp_impl_srcs
			_impl/window_win32.cxx
			_impl/window_win32.h
			_impl/rendercontext_wgl.cxx
			_impl/rendercontext_wgl.h
			_impl/dialogs_win32.h
			_impl/dialogs_win32.cxx
			)
endif()


list(APPEND tp_impl_srcs
	_impl/rendercontext_egl.cxx
	_impl/rendercontext_egl.h
	)


#find_package(GTK)
#if(GTK_FOUND)
#	list(APPEND tp_impl_srcs
#			_impl/dialogs_gtk.h
#			_impl/dialogs_gtk.cxx
#			)
#endif()


set(tp_srcs
	arguments.cxx
	allocator.cxx
	chunk.cxx
	dialogs.cxx
	egl.cxx
	event.cxx
	file.cxx
	filename.cxx
	functoid.cxx
	font.cxx
	gzipio.cxx
#	gl.cxx
	hash.cxx
	image.cxx
	imagehandler.cxx
	imageop.cxx
	ioutils.cxx
	log.cxx
	library.cxx
	module.cxx
	mutex.cxx
	opengl.cxx
	referenced.cxx
	rs232.cxx
	rtl.cxx
        script.cxx
	socket.cxx
	string.cxx
	stringtokenizer.cxx
	system.cxx
	thread.cxx
	timer.cxx
	version.cxx
	viewer.cxx
        widget.cxx
	xml.cxx
	)

#
# Image handlers
#
list(APPEND tp_srcs
#	_modules/image/bmp/bmp_handler.cxx
#	_modules/image/jpg/jpg_handler.cxx
#	_modules/image/ppm/ppm_handler.cxx
	)


# Sound

list(APPEND tp_srcs
	_impl/soundstream_ogg.cxx
	_impl/soundstream_ogg.h
	)

#
# Scenegraph
#
set(tp_hdrs_sg
	${tp_include_path}/tp/camera.h
	${tp_include_path}/tp/light.h
	${tp_include_path}/tp/material.h
	${tp_include_path}/tp/node.h
	${tp_include_path}/tp/nodeops.h
	${tp_include_path}/tp/object.h
	${tp_include_path}/tp/primitive.h
	${tp_include_path}/tp/projection.h
	${tp_include_path}/tp/renderable.h
	${tp_include_path}/tp/renderer.h
	${tp_include_path}/tp/rendercontext.h
	${tp_include_path}/tp/rendertarget.h
	${tp_include_path}/tp/window.h
	${tp_include_path}/tp/scene.h
	${tp_include_path}/tp/text.h
	${tp_include_path}/tp/texture.h
	${tp_include_path}/tp/transform.h
	${tp_include_path}/tp/traverser.h
	${tp_include_path}/tp/ui.h
	)

set(tp_srcs_sg
	camera.cxx
	light.cxx
	material.cxx
	node.cxx
	nodeops.cxx
	object.cxx
	primitive.cxx
	projection.cxx
	renderable.cxx
	renderer.cxx
	rendercontext.cxx
	window.cxx
	scene.cxx
	sound.cxx
	text.cxx
	texture.cxx
	transform.cxx
	traverser.cxx
	ui.cxx
	)




list(APPEND tp_impl_srcs
    _impl/script_lua.h
	_impl/script_lua.cxx
    )


configure_file(config.h.in
	${CMAKE_BINARY_DIR}/include/tp/config.h
	@ONLY
	)

set(tp_srcs_all
	${tp_srcs}
	${tp_hdrs}
	${tp_hdrs_sg}
	${tp_srcs_sg}
	${tp_impl_srcs}
	)

#set_property(SOURCE ${tp_srcs_all}
#	APPEND PROPERTY
#	COMPILE_DEFINITIONS
#	"TACIT_EXPORTS"
set(TACIT_LIB_TYPE SHARED)
if(IOS)
	set(TACIT_LIB_TYPE STATIC)
endif()

add_library(${tp_name} ${TACIT_LIB_TYPE}
	${tp_srcs_all}
	)

set_property(TARGET ${tp_name} PROPERTY COMPILE_DEFINITIONS "TACIT_EXPORTS")

target_link_libraries(${tp_name} ${tp_impl_libs} )

target_link_libraries(${tp_name} jpeg png zlib ogg vorbis lua)

if(APPLE)
	target_link_libraries(${tp_name}
		"-framework CoreFoundation"
		)
endif()


if (HAVE_DLFCN_H)
	target_link_libraries(${tp_name} dl)
endif()

if (HAVE_CLOCK_GETTIME AND UNIX)
	target_link_libraries(${tp_name} rt)
endif()

if(HAVE_PTHREAD_H AND NOT ANDROID)
	target_link_libraries(${tp_name}
		pthread)
endif()

if (ANDROID)
	target_link_libraries(${tp_name} log)
endif()

if (WIN32)
	target_link_libraries(${tp_name}
		wsock32
		)
endif()






#if(SWIG_FOUND)
#	include(${SWIG_USE_FILE})
#	set(SWIG_INTERFACES
#		_impl/_lua/window.i
#		)

#	SWIG_ADD_MODULE(luatp lua ${SWIG_INTERFACES})
#	SWIG_LINK_LIBRARIES(luatp ${tp_name})

#endif()



add_subdirectory(_modules)
add_subdirectory(_testing)

option(TP_BUILD_UNITTESTS "Build unit tests" OFF)
if(TP_BUILD_UNITTESTS)
	add_subdirectory(_unit)
endif()
