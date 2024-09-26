#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void llenar(int **c, int **a, int *disponible, int8_t fila, int8_t columna) {
  FILE *numeros = fopen("prueba.txt", "r");
  if (numeros != NULL) {
    for (int8_t i = 0; i < fila; i++) {
      fscanf(numeros, "%d,%d,%d", &c[i][0], &c[i][1], &c[i][2]);
    }
    for (int8_t i = 0; i < fila; i++) {
      fscanf(numeros, "%d,%d,%d", &a[i][0], &a[i][1], &a[i][2]);
    }
    fscanf(numeros, "%d,%d,%d", &disponible[0], &disponible[1], &disponible[2]);
    fclose(numeros);
  }
}

int **crearMatrizDinamica(int8_t fila, int8_t columna) {
  int **matriz = (int **)malloc(fila * sizeof(int *));
  for (int8_t i = 0; i < fila; i++) {
    matriz[i] = (int *)malloc(columna * sizeof(int));
  }
  return matriz;
}

void restar(int **c, int **a, int **ca, int8_t fila, int8_t columna) {
  for (int8_t i = 0; i < fila; i++) {
    for (int8_t j = 0; j < columna; j++) {
      ca[i][j] = c[i][j] - a[i][j];
    }
  }
}

void imprimir(int **matriz, int8_t fila, int8_t columna, char *caracter) {
  for (int8_t i = 0; i < fila; i++) {
    for (int8_t j = 0; j < columna; j++) {
      printf("%d ", matriz[i][j]);
    }
    printf("\n");
  }
  printf("%s\n", caracter);
  printf("--------\n");
}

int *arregloDinamico(int tamano) { return (int *)malloc(tamano * sizeof(int)); }

void imprimirArreglo(int *arreglo, int8_t tamano) {
  for (int8_t i = 0; i < tamano; i++) {
    printf("%d ", arreglo[i]);
  }
  printf("disponible");
  printf("\n-----------\n");
}

int8_t sumaFilaMatrizCA(int **ca, int fila) {
  int8_t sumar = 0;
  for (int8_t i = (fila - 1); i < fila; i++) {
    for (int8_t j = 0; j < 3; j++) {
      sumar += ca[i][j];
    }
  }

  return sumar;
}

int evaluarFilas(int **ca, int *disponible, int8_t fila) {
  int8_t contador = 0;
  for (int8_t i = (fila - 1); i < fila; i++) {
    for (int8_t j = 0; j < 3; j++) {
      if (ca[i][j] <= disponible[j] && sumaFilaMatrizCA(ca, fila) != 0) {
        contador++;
      }
    }
  }

  return contador;
}

void sumarFilaMatrizA(int **a, int **ca, int8_t fila) {
  for (int8_t i = 0; i < 3; i++) {
    a[fila][i] += ca[fila][i];
  }
}

void restarFilaMatrizCA(int **ca, int8_t posicion) {
  for (int8_t i = 0; i < 3; i++) {
    ca[posicion][i] -= ca[posicion][i];
  }
}

void restarRecursosDisponibles(int **ca, int *disponible, int8_t fila) {
  for (int8_t i = 0; i < 3; i++) {
    disponible[i] -= ca[fila][i];
  }
}

void sumarRecursosDisponibles(int **a, int *disponible, int8_t fila) {
  for (int8_t i = 0; i < 3; i++) {
    disponible[i] += a[fila][i];
  }
}

void liberarMemoriaMatrizA(int **c, int **a, int8_t fila) {
  for (int8_t i = 0; i < 3; i++) {
    if (c[fila][i] == a[fila][i]) {
      a[fila][i] = 0;
    }
  }
}

void procedimientoMatrices(int **c, int **a, int **ca, int *disponible,
                           int8_t fila, int8_t columna) {
  int8_t resultado = 0;
  for (int k = 1; k <= fila; k++) {
    resultado = evaluarFilas(ca, disponible, k);
    if (resultado == 3) {
      sumarFilaMatrizA(a, ca, k - 1);
      restarRecursosDisponibles(ca, disponible, k - 1);
      restarFilaMatrizCA(ca, k - 1);
      imprimir(a, fila, columna, "asignado");
      imprimir(ca, fila, columna, "resultante");
      imprimirArreglo(disponible, columna);
      sumarRecursosDisponibles(a, disponible, k - 1);
      liberarMemoriaMatrizA(c, a, k - 1);
      imprimir(a, fila, columna, "asignado");
      imprimirArreglo(disponible, columna);
    }
  }
}

int8_t validarEstadoMatrizCA(int **ca, int8_t fila, int8_t columna) {
  int8_t suma = 0;
  for (int8_t i = 0; i < fila; i++) {
    for (int8_t j = 0; j < columna; j++) {
      suma += ca[i][j];
    }
  }
  return suma;
}

void liberarMemoria(int **ca, int **a, int **c, int8_t fila) {
  for (int8_t i = 0; i < fila; i++) {
    free(c[i]);
    free(a[i]);
    free(ca[i]);
  }

  free(c);
  free(a);
  free(ca);
}

int main(int argc, char *argv[]) {
  int8_t fila = 4, columna = 3;
  int **c = crearMatrizDinamica(fila, columna);
  int **a = crearMatrizDinamica(fila, columna);
  int **ca = crearMatrizDinamica(fila, columna);
  int *disponible = arregloDinamico(columna);
  int estadoMatrizCA = 0;

  llenar(c, a, disponible, fila, columna);
  restar(c, a, ca, fila, columna);

  imprimir(c, fila, columna, "requiere");
  imprimir(a, fila, columna, "asignado");
  imprimir(ca, fila, columna, "resultante");
  imprimirArreglo(disponible, columna);

  do {
    procedimientoMatrices(c, a, ca, disponible, fila, columna);
    estadoMatrizCA = validarEstadoMatrizCA(ca, fila, columna);
  } while (estadoMatrizCA != 0);

  liberarMemoria(ca, a, c, fila);

  return 0;
}
