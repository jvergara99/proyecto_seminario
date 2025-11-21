#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// CONVENCIONES DEL SIMULADOR:
// R1: Registro de trabajo general
// R2: Registro de trabajo temporal
// R3: Registro de Bandera/Condición (0 para Falso, 1 para Verdadero)
// Las etiquetas de salto (Label) serán representadas por direcciones.

// Dirección de inicio para las instrucciones (asumimos que los datos están al final)
#define START_ADDRESS 0 
// Contador de la próxima dirección de memoria (PC virtual)
int current_address = START_ADDRESS; 

// Escribe la instruccion ASM y actualiza la dirección actual
void generate_asm(FILE *fp, const char *mnemonic, int arg1, int arg2, const char *comment) {
    if (strcmp(mnemonic, "HALT") == 0) {
        fprintf(fp, "%s\t\t; [%03d] %s\n", mnemonic, current_address, comment);
    } else if (strcmp(mnemonic, "ADD") == 0) {
        fprintf(fp, "%s %d, %d\t; [%03d] %s\n", mnemonic, arg1, arg2, current_address, comment);
    } else if (strcmp(mnemonic, "JUMP") == 0 || strcmp(mnemonic, "LOAD") == 0 || strcmp(mnemonic, "STORE") == 0) {
        fprintf(fp, "%s %d, %d\t; [%03d] %s\n", mnemonic, arg1, arg2, current_address, comment);
    }
    // Todas las instrucciones ocupan una celda de memoria.
    current_address++; 
}

// Genera un número de etiqueta único para los saltos
int label_counter = 0;
int next_label() {
    return 100 + label_counter++; // Usamos direcciones altas para etiquetas
}

