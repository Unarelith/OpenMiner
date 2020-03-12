# Lua API: Core API

## Functions

### `openminer:registry()`

This function returns the `Registry` instance of the server.

### `openminer:add_listener(event_type, listener)`

Adds a listener to a specific type of event.

Example:
```lua
openminer:add_listener(EventType.OnBlockPlaced, function(pos, player, world, client, server)
	server:send_chat_message(0, "Block placed at " .. pos.x .. ";" .. pos.y .. ";" .. pos.z .. " by Client" .. player:client_id(), client);
end)

openminer:add_listener(EventType.OnBlockActivated, function(pos, block, player, world, client, server)
	if block:string_id() == "default:portal" then
		server:send_chat_message(0, "Swoosh! Changing dimension...", client);
	end
end)
```

Possible events:

- `OnBlockPlaced`: `funcion(pos, player, world, client, server)`
- `OnBlockActivated`: `function(pos, block, player, world, client, server)`

