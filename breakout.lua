-- love = {}
-- love.graphics = {}
--
-- love.graphics.getWidth = function () return 500 end
-- love.graphics.getHeight = function () return 500 end

print(type(love.graphics.getWidth))

local screenWidth = 500 -- love.graphics.getWidth()
local screenHeight = getHeight()

local function initPlayer()
    return {
        width = 160,
        height = 10,
        speed = 550,
        position = {
            y = screenHeight * 0.9,
            x = screenWidth / 2 - 60
        }
    }
end

local function initProjectile()
    return {
        width = 20,
        height = 20,
        speed = 500,
        direction = { x = 1, y = -1 },
        position = {
            x = screenWidth / 2,
            y = screenHeight * 0.8
        }
    }
end

local function initObstacles()
    local obstacles = {
        rows = 4,
        columns = 5,
        offset = {
            x = 20,
            y = 15
        }
    }

    for i = 1, obstacles.rows do
        for j = obstacles.columns / 2 * -1, obstacles.columns / 2 do
            -- todo initialize values inside the map
            local obstacle = {}
            obstacle.width = 60
            obstacle.position = {}
            obstacle.position.x = (screenWidth / 2 - (obstacle.width / 2)) + (j * obstacle.width) +
                obstacles.offset.x * j
            obstacle.position.y = 20 * i + (obstacles.offset.y * i)
            obstacle.height = 20
            obstacle.live = true
            table.insert(obstacles, obstacle)
        end
    end

    return obstacles
end

local function initGame()
    return {
        score = 0,
        paused = true,
        dead = false,
        level = 1,
        levelColors = {
            { 0,   95,  115 },
            { 10,  147, 150 },
            { 148, 210, 189 },
            { 233, 216, 166 },
            { 238, 155, 0 },
            { 202, 103, 2 },
            { 187, 62,  3 },
            { 174, 32,  18 },
            { 155, 34,  38 },
        }
    }
end

local player = initPlayer()
local proj = initProjectile()
local obstacles = initObstacles()
local game = initGame()
-- local fontXG = love.graphics.setNewFont("Merchant.ttf", 60)
-- local fontS = love.graphics.setNewFont("Merchant.ttf", 25)

-- love.window.setTitle("Breakout lua")

-- local function clamp(value, min, max)
--     return math.min(math.max(value, min), max)
-- end

-- local function inputs(dt)
--     if love.keyboard.isDown("right") or love.keyboard.isDown("d") then
--         if not game.paused and not game.dead then
--             player.position.x = math.min(player.position.x + 1 * dt * player.speed, screenWidth - player.width)
--         end
--     end

--     if love.keyboard.isDown("left") or love.keyboard.isDown("a") then
--         if not game.paused and not game.dead then
--             player.position.x = math.max(player.position.x - 1 * dt * player.speed, 0);
--         end
--     end
-- end

-- function love.keyreleased(key)
--     if key == "return" then
--         if game.paused then
--             game.paused = false
--         end
--         if game.dead then
--             player = initPlayer()
--             proj = initProjectile()
--             obstacles = initObstacles()
--             game = initGame()
--         end
--     end
-- end

local function colisionWithBorders()
    if (proj.position.x <= 0) or (proj.position.x + proj.width >= screenWidth) then
        proj.direction.x = proj.direction.x * -1;
    end

    if (proj.position.y <= 0) then
        proj.direction.y = proj.direction.y * -1;
    end

    if proj.position.y + proj.height >= screenHeight then
        game.dead = true;
    end
end

local insidePlayer = false

local function collision(a, b)
    if (a.position.x + a.width >= b.position.x) and
        (a.position.x <= b.position.x + b.width) and
        (a.position.y + a.height >= b.position.y) and
        (a.position.y <= b.position.y + b.height) then
        return true
    end
    return false
end

local function colisionWithPLayer()
    if collision(proj, player) then
        if (insidePlayer == false) then
            insidePlayer = true
            game.score = game.score + 5
            proj.direction.y = proj.direction.y * -1;
        end
    else
        insidePlayer = false
    end
end

local function moveProjectile(dt)
    if not game.paused and not game.dead then
        proj.position.x = clamp(proj.position.x + (proj.speed * dt * proj.direction.x), 0, screenWidth - proj.width)
        proj.position.y = clamp(proj.position.y + (proj.speed * dt * proj.direction.y), 0, screenHeight - proj.height)
    end
end

local function collisionWithObstacles()
    for _, obstacle in ipairs(obstacles) do
        if (obstacle.live) then
            if collision(proj, obstacle) then
                game.score = game.score + 5
                obstacle.live = false
                proj.direction.y = proj.direction.y * -1;
            end
        end
    end
end

local function projectile(dt)
    moveProjectile(dt)
    colisionWithBorders()
    colisionWithPLayer()
    collisionWithObstacles()
end

local function levelManagement()
    for _, obstacle in ipairs(obstacles) do
        if (obstacle.live) then
            return
        end
    end

    proj.speed = proj.speed + 80
    game.level = game.level + 1
    player.width = player.width - 20

    proj = initProjectile()
    obstacles = initObstacles()
end

-- local function ui()
--     if game.dead then
--         love.graphics.print('score: ' .. game.score, fontS, screenWidth * 0.02, screenHeight * 0.05)
--         love.graphics.print('level: ' .. game.level, fontS, screenWidth * 0.02, screenHeight * 0.05 + 30)
--         love.graphics.setNewFont(40)
--         love.graphics.setColor(1, 1, 1)
--         love.graphics.printf('Game Over!', fontXG, screenWidth / 2 - 200, 300, 400, 'center')
--         love.graphics.setNewFont(18)
--         love.graphics.printf('Press return to restart!', fontS, screenWidth / 2 - 200, 400, 400, 'center')
--     elseif game.paused then
--         love.graphics.setNewFont(25)
--         love.graphics.setColor(1, 1, 1)
--         love.graphics.printf('Press return to start!', fontS, screenWidth / 2 - 200, 300, 400, 'center')
--     else
--         love.graphics.print('score: ' .. game.score, fontS, screenWidth * 0.02, screenHeight * 0.05)
--         love.graphics.print('level: ' .. game.level, fontS, screenWidth * 0.02, screenHeight * 0.05 + 30)
--     end
-- end

-- local function renderObstacles()
--     local colorLevel = ((game.level + 1) % (#game.levelColors)) + 1

--     love.graphics.setColor(
--         game.levelColors[colorLevel][1] / 255,
--         game.levelColors[colorLevel][2] / 255,
--         game.levelColors[colorLevel][3] / 255
--     )

--     for _, obstacle in ipairs(obstacles) do
--         if obstacle.live then
--             love.graphics.rectangle(
--                 "fill",
--                 obstacle.position.x,
--                 obstacle.position.y,
--                 obstacle.width,
--                 obstacle.height)
--         end
--     end
-- end

-- local function renderProjectile()
--     love.graphics.setColor(1, 1, 0)
--     love.graphics.rectangle("fill", proj.position.x, proj.position.y, proj.width, proj.height)
-- end

-- local function renderPlayer()
--     love.graphics.setColor(1, 1, 1)
--     love.graphics.rectangle("fill", player.position.x, player.position.y, player.width, player.height)
-- end

function love.update(dt)
    -- inputs(dt)
    -- projectile(dt)
    -- levelManagement()
    --
  -- print("from love update: " ..dt)
end

function love.draw()
--     renderPlayer()
--     renderProjectile()
--     renderObstacles()
--     ui()
    print("love draw called")
end

function draw()
    print("aaa")
end

function hello_world(name)
   print("Hello " ..name)
end
