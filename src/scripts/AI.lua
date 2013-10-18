------------------------------------------------------------------------------------

-- Vector implementation

Vector = {} -- Create a table to hold the class methods
VectorMT = { -- Create a table to hold the Methamethods
    __index = Vector, -- Inheritance

    __add = function (vec1, vec2)
        new = {x = (vec1.x + vec2.x), y = (vec1.y + vec2.y)}
        setmetatable(new, VectorMT)
        return new
    end,

    __sub = function (vec1, vec2)
        new = {x = (vec1.x - vec2.x), y = (vec1.y - vec2.y)}
        setmetatable(new, VectorMT)
        return new
    end,

    __pow = function (vec, potency)
        new = {x = (vec.x ^ potency), y = (vec.y ^ potency)}
        setmetatable(new, VectorMT)
        return new
    end,

    __pow = function (vec, potency)
        new = {x = (vec.x ^ potency), y = (vec.y ^ potency)}
        setmetatable(new, VectorMT)
        return new
    end,

    
    __mul = function (op1, op2)
        if type(op2) == "number" then
            new = {x = (op1.x * op2), y = (op1.y * op2)}
            setmetatable(new, VectorMT)
            return new
        elseif type(op1) == "number" then
            new = {x = (op2.x * op1), y = (op2.y * op1)}
            setmetatable(new, VectorMT)
            return new
        end
    end,

    __eq = function (vec1, vec2)
        if vec1.x == vec2.x and vec1.y == vec2.y then
            return true
        else
            return false
        end
    end,

    __tostring = function(vec)
        return vec.x..", "..vec.y
    end,
}

function Vector:new(x, y)  -- The constructor
    local object = { x = x, y = y }
    setmetatable(object, VectorMT)   
    return object
end

------------------------------------------------------------------------------------

-- AI Implementation

avaliablePaths = {}

function make_table(...)
    table = {...}
    return table
end

function get_objetive_tile(pacman_tile, pacman_direction_vec, ghostAI_level, blinky_tile)
    if ghostAI_level == 1 then
        return pacman_tile
    elseif ghostAI_level == 2 then
        return pacman_tile + pacman_direction_vec * 4
    elseif ghostAI_level == 3 then
        -- Not the real AI for this level
        return pacman_tile + pacman_direction_vec * -4
    elseif ghostAI_level == 4 then
        return (2 * (pacman_tile + pacman_direction_vec * 2)) - blinky_tile
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
        ghost_moved_tiles[index] = ghost_tile + vector
    end

    tile_lowest_distance = get_tile_lowest_distance(objetive_tile, ghost_moved_tiles)
    return tile_lowest_distance - ghost_tile
end

function direction_scatter(ghost_tile, ghostAI_level, paths_table)
    if(ghostAI_level == 1) then 
        objetive_tile = Vector:new(25, 0)
    elseif(ghostAI_level == 2) then
        objetive_tile = Vector:new(2, 0)
    elseif(ghostAI_level == 3) then
        objetive_tile = Vector:new(0, 35)
    elseif(ghostAI_level == 4) then
        objetive_tile = Vector:new(27, 35)
    end

    return direction(ghost_tile, paths_table)
end

function direction_chase(pacman_tile, pacman_direction_vec, ghost_tile, ghostAI_level, blinky_tile, paths_table)
    objetive_tile = get_objetive_tile(pacman_tile, pacman_direction_vec, ghostAI_level, blinky_tile)

    return direction(ghost_tile, paths_table)
end
