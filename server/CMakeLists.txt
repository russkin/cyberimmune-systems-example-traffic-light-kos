project (server)

# Toolchain for working with the NK-parser.
include (platform/nk)

# Set compiler flags.
project_header_default ("STANDARD_GNU_11:YES" "STRICT_WARNINGS:NO")

add_executable (Server "src/server.c")
add_dependencies (Server server_edl_files)
target_link_libraries (Server ${vfs_CLIENT_LIB})

# We do not need the default VFS entity which comes from ${vfs_CLIENT_LIB}.
set_target_properties (Server PROPERTIES ${vfs_ENTITY}_REPLACEMENT "")
set_target_properties (Server PROPERTIES LINK_FLAGS ${VULN_LDFLAGS})
