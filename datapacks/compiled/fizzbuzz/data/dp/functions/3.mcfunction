scoreboard objectives add mclang dummy
scoreboard players set 0tmp0 mclang 1
execute as abfr	v@e[type=pig,limit=1] if score 0ret mclang matches 0 run function dp:4
execute if score 0ret mclang matches 0 run function dp:5
