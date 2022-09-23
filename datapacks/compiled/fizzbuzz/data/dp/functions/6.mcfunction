scoreboard objectives add mclang dummy
scoreboard players operation 0res mclang = n mclang
scoreboard players operation 0tmp1 mclang = 0res mclang
scoreboard players set 0itmp mclang 5
scoreboard players operation 0tmp1 mclang %= 0itmp mclang
scoreboard players operation 0tmp1 mclang = 0tmp1 mclang
scoreboard players set 0tmp2 mclang 0
scoreboard players set 0itmp mclang 0
execute if score 0tmp1 mclang = 0tmp2 mclang run scoreboard players set 0itmp mclang 1
scoreboard players operation 0tmp1 mclang = 0itmp mclang
execute unless score 0tmp1 mclang matches 0 run function dp:7
execute if score 0tmp0 mclang matches 0 run function dp:8
