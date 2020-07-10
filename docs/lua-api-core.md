# Lua API: Core API

## Attributes

### `openminer.registry`

`Registry` instance of the server. See [C++ classes in lua#Registry](lua-api-cpp.md#registry) for more details.

### `openminer.mod_loader`

Mod loader instance in the server. See [C++ classes in lua#ServerModLoader](lua-api-cpp.md#servermodloader) for more details.

## Functions

### `openminer:add_listener(event_type, listener)`

Adds a listener to a specific type of event.

Example:
```lua
openminer:add_listener(Event.BlockPlaced, function(pos, block, player, world, client, server)
	server:send_chat_message(0, "Block placed at " .. pos.x .. ";" .. pos.y .. ";" .. pos.z .. " by Client" .. player:client_id(), client);
end)

openminer:add_listener(Event.BlockDigged, function(pos, block, player, world, client, server)
	server:send_chat_message(0, "Block digged at " .. pos.x .. ";" .. pos.y .. ";" .. pos.z .. " by Client" .. player:client_id(), client);
end)

openminer:add_listener(Event.BlockActivated, function(pos, block, player, world, client, server)
	if block:string_id() == "default:portal" then
		server:send_chat_message(0, "Swoosh! Changing dimension...", client);
	end
end)
```

Possible events:

- `BlockPlaced`: `funcion(pos, block_state, player, world, client, server)`
- `BlockDigged`: `funcion(pos, block_state, player, world, client, server)`
- `BlockActivated`: `function(pos, block, player, world, client, server)`
- `ItemActivated`: `function(pos, block, player, world, client, server)`
- `PlayerConnected`: `function(pos, player, client, server)`

### `openminer:get_config(name)`

Get value for config option.

Example:
```lua
use_item_drop = openminer:get_config("default:use_item_drop")
```

See [this page](lua-api-mod.md#config) for more details about config options.

