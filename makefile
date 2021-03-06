config = debug
target = test

all: build/windows/$(config)/rules.ninja
	@cmake --build build/windows/$(config)

run: build/windows/$(config)/rules.ninja
	@cmake --build build/windows/$(config)
	@cmake -E chdir build/windows/$(config) ./$(target)

install: build/windows/release/rules.ninja
	@cmake --build build/windows/release --target install

clean:
	@cmake -E remove_directory build/windows

build/windows/debug/rules.ninja: CMakeLists.txt
	@cmake -GNinja -DCMAKE_BUILD_TYPE=Debug \
	  -DCMAKE_INSTALL_PREFIX="$(MAKEDIR)\build\install" \
	  -DCMAKE_TOOLCHAIN_FILE="$(VCPKG_ROOT)\scripts\buildsystems\vcpkg.cmake" \
	  -DVCPKG_DEFAULT_TRIPLET="x64-windows" \
	  -B build/windows/debug

build/windows/release/rules.ninja: CMakeLists.txt
	@cmake -GNinja -DCMAKE_BUILD_TYPE=Release \
	  -DCMAKE_INSTALL_PREFIX="$(MAKEDIR)\build\install" \
	  -DCMAKE_TOOLCHAIN_FILE="$(VCPKG_ROOT)\scripts\buildsystems\vcpkg.cmake" \
	  -DVCPKG_DEFAULT_TRIPLET="x64-windows" \
	  -B build/windows/release
