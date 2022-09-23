scoreboard objectives add mclang dummy
scoreboard players set 0ret mclang 0
scoreboard players operation 0res mclang = makeFloor mclang
scoreboard players set 0itmp mclang 0
execute if score 0res mclang matches 0 run scoreboard players set 0itmp mclang 1
scoreboard players operation 0res mclang = 0itmp mclang
scoreboard players operation makeFloor mclang = 0res mclang
