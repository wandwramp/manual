.text
.global main
main:
	ori	$4, $0, 14
	addui	$5, $0, 0x16
	sne	$3, $4, $5
	bnez	$3, okay

forever:
	j	forever

okay:
	jal	add_two
	syscall

add_two:
	add	$2, $4, $5
	jr	$ra