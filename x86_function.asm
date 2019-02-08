section .data align=64
    dwa: dq 2.0


section .text
	
global x86_function

x86_function:
	push rbp	; push "calling procedure" frame pointer
	mov rbp, rsp	; set new frame pointer 
			;	- "this procedure" frame pointer

; -------------------------Calling convention---------------------------------
; leftX XMMO
; upY XMM1
; displayWidth rdi
; displayHeight rsi
; scale XMM2
; *data RDX
; pitch rcx
; cReal XMM3
; cImaginary XMM4
;------------------------------------------------------------------------------

;-------------------------Current usage of registers---------------------------
;rax - pomocniczy do mnozenia przez 2
;rbx - licznik koloru
;rcx - pitch
;rdx - *data
;rbp - frame pointer
;rsp - stack pointer
;rsi - displayHeight -> height counter
;rdi - displayWidth
;r8  - widtdh counter (licznik kolumn)
;r9  -
;r10 -
;r11 -
;XMMO- leftX -> starting left real part
;XMM1- UPY -> actual imaginary part
;XMM2- scale (distance/pixel)
;XMM3- cReal
;XMM4- cImaginary
;XMM5- actual real part
;XMM6- roboczy imaginary part na potrzeby liczenia ciagu
;XMM7- roboczy real part na potrzeby liczenia ciagu
;XMM8- roboczo kwadrat imaginary ^
;XMM9-
;XMM10-do mnozenia przez 2
;------------------------------------------------------------------------------
    xor r9,r9;debug

forEveryPixelRow:
    movapd xmm5, xmm0 ;ustawienie actual real part
    mov r8, rdi;ustawienie licznika kolumn

    forEveryPixel:
        movapd xmm6,xmm1;zapamietujemy roboczo imganery part
        movapd xmm7,xmm5;zapamietujemy roboczo real part
        xor rbx,rbx;zerujemy licznik koloru

        checkNextTermOfSequence:
            ;obliczenie kwadratu imaginary
            movapd xmm8, xmm6;
            mulsd xmm8, xmm8;

            ;obliczenie kwadratu real
            movapd xmm9, xmm7
            mulsd xmm9, xmm9

            ;obliczenie czesci urojonej = im * real * 2 + cImaginary
            mulsd xmm6, xmm7; im * real
            mov     rax,2
            cvtsi2sd xmm10, rax
            mulsd xmm6, xmm10; *2
            addsd xmm6, xmm4; +cImaginary

            ;obliczenie czesci rzeczywistej = real^2 - im^2 + cReal
            movapd xmm7, xmm9; real^2
            subsd xmm7, xmm8; -im^2
            addsd xmm7, xmm3; +cReal

            ;xmm8 zamiast kwadratu imaginary bedzie przechowywac kwadrat modulu
            addsd xmm8, xmm9; im^2 + real^2

            inc bl; zwieksz licznik koloru

            ;zakoncz jesli kwadrat modulu wieksz niz 4
            mov     rax,4
            cvtsi2sd xmm10, rax
            comisd xmm8,xmm10
            jnb dontDraw


            ;mov     rax,2
            ;cvtsi2sd xmm11, rax
            ;comisd xmm11,xmm10

            ;zakoncz jestli przekroczylismy maksymalna ilosc obrotow petli
            cmp bx, 124 ; im wieksza wartosc tym dokladniejsze obrazki, ale program wolniej dziala
            jge endLoop

            jmp checkNextTermOfSequence

        dontDraw:
        xor bx,bx
        endLoop:

        ;inc r9 ;debug


        ;int pixel = lr->data + lr->pitch*y + x*4;
        shl bx, 1;

        mov BYTE [rdx], bl ;zapis odcienia koloru do talibcy
        inc rdx;
        mov BYTE [rdx], bl ;zapis odcienia koloru do talibcy
        inc rdx;
        mov BYTE [rdx], bl ;zapis odcienia koloru do talibcy
        inc rdx;
        addsd xmm5, xmm2 ; dodanie skali do czesci rzeczywistej
        dec r8; dekrementacja licznika kolumn, width
        jnz forEveryPixel;


    ;TODO uwzglednienie paddingu
    add rdx, rcx
    add rdx, rcx

    subsd xmm1, xmm2 ; odjecie skali od czesci urojonej
    dec rsi ; zmniejszenie licznika wierzy
    jnz forEveryPixelRow ;skok jesli licznik wierszy nie zero

;------------------------------------------------------------------------------
endProgram:

	mov rsp, rbp	; restore original stack pointer
	pop rbp		; restore "calling procedure" frame pointer
	ret
