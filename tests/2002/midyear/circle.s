.global	volume_cylinder
.text
volume_cylinder:
	subui	$sp, $sp, 7
	sw	$12, 1($sp)
	sw	$13, 2($sp)
	sw	$ra, 3($sp)
	lw	$13, 7($sp)
	srai	$13, $13, 1
	sw	$13, 6($sp)
	lw	$13, 6($sp)
	sw	$13, 0($sp)
	jal	area_circle
	addu	$13, $0, $1
	sw	$13, 5($sp)
	lw	$13, 5($sp)
	lw	$12, 8($sp)
	mul	$13, $13, $12
	sw	$13, 4($sp)
	lw	$1, 4($sp)
L.1:
	lw	$12, 1($sp)
	lw	$13, 2($sp)
	lw	$ra, 3($sp)
	addui	$sp, $sp, 7
	jr	$ra
