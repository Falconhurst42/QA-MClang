scoreboard objectives add mclang dummy
function dp:toggle.floor
scoreboard players set 0ret mclang 0
execute at @s if score 0ret mclang matches 0 run function dp:2
execute if score 0ret mclang matches 0 run function dp:3
