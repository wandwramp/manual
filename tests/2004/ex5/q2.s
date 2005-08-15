	.global main
	.text

main:	ori  $8,  $0, 0x19
	ori  $9,  $0, 0x58
loop:	beqz $8,  endloop
	lw   $11, 0x70003($0)
	andi $11, $11, 0x2
	beqz $11, loop
	sw   $9,  0x70000($0)
	subi $8,  $8, 1
	j    loop
endloop:
	jr $15
