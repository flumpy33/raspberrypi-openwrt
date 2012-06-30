cmake_minimum_required(VERSION 2.8)

project(genfatfs)

add_subdirectory(fatfs)

include_directories(${genfatfs_SOURCE_DIR}/fatfs/src)
link_directories(${genfatfs_BINARY_DIR}/fatfs)

add_executable (genfatfs genfatfs.c diskio.c)

# Link the executable to the library.
target_link_libraries (genfatfs fatfs) 

# search for programs
find_program(CTAGS ctags)

# Create ctags for vim, etc
add_custom_target(ctags
    COMMAND ${CTAGS} -R --c++-kinds=+p --fields=+iaS --extra=+fq --sort=foldcase
)

add_custom_target(tags
		COMMAND make ctags
)
