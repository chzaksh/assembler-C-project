
.entry LOOP
.entry LENGTH
MAIN:		mov  S1.1 ,W
	add	r2,STR       
LOOP:		jmp	W      
   macro m1         
			 	inc K          
			 	mov S1.2, r3   
			endmacro         
			prn #-5           
			sub r1, r4       
			m1               
			bne L3   

END:		hlt           
STR:		.string "abcdef"
LENGTH:	.data 6,-9,15
K:			.data 22
S1:		.struct 8, "ab"
macro m2         
			 	inc K          
			 	mov S1.2, r3   
			endmacro         
			prn #-5           
			sub r1, r4   
macro m3         
			 	inc K          
			 	mov S1.2, r3   
			endmacro         
			  
macro m4         
			 	inc K          
			 	mov S1.2, r3   
			endmacro         
			          
			sub r1, r4   
macro m5         
			 	    
			 	mov S1.2, r3   
			endmacro         
			prn #-5           
			
macro m6         
			 	        
			 	mov S1.2, r3   
			endmacro         
			prn #-5           
			sub r1, r4   
macro m7         
			 	inc K          
			 	
			endmacro         
			prn #-5           
			sub r1, r4   
macro m8         
			 	inc K          
			 	mov S1.2, r3   
			endmacro         
			prn #-5           
			sub r1, r4     
