#ifndef X86_FUNCTION_H_
#define X86_FUNCTION_H_

void x86_function(double leftX, double upY, int displayWidth, int displayHeight, double scale, char * data, long long pitch, double cReal, double cImaginary);

#endif // X86_FUNCTION_H_



//RDI, RSI, RDX, RCX, R8, R9 pierwsze 6 intow
// XMM0, XMM1, XMM2, XMM3, XMM4, XMM5, XMM6 and XMM7 przecinkowe
// RBX, RBP, and R12–R15 sa zapamietywane przez funckje wywolana
// double word to 32 bity a quad word to 64 bity

// leftX XMMO
// upY XMM1
// displayWidth edi
// displayHeight esi
// scale XMM2
// *data RDX
// pitch ecx
// cReal XMM3
// CImaginary XMM4


//mov BYTE PTR [ebx], 2	; Move 2 into the single byte at the address stored in EBX.
//mov WORD PTR [ebx], 2	; Move the 16-bit integer representation of 2 into the 2 bytes starting at the address in EBX.
//mov DWORD PTR [ebx], 2    	; Move the 32-bit integer representation of 2 into the 4 bytes starting at the address in EBX.
