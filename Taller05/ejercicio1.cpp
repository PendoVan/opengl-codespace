#include <stdio.h>
#include <math.h>
#include <GL/glut.h>

// ── Parámetros de la estructura ─────────────────────────────────────────────
float A = 2.0f;
float grosor = 2.5f; // Mayor a un píxel como pide el enunciado

// ── Variables de la Cámara ──────────────────────────────────────────────────
float camX  = 0.0f, camY  = 5.0f, camZ  = 20.0f;   // Posición
float dirX  = 0.0f, dirY  = 0.0f, dirZ  = -1.0f;   // Vector Dirección (Hacia dónde mira)
float upX   = 0.0f, upY   = 1.0f, upZ   = 0.0f;    // Vector Arriba (Inclinación)

float angH  = 0.0f;   // Ángulo horizontal (Yaw)
float angV  = 0.0f;   // Ángulo vertical  (Pitch)

float paso  = 0.5f;   // Velocidad de traslación
float giro  = 0.05f;  // Velocidad de rotación

// ── Recalcula los vectores de la cámara desde los ángulos ───────────────────
void actualizarCamara() {
    // Vector de Dirección (Look)
    dirX = cos(angV) * sin(angH);
    dirY = sin(angV);
    dirZ = -cos(angV) * cos(angH);

    // Vector Arriba (Up) - Es perpendicular a la dirección
    // Se calcula sumando 90 grados (PI/2) al ángulo vertical
    upX = -sin(angV) * sin(angH);
    upY = cos(angV);
    upZ = sin(angV) * cos(angH);
}

// ── Dibuja la estructura solicitada ─────────────────────────────────────────
void dibujaEstructura() {
    glLineWidth(grosor);

    // 1. Dibujar los ejes de referencia centrales (como en la imagen del PDF)
    glBegin(GL_LINES);
        glColor3f(1.0f, 0.0f, 0.0f); // Rojo
        glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(A, 0.0f, 0.0f); // X
        glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, A, 0.0f); // Y
        glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 0.0f, A); // Z
    glEnd();

    // Color azul para la estructura
    glColor3f(0.0f, 0.0f, 1.0f);

    float altura = A * 1.5f;       // Altura del hexágono central
    int lados = 6;                 // Hexágono
    float radio = A * 2.0f;        // Separación desde el centro

    float vx[6], vz[6];
    for (int i = 0; i < lados; i++) {
        float angulo = i * (2.0f * M_PI / lados);
        vx[i] = radio * cos(angulo);
        vz[i] = radio * sin(angulo);
    }

    // Dibujar las líneas del hexágono (Base)
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < lados; i++) glVertex3f(vx[i], -altura/2, vz[i]);
    glEnd();

    // Dibujar las líneas del hexágono (Tapa)
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < lados; i++) glVertex3f(vx[i], altura/2, vz[i]);
    glEnd();

    // Dibujar aristas verticales
    glBegin(GL_LINES);
    for (int i = 0; i < lados; i++) {
        glVertex3f(vx[i], -altura/2, vz[i]);
        glVertex3f(vx[i], altura/2, vz[i]);
    }
    glEnd();

    // Dibujar los cubos usando la primitiva glutWireCube en cada vértice
    for (int i = 0; i < lados; i++) {
        float anguloGrados = i * (360.0f / lados);

        // Cubo en la base
        glPushMatrix();
            glTranslatef(vx[i], -altura/2, vz[i]);
            glRotatef(-anguloGrados, 0, 1, 0); // Rotar para que miren al centro
            glutWireCube(A);
        glPopMatrix();

        // Cubo en la tapa
        glPushMatrix();
            glTranslatef(vx[i], altura/2, vz[i]);
            glRotatef(-anguloGrados, 0, 1, 0); 
            glutWireCube(A);
        glPopMatrix();
    }
}

// ── Display ─────────────────────────────────────────────────────────────────
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Se configura la cámara con los vectores calculados
    gluLookAt(
        camX, camY, camZ,                     // Ojo
        camX + dirX, camY + dirY, camZ + dirZ, // Centro/Target
        upX, upY, upZ                         // Vector Up
    );

    dibujaEstructura();

    glutSwapBuffers();
}

// ── Reshape ─────────────────────────────────────────────────────────────────
void reshape(int w, int h) {
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (float)w / h, 0.1, 200.0);
    glMatrixMode(GL_MODELVIEW);
}

// ── Teclado (Control Total de Cámara) ───────────────────────────────────────
void teclado(unsigned char tecla, int x, int y) {
    switch (tecla) {
        // Desplazamientos en la dirección en la que se mira (Adelante/Atrás)
        case 'i': camX += paso*dirX; camY += paso*dirY; camZ += paso*dirZ; break;
        case 'm': camX -= paso*dirX; camY -= paso*dirY; camZ -= paso*dirZ; break;
        
        // Desplazamientos en la dirección "Arriba/Abajo" relativas a la cámara
        case 'q': camX += paso*upX; camY += paso*upY; camZ += paso*upZ; break;
        case 'e': camX -= paso*upX; camY -= paso*upY; camZ -= paso*upZ; break;

        // Rotación horizontal (Izquierda/Derecha)
        case 'j': angH -= giro; actualizarCamara(); break;
        case 'k': angH += giro; actualizarCamara(); break;

        // Rotación vertical (Arriba/Abajo)
        case 'u': 
            angV += giro; 
            if(angV > 1.5f) angV = 1.5f; // Limitar para no dar la vuelta completa
            actualizarCamara(); 
            break;
        case 'n': 
            angV -= giro; 
            if(angV < -1.5f) angV = -1.5f; 
            actualizarCamara(); 
            break;

        case 27: exit(0); // ESC
    }
    glutPostRedisplay();
}

// ── Main ────────────────────────────────────────────────────────────────────
int main(int argc, char** argv) {
    printf("Ingrese el valor de A (Dimension base): ");
    if(scanf("%f", &A) != 1) { A = 2.0f; } // Valor por defecto si falla el input

    actualizarCamara();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Taller 05 - Ejercicio 01");

    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    glEnable(GL_DEPTH_TEST);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(teclado);

    printf("\n--- Controles de Camara ---\n");
    printf("  i / m : Avanzar / Retroceder (Eje Z local)\n");
    printf("  j / k : Rotar Izquierda / Derecha (Yaw)\n");
    printf("  u / n : Mirar Arriba / Abajo (Pitch)\n");
    printf("  q / e : Desplazar Arriba / Abajo (Eje Y local)\n");
    printf("  ESC   : Salir\n");

    glutMainLoop();
    return 0;
}