#------------------------------------------------------------------------------
# Set project name
#------------------------------------------------------------------------------
set(PROJECT_NAME ${PROJECT_NAME}_server)

#------------------------------------------------------------------------------
# Get source files
#------------------------------------------------------------------------------
set(SOURCE_FILES
	core/PlayerList.cpp
	core/ServerApplication.cpp
	core/ServerConfig.cpp

	gui/CraftingWidgetDef.cpp
	gui/ImageWidgetDef.cpp
	gui/InventoryWidgetDef.cpp
	gui/ProgressBarWidgetDef.cpp
	gui/ScrollBarWidgetDef.cpp
	gui/TextButtonWidgetDef.cpp
	gui/TextInputWidgetDef.cpp
	gui/WidgetDef.cpp

	lua/LuaCore.cpp
	lua/LuaGUI.cpp
	lua/LuaMod.cpp
	lua/ScriptEngine.cpp
	lua/ServerModLoader.cpp
	lua/loader/LuaBiomeLoader.cpp
	lua/loader/LuaBlockLoader.cpp
	lua/loader/LuaDimensionLoader.cpp
	lua/loader/LuaEntityLoader.cpp
	lua/loader/LuaItemLoader.cpp
	lua/loader/LuaKeyLoader.cpp
	lua/loader/LuaRecipeLoader.cpp
	lua/loader/LuaSkyLoader.cpp

	network/ChatCommandHandler.cpp
	network/Server.cpp
	network/ServerCommandHandler.cpp
	network/ServerInfo.cpp

	scene/EntityWrapper.cpp
	scene/ServerScene.cpp
	scene/controller/CollisionController.cpp
	scene/controller/NetworkController.cpp

	world/ServerBlock.cpp
	world/ServerChunk.cpp
	world/ServerItem.cpp
	world/ServerPlayer.cpp
	world/ServerWorld.cpp
	world/TerrainBiomeSampler.cpp
	world/TerrainGenerator.cpp
	world/WorldController.cpp
	world/save/WorldSaveBasicBackend.cpp
)

set(HEADER_FILES
	core/NetworkInputHandler.hpp
	core/PlayerList.hpp
	core/ServerApplication.hpp
	core/ServerConfig.hpp

	gui/CraftingWidgetDef.hpp
	gui/ImageWidgetDef.hpp
	gui/InventoryWidgetDef.hpp
	gui/ProgressBarWidgetDef.hpp
	gui/ScrollBarWidgetDef.hpp
	gui/TextButtonWidgetDef.hpp
	gui/TextInputWidgetDef.hpp
	gui/WidgetDef.hpp

	lua/LuaCore.hpp
	lua/LuaGUI.hpp
	lua/LuaMod.hpp
	lua/ScriptEngine.hpp
	lua/ServerModLoader.hpp
	lua/loader/LuaBiomeLoader.hpp
	lua/loader/LuaBlockLoader.hpp
	lua/loader/LuaDimensionLoader.hpp
	lua/loader/LuaEntityLoader.hpp
	lua/loader/LuaItemLoader.hpp
	lua/loader/LuaKeyLoader.hpp
	lua/loader/LuaRecipeLoader.hpp
	lua/loader/LuaSkyLoader.hpp

	network/ChatCommandHandler.hpp
	network/ClientInfo.hpp
	network/Server.hpp
	network/ServerCommandHandler.hpp
	network/ServerInfo.hpp

	scene/EntityWrapper.hpp
	scene/ServerScene.hpp
	scene/controller/CollisionController.hpp
	scene/controller/NetworkController.hpp

	world/ServerBlock.hpp
	world/ServerChunk.hpp
	world/ServerItem.hpp
	world/ServerPlayer.hpp
	world/ServerWorld.hpp
	world/TerrainBiomeSampler.hpp
	world/TerrainGenerator.hpp
	world/WorldController.hpp
	world/save/WorldSaveBackend.hpp
	world/save/WorldSaveBasicBackend.hpp
)

#------------------------------------------------------------------------------
# Add executables
#------------------------------------------------------------------------------
om_add_library(${PROJECT_NAME}_lib "${HEADER_FILES}" "${SOURCE_FILES}")

add_executable(${PROJECT_NAME} main.cpp)

#------------------------------------------------------------------------------
# Setup include directories
#------------------------------------------------------------------------------
target_include_directories(${PROJECT_NAME}_lib PUBLIC
	../../external
	../../external/entt/single_include
)

#------------------------------------------------------------------------------
# Link options
#------------------------------------------------------------------------------
target_link_libraries(${PROJECT_NAME}_lib PUBLIC ${CMAKE_PROJECT_NAME}_common)
target_link_libraries(${PROJECT_NAME}     PRIVATE ${PROJECT_NAME}_lib)
