ldi A 10
mov M A 100
ldi A 20
mov M A 101
mov A M 100
add M 101
mov M A 100
mov A M 102
cmp A 30
jne end_if_0
jmp end_else_1
mov A M 102
add M 101
mov M A 102
end_if_0:
jmp end_else_1
mov A M 102
sub M 101
mov M A 102
end_else_1:
hlt
