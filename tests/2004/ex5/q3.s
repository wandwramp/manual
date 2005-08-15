	.global main
	.text
main:
	lw     $2,  0x70003($0)
	andi   $2,  $2, 1
	beqz   $2,  main
	lw     $3,  0x70001($0)
	sgei   $10, $3, 'a'
	slei   $11, $3, 'z'
	and    $10, $10, $11
	beqz   $10, outch
	subi   $3,  $3, 0x20
outch:	  	
	lw     $2,  0x70003($0)
	andi   $2,  $2, 2
	beqz   $2,  outch
	sw     $3,  0x70000($0)
	j      main
	
