add_library(RCLRW SHARED 
	"src/include/RCLRW.h"
	"src/RCLRW.cpp"
)

# Enable CLR Support v2
target_compile_options(RCLRW PRIVATE /clr:netcore)
target_compile_options(RCLRW PRIVATE /fp:precise) # /fp:strict is incompatible with /clr
set_property(TARGET RCLRW PROPERTY VS_GLOBAL_CLRSupport "true")
	

# Remove unsupported flags
string(REPLACE "/RTC1" "" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
string(REPLACE "/EHsc" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

# Enable CLR Support
#target_compile_options(RCLRW PRIVATE /clr:netcore)
#target_compile_options(RCLRW PRIVATE /fp:precise) # /fp:strict is incompatible with /clr
#set_target_properties(RCLRW PROPERTIES
#	COMMON_LANGUAGE_RUNTIME "netcore" # Not supported lol
#	VS_DOTNET_TARGET_FRAMEWORK_VERSION "7.0" # Hacky way of setting the version, works cause it's a literal string replacement
#	#VS_DOTNET_REFERENCES "Microsoft.Cpp.CLI.user"
#	#VS_GLOBAL_ROOTNAMESPACE RCLRW
#)
##set_property(TARGET RCLRW PROPERTY VS_GLOBAL_KEYWORD "ManagedCProj")
##set_property(TARGET RCLRW PROPERTY VS_GLOBAL_CLRSupport "true")
#set_property(TARGET RCLRW PROPERTY VS_DOTNET_REFERENCES 
#	"System" 
#	"System.Data" 
#	"System.Drawing" 
#	"System.Xml"
#)
#
## Note: Modification of compiler flags is required for CLR compatibility now that we are using .resx files.
#string(REPLACE "/EHsc" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
#string(REPLACE "/RTC1" "" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")

# Link RSE
target_link_libraries(RCLRW RSE)