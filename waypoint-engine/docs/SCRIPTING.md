# Scripting

Waypoint Engine uses Lua for beginner-friendly gameplay scripts.

A project script usually has two functions:

```lua
function on_start()
  waypoint.log("Game started")
end

function on_update(dt)
  -- dt is seconds since the previous frame
end
```

## Available helpers

| Helper | Meaning |
|---|---|
| `waypoint.log(message)` | Write an info message. |
| `waypoint.warn(message)` | Write a warning. |
| `waypoint.error(message)` | Write an error. |
| `waypoint.input.is_key_down("w")` | True while a key is held. |
| `waypoint.input.is_key_pressed("space")` | True only on the frame the key goes down. |
| `waypoint.input.is_mouse_down(button)` | True while a mouse button is held (0 left, 1 middle, 2 right). |
| `waypoint.input.mouse_delta()` | Returns mouse movement as `x, y`. |

Key names are case-insensitive and tolerate spaces/underscores/hyphens: `"w"`, `"Left_Shift"`, `"space"`, `"escape"`.

```lua
function on_update(dt)
  if waypoint.input.is_key_pressed("space") then
    waypoint.log("Jump!")
  end
end
```

More gameplay helpers will be added as the ECS scene API stabilizes.
