	.code
asmSum proc
	xor eax, eax
	cmp edx, 0
	jle InvalidArray
@@:
	add eax, [rcx]
	add rcx, 4
	dec edx
	jnz @B
InvalidArray:
	ret
asmSum endp
	end