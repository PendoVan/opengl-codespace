#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>


enum DrawType { DRAW_FILL, DRAW_STROKE, DRAW_DASHED };

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
void drawAxes(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top);
void drawPolarGrid(int circleCount, int radialCount, GLfloat maxRadius);
void drawPolarCurve(GLfloat (*f)(GLfloat), GLfloat thetaMin, GLfloat thetaMax,
                    int steps, GLfloat red, GLfloat green, GLfloat blue);

// ============================================================
//  Funciones polares  r = f(θ)
// ============================================================

// Caracol de Pascal: r = 2 + 4·sen(θ)
GLfloat caracol(GLfloat theta) {
    return 2.0f + 4.0f * sin(theta);
}

// Rosal de 4 pétalos: r = 3·cos(2θ)
GLfloat rosal(GLfloat theta) {
    return 3.0f * cos(2.0f * theta);
}

// Cardioide: r = 3 - 3·sen(θ)
GLfloat cardioide(GLfloat theta) {
    return 3.0f - 3.0f * sin(theta);
}

// Espiral de Fermat: r = 2θ
GLfloat espiral(GLfloat theta) {
    return sqrt(4.0f) * theta;
}

// ============================================================
//  main
// ============================================================
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Ejercicio 01 - Curvas Polares");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMainLoop();
    return 0;
}

// ============================================================
//  Inicialización
// ============================================================
void init(void)
{
    glClearColor(0.08f, 0.08f, 0.15f, 0.0f);   // fondo azul oscuro
    glShadeModel(GL_FLAT);
}

// ============================================================
//  Ejes cartesianos
// ============================================================
void drawAxes(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top)
{
    glPushMatrix();
    glLineWidth(1.2f);
    glDisable(GL_LINE_STIPPLE);
    glColor3f(0.5f, 0.5f, 0.6f);   // gris azulado
    glBegin(GL_LINES);
        glVertex2f(left,  0.0f);
        glVertex2f(right, 0.0f);
        glVertex2f(0.0f, bottom);
        glVertex2f(0.0f, top);
    glEnd();
    glPopMatrix();
}

// ============================================================
//  Grilla polar: círculos concéntricos + líneas radiales
//  circleCount  : cantidad de círculos (cada uno a radio = k)
//  radialCount  : cantidad de líneas radiales
//  maxRadius    : radio del círculo más grande
// ============================================================
void drawPolarGrid(int circleCount, int radialCount, GLfloat maxRadius)
{
    const int SEGMENTS = 360;

    glPushMatrix();
    glLineWidth(0.5f);
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 0xAAAA);           // línea punteada fina
    glColor3f(0.28f, 0.28f, 0.45f);    // violeta tenue

    // --- Círculos concéntricos ---
    for (int c = 1; c <= circleCount; ++c)
    {
        GLfloat r = maxRadius * c / circleCount;
        glBegin(GL_LINE_LOOP);
        for (int s = 0; s < SEGMENTS; ++s)
        {
            GLfloat ang = 2.0f * M_PI * s / SEGMENTS;
            glVertex2f(r * cos(ang), r * sin(ang));
        }
        glEnd();
    }

    // --- Líneas radiales ---
    for (int k = 0; k < radialCount; ++k)
    {
        GLfloat ang = M_PI * k / radialCount;   // semiángulo (opuesto incluido)
        glBegin(GL_LINES);
            glVertex2f( maxRadius * cos(ang),  maxRadius * sin(ang));
            glVertex2f(-maxRadius * cos(ang), -maxRadius * sin(ang));
        glEnd();
    }

    glDisable(GL_LINE_STIPPLE);
    glPopMatrix();
}

// ============================================================
//  Dibujado genérico de una curva polar
//  f         : puntero a función  r = f(θ)
//  thetaMin  : ángulo inicial
//  thetaMax  : ángulo final
//  steps     : resolución (más alto = más suave)
// ============================================================
void drawPolarCurve(GLfloat (*f)(GLfloat), GLfloat thetaMin, GLfloat thetaMax,
                    int steps, GLfloat red, GLfloat green, GLfloat blue)
{
    glColor3f(red, green, blue);
    glLineWidth(2.0f);
    glDisable(GL_LINE_STIPPLE);
    glBegin(GL_LINE_STRIP);

    for (int i = 0; i <= steps; ++i)
    {
        GLfloat theta = thetaMin + (thetaMax - thetaMin) * (GLfloat)i / steps;
        GLfloat r     = f(theta);
        // r puede ser negativo en el rosal; OpenGL lo maneja bien al convertir
        glVertex2f(r * cos(theta), r * sin(theta));
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

    // --- Grilla polar de fondo ---
    // 8 círculos, 12 líneas radiales, radio máximo = 8
    drawPolarGrid(8, 12, 8.0f);

    // --- Ejes cartesianos ---
    drawAxes(ORTHO_LEFT, ORTHO_RIGHT, ORTHO_BOTTOM, ORTHO_TOP);

    // --- Curva 1: Caracol  r = 2 + 4·sen(θ) ---
    // Una vuelta completa es suficiente
    /*drawPolarCurve(caracol, 0.0f, 2.0f * M_PI, 1000,
                   1.0f, 0.25f, 0.25f);      // rojo

    
    // --- Curva 2: Rosal  r = 3·cos(2θ) ---
    // Con cos(2θ) se necesita una vuelta completa para los 4 pétalos
    drawPolarCurve(rosal, 0.0f, 2.0f * M_PI, 1000,
                   0.25f, 0.85f, 0.35f);     // verde

    // --- Curva 3: Cardioide  r = 3 - 3·sen(θ) ---
    drawPolarCurve(cardioide, 0.0f, 2.0f * M_PI, 1000,
                   0.30f, 0.55f, 1.0f);      // azul*/

    // --- Curva 4: Espiral de Fermat  r = √θ ---
    // 3 vueltas (0 a 6π) para que la espiral sea visible
    drawPolarCurve(espiral, 0.0f, 6.0f * M_PI, 2000,
                   1.0f, 0.85f, 0.10f);      // amarillo

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