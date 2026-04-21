#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>

// ============================================================
//    Círculo exterior       : radio R   = 6.0
//    Semicírculos medianos  : radio R/2 = 3.0
//    Círculos pequeños      : radio R/6 = 1.0
//
//  La curva en S que divide el símbolo se construye con
//  tres semicírculos apilados verticalmente:
//    - Semicírculo superior (radio R/2, centro arriba): arco blanco
//    - Semicírculo inferior (radio R/2, centro abajo) : arco negro
//  Esto crea la curva característica en "S".
// ============================================================

const int   SEGMENTS    = 360;
const float R           = 6.0f;     // radio del círculo grande
const float R2          = R / 2.0f; // radio de los semicírculos medianos
const float R6          = R / 6.0f; // radio de los círculos pequeños

const GLfloat ORTHO_LEFT   = -10.0f;
const GLfloat ORTHO_RIGHT  =  10.0f;
const GLfloat ORTHO_BOTTOM = -10.0f;
const GLfloat ORTHO_TOP    =  10.0f;

// ============================================================
//  Prototipos
// ============================================================
void init(void);
void display(void);
void reshape(int, int);

void drawFilledCircle(float cx, float cy, float r, float red, float green, float blue);
void drawFilledSemiCircle(float cx, float cy, float r,
                          float angStart, float angEnd,
                          float red, float green, float blue);
void drawYinYang(float cx, float cy);

// ============================================================
//  main
// ============================================================
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(700, 700);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Ejercicio 02 - Yin Yang");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMainLoop();
    return 0;
}

void init(void)
{
    glClearColor(0.15f, 0.15f, 0.20f, 0.0f);
    glShadeModel(GL_FLAT);
}

// ============================================================
//  Dibuja un círculo sólido completo
// ============================================================
void drawFilledCircle(float cx, float cy, float r,
                      float red, float green, float blue)
{
    glColor3f(red, green, blue);
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(cx, cy);  // centro
        for (int i = 0; i <= SEGMENTS; ++i)
        {
            float ang = 2.0f * M_PI * i / SEGMENTS;
            glVertex2f(cx + r * cosf(ang), cy + r * sinf(ang));
        }
    glEnd();
}

// ============================================================
//  Dibuja un semicírculo (sector) sólido
//  angStart y angEnd en radianes, sentido antihorario
// ============================================================
void drawFilledSemiCircle(float cx, float cy, float r,
                          float angStart, float angEnd,
                          float red, float green, float blue)
{
    glColor3f(red, green, blue);
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(cx, cy);  // centro del sector
        int steps = (int)(SEGMENTS * fabsf(angEnd - angStart) / (2.0f * M_PI));
        if (steps < 2) steps = 2;
        for (int i = 0; i <= steps; ++i)
        {
            float ang = angStart + (angEnd - angStart) * i / steps;
            glVertex2f(cx + r * cosf(ang), cy + r * sinf(ang));
        }
    glEnd();
}

// ============================================================
//  Construye el símbolo Yin-Yang centrado en (cx, cy)
//
//  Estrategia de pintado (pintor, de atrás hacia adelante):
//
//  1. Mitad izquierda negra  (semicírculo grande, PI/2 a 3PI/2)
//  2. Mitad derecha blanca   (semicírculo grande, -PI/2 a PI/2)
//  3. Semicírculo mediano blanco arriba (centro en cy+R2, arco derecho)
//  4. Semicírculo mediano negro  abajo  (centro en cy-R2, arco izquierdo)
//  5. Punto blanco sobre el negro  (en cy+R2, radio R6)
//  6. Punto negro sobre el blanco  (en cy-R2, radio R6)
//  7. Borde exterior negro
// ============================================================
void drawYinYang(float cx, float cy)
{
    const float WHITE_R = 1.0f, WHITE_G = 1.0f, WHITE_B = 1.0f;
    const float BLACK_R = 1.0f, BLACK_G = 0.64f, BLACK_B = 0.01f;

    // ----------------------------------------------------------
    // 1. Mitad izquierda: negra (arco de PI/2 a 3*PI/2 = 270°)
    // ----------------------------------------------------------
    drawFilledSemiCircle(cx, cy, R,
                         (float)M_PI / 2.0f,
                         3.0f * (float)M_PI / 2.0f,
                         WHITE_R, WHITE_G, WHITE_B);

    // ----------------------------------------------------------
    // 2. Mitad derecha: blanca (arco de -PI/2 a PI/2)
    // ----------------------------------------------------------
    drawFilledSemiCircle(cx, cy, R,
                         -(float)M_PI / 2.0f,
                         (float)M_PI / 2.0f,
                         BLACK_R, BLACK_G, BLACK_B);

    // ----------------------------------------------------------
    // 3. Semicírculo mediano BLANCO en la zona superior
    //    Centro desplazado hacia arriba en R2 (= R/2)
    //    Dibuja el arco del lado DERECHO → suma área blanca al negro
    // ----------------------------------------------------------
    drawFilledSemiCircle(cx, cy + R2, R2,
                         -(float)M_PI / 2.0f,
                         (float)M_PI / 2.0f,
                         WHITE_R, WHITE_G, WHITE_B);

    // ----------------------------------------------------------
    // 4. Semicírculo mediano NEGRO en la zona inferior
    //    Centro desplazado hacia abajo en R2
    //    Dibuja el arco del lado IZQUIERDO → suma área negra al blanco
    // ----------------------------------------------------------
    drawFilledSemiCircle(cx, cy - R2, R2,
                         (float)M_PI / 2.0f,
                         3.0f * (float)M_PI / 2.0f,
                         BLACK_R, BLACK_G, BLACK_B);

    // ----------------------------------------------------------
    // 5. Punto BLANCO centrado en la zona negra superior
    // ----------------------------------------------------------
    drawFilledCircle(cx, cy + R2, R6, BLACK_R, BLACK_G, BLACK_B);

    // ----------------------------------------------------------
    // 6. Punto NEGRO centrado en la zona blanca inferior
    // ----------------------------------------------------------
    drawFilledCircle(cx, cy - R2, R6, WHITE_R, WHITE_G, WHITE_B);

    // ----------------------------------------------------------
    // 7. Borde exterior negro (LINE_LOOP)
    // ----------------------------------------------------------
    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < SEGMENTS; ++i)
    {
        float ang = 2.0f * M_PI * i / SEGMENTS;
        glVertex2f(cx + R * cosf(ang), cy + R * sinf(ang));
    }
    glEnd();
}

// ============================================================
//  display
// ============================================================
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    drawYinYang(0.0f, 0.0f);   // centrado en el origen

    glFlush();
}

// ============================================================
//  reshape
// ============================================================
void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(ORTHO_LEFT, ORTHO_RIGHT, ORTHO_BOTTOM, ORTHO_TOP, -20.0, 20.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}