.text
.global main
main:
	# Get the address of the welcome message
	la	$2, welcome_msg
	# Display the message
	jal	putstr

	# Clear our sum register
	add	$4, $0, $0
	# Initialise the loop counter
	addi	$5, $0, 1
loop:
	# Read a number from the user
	jal	readnum
	# Addit to our running total
	add	$4, $4, $1

	# Increment our loop counter
	addi	$5, $5, 1
	# Test to see if we have done all 4 numbers
	slti	$1, $5, 4
	# Keep looping until 4 numbers have been entered
	bnez	$1, loop

	# Get the output message
	la	$2, output_msg
	# Print it
	jal	putstr
	
	# Move our sum into register $2 to display it
	add	$2, $0, $4
	# Print out the number
	jal	writenum

	# Return to the monitor
	j	exit

.data
	# This is our welcome message
welcome_msg:
	.asciiz	"Welcome to the world of WRAMP!!!\n\nPlease type four numbers, pressing enter after each:\n"
	# This is the output message
output_msg:
	.asciiz	"The sum of the numbers is : "