scoreboard objectives add mclang dummy
scoreboard players set 0ret mclang 0
scoreboard players set 0tmp0 mclang 0
scoreboard players operation 0res mclang = n mclang
scoreboard players operation 0tmp1 mclang = 0res mclang
scoreboard players set 0itmp mclang 3
scoreboard players operation 0tmp1 mclang %= 0itmp mclang
scoreboard players operation 0tmp1 mclang = 0tmp1 mclang
scoreboard players set 0tmp2 mclang 0
scoreboard players set 0itmp mclang 0
execute if score 0tmp1 mclang = 0tmp2 mclang run scoreboard players set 0itmp mclang 1
scoreboard players operation 0tmp1 mclang = 0itmp mclang
scoreboard players operation 0tmp1 mclang = 0tmp1 mclang
scoreboard players operation 0res mclang = n mclang
scoreboard players operation 0tmp2 mclang = 0res mclang
scoreboard players set 0itmp mclang 5
scoreboard players operation 0tmp2 mclang %= 0itmp mclang
scoreboard players operation 0tmp2 mclang = 0tmp2 mclang
scoreboard players set 0tmp3 mclang 0
scoreboard players set 0itmp mclang 0
execute if score 0tmp2 mclang = 0tmp3 mclang run scoreboard players set 0itmp mclang 1
scoreboard players operation 0tmp2 mclang = 0itmp mclang
execute unless score 0tmp2 mclang matches 0 run scoreboard players set 0tmp2 mclang 1
execute if score 0tmp1 mclang matches 0 run scoreboard players set 0tmp2 mclang 0
execute unless score 0tmp2 mclang matches 0 run function dp:1
execute if score 0tmp0 mclang matches 0 run function dp:2
execute if score 0ret mclang matches 0 run function dp:9
