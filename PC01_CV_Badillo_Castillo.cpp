/*
    PC 01 Computación Visual
    Alumno : Badillo Castillo, Edwin Piero
    Codigo : 24200150
    Fecha  : 28/04/26
    Hora   : 12:00
 */

#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>

float R = 200.0f;   // Radio de la circunferencia exterior (en pixeles)
float r = 130.0f;   // Radio de la circunferencia interior (en pixeles)
int   k = 8;
int   N = 8*k;       // Numero de vertices del engranaje: N = 8k  (k=2 → 4 dientes)
const int WIN_W = 800;
const int WIN_H = 600;

const int CX = WIN_W / 2;
const int CY = WIN_H / 2;

void init(void);
void display(void);
void reshape(int w, int h);

void dibujarPixel(int x, int y);
void lineaPuntoMedio(int x0, int y0, int x1, int y1);
void circuloPuntoMedio(int cx, int cy, int radio);

void dibujarEngranaje(void);
void dibujarPoliDentada(void);

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WIN_W, WIN_H);
    glutInitWindowPosition(100, 80);
    glutCreateWindow("24200150 - Evaluacion 01");

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMainLoop();
    return 0;
}

void init(void)
{
    /* Fondo gris tenue*/
    glClearColor(0.82f, 0.82f, 0.82f, 1.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    glOrtho(0.0, (double)WIN_W, 0.0, (double)WIN_H, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    dibujarEngranaje();
    dibujarPoliDentada();
    glFlush();
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

void dibujarPixel(int x, int y)
{
    glBegin(GL_POINTS);
        glVertex2i(x, y);
    glEnd();
}

void lineaPuntoMedio(int x0, int y0, int x1, int y1)
{
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);

    /* Direcciones de avance en cada eje */
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;

    int x = x0, y = y0;

    if (dx >= dy)
    {
        /* Pendiente |m| <= 1: avanzamos en X */
        int d = 2 * dy - dx;

        for (int i = 0; i <= dx; i++)
        {
            dibujarPixel(x, y);

            if (d > 0)
            {
                y += sy;
                d += 2 * (dy - dx);
            }
            else
            {
                d += 2 * dy;
            }
            x += sx;
        }
    }
    else
    {
        /* Pendiente |m| > 1: avanzamos en Y */
        int d = 2 * dx - dy;

        for (int i = 0; i <= dy; i++)
        {
            dibujarPixel(x, y);

            if (d > 0)
            {
                x += sx;
                d += 2 * (dx - dy);
            }
            else
            {
                d += 2 * dx;
            }
            y += sy;
        }
    }
}

void circuloPuntoMedio(int cx, int cy, int radio)
{
    int x = 0;
    int y = radio;
    int d = 1 - radio;   /* decision inicial */

    /* Funcion auxiliar inline: dibuja los 8 puntos simetricos */
    auto plot8 = [&](int px, int py) {
        dibujarPixel(cx + px, cy + py);
        dibujarPixel(cx - px, cy + py);
        dibujarPixel(cx + px, cy - py);
        dibujarPixel(cx - px, cy - py);
        dibujarPixel(cx + py, cy + px);
        dibujarPixel(cx - py, cy + px);
        dibujarPixel(cx + py, cy - px);
        dibujarPixel(cx - py, cy - px);
    };

    plot8(x, y);

    while (x < y)
    {
        if (d < 0)
        {
            /* Punto medio dentro del circulo: avanzar solo en X */
            d += 2 * x + 3;
        }
        else
        {
            /* Punto medio fuera o sobre el circulo: avanzar en X e Y */
            d += 2 * (x - y) + 5;
            y--;
        }
        x++;
        plot8(x, y);
    }
}

void dibujarEngranaje(void)
{
    int num_dientes = N / 4;          /* N=8k, 4 vertices/diente → k*2 dientes */
    float paso = 2.0f * M_PI / num_dientes;

    int total_v = num_dientes * 4;
    int *vx = new int[total_v];
    int *vy = new int[total_v];

    for (int i = 0; i < num_dientes; i++)
    {
        float ang_base = i * paso;
        float ang_tope = ang_base + paso * 0.5f;  // mitad del paso = ancho del diente

        int base = i * 4;

        /* V0: base izquierda del diente (sobre r) */
        vx[base + 0] = CX + (int)(r * cos(ang_base));
        vy[base + 0] = CY + (int)(r * sin(ang_base));

        /* V1: tope izquierdo del diente (sobre R, mismo angulo) */
        vx[base + 1] = CX + (int)(R * cos(ang_base));
        vy[base + 1] = CY + (int)(R * sin(ang_base));

        /* V2: tope derecho del diente (sobre R, ang_tope) */
        vx[base + 2] = CX + (int)(R * cos(ang_tope));
        vy[base + 2] = CY + (int)(R * sin(ang_tope));

        /* V3: base derecha del diente (sobre r, ang_tope) */
        vx[base + 3] = CX + (int)(r * cos(ang_tope));
        vy[base + 3] = CY + (int)(r * sin(ang_tope));
    }

    /* Trazar cada segmento con el algoritmo de punto medio */
    glColor3f(0.85f, 0.15f, 0.0f); // Color rojo para los engranajes
    glPointSize(3.0f);  // Grosor más grueso

    for (int i = 0; i < total_v; i++)
    {
        int siguiente = (i + 1) % total_v;
        lineaPuntoMedio(vx[i], vy[i], vx[siguiente], vy[siguiente]);
    }

    delete[] vx;
    delete[] vy;
}

void dibujarPoliDentada(void)
{
    /* Para N vértices, el paso angular es una vuelta completa dividida entre N */
    float paso = 2.0f * M_PI / N;

    int *vx = new int[N];
    int *vy = new int[N];

    /* Calculamos las coordenadas de los vértices*/
    for (int i = 0; i < N; i++)
    {
        float angulo = i * paso;
        
        // Alternar el radio: los índices pares van al radio R, los impares al radio r
        float radio_actual = (i % 2 == 0) ? R : r;

        vx[i] = CX + (int)(radio_actual * cos(angulo));
        vy[i] = CY + (int)(radio_actual * sin(angulo));
    }

    /* Aplicando colores y grosores diferenciados con respecto al engranaje*/
    glColor3f(0.0f, 0.5f, 0.0f); // Color verde opaco para los dientes
    glPointSize(1.0f);             // Grosor más delgado

    /* Trazar cada segmento conectando los N vértices de forma cerrada */
    for (int i = 0; i < N; i++)
    {
        int siguiente = (i + 1) % N;
        lineaPuntoMedio(vx[i], vy[i], vx[siguiente], vy[siguiente]);
    }

    delete[] vx;
    delete[] vy;
}