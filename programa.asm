; Este es el archivo de prueba (programa.asm)
; Asume que los datos 10 y 5 están en las direcciones de memoria 20 y 21.

LOAD 1, 20      ; Cargar el valor de Memoria[20] (10) en el Registro R1
LOAD 2, 21      ; Cargar el valor de Memoria[21] (5) en el Registro R2
ADD 1, 2        ; Sumar R1 + R2. El resultado (15) queda en R1
STORE 1, 22     ; Almacenar el resultado de R1 en Memoria[22]
HALT            ; Detener la ejecucion
