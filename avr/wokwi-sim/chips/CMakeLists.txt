project(simulator)
set(CMAKE_SYSTEM_PROCESSOR wasm)

set(ZIG_TARGET "wasm32-wasi-clang")
include("../../../lib/wokwi-sim/zig-cross/cmake/zig-toolchain.cmake")

configure_file(tb6612fng.json ${CMAKE_CURRENT_BINARY_DIR}/tb6612fng.json COPYONLY)

	set(CMAKE_EXECUTABLE_SUFFIX ".wasm")
	#set(CMAKE_C_LINK_EXECUTABLE "zig build-exe <FLAGS> <CMAKE_CXX_LINK_FLAGS> <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")
	set(CMAKE_C_LINK_EXECUTABLE "zig build-exe <OBJECTS> <LINK_FLAGS> <CMAKE_C_LINK_FLAGS>")

	add_executable(tb6612fng ../../../lib/wokwi-sim/walloc/walloc.c tb6612fng.chip.c)
	set_target_properties(tb6612fng PROPERTIES COMPILE_FLAGS "--target=wasm32-wasi -Werror")
	set_target_properties(tb6612fng PROPERTIES LINK_FLAGS "-target wasm32-wasi -O ReleaseSmall --verbose-link -rdynamic -fno-entry --export-memory --export-table -DNDEBUG --name tb6612fng")#--initial-memory=16842752")# 16842752/65536=257


target_sources(tb6612fng INTERFACE wokwi-api.h)