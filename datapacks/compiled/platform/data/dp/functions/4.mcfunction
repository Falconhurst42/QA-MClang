scoreboard objectives add mclang dummy
scoreboard players operation 0res mclang = makeFloor mclang
execute unless score 0res mclang matches 0 run function dp:5
execute if score 0ret mclang matches 0 run function dp:10
