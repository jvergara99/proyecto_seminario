#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Definiciones (Deben coincidir con las de cpu_simulator.c)
enum Opcode {
    HALT = 0,
    LOAD = 1,
    STORE = 2,
    ADD = 3,
    JUMP = 4
};

// Función de utilidad para mapear mnemónicos a Opcodes
int get_opcode_value(char *mnemonic) {
    if (strcmp(mnemonic, "HALT") == 0) return HALT;
    if (strcmp(mnemonic, "LOAD") == 0) return LOAD;
    if (strcmp(mnemonic, "STORE") == 0) return STORE;
    if (strcmp(mnemonic, "ADD") == 0) return ADD;
    if (strcmp(mnemonic, "JUMP") == 0) return JUMP;
    return -1; // Opcode no reconocido
}

// Función para limpiar una línea (eliminar comentarios y convertir a mayúsculas)
void cleanup_line(char *line) {
    char *comment = strchr(line, ';'); // Buscar inicio de comentario
    if (comment != NULL) {
        *comment = '\0'; // Terminar la cadena en el comentario
    }
    // Convertir a mayúsculas y limpiar espacios en blanco (simplificado para el ejemplo)
    for (int i = 0; line[i]; i++) {
        line[i] = toupper((unsigned char)line[i]);
    }
}

void assemble(const char *input_file, const char *output_file) {
    FILE *in_fp = fopen(input_file, "r");
    FILE *out_fp = fopen(output_file, "w");
    
    if (in_fp == NULL) { perror("Error abriendo el archivo ASM"); return; }
    if (out_fp == NULL) { perror("Error abriendo el archivo MEM"); fclose(in_fp); return; }

    char line[256];
    int instruction_count = 0;

    printf("Iniciando traducción de '%s' a '%s'...\n", input_file, output_file);

    while (fgets(line, sizeof(line), in_fp) != NULL) {
        cleanup_line(line); // Preprocesamiento
        
        char mnemonic[10];
        int arg1 = -1, arg2 = -1;
        
        // Intentar leer la instrucción y hasta dos argumentos
        // Los argumentos pueden ser registros (R) o direcciones (D)
        int num_scanned = sscanf(line, "%s %d, %d", mnemonic, &arg1, &arg2);

        // Si solo se leyó el mnemónico (ej. HALT), procesar
        if (num_scanned < 1) continue; 
        
        int opcode = get_opcode_value(mnemonic);
        int machine_code = 0;
        
        // --- PROCESAMIENTO Y GENERACIÓN DEL CÓDIGO MÁQUINA ---
        
        if (opcode == -1) {
            fprintf(stderr, "Error en linea %d: Mnemónico no reconocido '%s'\n", instruction_count + 1, mnemonic);
            continue;
        }

        if (opcode == HALT) {
            // HALT no tiene argumentos: Codigo 000
            machine_code = HALT * 100;
            printf("[%03d] HALT -> %03d\n", instruction_count, machine_code);
        
        } else if (opcode == LOAD || opcode == STORE || opcode == JUMP) {
            // LOAD/STORE/JUMP requieren un registro/dirección (arg1) y una dirección/destino (arg2)
            // Asumiremos que los argumentos ya están en formato numérico correcto (ej. 1 para R1, 20 para dirección 20)
            
            // Formato: [Opcode * 1000] + [Arg1 * 100] + [Arg2]
            machine_code = (opcode * 1000) + (arg1 * 100) + arg2;
            printf("[%03d] %s R%d, %d -> %04d\n", instruction_count, mnemonic, arg1, arg2, machine_code);

        } else if (opcode == ADD) {
             // ADD requiere dos registros (arg1, arg2)
            machine_code = (opcode * 100) + (arg1 * 10) + arg2;
            printf("[%03d] %s R%d, R%d -> %03d\n", instruction_count, mnemonic, arg1, arg2, machine_code);
        
        }
        
        // Escribir el código máquina generado en el archivo de salida
	if ( machine_code != 0) {
        	fprintf(out_fp, "%d\n", machine_code);
        	instruction_count++;
	}    
    }

	// Agregar cambio
	// --- SEGUNDA FASE: INSERCIÓN DE DATOS Y RELLENO ---
    
    // 1. Definir la dirección de inicio de los datos
    	const int DATA_START_ADDRESS = 20; 
    
    // 2. Calcular el número de celdas de relleno (ceros) necesarias
    	int padding_needed = DATA_START_ADDRESS - instruction_count; 
    
    	if (padding_needed > 0) {
        	printf("\nInsertando %d celdas de relleno (ceros) para alcanzar la dirección %d...\n", 
               		padding_needed, DATA_START_ADDRESS);
        
        for (int i = 0; i < padding_needed; i++) {
            fprintf(out_fp, "0\n"); // Escribir un cero
        }
        instruction_count += padding_needed; // Actualizar el contador
    }

    // 3. Insertar los datos de entrada de la prueba de suma (10 y 5) y el espacio de resultado (0)
    
    // Memoria[20] = 10
    fprintf(out_fp, "10\n");
    printf("Dato: 10 insertado en la Memoria[%d]\n", instruction_count++);
    
    // Memoria[21] = 5
    fprintf(out_fp, "5\n");
    printf("Dato: 5 insertado en la Memoria[%d]\n", instruction_count++);

    // Memoria[22] = 0 (Espacio para resultado)
    fprintf(out_fp, "0\n");
    printf("Espacio de resultado (0) insertado en la Memoria[%d]\n", instruction_count++);
// Fin de agregar cambio

    printf("Traducción completada. %d instrucciones ensambladas.\n", instruction_count);
    fclose(in_fp);
    fclose(out_fp);
}


int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <archivo_entrada.asm> <archivo_salida.mem>\n", argv[0]);
        return 1;
    }

    assemble(argv[1], argv[2]);

    return 0;
}
