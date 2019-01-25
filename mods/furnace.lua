mod:block {
	id = "furnace",
	name = "Furnace",
	-- texture = 164, -- Vanilla
	texture = 205, -- Faithful 32x

	on_block_activated = function(pos, player, world, client)
		local gui = LuaGUI.new()

		-- FIXME: Replace this by gui:set_size() and gui:set_centered()
		local gui_pos = {
			x = SCREEN_WIDTH / GUI_SCALE / 2.0 - 176 / 2.0,
			y = SCREEN_HEIGHT / GUI_SCALE / 2.0 - 166 / 2.0
		}

		gui:furnace {
			name = "furnace",
			pos = gui_pos,
			block = {x = pos.x, y = pos.y, z = pos.z}
		}

		gui:show(client)
	end,

	on_tick = function(pos, player, chunk, world)
		local data = world:get_block_data(pos.x, pos.y, pos.z)
		if not data then return end

		local input_stack = data.inventory:get_stack(0, 0)
		local output_stack = data.inventory:get_stack(1, 0)
		local fuel_stack = data.inventory:get_stack(2, 0)

		local ticks_remaining = bit.band(data.data, 0xfff)
		local current_burn_time = bit.band(bit.brshift(data.data, 12), 0xfff)
		local item_progress = bit.band(bit.brshift(data.data, 24), 0xff)

		-- print(tostring(ticks_remaining).." "..tostring(current_burn_time).." "..tostring(item_progress))

		local recipe = openminer:registry():get_recipe(data.inventory)
		if recipe and recipe:type() ~= "smelt" then
			recipe = nil
		end

		if ticks_remaining == 0 and recipe and fuel_stack:amount() > 0 and
				(output_stack:item():id() == 0 or output_stack:amount() == 0
				or output_stack:item():id() == recipe:result():item():id()) then
			ticks_remaining = fuel_stack:item():burn_time()
			current_burn_time = fuel_stack:item():burn_time()
			world:set_data(pos.x, pos.y, pos.z, 1)
		elseif ticks_remaining > 0 then
			ticks_remaining = ticks_remaining - 1

			if recipe and (output_stack:item():id() == 0 or output_stack:amount() == 0
					or output_stack:item():id() == recipe:result():item():id()) then
				item_progress = item_progress + 1
			else
				item_progress = 0
			end
		elseif ticks_remaining == 0 then
			current_burn_time = 0
			world:set_data(pos.x, pos.y, pos.z, 0)
		end

		if item_progress >= 200 and recipe then
			item_progress = 0;
			data.inventory:set_stack(0, 0, (input_stack:amount() - 1 > 0) and input_stack:item():name() or "", input_stack:amount() - 1)
			data.inventory:set_stack(1, 0, recipe:result():item():name(), output_stack:amount() + recipe:result():amount())
		end

		local a = bit.blshift(bit.band(current_burn_time, 0xfff), 12)
		local b = bit.blshift(bit.band(item_progress, 0xff), 24)
		data.data = bit.band(ticks_remaining, 0xfff) + a + b
	end,
}
