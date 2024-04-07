# Add the module variant configuration
add_library (logfmtxxModule)

target_sources(logfmtxxModule
  PUBLIC
    FILE_SET CXX_MODULES FILES
    logfmtxx.ixx
)