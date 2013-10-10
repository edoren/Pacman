avaliablePaths = {}

function make_table(...)
    table = {...}
    return table
end

function get_objetive_tile(pacman_tile, pacman_direction_vec, ghostAI_level, blinky_tile)
    if ghostAI_level == 1 then
        return pacman_tile
    elseif ghostAI_level == 2 then
        return {x = (pacman_tile.x + pacman_direction_vec.x * 4), y = (pacman_tile.y + pacman_direction_vec.y * 4)}
    elseif ghostAI_level == 3 then
        -- Not the real AI for this level
        return {x = (pacman_tile.x + pacman_direction_vec.x * -4), y = (pacman_tile.y + pacman_direction_vec.y * -4)}
    elseif ghostAI_level == 4 then
        return {x = 2 * (pacman_tile.x + pacman_direction_vec.x * 2) - blinky_tile.x, y = 2 * (pacman_tile.y + pacman_direction_vec.y * 2) - blinky_tile.y}
    end
end

function calculate_distance(objetive_tile, ghost_moved_tile)
    return (ghost_moved_tile.x - objetive_tile.x)^2 + (ghost_moved_tile.y - objetive_tile.y)^2
end

-- Return the ghost_moved_tile with the lowest distance to the objective_tile
function get_tile_lowest_distance(objetive_tile, ghost_moved_tiles)
    lowest = calculate_distance(objetive_tile, ghost_moved_tiles[1])
    lowest_index = 1
    for index, ghost_tile in ipairs(ghost_moved_tiles) do
        if calculate_distance(objetive_tile, ghost_moved_tiles[index]) < lowest then
            lowest = calculate_distance(objetive_tile, ghost_moved_tiles[index])
            lowest_index = index
        end
    end
    return ghost_moved_tiles[lowest_index]
end

function direction(ghost_tile, paths_table)
    ghost_moved_tiles = {}
    for index, vector in ipairs(paths_table) do
        ghost_moved_tiles[index] = {x = (ghost_tile.x + vector.x), y = (ghost_tile.y + vector.y)}
    end

    tile_lowest_distance = get_tile_lowest_distance(objetive_tile, ghost_moved_tiles)
    return {x = (tile_lowest_distance.x - ghost_tile.x), y = (tile_lowest_distance.y - ghost_tile.y)}
end

function direction_scatter(ghost_tile, ghostAI_level, paths_table)
    if(ghostAI_level == 1) then 
        objetive_tile = {x=25, y=0}
    elseif(ghostAI_level == 2) then
        objetive_tile = {x=2, y=0}
    elseif(ghostAI_level == 3) then
        objetive_tile = {x=0, y=35}
    elseif(ghostAI_level == 4) then
        objetive_tile = {x=27, y=35}
    end

    return direction(ghost_tile, paths_table)
end

function direction_chase(pacman_tile, pacman_direction_vec, ghost_tile, ghostAI_level, blinky_tile, paths_table)
    objetive_tile = get_objetive_tile(pacman_tile, pacman_direction_vec, ghostAI_level, blinky_tile)

    return direction(ghost_tile, paths_table)
end
