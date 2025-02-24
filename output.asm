ldi A 10
mov M A 100
ldi A 20
mov M A 101
mov A M 100
add M 101
mov M A 101
cmp M 102 30
jne else_block
mov A M 102
add M 103
mov M A 103
else_block:
mov A M 102
add M 103
mov M A 103
end:
hlt
