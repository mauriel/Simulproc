//-----------------
// Instructions
//-----------------
        TEXT 30

        // Programme principal
main    EQU *
        PUSH @op1
        PUSH @op2     
	PUSH @op1
        PUSH @op2  
        PUSH @op1
        PUSH @op2  
	PUSH @op1
        PUSH @op2  
	PUSH @op1
        PUSH @op2  
	PUSH @op1
        PUSH @op2  
	PUSH @op1
        PUSH @op2 
	PUSH @op1
        PUSH @op2  
	PUSH @op1
        PUSH @op2
        PUSH @op1
        PUSH @op2  
	PUSH @op1
        PUSH @op2    

return  RET
        
        END
        
//-----------------
// Données et pile
//-----------------
        DATA 30
        
        WORD 0
result  WORD 0
op1     WORD 20
op2     WORD 5
        
        END
