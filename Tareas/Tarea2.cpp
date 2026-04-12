#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>

enum DrawType { DRAW_FILL, DRAW_STROKE, DRAW_DASHED };

const GLfloat ORTHO_LEFT   = -30.0f;
const GLfloat ORTHO_RIGHT  =  30.0f;
const GLfloat ORTHO_BOTTOM = -30.0f;
const GLfloat ORTHO_TOP    =  30.0f;

void init(void);
void display(void);
void reshape(int,int);
void computeRegularPolygonVertex(int index, int count, GLfloat radius, GLfloat centerX, GLfloat centerY, GLfloat &x, GLfloat &y);
void configureDrawMode(DrawType mode);
void drawShape(int points, GLfloat radius, GLfloat centerX, GLfloat centerY, DrawType mode, GLfloat red, GLfloat green, GLfloat blue);
void drawAxes(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top);

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
    glutInitWindowSize(750,750);
    glutInitWindowPosition(100,100);
    glutCreateWindow(argv[0]);
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMainLoop();
    return 0;
}

void init(void)
{
    glClearColor(0.0,0.0,0.0,0.0);
    glShadeModel(GL_FLAT);
}

void computeRegularPolygonVertex(int index, int count, GLfloat radius, GLfloat centerX, GLfloat centerY, GLfloat &x, GLfloat &y)
{
    GLfloat ang = 2.0f * M_PI * index / count;
    x = centerX + radius * cos(ang);
    y = centerY + radius * sin(ang);
}

void configureDrawMode(DrawType mode)
{
    glLineWidth(2.0f);
    if (mode == DRAW_DASHED)
    {
        glEnable(GL_LINE_STIPPLE);
        glLineStipple(1, 0x00FF);
    }
    else
    {
        glDisable(GL_LINE_STIPPLE);
    }
}

void drawShape(int points, GLfloat radius, GLfloat centerX, GLfloat centerY, DrawType mode, GLfloat red, GLfloat green, GLfloat blue)
{
    glPushMatrix();
    glColor3f(red, green, blue);
    configureDrawMode(mode);

    if (mode == DRAW_FILL)
    {
        glBegin(GL_POLYGON);
    }
    else
    {
        glBegin(GL_LINE_LOOP);
    }

    for (int i = 0; i < points; ++i)
    {
        GLfloat x, y;
        computeRegularPolygonVertex(i, points, radius, centerX, centerY, x, y);
        glVertex2f(x, y);
    }

    glEnd();
    if (mode == DRAW_DASHED)
    {
        glDisable(GL_LINE_STIPPLE);
    }
    glPopMatrix();
}

void drawAxes(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top)
{
    glPushMatrix();
    glLineWidth(1.0f);
    glDisable(GL_LINE_STIPPLE);
    glColor3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_LINES);
    glVertex2f(left, 0.0f);
    glVertex2f(right, 0.0f);
    glVertex2f(0.0f, bottom);
    glVertex2f(0.0f, top);
    glEnd();
    glPopMatrix();
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    drawAxes(ORTHO_LEFT, ORTHO_RIGHT, ORTHO_BOTTOM, ORTHO_TOP);

    //drawShape(int points, GLfloat radius, GLfloat centerX, GLfloat centerY, DrawType mode, GLfloat red, GLfloat green, GLfloat blue)
    // DrawType: DRAW_FILL, DRAW_STROKE, DRAW_DASHED
    drawShape(100, 4.0f, 0.0f, 0.0f, DRAW_DASHED, 0.0f, 0.7f, 0.2f);
    drawShape(100, 7.0f, 0.0f, 0.0f, DRAW_DASHED, 1.0f, 0.6f, 0.0f);

    //-----------------------------------------------------------------
    glPushMatrix();
    glColor3f(1.0f, 0.0f, 0.0f);
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 0x00FF);
    glLineWidth(3.0f);
    glBegin(GL_LINE_LOOP);
    int vertices = 16;
    for (int i = 0; i < vertices; ++i)
    {
        GLfloat x, y;
        GLfloat ang = 2.0f * M_PI * i / vertices;
        GLfloat radius = (i % 2 == 0) ? 7.0f : 4.0f;
        x = radius * cos(ang);
        y = radius * sin(ang);
        glVertex2f(x, y);
    }

    glEnd();
    glDisable(GL_LINE_STIPPLE);
    glPopMatrix();
    //-----------------------------------------------------------------
    glFlush();
}

void reshape(int w, int h)
{
    glViewport(0,0,(GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(ORTHO_LEFT, ORTHO_RIGHT, ORTHO_BOTTOM, ORTHO_TOP, -10.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
