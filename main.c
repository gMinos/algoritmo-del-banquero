#include <stdio.h>
#include <stdlib.h>

void llenar(int **c, int **a, int *disponible, int fila, int columna) {
  FILE *numeros = fopen("prueba.txt", "r");
  if (numeros != NULL) {
    for (int i = 0; i < fila; i++) {
      fscanf(numeros, "%d,%d,%d", &c[i][0], &c[i][1], &c[i][2]);
    }
    for (int i = 0; i < fila; i++) {
      fscanf(numeros, "%d,%d,%d", &a[i][0], &a[i][1], &a[i][2]);
    }
    fscanf(numeros, "%d,%d,%d", &disponible[0], &disponible[1], &disponible[2]);
    fclose(numeros);
  }
}

int **crearMatrizDinamica(int fila, int columna) {
  int **matriz = (int **)malloc(fila * sizeof(int *));
  for (int i = 0; i < fila; i++) {
    matriz[i] = (int *)malloc(columna * sizeof(int));
  }
  return matriz;
}

void restar(int **c, int **a, int **ca, int fila, int columna) {
  for (int i = 0; i < fila; i++) {
    for (int j = 0; j < columna; j++) {
      ca[i][j] = c[i][j] - a[i][j];
    }
  }
}

void imprimir(int **matriz, int fila, int columna, char *caracter) {
  for (int i = 0; i < fila; i++) {
    for (int j = 0; j < columna; j++) {
      printf("%d ", matriz[i][j]);
    }
    printf("\n");
  }
  printf("%s\n", caracter);
  printf("--------\n");
}

int *arregloDinamico(int tamano) { return (int *)malloc(tamano * sizeof(int)); }

void imprimirArreglo(int *arreglo, int tamano) {
  for (int i = 0; i < tamano; i++) {
    printf("%d ", arreglo[i]);
  }
  printf("disponible");
  printf("\n-----------\n");
}

int sumaFilaMatrizCA(int **ca, int fila) {
  int sumar = 0;
  for (int i = (fila - 1); i < fila; i++) {
    for (int j = 0; j < 3; j++) {
      sumar += ca[i][j];
    }
  }

  return sumar;
}

int evaluarFilas(int **ca, int *disponible, int aux) {
  int contador = 0;
  for (int i = (aux - 1); i < aux; i++) {
    for (int j = 0; j < 3; j++) {
      if (ca[i][j] <= disponible[j] && sumaFilaMatrizCA(ca, aux) != 0) {
        contador++;
      }
    }
  }

  return contador;
}

void sumarFilaMatrizA(int **a, int **ca, int fila) {
  for (int i = 0; i < 3; i++) {
    a[fila][i] += ca[fila][i];
  }
}

void restarFilaMatrizCA(int **ca, int posicion) {
  for (int i = 0; i < 3; i++) {
    ca[posicion][i] -= ca[posicion][i];
  }
}

void restarRecursosDisponibles(int **ca, int *disponible, int fila) {
  for (int i = 0; i < 3; i++) {
    disponible[i] -= ca[fila][i];
  }
}

void sumarRecursosDisponibles(int **a, int *disponible, int fila) {
  for (int i = 0; i < 3; i++) {
    disponible[i] += a[fila][i];
  }
}

void liberarMemoriaMatrizA(int **c, int **a, int fila) {
  for (int i = 0; i < 3; i++) {
    if (c[fila][i] == a[fila][i]) {
      a[fila][i] = 0;
    }
  }
}

void procedimientoMatrices(int **c, int **a, int **ca, int *disponible, int fila, int columna) {
  int resultado = 0;
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

int validarEstadoMatrizCA(int **ca, int fila, int columna) {
  int suma = 0;
  for (int i = 0; i < fila; i++) {
    for (int j = 0; j < columna; j++) {
      suma += ca[i][j];
    }
  }
  return suma;
}

int main() {
  int fila = 4, columna = 3;
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

  for (int i = 0; i < 4; i++) {
    free(c[i]);
    free(a[i]);
    free(ca[i]);
  }

  free(c);
  free(a);
  free(ca);

  return 0;
}
