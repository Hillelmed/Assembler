MAIN:	add	$3,$5,$9
LOOP:	ori	$9,-5,$2
		la	vall
;R Commands
Kacc:	add	$0 ,$1 ,$2  
		sub	$3 ,$4 ,$5
;	I Commands
HILLEL:	addi	$24 ,  123 , $25
		subi	$26  , 0 , $27
		andi	$28, -30, $29
		ori		$30,-32768,$31 
		nori	$17,32767,$20
		bne		$23 , $23, HILLEL   
		beq		$16, $6, HILLEL
		blt		$15, $0, HILLEL  
		bgt		$20, $15, HILLEL
		lb		$24 , 123  , $25
		sb		$26 , 0 , $27
		lw		$28, -30, $29
		sw		$30,-32768,$31 
		lh		$17,32767,$20
		sh		$26 , 0 , $27
		jmp	Next
Next:	move	$20,$4
List:	.db	6,-9
		bgt	$4,$2,END
		la	K
		sw	$0,4,$10
		bne	$31,$9,LOOP
		call	vall
		jmp	$4
;		la	wNumber
HelloJ:	jmp		Next
		jmp		$7
		la		HILLEL
		la		Next
		call	HILLEL
		call	Next
BYE:	stop

.extern	vall
		.dh	27056
K:		.dw	31,-12
END		stop
.entry	K
STR:	.asciz	"Hello World"
