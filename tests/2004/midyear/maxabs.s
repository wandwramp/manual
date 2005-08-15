.global	max_absolute
.text
max_absolute:
	subui	$sp, $sp, 7
	sw	$4, 1($sp)
	sw	$5, 2($sp)
	sw	$6, 3($sp)
	sw	$7, 4($sp)
	sw	$13, 5($sp)
	sw	$ra, 6($sp)
	addu	$13, $0, $0
	addu	$5, $0, $13
	addu	$7, $0, $13
	j	L.5
L.2:
	lw	$13, 7($sp)
	addu	$13, $7, $13
	lw	$13, 0($13)
	sw	$13, 0($sp)
	jal	absolute
	addu	$13, $0, $1
	addu	$6, $0, $13
	slt	$13, $6, $5
	bnez	$13, L.6
	addu	$4, $0, $7
	addu	$5, $0, $6
L.6:
L.3:
	addi	$7, $7, 1
L.5:
	lw	$13, 8($sp)
	slt	$13, $7, $13
	bnez	$13, L.2
	addu	$1, $0, $4
L.1:
	lw	$4, 1($sp)
	lw	$5, 2($sp)
	lw	$6, 3($sp)
	lw	$7, 4($sp)
	lw	$13, 5($sp)
	lw	$ra, 6($sp)
	addui	$sp, $sp, 7
	jr	$ra
