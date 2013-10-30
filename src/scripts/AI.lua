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

-- Tile Implementation

Tile = {}
TileMT = {
    __index = Tile,

    __eq = function (tile1, tile2)
        if tile1.pos.x == tile2.pos.x and tile1.pos.y == tile2.pos.y and tile1.value == tile2.value then
            return true
        else
            return false
        end
    end,

    __tostring = function(tile)
        return tile.value
    end
}

function Tile:new(value, pos_vec)
    local objects = {value = value, pos = pos_vec, parent = nil, F = nil, G = nil, H = nil}
    setmetatable(objects, TileMT)
    return objects
end

------------------------------------------------------------------------------------

-- List implementation

function get_key(tile)
    return tile.x .. ", " .. tile.y
end

List = {}

function List:new()
    local list = {}
    setmetatable(list, {__index = self})
    return list
end

function List:find_lowest_F()
    local _, last_tile = next(self)
    for key,tile in pairs(self) do
        if(tile.F <= last_tile.F) then
            last_tile = tile
        end
    end
    return last_tile
end

function List:add(tile)
    local new_tile = tile
    self[get_key(tile.pos)] = new_tile
end

function List:remove(tile)
    self[get_key(tile.pos)] = nil
end

function List:is_in(tile)
    if self[get_key(tile.pos)] ~= nil then
        return true
    else
        return false
    end
end

function List:empty()
    for k,v in pairs(self) do
        return false
    end
    return true
end

------------------------------------------------------------------------------------

-- A* Algorithm

function distance_to_objective(tile, objetive_tile)
    local diference = objetive_tile.pos - tile.pos
    return math.abs(diference.x) + math.abs(diference.y)
end

function get_shortest_direction(tile)
    directions = {}
    if(tile.parent ~= nil) then
        get_shortest_direction(tile.parent)
        directions[#directions + 1] = tile.pos - tile.parent.pos
    end
    return directions
end

function Astar(actual_vec, objetive_vec, Map)
    assert(actual_vec.x > 0 and actual_vec.y > 0)

    -- for i,table in ipairs(Map) do
    --     for j,v in ipairs(table) do
    --         io.write("(" .. v.pos.x .. "," .. v.pos.y .. ")")
    --     end
    --     io.write("\n")
    -- end

    local openList = List:new()
    local closedList = List:new()

    local objetive_tile = Map[objetive_vec.y][objetive_vec.x]
    local origin_tile = Map[actual_vec.y][actual_vec.x]
    origin_tile.G = 0
    origin_tile.H = distance_to_objective(origin_tile, objetive_tile)
    origin_tile.F = origin_tile.G + origin_tile.H

    openList:add(origin_tile)

    local actual_tile = nil

    while true do
        actual_tile = openList:find_lowest_F()

        closedList:add(actual_tile)
        openList:remove(actual_tile)

        check_tiles = {}

        if Map[actual_tile.pos.y-1] ~= nil then
            check_tiles[#check_tiles + 1] = Map[actual_tile.pos.y-1][actual_tile.pos.x]
        end
        if Map[actual_tile.pos.y+1] ~= nil then 
            check_tiles[#check_tiles + 1] = Map[actual_tile.pos.y+1][actual_tile.pos.x]
        end
        if Map[actual_tile.pos.y][actual_tile.pos.x-1] ~= nil then 
            check_tiles[#check_tiles + 1] = Map[actual_tile.pos.y][actual_tile.pos.x-1]
        end
        if Map[actual_tile.pos.y][actual_tile.pos.x+1] ~= nil then 
            check_tiles[#check_tiles + 1] = Map[actual_tile.pos.y][actual_tile.pos.x+1]
        end

        for _,tile in ipairs(check_tiles) do
            if(tile.value ~= 0 and closedList:is_in(tile) == false) then
                if(openList:is_in(tile) == false) then
                    tile.G = actual_tile.G + 10
                    tile.H = distance_to_objective(tile, objetive_tile)
                    tile.F = tile.G + tile.H
                    tile.parent = actual_tile
                    openList:add(tile)
                else
                    if(tile.G < actual_tile.G) then
                        tile.G = actual_tile.G + 10
                        tile.F = tile.G + tile.H
                        tile.parent = actual_tile
                    end
                end
            end
        end

        if(openList:is_in(objetive_tile)) then
            return get_shortest_direction(openList[get_key(objetive_tile.pos)])
        elseif(openList:empty()) then
            return nil
        end
    end
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
