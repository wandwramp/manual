.text
.global main
main:
	addui	$2, $0, 201
	lw	$3, sum($0)
	add	$4, $2, $3
	sw	$4, sum($0)
	syscall

.data
sum:
	.word	657
