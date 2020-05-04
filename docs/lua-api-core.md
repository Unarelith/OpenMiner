# Lua API: Core API

## Attributes

### `openminer.registry`

`Registry` instance of the server. See [C++ classes in lua#Registry](/lua-api-cpp#registry) for more details.

### `openminer.mod_loader`

Mod loader instance in the server. See [C++ classes in lua#ServerModLoader](/lua-api-cpp#servermodloader) for more details.

## Functions

### `openminer:add_listener(event_type, listener)`

Adds a listener to a specific type of event.

Example:
```lua
openminer:add_listener(EventType.OnBlockPlaced, function(pos, player, world, client, server)
	server:send_chat_message(0, "Block placed at " .. pos.x .. ";" .. pos.y .. ";" .. pos.z .. " by Client" .. player:client_id(), client);
end)

openminer:add_listener(EventType.OnBlockDigged, function(pos, player, world, client, server)
	server:send_chat_message(0, "Block digged at " .. pos.x .. ";" .. pos.y .. ";" .. pos.z .. " by Client" .. player:client_id(), client);
end)

openminer:add_listener(EventType.OnBlockActivated, function(pos, block, player, world, client, server)
	if block:string_id() == "default:portal" then
		server:send_chat_message(0, "Swoosh! Changing dimension...", client);
	end
end)
```

Possible events:

- `OnBlockPlaced`: `funcion(pos, player, world, client, server)`
- `OnBlockDigged`: `funcion(pos, player, world, client, server)`
- `OnBlockActivated`: `function(pos, block, player, world, client, server)`

