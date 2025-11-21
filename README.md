# Proyecto de Seminario de Programación de Computadoras
Construir desde la arquitectura más básica hasta la abstracción de alto nivel, una computadora simulada funcional en lenguaje C, comprendiendo la relación directa entre hardware, lenguaje ensamblador y programación estructurada.

### Semana 1. Diseñar la máquina (Arquitectura de Computadoras)
Comprenderán y modelarán la arquitectura Von Neumann en lenguaje C, implementando una CPU básica con memoria, registros, unidad de control y ciclo de ejecución.

**Entregables:**

1. Simulador funcional de CPU (cpu_simulator.c) con ciclo fetch–decode–execute.
2. Diagrama de arquitectura que muestre los componentes: memoria, ALU, UC y buses

Primera sección del paper:
- Introducción, justificación, modelo arquitectónico y fundamentos teóricos (Von Neumann, control, memoria, E/S).
- Evidencia de ejecución de un programa mínimo (por ejemplo, suma de dos números en memoria).

### Semana 2.Crear el lenguaje máquina (Ensamblador)
Diseñarán el lenguaje ensamblador propio y programarán en C un traductor ASM → código binario que sea ejecutable por el simulador de la Semana 1.

**Entregables:**

3. Ensamblador en C (assembler.c) que traduzca mnemónicos a instrucciones binarias.
4. Programa ASM de prueba (programa.asm) y su correspondiente salida binaria (programa.mem).

Segunda sección del paper:
- Diseño del conjunto de instrucciones (ISA), modos de direccionamiento, estructura del ensamblador y validación experimental.
- Prueba ejecutada en su CPU simulada, demostrando la correcta ejecución del programa ensamblado.

### Semana 3. Agregar el lenguaje de alto nivel (C)
 Desarrollarán una capa en C que genere instrucciones ASM para su ensamblador, simulando un mini compilador.
 Deberán demostrar la relación semántica entre el código C, el ASM resultante y la ejecución en su máquina simulada.

**Entregables:**

5. Programa generador de ASM en C (c_to_asm.c) que traduzca operaciones básicas (asignaciones, estructuras condicionales, ciclos).
6. Diagrama del flujo de traducción: C → ASM → Binario → CPU simulada.

Tercera sección del paper:
- Abstracción de procedimientos, estructuras de control, parámetros y tipos de datos.
- Ejecución funcional de un programa generado en C y ejecutado por su ensamblador y CPU.

### Semana 4. Integrar todo y ejecutar programas completos (estructurados y recursivos)
 Integrarán las tres capas del sistema (CPU, ensamblador, C) y ejecutarán un programa recursivo, demostrando la interacción completa entre niveles.
 Se evaluará la eficiencia, correctitud y capacidad de representación del sistema desarrollado.

**Entregables:**

Sistema integrado completo en C:
7. cpu_simulator.c, assembler.c, c_to_asm.c, y programa final (main.c).
8. Ejecución de un programa recursivo (por ejemplo, factorial o Fibonacci).

Cuarta y última sección del paper:
- Resultados experimentales, validación formal, análisis de rendimiento, discusión crítica y conclusiones.

Presentación final: defensa oral y demo en video (15 a 30 minutos) donde participen todos los integrantes.
Deben mostrar el funcionamiento del sistema.
Cada miembro explica su parte (arquitectura, ensamblador, compilador, programa final).
El video debe mostrar sus rostros y la ejecución de la PoC.
