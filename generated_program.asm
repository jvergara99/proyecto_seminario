; --- C GENERATOR TEST: IF-ELSE STRUCTURE ---
; IF (Mem[100] == 1) { R1 = R1 + 10 } ELSE { R1 = R1 + 5 }

LOAD 2, 100	; [000] Cargar valor de condicion (0/1) en R2
JUMP 0, 100	; [001] JUMP CONDICIONAL (si R2 es 0/Falso) a ELSE_LABEL
LOAD 2, 250	; [002] Cargar valor 10 (Dato en Memoria[250]) en R2
ADD 1, 2	; [003] Ejecutar R1 = R1 + R2 (cuerpo IF)
JUMP 0, 101	; [004] Saltar al final de la estructura IF/ELSE
LOAD 2, 251	; [005] Cargar valor 5 (Dato en Memoria[251]) en R2
ADD 1, 2	; [006] Ejecutar R1 = R1 + R2 (cuerpo ELSE)
HALT		; [007] Fin del programa
