.global	myfunc
.text
myfunc:
	subui	$sp, $sp, 7
	sw	$5, 1($sp)
	sw	$6, 2($sp)
	sw	$7, 3($sp)
	sw	$12, 4($sp)
	sw	$13, 5($sp)
	sw	$ra, 6($sp)
	addu	$6, $0, $0
	lw	$13, 7($sp)
	lw	$12, 8($sp)
	
	add	$5, $13, $12

	
	addu	$7, $0, $13
	j	L.5
L.2:
	
	sw	$7, 0($sp)
	jal	lookup
	addu	$13, $0, $1
	add	$6, $6, $13

	
L.3:
	addi	$7, $7, 1
L.5:
	sle	$13, $7, $5
	bnez	$13, L.2
	sge	$13, $6, $0
	bnez	$13, L.6
	subu	$6, $0, $6
L.6:
	sw	$6, 0($sp)
	jal	printnum
L.1:
	lw	$5, 1($sp)
	lw	$6, 2($sp)
	lw	$7, 3($sp)
	lw	$12, 4($sp)
	lw	$13, 5($sp)
	lw	$ra, 6($sp)
	addui	$sp, $sp, 7
	jr	$ra
