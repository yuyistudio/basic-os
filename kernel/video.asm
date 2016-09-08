; print on screen
global	put_char
global	get_char
global	put_char_only


;屏幕存储从0xb8000开始
;单屏幕字符数:80*25=2000
;单屏幕字节数:4000
;总显存:64K
;偏移量可以用2B存储
;颜色值和字符值可以分别用1B存储
;颜色8位从高位到地位: 背景(高亮 红 绿 蓝) 前景(高亮 红 绿蓝)
;字符为ASCII码

;参数四个字节的使用情况:
; 颜色 字符 偏移h 偏移l
; void put_char( int _info)
put_char:
	push	ebp
	mov	ebp, esp
	pushad
	;取参数
	mov	ebx, [ebp + 8]
	;得到字符偏移量,并转化为字节偏移量
	;字节偏移量最大128K,可能超过两个字节
	mov	eax, ebx
	and	eax, 0x0000FFFF
	mov	ecx, 2
	mul	ecx	;结果保存于edx:eax中,但是我们知道,无论如何,edx肯定为0
	mov	edx, eax
	;得到字符和颜色
	shr	ebx, 16
	;打印字符
	mov	word [gs:edx], bx
	
	popad
	pop	ebp
ret



;得到字符偏移地址处的字符及其信息
;int get_char( int _char_offset);
;返回的格式(高-->低)
; 颜色 字符 偏移h 偏移l
get_char:	
	;eax作为返回值寄存器,不需要push
	push	ebp
	mov	ebp, esp
	push	ecx
	push	edx
	
	;取参数
	mov	edx, [ebp + 8]
	;防止不合法的数据
	and	edx, 0x0000FFFF
	push	edx
	;得到字符偏移量,变换为字节偏移量,存在eax中
	;字符偏移量仍然在edx中
	mov	ecx, 2
	mov	eax, edx
	mul	ecx		;这一步会将edx清0
	;得到新的字符信息
	mov	ax, word [gs:eax]
	;将新的字符信息保存到eax的高位
	shl	eax, 16
	;再加上参数中的偏移信息,作为返回值
	pop	edx
	add	eax, edx
	
	pop	edx
	pop	ecx
	pop	ebp
ret

;只改变字符,不改变颜色
;参数4个字节使用情况
; 空 字符 偏移h 偏移l
;void put_char_only( int _info);
put_char_only:
	push	ebp
	mov	ebp, esp
	pushad
	;取参数
	mov	ebx, [ebp + 8]
	;得到字符偏移量,并转化为字节偏移量
	;字节偏移量最大128K,可能超过两个字节
	mov	eax, ebx
	and	eax, 0x0000FFFF
	mov	ecx, 2
	mul	ecx	;结果保存于edx:eax中,但是我们知道,无论如何,edx肯定为0
	mov	edx, eax
	;得到字符
	shr	ebx, 16
	;打印字符
	mov	byte [gs:edx], bl
	
	popad
	pop	ebp
ret
