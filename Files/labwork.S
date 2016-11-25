# labwork.S
  # Written 2015 by F Lundevall
  # Skeleton file for IS1200/IS1500 lab 1 2015-2016.
  # The macros PUSH and POP are in the public domain.
.macro	PUSH reg
	addi	$sp,$sp,-4
	sw	\reg,0($sp)
.endm
.macro	POP reg
	lw	\reg,0($sp)
	addi	$sp,$sp,4
.endm
# You can add your own code here
.global enable_interrupts
enable_interrupts:
  ei
  jr $ra
  nop
.global delay
delay:
	add $t1, $zero, $zero
	addi $t2, $zero,1000 #antalet loopar av en millisekund
	addi $t6, $zero,5355 #konstant för forslinga i delay
	j loop2 #hoppar till vilkoret f�rr while- slingan
	nop
.global whileloop
whileloop:
	addi $t2, $t2, -1 #m = m-1
	addi $t4, $zero, -1 #i = -1 fr�n b�rjan
.global forslinga
forslinga:
	addi $t4, $t4, 1 #i = -1 fr�n b�rjan blir h�r 0 f�rsta varvet i = i+1
	slt $t5, $t4, $t6 #vilkor f�r forslinga, 4711 enkelt att �ndra i < 4711
	beq $t5, 1, forslinga
	nop
.global loop2
loop2:
	slt $t0, $t1, $t2 #vilkor f�r while- slingan
	beq $t0, 1, whileloop
	nop
	jr $ra #hoppar ur delay
	nop
.global hexasc
hexasc:
	addi $t0 , $0 , 0x3a #Saves the symbol after digit 9 in ASKII for comparison.
	addi $t2 , $0 , 1 #used for true/false
	addi $t7, $zero, 0
	andi $t7, $a0, 0xf
	addi $v0 , $t7 , 0x30 #Adds the input number + 30 in hexa to get the correct char.
	slt $t1 , $v0 , $t0 #Compares the modified input with the comparison argument.
	beq $t1 , $t2 , done #Branches if true
	nop
	addi $v0 , $v0 , 0x7 # Adds enough numbers to become a letter according to ASKII hex table.
	done:
	jr $ra #Jumps back to main
	nop
.global time2string
time2string:
	#add $t0 $a1 $t0
	PUSH $s3
	PUSH $ra #pushar adressen från efter time2string
	PUSH $a0 #pushar addresen d�r vi ska spara str�ngen
	addi $t4, $zero, 1 #skapar jämförelse variabel
	addi $t5, $zero, 13 #gränsvärde för loopen
	add $t6, $zero, $zero #skapar värde i som vi loopar (ta bort?)
	addi $s3, $zero, 0xf #skapar variabel för att ta ut vilken byte som tas ut
.global loop
loop:
	slt $t7, $t6, $t5
	bne $t7, $t4,done2 #hoppar ur lopen om det är sant dvs
	nop
	and $t1, $a1, $s3 #tar ur rätt element ur a1, sparar till t1
	srlv $t1, $t1, $t6 #flyttar element i t1 till första platsen i t1
	add $a0, $zero, $t1 #sparar över a0 till värdet i t1
	jal hexasc
	nop
	PUSH $v0
	addi $t6, $t6, 4
	sll $s3, $s3, 4
	j loop
	nop
.global done2
done2:
	POP $t1
	POP $t2
	POP $t3
	POP $t4
	POP $a0
	addi $t0, $zero, 1
	slti $t6,$t1,0x31
	bne $t0, $t6, done3
	slti $t6,$t2,0x31
	bne $t0, $t6, done3
	slti $t6,$t3,0x31
	bne $t0, $t6, done3
	slti $t6,$t4,0x31
	bne $t0, $t6, done3
	addi $t1, $zero, 0x48
	addi $t2, $zero, 0x4f
	addi $t3, $zero, 0x55
	addi $t4, $zero, 0x52
	addi $t5, $zero, 0x00
	sb $t1, 0($a0)
	sb $t2, 1($a0)
	sb $t3, 2($a0)
	sb $t4,3($a0)
	sb $t5,4($a0)
	sb $zero, 5($a0)
	POP $ra
	POP $s3
	jr $ra
done3:
	sb $t1, 0($a0)
	sb $t2, 1($a0)
	addi $t5, $zero, 0x3A
	sb $t5, 2($a0)
	sb $t3, 3($a0)
	sb $t4,4($a0)
	sb $zero, 5($a0)
	POP $ra
	POP $s3
	jr $ra
	nop
	#$a0 ineh�ller adressen d�r str�ngen ska sparas, skriv till den genom sb