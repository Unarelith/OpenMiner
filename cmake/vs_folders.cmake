#-----------------------------------------------------------------------------------------
# Startup project
#-----------------------------------------------------------------------------------------
set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${CMAKE_PROJECT_NAME})

#-----------------------------------------------------------------------------------------
# Client/server
#-----------------------------------------------------------------------------------------
set_target_properties(${CMAKE_PROJECT_NAME}            PROPERTIES FOLDER "source")
set_target_properties(${CMAKE_PROJECT_NAME}_common     PROPERTIES FOLDER "source")
set_target_properties(${CMAKE_PROJECT_NAME}_server     PROPERTIES FOLDER "source")
set_target_properties(${CMAKE_PROJECT_NAME}_server_lib PROPERTIES FOLDER "source")

#-----------------------------------------------------------------------------------------
# External libraries
#-----------------------------------------------------------------------------------------
set_target_properties(glm_static                       PROPERTIES FOLDER "external")
set_target_properties(gamekit                          PROPERTIES FOLDER "external")
set_target_properties(libglew_static                   PROPERTIES FOLDER "external")
set_target_properties(libluajit                        PROPERTIES FOLDER "external")
set_target_properties(luajit                           PROPERTIES FOLDER "external")
set_target_properties(SDL2_image                       PROPERTIES FOLDER "external")
set_target_properties(SDL2_test                        PROPERTIES FOLDER "external")
set_target_properties(SDL2main                         PROPERTIES FOLDER "external")
set_target_properties(SDL2-static                      PROPERTIES FOLDER "external")
set_target_properties(tinyxml2                         PROPERTIES FOLDER "external")
set_target_properties(zlib                             PROPERTIES FOLDER "external")

#-----------------------------------------------------------------------------------------
# BGFX
#-----------------------------------------------------------------------------------------
set_target_properties(bgfx                             PROPERTIES FOLDER "external/bgfx")
set_target_properties(bimg                             PROPERTIES FOLDER "external/bgfx")
set_target_properties(bimg_decode                      PROPERTIES FOLDER "external/bgfx")
set_target_properties(bimg_encode                      PROPERTIES FOLDER "external/bgfx")
set_target_properties(bx                               PROPERTIES FOLDER "external/bgfx")

set_target_properties(example-common                   PROPERTIES FOLDER "external/bgfx/misc")
set_target_properties(fcpp                             PROPERTIES FOLDER "external/bgfx/misc")
set_target_properties(glslang                          PROPERTIES FOLDER "external/bgfx/misc")
set_target_properties(glsl-optimizer                   PROPERTIES FOLDER "external/bgfx/misc")
set_target_properties(spirv-cross                      PROPERTIES FOLDER "external/bgfx/misc")
set_target_properties(spirv-opt                        PROPERTIES FOLDER "external/bgfx/misc")

set_target_properties(bin2c                            PROPERTIES FOLDER "external/bgfx/tools")
set_target_properties(geometryc                        PROPERTIES FOLDER "external/bgfx/tools")
set_target_properties(geometryv                        PROPERTIES FOLDER "external/bgfx/tools")
set_target_properties(shaderc                          PROPERTIES FOLDER "external/bgfx/tools")
set_target_properties(texturec                         PROPERTIES FOLDER "external/bgfx/tools")
set_target_properties(texturev                         PROPERTIES FOLDER "external/bgfx/tools")

#-----------------------------------------------------------------------------------------
# Misc projects
#-----------------------------------------------------------------------------------------
set_target_properties(aob                              PROPERTIES FOLDER "external/misc")
set_target_properties(buildvm                          PROPERTIES FOLDER "external/misc")
set_target_properties(buildvm_arch_h                   PROPERTIES FOLDER "external/misc")
set_target_properties(lj_gen_folddef                   PROPERTIES FOLDER "external/misc")
set_target_properties(lj_gen_headers                   PROPERTIES FOLDER "external/misc")
set_target_properties(lj_gen_vm_s                      PROPERTIES FOLDER "external/misc")
set_target_properties(minilua                          PROPERTIES FOLDER "external/misc")
set_target_properties(sdl_headers_copy                 PROPERTIES FOLDER "external/misc")
set_target_properties(showanim                         PROPERTIES FOLDER "external/misc")
set_target_properties(showimage                        PROPERTIES FOLDER "external/misc")
set_target_properties(uninstall                        PROPERTIES FOLDER "external/misc")
set_target_properties(xmltest                          PROPERTIES FOLDER "external/misc")

