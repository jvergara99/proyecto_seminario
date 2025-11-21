#include <stdio.h>
#include <stdlib.h>

// Definiciones de la arquitectura simulada
#define MEMORY_SIZE 256  // 256 celdas de memoria (datos e instrucciones)
#define NUM_REGISTERS 4  // R0, R1, R2, R3

// Códigos de Operación (Opcodes) de nuestro Ensamblador Simple
// Estos son los valores que el CPU lee en la fase FETCH.
enum Opcode {
    HALT = 0, // Detener la ejecución
    LOAD = 1, // Cargar valor de memoria a un registro: LOAD R[reg], Mem[addr]
    STORE = 2, // Almacenar valor de registro en memoria: STORE R[reg], Mem[addr]
    ADD = 3, // Sumar dos registros y guardar en el primero: ADD R[dest], R[src]
    JUMP = 4  // Salto incondicional: JUMP addr
};

// Estado del CPU (Unidad Central de Procesamiento)
typedef struct {
    int memory[MEMORY_SIZE];  // Memoria principal (RAM) para datos e instrucciones 
    int registers[NUM_REGISTERS]; // Registros de propósito general 
    int pc; // Program Counter (Contador de Programa) - Direccion de la siguiente instruccion
    int ir; // Instruction Register (Registro de Instrucción) - Almacena la instruccion actual 
} CPU_State;

CPU_State cpu;

// --- Funciones de utilidad ---

// Cargar programa (código máquina) desde un archivo .mem a la memoria
void load_program(const char* filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error al abrir el archivo de programa (.mem)");
        exit(EXIT_FAILURE);
    }

    // Carga simple: leer enteros hasta el final del archivo
    int i = 0;
    int instruction_count = 0;
    int data;
    while (i < MEMORY_SIZE && fscanf(file, "%d", &data) == 1) {
        cpu.memory[i] = data;
	if ( i < 5 ) {
		instruction_count++;
	}
	i++;
    }
    fclose(file);
    printf("Programa cargado exitosamente en la memoria. Total de %d celdas ocupadas.\n", i);
    // --- VERIFICACIÓN DE DATOS CARGADOS (Opcional, pero muy útil) ---
    printf("DEBUG: Memoria[20] (debería ser 10) = %d\n", cpu.memory[20]);
    printf("DEBUG: Memoria[21] (debería ser 5) = %d\n", cpu.memory[21]);
}

// Ejecuta el ciclo Fetch-Decode-Execute
void execute_cycle() {
    int running = 1;
    printf("\n--- INICIO DE EJECUCIÓN ---\n");

    while (running) {
        // 1. FETCH (Obtener Instrucción) 
        if (cpu.pc >= MEMORY_SIZE) {
            printf("Error: Program Counter fuera de los límites de la memoria.\n");
            break;
        }
        // Leer la instruccion de la memoria en la dirección apuntada por el PC
        cpu.ir = cpu.memory[cpu.pc];
        printf("PC: %03d | FETCH: IR = %d\n", cpu.pc, cpu.ir);

        // Incrementar el PC para apuntar a la siguiente instruccion por defecto
        cpu.pc++;

	// 2. DECODE (Decodificar Instrucción)
	int ir = cpu.ir;
	int opcode, arg1, arg2;

	// Verificamos si la instrucción es de 4 dígitos (LOAD/STORE/JUMP con dirección de 2 dígitos)
	if (ir >= 1000) {
    		// Formato: [Opcode(1)] [Registro(1)] [Direccion(2)]
    
    		opcode = ir / 1000;           // Obtiene el primer dígito (ej. 1120 -> 1)
    		arg1 = (ir / 100) % 10;       // Obtiene el segundo dígito (ej. 1120 -> 1)
    		arg2 = ir % 100;              // Obtiene los últimos dos dígitos (ej. 1120 -> 20)
    
	} else if (ir >= 100) {
    		// Verificamos si la instrucción es de 3 dígitos (ADD con 2 Registros o HALT)
    		// Formato: [Opcode(1)] [Registro1(1)] [Registro2(1)] o [000]
    
    		opcode = ir / 100;            // Obtiene el primer dígito (ej. 312 -> 3)
    		arg1 = (ir / 10) % 10;        // Obtiene el segundo dígito (ej. 312 -> 1)
    		arg2 = ir % 10;               // Obtiene el tercer dígito (ej. 312 -> 2)
	} else {
    		// Si es un número pequeño, asumimos HALT o un error.
    		opcode = ir; 
    		arg1 = 0;
    		arg2 = 0;
	}

	printf("DECODE: Opcode: %d, Arg1: %d, Arg2: %d\n", opcode, arg1, arg2);

	// El resto del switch(opcode) permanece igual.


        // 3. EXECUTE (Ejecutar Instrucción) 
        switch (opcode) {
            case HALT:
                printf("EXECUTE: HALT. Deteniendo el CPU.\n");
                running = 0;
                break;

            case LOAD: // LOAD R[arg1], Mem[arg2]
                // arg1 es el registro de destino, arg2 es la dirección de memoria.
                cpu.registers[arg1] = cpu.memory[arg2];
                printf("EXECUTE: R%d = Mem[%d] = %d\n", arg1, arg2, cpu.registers[arg1]);
                break;

            case STORE: // STORE R[arg1], Mem[arg2]
                // arg1 es el registro de origen, arg2 es la dirección de memoria.
                cpu.memory[arg2] = cpu.registers[arg1];
                printf("EXECUTE: Mem[%d] = R%d = %d\n", arg2, arg1, cpu.memory[arg2]);
                break;

            case ADD: // ADD R[arg1], R[arg2]
                // arg1 es el registro destino, arg2 es el registro fuente.
                cpu.registers[arg1] += cpu.registers[arg2];
                printf("EXECUTE: R%d += R%d. Nuevo valor: %d\n", arg1, arg2, cpu.registers[arg1]);
                break;

            case JUMP: // JUMP arg2 (Saltar a la dirección)
                // arg2 es la nueva dirección del PC. Sobrescribe la lógica de incremento.
                cpu.pc = arg2;
                printf("EXECUTE: JUMP a la dirección %d.\n", cpu.pc);
                break;

            default:
                printf("Error: Opcode desconocido (%d) en la dirección %d\n", opcode, cpu.pc - 1);
                running = 0;
                break;
        }

        printf("--- Estado de Registros: R0=%d, R1=%d, R2=%d, R3=%d ---\n",
               cpu.registers[0], cpu.registers[1], cpu.registers[2], cpu.registers[3]);
        if (running) {
            // Pequeña pausa para simulación visual (opcional)
            // usleep(50000); 
        }
    }
    printf("--- FIN DE EJECUCIÓN ---\n");
}

int main(int argc, char *argv[]) {
    // Inicializar el estado del CPU
    for (int i = 0; i < MEMORY_SIZE; i++) cpu.memory[i] = 0;
    for (int i = 0; i < NUM_REGISTERS; i++) cpu.registers[i] = 0;
    cpu.pc = 0;
    cpu.ir = 0;

    if (argc != 2) {
        fprintf(stderr, "Uso: %s <archivo_programa.mem>\n", argv[0]);
        return 1;
    }

    // Cargar el programa binario (código máquina)
    load_program(argv[1]);

    // Iniciar el ciclo de ejecución
    execute_cycle();

    // Mostrar el contenido de memoria después de la ejecución
    printf("\nContenido de la celda de memoria 22 (donde podria estar el resultado): %d\n", cpu.memory[22]);
    
    return 0;
}
