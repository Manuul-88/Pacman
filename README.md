# Pac-Man 2D en OpenGL

## Descripción

Este proyecto consiste en una recreación simplificada del videojuego clásico Pac-Man utilizando OpenGL y GLUT en C++.

El objetivo principal fue aplicar conceptos de graficación 2D como:

- primitivas gráficas,
- animación,
- matrices bidimensionales,
- renderizado en tiempo real,
- representación de objetos en pantalla,
- control de movimiento y colisiones.

El juego incluye:

- movimiento automático de Pac-Man,
- fantasmas con comportamiento básico,
- power pellets,
- modo donde Pac-Man puede comer fantasmas,
- sistema de reinicio,
- animaciones y detección de colisiones.

---

# Tecnologías Utilizadas

- C++
- OpenGL
- GLUT / FreeGLUT
- VSCode
- g++

---

# Conceptos de Graficación Aplicados

## Representación mediante matriz

El laberinto se representa utilizando una matriz bidimensional:

```cpp
int mapa[FILAS][COLUMNAS];
```

Donde:

- `1` = pared
- `2` = punto
- `3` = power pellet
- `0` = espacio vacío

---

# Construcción del Mapa

Cada celda de la matriz se convierte en coordenadas dentro de la ventana gráfica utilizando:

```cpp
x = offsetX + columna * CELDA;
y = offsetY + fila * CELDA;
```

Esto permitió construir el laberinto completo utilizando coordenadas 2D.

---

# Primitivas Gráficas Utilizadas

| Primitiva | Uso |
|---|---|
| GL_QUADS | paredes |
| GL_TRIANGLE_FAN | círculos |
| GL_LINE_LOOP | bordes |

---

# Dibujo de Pac-Man

Pac-Man fue construido utilizando un círculo parcial.

```cpp
circuloParcial(...)
```

La boca se genera eliminando un sector angular del círculo.

La apertura y cierre de la boca se controla mediante animación.

---

# Fantasmas

Los fantasmas se dibujan utilizando:

- círculos parciales,
- rectángulos,
- triángulos,
- ojos construidos con círculos.

Se implementaron dos comportamientos básicos:

## Fantasma rojo
Persigue directamente a Pac-Man.

## Fantasma rosa
Intenta interceptar al jugador adelantándose a su dirección.

---

# Movimiento

El movimiento se controla mediante validación dentro de la matriz.

Proceso:

1. Se detecta la dirección.
2. Se calcula la siguiente celda.
3. Se verifica si existe una pared.
4. Si no existe pared, el objeto avanza.

---

# Power Pellets

Al consumir un pellet especial:

- los fantasmas cambian de color,
- Pac-Man puede comerlos temporalmente,
- se otorgan puntos adicionales.

---

# Animación

La animación se realiza utilizando:

```cpp
glutTimerFunc()
```

Esto permite actualizar continuamente:

- movimiento de Pac-Man,
- movimiento de fantasmas,
- animación de la boca,
- temporizador del modo poder.

---

# Detección de Colisiones

Se calcula la distancia entre Pac-Man y los fantasmas mediante:

```cpp
sqrt(dx * dx + dy * dy)
```

Dependiendo del estado del juego:

- si el modo poder está desactivado → Game Over,
- si el modo poder está activo → el fantasma es comido.

---

# Instalación

## Descargar FreeGLUT

https://sourceforge.net/projects/freeglut/

---

# Compilación

```bash
g++ main.cpp -o pacman -I"C:\freeglut\mingw64\include" -L"C:\freeglut\mingw64\lib" -lfreeglut -lopengl32 -lglu32
```

---

# Ejecución

```bash
pacman
```

---

# Controles

| Tecla | Acción |
|---|---|
| Flechas | mover |
| R | reiniciar |

---

# Capturas Recomendadas

Agregar capturas de:

- código fuente,
- compilación,
- ejecución del programa,
- gameplay,
- modo poder,
- pantalla de Game Over.

---

# Problemas Encontrados

Uno de los principales retos fue sincronizar la lógica de la matriz con la representación visual del laberinto para evitar que Pac-Man atravesara paredes o se desalineara gráficamente.

También fue necesario ajustar la velocidad de los fantasmas para mantener una dificultad equilibrada.

---

# Conclusión

Este proyecto permitió aplicar conceptos fundamentales de graficación computacional mediante OpenGL, utilizando:

- primitivas gráficas,
- matrices bidimensionales,
- animación,
- renderizado 2D,
- control de objetos,
- colisiones,
- actualización en tiempo real.

El resultado fue un entorno gráfico interactivo inspirado en el videojuego clásico Pac-Man.