// Esta función simula la traducción de la estructura: 
// if (R3 == 1) { 
//   // Codigo IF (True Block)
// } else {
//   // Codigo ELSE (False Block)
// }
void translate_if_else(FILE *fp) {
    
    // --- Etapa de Planificación ---
    // 1. Etiqueta para el bloque ELSE: Donde saltamos si la condicion es falsa.
    // 2. Etiqueta para el final del IF/ELSE: Donde salta la parte IF despues de ejecutarse.
    int ELSE_LABEL = next_label();
    int END_IF_LABEL = next_label();

    // Resetear el contador de dirección virtual para que la función genere las direcciones correctas
    current_address = START_ADDRESS; 
    
    // Suponemos que la condición (R3 = 1 o R3 = 0) ya está cargada.
    
    // --- I. Simular la CONDICIÓN (IF R3 == 1) ---
    
    // [000] LOAD 2, R3 (Cargar el valor del registro de condición R3 en R2)
    // NOTE: En nuestro ASM simple, LOAD solo puede cargar de memoria.
    // **Ajuste Didáctico:** Asumiremos que el valor de la condición (0 o 1) está en la Memoria[100]
    int CONDITION_ADDR = 100;
    
    generate_asm(fp, "LOAD", 2, CONDITION_ADDR, "Cargar valor de condicion (0/1) en R2"); 
    
    // [001] ADD 2, 2 (Realizar una operacion aritmetica para establecer una 'bandera')
    // **AJUSTE CRÍTICO:** Como no tenemos salto condicional, solo podemos simularlo.
    // Simularemos la condicion falsa (R2 = 0) de manera forzada.
    
    // Si la condicion es FALSA (R2=0), NO queremos saltar. Si es VERDADERA (R2=1), queremos saltar 1 instruccion.
    
    // Para simplificar, asumimos que SOLO podemos simular la negación:
    
    // INSTRUCCION 1: Si R2 es 0, ejecutar la siguiente. Si R2 es 1, omitir la siguiente instruccion (SALTO CONDICIONAL SIMULADO)
    // Esta simulación es conceptual. Un CPU real tendría un JUMP_IF_ZERO.
    
    // --- TRADUCCIÓN DEL SALTO ---
    
    // [001] JUMP ELSE_LABEL (Si R2 es 0, saltar a ELSE_LABEL) - Este salto es condicional en un CPU real. 
    // Como solo tenemos JUMP incondicional, la única forma de simular el IF es a través de un JUMP a la dirección que indica la línea ELSE_LABEL.
    
    // **TRUCO DE IMPLEMENTACIÓN:** Usamos JUMP para indicar un salto incondicional, 
    // pero en este modelo, si la condición fuese evaluada como FALSA, la UC REALIZARÍA el salto.
    // Ya que no tenemos lógica en la UC para esto, GENERAREMOS el código para el camino TRUE:
    
    // [001] JUMP (a la dirección que representa el ELSE) 
    // Nota: Aquí, R2 es el registro que contiene 0 (Falso) o 1 (Verdadero). 
    // Un ensamblador REAL generaría: JUMP_IF_ZERO R2, ELSE_LABEL
    // En nuestro caso, forzamos un JUMP incondicional por simplicidad, pero DIDÁCTICAMENTE representa el salto condicional.
    // Asumiremos que el salto a ELSE_LABEL ocurre SÓLO si la condición es FALSE.
    
    // 1. Simular la Condición: Saltamos a ELSE si la condición es FALSA.
    generate_asm(fp, "JUMP", 0, ELSE_LABEL, "JUMP CONDICIONAL (si R2 es 0/Falso) a ELSE_LABEL"); 
    // Dirección del ELSE_LABEL se llenará en el ensamblador (Backward Reference)
    
    // --- II. Bloque IF (TRUE Block) ---
    
    // [002] Código generado para: R1 = R1 + 10 (Ejemplo del cuerpo del IF)
    generate_asm(fp, "LOAD", 2, 250, "Cargar valor 10 (Dato en Memoria[250]) en R2"); // R2=10
    generate_asm(fp, "ADD", 1, 2, "Ejecutar R1 = R1 + R2 (cuerpo IF)"); // R1 = R1 + 10
    
    // [004] JUMP (a la dirección de FIN del IF/ELSE)
    generate_asm(fp, "JUMP", 0, END_IF_LABEL, "Saltar al final de la estructura IF/ELSE");
    
    // --- III. Bloque ELSE (FALSE Block) ---
    
    // [ELSE_LABEL] (La dirección que apunta aquí será donde se coloca esta instrucción)
    int else_address = current_address;
    
    // Código generado para: R1 = R1 + 5 (Ejemplo del cuerpo del ELSE)
    generate_asm(fp, "LOAD", 2, 251, "Cargar valor 5 (Dato en Memoria[251]) en R2"); // R2=5
    generate_asm(fp, "ADD", 1, 2, "Ejecutar R1 = R1 + R2 (cuerpo ELSE)"); // R1 = R1 + 5
    
    // --- IV. Final del Bloque ---
    
    // [END_IF_LABEL] (La dirección que apunta aquí será donde se coloca esta instrucción)
    int end_address = current_address;
    
    // Actualizar las etiquetas reales en el archivo ASM generado (idealmente esto se hace en el ensamblador)
    // Para la PoC, podemos escribir las direcciones reales:
    
    printf("\n// --- Generacion de Etiquetado --- \n");
    printf("// La etiqueta ELSE_LABEL (JUMP 0, %d) debe apuntar a la direccion %d\n", ELSE_LABEL, else_address);
    printf("// La etiqueta END_IF_LABEL (JUMP 0, %d) debe apuntar a la direccion %d\n", END_IF_LABEL, end_address);
    
    generate_asm(fp, "HALT", 0, 0, "Fin del programa");
    
    // NOTA DIDÁCTICA: El ensamblador (assembler.c) en un compilador real
    // se encargaría de resolver estas etiquetas numéricas (ELSE_LABEL, END_IF_LABEL) 
    // durante su proceso de traducción (normalmente en dos pasadas).
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <archivo_salida.asm>\n", argv[0]);
        return 1;
    }

    FILE *out_fp = fopen(argv[1], "w");
    if (out_fp == NULL) { perror("Error abriendo el archivo ASM de salida"); return 1; }
    
    // Encabezado para la prueba
    fprintf(out_fp, "; --- C GENERATOR TEST: IF-ELSE STRUCTURE ---\n");
    fprintf(out_fp, "; IF (Mem[100] == 1) { R1 = R1 + 10 } ELSE { R1 = R1 + 5 }\n\n");
    
    translate_if_else(out_fp);

    fclose(out_fp);
    printf("\nArchivo ASM generado: %s\n", argv[1]);

    return 0;
}
