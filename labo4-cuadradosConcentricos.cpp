#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>

// -------------------------------------------------------
// Parámetros del ejercicio
// -------------------------------------------------------
#define NUM_CUADRADOS  32       // cantidad de cuadrados concéntricos
#define ANGULO_PASO    8.0f     // grados de rotación incremental entre cuadrados
#define TAMANO_BASE    2.4f     // lado del cuadrado más externo
#define FACTOR_ESCALA  0.9f    // reducción de tamaño entre cuadrados consecutivos
// Profundidad: cada cuadrado se desplaza un poco en Z para efecto 3D
#define DELTA_Z        0.04f

// Velocidad de rotación global (sentido antihorario → incremento negativo)
static GLfloat velocidad = -0.9f;   // grados por frame

// Ángulos de rotación acumulados en los 3 ejes
static GLfloat theta[] = {0.0f, 0.0f, 0.0f};
// Eje activo para la rotación con teclado
static GLint eje = 2;


// Dibuja un cuadrado centrado en el origen en el plano XY
// con semilado = 'lado/2', desplazado en Z = 'z'
void dibujarCuadrado(float lado, float z)
{
    float h = lado * 0.5f;
    glBegin(GL_LINE_LOOP);
        glVertex3f( h,  h, z);
        glVertex3f(-h,  h, z);
        glVertex3f(-h, -h, z);
        glVertex3f( h, -h, z);
    glEnd();
}

// Dibuja todos los cuadrados concéntricos con rotación
// incremental y desplazamiento en Z (efecto 3D)
void dibujarEspiral(void)
{
    float lado    = TAMANO_BASE;
    float angulo  = 0.0f;
    float z       = 0.0f;
    

    for (int i = 0; i < NUM_CUADRADOS; ++i)
    {
        // Color degradado del rojo hacia el naranja/amarillo según profundidad
        float t = (float)i / (NUM_CUADRADOS - 1);   // 0..1
        glColor3f(1.0f, t * 0.6f, t * 0.2f);

        glPushMatrix();
            // Rotación individual acumulada por índice (giro antihorario en XY)
            glRotatef(angulo, 0.0f, 0.0f, 1.0f);
            // Ligera inclinación en X para apreciar el efecto 3D
            glRotatef(t * 15.0f, 1.0f, 0.0f, 0.0f);
            dibujarCuadrado(lado, 0);
        glPopMatrix();

        // Preparar siguiente cuadrado
        angulo += ANGULO_PASO;      // rotación incremental
        if(i%2 == 0) lado   *= FACTOR_ESCALA;    // reducir tamaño
        z      += DELTA_Z;          // desplazar en profundidad
    }
}


void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Rotación global del conjunto (controlada por teclado y animación)
    glRotatef(theta[0], 1.0f, 0.0f, 0.0f);
    glRotatef(theta[1], 0.0f, 1.0f, 0.0f);
    glRotatef(theta[2], 0.0f, 0.0f, 1.0f);

    glLineWidth(1.5f);
    dibujarEspiral();

    glutSwapBuffers();
}


// Animación: gira en sentido antihorario
void girar_objeto_geometrico(void)
{
    theta[eje] += velocidad;
    if (theta[eje] >  360.0f) theta[eje] -= 360.0f;
    if (theta[eje] < -360.0f) theta[eje] += 360.0f;
    display();
}

// Teclado
//   a/s/d  → selecciona eje X/Y/Z
//   f      → salir
//   +/-    → aumentar/disminuir velocidad
//   r      → reinicia ángulos
void teclado(unsigned char tecla, int x, int y)
{
    switch (tecla)
    {
        case 'a': eje = 0; break;
        case 's': eje = 1; break;
        case 'd': eje = 2; break;
        case '+': velocidad -= 0.1f; break;   // más rápido antihorario
        case '-': velocidad += 0.1f; break;   // más lento
        case 'r': theta[0] = theta[1] = theta[2] = 0.0f; break;
        case 'f': exit(0); break;
    }
}


void myReshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if (w <= h)
        glOrtho(-2.5, 2.5,
                -2.5 * (GLfloat)h / (GLfloat)w,
                 2.5 * (GLfloat)h / (GLfloat)w,
                -10.0, 10.0);
    else
        glOrtho(-2.5 * (GLfloat)w / (GLfloat)h,
                 2.5 * (GLfloat)w / (GLfloat)h,
                -2.5, 2.5,
                -10.0, 10.0);

    glMatrixMode(GL_MODELVIEW);
}


// Main
int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    glutCreateWindow("cuadrados concentricos");

    // Fondo negro
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    glutReshapeFunc(myReshape);
    glutDisplayFunc(display);
    glutIdleFunc(girar_objeto_geometrico);
    glutKeyboardFunc(teclado);

    glutMainLoop();
    return 0;
}