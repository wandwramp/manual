.global main
main:
	addi	$3, $0, 0x095d
	addi	$4, $0, 0
loop:
	beqz	$3, finished

	addi	$4, $4, 1
	
	subi	$5, $3, 1
	and	$3, $3, $5 # breakpoint here
	
	j	loop
finished:
	j	exit


