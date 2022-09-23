scoreboard objectives add mclang dummy
scoreboard players set 0ret mclang 0
execute as @e[type=snowball,nbt={Item:{tag:{display:{Name:'{"text":"Toggle Platform"}'}}}}] if score 0ret mclang matches 0 run function dp:1
execute if score 0ret mclang matches 0 run function dp:4
