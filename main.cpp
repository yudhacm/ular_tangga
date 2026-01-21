#include <windows.h>
#ifdef _APPLE_
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <math.h>
#include <string>

using namespace std;

int mode = 1;
bool menang = false;
bool diff = true, spec = true, amb = true;
int ganjil = 1, genap = 0;
int kotak = 2;
int turn = 1;
int dx1 = 0, dy1 = 0;
int dx2 = 0, dy2 = 0;
float dx_ob = 0.0, dy_ob = 1;
string arah_observer = "atas";
int mode_ob = 1;
float spin_observer[] = {0, 0, 0};
float spin_ob_save[] = {0, 0, 0};
float spin[] = {0, 0, 0};
float spin_save[] = {-25, 0, 0};
float pos[3] = {1, 1, 0.05};
float viewdir[3] = {1, 0, 0};
double alpha = 0;
float observer_speed = 0.2f; // Nilai default lebih kecil = lebih lambat

float *vektor_normal(float x1, float y1, float z1,
                      float x2, float y2, float z2,
                      float x4, float y4, float z4)
{
    /*
    x1-z1 == pusat
    x2-z2 == arah v1
    x4-z4 == arah v2
    */

    float v1[] = {(x2 - x1), (y2 - y1), (z2 - z1)};
    float v2[] = {(x4 - x1), (y4 - y1), (z4 - z1)};

    float vx = v1[1] * v2[2] - v1[2] * v2[1];
    float vy = v1[2] * v2[0] - v1[0] * v2[2];
    float vz = v1[0] * v2[1] - v1[1] * v2[0];

    float bv = pow((vx * vx + vy * vy + vz * vz), 0.5);
    static float v_normal[3];
    v_normal[0] = (vx / bv);
    v_normal[1] = (vy / bv);
    v_normal[2] = (vz / bv);
    return v_normal;
}
void material(float r, float g, float b, float a)
{
    GLfloat mat_specular[] = {r, g, b, a};
    GLfloat mat_shininess[] = {100};
    GLfloat mat_diffuse[] = {r, g, b, a};
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
}


void box(float x1, float y1, float z1,
         float x2, float y2, float z2)
{
    //kiri bawah belakang ----- kanan atas depan
    float *vn = vektor_normal(x1, y1, z1,
                              x1, y2, z1,
                              x2, y1, z1);
    glBegin(GL_POLYGON);
    //belakang
    // glColor3f(1,0,0);
    glNormal3f(vn[0], vn[1], vn[2]);
    glVertex3f(x1, y1, z1);
    glVertex3f(x1, y2, z1);
    glVertex3f(x2, y2, z1);
    glVertex3f(x2, y1, z1);
    glEnd();
    vn = vektor_normal(x1, y1, z2,
                       x2, y1, z2,
                       x1, y2, z2);
    glBegin(GL_POLYGON);
    //depan
    // glColor3f(1,1,0);
    glNormal3f(vn[0], vn[1], vn[2]);
    glVertex3f(x1, y1, z2);
    glVertex3f(x2, y1, z2);
    glVertex3f(x2, y2, z2);
    glVertex3f(x1, y2, z2);
    glEnd();
    vn = vektor_normal(x1, y1, z1,
                       x2, y1, z1,
                       x1, y1, z2);
    glBegin(GL_POLYGON);
    //bottom
    // glColor3f(1,0,1);
    glNormal3f(vn[0], vn[1], vn[2]);
    glVertex3f(x1, y1, z1);
    glVertex3f(x2, y1, z1);
    glVertex3f(x2, y1, z2);
    glVertex3f(x1, y1, z2);
    glEnd();
    vn = vektor_normal(x1, y2, z1,
                       x1, y2, z2,
                       x2, y2, z1);
    glBegin(GL_POLYGON);
    //top
    // glColor3f(0,1,0);
    glNormal3f(vn[0], vn[1], vn[2]);
    glVertex3f(x1, y2, z1);
    glVertex3f(x1, y2, z2);
    glVertex3f(x2, y2, z2);
    glVertex3f(x2, y2, z1);
    glEnd();
    vn = vektor_normal(x1, y1, z1,
                       x1, y1, z2,
                       x1, y2, z1);
    glBegin(GL_POLYGON);
    //left
    // glColor3f(0,1,1);
    glNormal3f(vn[0], vn[1], vn[2]);
    glVertex3f(x1, y1, z1);
    glVertex3f(x1, y1, z2);
    glVertex3f(x1, y2, z2);
    glVertex3f(x1, y2, z1);
    glEnd();
    vn = vektor_normal(x2, y1, z1,
                       x2, y2, z1,
                       x2, y1, z2);
    glBegin(GL_POLYGON);
    //right
    // glColor3f(1,1,1);
    glNormal3f(vn[0], vn[1], vn[2]);
    glVertex3f(x2, y1, z1);
    glVertex3f(x2, y2, z1);
    glVertex3f(x2, y2, z2);
    glVertex3f(x2, y1, z2);
    glEnd();
}

void box24(float x1, float y1, float z1, float x2, float y2, float z2,
           float x3, float y3, float z3, float x4, float y4, float z4,
           float x5, float y5, float z5, float x6, float y6, float z6,
           float x7, float y7, float z7, float x8, float y8, float z8)
{
    /*
        x1 = pojok kiri bawah depan
        x2 = pojok kanan bawah depan
        x3 = pojok kanan atas depan
        x4 = pojok kiri atas depan
        x5 = pojok kiri bawah belakang
        x6 = pojok kanan bawah belakang
        x7 = pojok kanan atas belakang
        x8 = pojok kiri atas belakang
    */
    float *vn = vektor_normal(x1, y1, z1,
                              x2, y2, z2,
                              x4, y4, z4);
    glBegin(GL_POLYGON);
    //depan
    // glColor3f(1,1,0);
    glNormal3f(vn[0], vn[1], vn[2]);
    glVertex3f(x1, y1, z1);
    glVertex3f(x2, y2, z2);
    glVertex3f(x3, y3, z3);
    glVertex3f(x4, y4, z4);
    glEnd();
    vn = vektor_normal(x5, y5, z5,
                       x8, y8, z8,
                       x6, y6, z6);
    glBegin(GL_POLYGON);
    //belakang
    // glColor3f(1,0,0);
    glNormal3f(vn[0], vn[1], vn[2]);
    glVertex3f(x5, y5, z5);
    glVertex3f(x8, y8, z8);
    glVertex3f(x7, y7, z7);
    glVertex3f(x6, y6, z6);
    glEnd();
    vn = vektor_normal(x1, y1, z1,
                       x5, y5, z5,
                       x2, y2, z2);
    glBegin(GL_POLYGON);
    //bottom
    // glColor3f(1,0,1);
    glNormal3f(vn[0], vn[1], vn[2]);
    glVertex3f(x1, y1, z1);
    glVertex3f(x5, y5, z5);
    glVertex3f(x6, y6, z6);
    glVertex3f(x2, y2, z2);
    glEnd();
    vn = vektor_normal(x4, y4, z4,
                       x3, y3, z3,
                       x8, y8, z8);
    glBegin(GL_POLYGON);
    //top
    // glColor3f(0,1,0);
    glNormal3f(vn[0], vn[1], vn[2]);
    glVertex3f(x4, y4, z4);
    glVertex3f(x3, y3, z3);
    glVertex3f(x7, y7, z7);
    glVertex3f(x8, y8, z8);
    glEnd();
    vn = vektor_normal(x1, y1, z1,
                       x4, y4, z4,
                       x5, y5, z5);
    glBegin(GL_POLYGON);
    //left
    // glColor3f(0,1,1);
    glNormal3f(vn[0], vn[1], vn[2]);
    glVertex3f(x1, y1, z1);
    glVertex3f(x4, y4, z4);
    glVertex3f(x8, y8, z8);
    glVertex3f(x5, y5, z5);
    glEnd();
    vn = vektor_normal(x3, y3, z3,
                       x2, y2, z2,
                       x7, y7, z7);
    glBegin(GL_POLYGON);
    //right
    // glColor3f(1,1,1);
    glNormal3f(vn[0], vn[1], vn[2]);
    glVertex3f(x3, y3, z3);
    glVertex3f(x2, y2, z2);
    glVertex3f(x6, y6, z6);
    glVertex3f(x7, y7, z7);
    glEnd();
}


void dua(int x, int y, bool puluhan)
{
    float x1 = 0;
    float y1 = y + 0.1;
    if (puluhan)
    {
        x1 = x + 0.1;
    }
    else
    {
        x1 = x + 0.5;
    }
    glPushMatrix();
    if (mode == 3)
    {
        glTranslated(x1, y1 + 0.7, 0);
        glRotated(90, 1, 0, 0);
        glScaled(0.2, 0.2, 0.1);
        glTranslated(-x1, -y1, 0);
    }
    box(x1, y1, 0, x1 + 0.1, y1 + 0.3, 0.5);
    box(x1 + 0.2, y1 + 0.2, 0, x1 + 0.3, y1 + 0.5, 0.5);
    box(x1, y1, 0, x1 + 0.3, y1 + 0.1, 0.5);
    box(x1, y1 + 0.2, 0, x1 + 0.3, y1 + 0.3, 0.5);
    box(x1, y1 + 0.4, 0, x1 + 0.3, y1 + 0.5, 0.5);
    glPopMatrix();
}
void tiga(int x, int y, bool puluhan)
{
    float x1 = 0;
    float y1 = y + 0.1;
    if (puluhan)
    {
        x1 = x + 0.1;
    }
    else
    {
        x1 = x + 0.5;
    }
    glPushMatrix();
    if (mode == 3)
    {
        glTranslated(x1, y1 + 0.7, 0);
        glRotated(90, 1, 0, 0);
        glScaled(0.2, 0.2, 0.1);
        glTranslated(-x1, -y1, 0);
    }
    box(x1 + 0.2, y1, 0, x1 + 0.3, y1 + 0.5, 0.5);
    box(x1, y1, 0, x1 + 0.3, y1 + 0.1, 0.5);
    box(x1, y1 + 0.2, 0, x1 + 0.3, y1 + 0.3, 0.5);
    box(x1, y1 + 0.4, 0, x1 + 0.3, y1 + 0.5, 0.5);
    glPopMatrix();
}

void enam(int x, int y, bool puluhan)
{
    float x1 = 0;
    float y1 = y + 0.1;
    if (puluhan) x1 = x + 0.1;
    else x1 = x + 0.5;
    glPushMatrix();
    if (mode == 3)
    {
        glTranslated(x1, y1 + 0.7, 0);
        glRotated(90, 1, 0, 0);
        glScaled(0.2, 0.2, 0.1);
        glTranslated(-x1, -y1, 0);
    }
    // left vertical (full)
    box(x1, y1, 0, x1 + 0.1, y1 + 0.5, 0.5);
    // bottom horizontal
    box(x1, y1, 0, x1 + 0.3, y1 + 0.1, 0.5);
    // middle horizontal
    box(x1, y1 + 0.2, 0, x1 + 0.3, y1 + 0.3, 0.5);
    // top horizontal
    box(x1, y1 + 0.4, 0, x1 + 0.3, y1 + 0.5, 0.5);
    // right vertical, bottom half ONLY (y1 to y1+0.3)
    box(x1 + 0.2, y1, 0, x1 + 0.3, y1 + 0.3, 0.5);
    glPopMatrix();
}
void tujuh(int x, int y, bool puluhan)
{
    float x1 = 0;
    float y1 = y + 0.1;
    if (puluhan) x1 = x + 0.1;
    else x1 = x + 0.5;
    glPushMatrix();
    if (mode == 3)
    {
        glTranslated(x1, y1 + 0.7, 0);
        glRotated(90, 1, 0, 0);
        glScaled(0.2, 0.2, 0.1);
        glTranslated(-x1, -y1, 0);
    }
    // top horizontal
    box(x1, y1 + 0.4, 0, x1 + 0.3, y1 + 0.5, 0.5);
    // right vertical
    box(x1 + 0.2, y1, 0, x1 + 0.3, y1 + 0.5, 0.5);
    glPopMatrix();
}
void sembilan(int x, int y, bool puluhan)
{
    float x1 = 0;
    float y1 = y + 0.1;
    if (puluhan) x1 = x + 0.1;
    else x1 = x + 0.5;
    glPushMatrix();
    if (mode == 3)
    {
        glTranslated(x1, y1 + 0.7, 0);
        glRotated(90, 1, 0, 0);
        glScaled(0.2, 0.2, 0.1);
        glTranslated(-x1, -y1, 0);
    }
    // right vertical
    box(x1 + 0.2, y1, 0, x1 + 0.3, y1 + 0.5, 0.5);
    // top horizontal
    box(x1, y1 + 0.4, 0, x1 + 0.3, y1 + 0.5, 0.5);
    // bottom horizontal
    box(x1, y1, 0, x1 + 0.3, y1 + 0.1, 0.5);
    // middle horizontal
    box(x1, y1 + 0.2, 0, x1 + 0.3, y1 + 0.3, 0.5);
    // left vertical (top half)
    box(x1, y1 + 0.2, 0, x1 + 0.1, y1 + 0.5, 0.5);
    glPopMatrix();
}


void angka(int x, int y, int counter)
{
    int puluhan = counter / 10;
    int satuan = counter % 10;
    material(1, 0.5, 0.5, 1.0);

    // Satuan
    if (satuan == 2) dua(x * kotak + 1, y * kotak + 1, false);
    else if (satuan == 3) tiga(x * kotak + 1, y * kotak + 1, false);
    else if (satuan == 6) enam(x * kotak + 1, y * kotak + 1, false);
    else if (satuan == 7) tujuh(x * kotak + 1, y * kotak + 1, false);
    else if (satuan == 9) sembilan(x * kotak + 1, y * kotak + 1, false);

    // Puluhan
    if (puluhan == 2) dua(x * kotak + 1, y * kotak + 1, true);
    else if (puluhan == 3) tiga(x * kotak + 1, y * kotak + 1, true);
    else if (puluhan == 6) enam(x * kotak + 1, y * kotak + 1, true);
    else if (puluhan == 7) tujuh(x * kotak + 1, y * kotak + 1, true);
    else if (puluhan == 9) sembilan(x * kotak + 1, y * kotak + 1, true);
}

void tangga(float x1, float y1, float x2, float y2)
{
    // glColor4f(0.84, 0.46, 0.04, 1.0);
    material(0.84, 0.46, 0.04, 1.0);
    if (y1 == y2)
    {
        for (float x = x1; x < x2 - 1; x += 1)
        {
            box(x, y1 - 0.6, 0.2,
                x + 2, y1 - 0.4, 0.3);
            box(x, y1 + 0.4, 0.2,
                x + 2, y1 + 0.6, 0.3);
            box(x + 0.9, y1 - 0.6, 0.2,
                x + 1.1, y1 + 0.6, 0.3);
        }
    }
    else
    {
        float derajat;
        if (x1 < x2)
        {
            derajat = 0.25;
        }
        else if (x1 == x2)
        {
            derajat = 0;
        }
        else
        {
            derajat = -0.25;
        }

        for (float i = 0.5; i < 5; i += 0.5)
        {
            box24(x1 + ((x2 - x1) * i / 5) - 0.4, y1 + ((y2 - y1) * i / 5) + 0.1 + derajat, 0.3, x1 + ((x2 - x1) * i / 5) - 0.55, y1 + ((y2 - y1) * i / 5) - 0.1 + derajat, 0.3,
                  x1 + ((x2 - x1) * i / 5) + 0.4, y1 + ((y2 - y1) * i / 5) - 0.1 - derajat, 0.3, x1 + ((x2 - x1) * i / 5) + 0.55, y1 + ((y2 - y1) * i / 5) + 0.1 - derajat, 0.3,
                  x1 + ((x2 - x1) * i / 5) - 0.4, y1 + ((y2 - y1) * i / 5) + 0.1 + derajat, 0.2, x1 + ((x2 - x1) * i / 5) - 0.55, y1 + ((y2 - y1) * i / 5) - 0.1 + derajat, 0.2,
                  x1 + ((x2 - x1) * i / 5) + 0.4, y1 + ((y2 - y1) * i / 5) - 0.1 - derajat, 0.2, x1 + ((x2 - x1) * i / 5) + 0.55, y1 + ((y2 - y1) * i / 5) + 0.1 - derajat, 0.2);
        }

        box24(x1 - 0.6, y1 + derajat, 0.3, x1 - 0.4, y1 + derajat, 0.3,
              x2 - 0.4, y2 + derajat, 0.3, x2 - 0.6, y2 + derajat, 0.3,
              x1 - 0.6, y1 + derajat, 0.2, x1 - 0.4, y1 + derajat, 0.2,
              x2 - 0.4, y2 + derajat, 0.2, x2 - 0.6, y2 + derajat, 0.2);

        box24(x1 + 0.4, y1 - derajat, 0.3, x1 + 0.6, y1 - derajat, 0.3,
              x2 + 0.6, y2 - derajat, 0.3, x2 + 0.4, y2 - derajat, 0.3,
              x1 + 0.4, y1 - derajat, 0.2, x1 + 0.6, y1 - derajat, 0.2,
              x2 + 0.6, y2 - derajat, 0.2, x2 + 0.4, y2 - derajat, 0.2);
    }
}

void ular(float x1, float y1, float x2, float y2)
{
    // glColor4f(0.49, 0.49, 0.67, 1.0);
    material(0.49, 0.49, 0.67, 1.0);
    /* box24(LDF, RDF,
                 RUF, LUF,
                 LDB, RDB,
                 RUB, LUB}
    */
    box24(x1 - 0.25, y1 + 0 * (y2 - y1) / 3, 0.3, x1 + 0.25, y1 + 0 * (y2 - y1) / 3, 0.3,
          x1 + (x2 - x1) / 2 + 0.25, y1 + 1 * (y2 - y1) / 3, 0.3, x1 + (x2 - x1) / 2 - 0.25, y1 + 1 * (y2 - y1) / 3, 0.3,
          x1 - 0.25, y1 + 0 * (y2 - y1) / 3, 0.2, x1 + 0.25, y1 + 0 * (y2 - y1) / 3, 0.2,
          x1 + (x2 - x1) / 2 + 0.25, y1 + 1 * (y2 - y1) / 3, 0.2, x1 + (x2 - x1) / 2 - 0.25, y1 + 1 * (y2 - y1) / 3, 0.2);
    // glColor4f(0.35, 0.25,0.29, 1.0);
    material(0.35, 0.25, 0.29, 1.0);
    box24(x1 + (x2 - x1) / 2 - 0.25, y1 + 1 * (y2 - y1) / 3, 0.3, x1 + (x2 - x1) / 2 + 0.25, y1 + 1 * (y2 - y1) / 3, 0.3,
          x1 + (x2 - x1) / 2 + 0.25, y1 + 2 * (y2 - y1) / 3, 0.3, x1 + (x2 - x1) / 2 - 0.25, y1 + 2 * (y2 - y1) / 3, 0.3,
          x1 + (x2 - x1) / 2 - 0.25, y1 + 1 * (y2 - y1) / 3, 0.2, x1 + (x2 - x1) / 2 + 0.25, y1 + 1 * (y2 - y1) / 3, 0.2,
          x1 + (x2 - x1) / 2 + 0.25, y1 + 2 * (y2 - y1) / 3, 0.2, x1 + (x2 - x1) / 2 - 0.25, y1 + 2 * (y2 - y1) / 3, 0.2);
    // box(x1+ (x2-x1)/2-0.25, y1+ 1*(y2-y1)/3, 0.2,
    //    x1+ (x2-x1)/2+0.25, y1+ 2*(y2-y1)/3, 0.3);
    // glColor4f(0.49, 0.49, 0.67, 1.0);
    material(0.49, 0.49, 0.67, 1.0);
    box24(x1 + (x2 - x1) / 2 - 0.25, y1 + 2 * (y2 - y1) / 3, 0.3,
          x1 + (x2 - x1) / 2 + 0.25, y1 + 2 * (y2 - y1) / 3, 0.3,
          x2 + 0.25, y1 + 3 * (y2 - y1) / 3, 0.3,
          x2 - 0.25, y1 + 3 * (y2 - y1) / 3, 0.3,

          x1 + (x2 - x1) / 2 - 0.25, y1 + 2 * (y2 - y1) / 3, 0.2,
          x1 + (x2 - x1) / 2 + 0.25, y1 + 2 * (y2 - y1) / 3, 0.2,
          x2 + 0.25, y1 + 3 * (y2 - y1) / 3, 0.2,
          x2 - 0.25, y1 + 3 * (y2 - y1) / 3, 0.2);
}

void papan()
{
    // Dinding luar: HIJAU
    material(0.0, 0.8, 0.0, 1.0); // warna hijau
    box(-0.15, -0.15, -0.2, 42.0, 0, 0.1);    //bawah
    box(0.0, 8, -0.2, 42.15, 8.15, 0.1);      //atas
    box(-0.15, 0.0, -0.2, 0, 8.15, 0.1);      //kiri
    box(42, -0.15, -0.2, 42.15, 8.0, 0.1);    //kanan
    int counter = 1;
    int max_kolom = 20;
    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 21; x++)
        {
            if (y % 2 == ganjil)
            {
                angka(max_kolom - x, y, counter);
            }
            else
            {
                angka(x, y, counter);
            }
            counter++;
            glPopMatrix();
        }
        // Dinding garis dalam: HIJAU
        material(0.0, 0.8, 0.0, 1.0);
        if (y % 2 == ganjil)
        {
            box(0 * kotak, y * kotak - 0.1, 0, 20 * kotak, y * kotak + 0.1, 0.1);
        }
        else if (y != 0)
        {
            box(1 * kotak, y * kotak - 0.1, 0, 21 * kotak, y * kotak + 0.1, 0.1);
        }

        for (int x = 0; x < 21; x++)
        {
            // PETAK: biru dan putih
            if (x % 2 == genap && y % 2 == ganjil ||
                x % 2 == ganjil && y % 2 == genap)
            {
                if (mode == 2)
                {
                    material(1.0, 1.0, 1.0, 0.5); // putih transparan
                }
                else
                {
                    material(1.0, 1.0, 1.0, 1.0); // putih
                }
            }
            else
            {
                if (mode == 2)
                {
                    material(0.2, 0.4, 1.0, 0.2); // biru transparan
                }
                else
                {
                    material(0.2, 0.4, 1.0, 1.0); // biru
                }
            }
            box(x * kotak, y * kotak, -0.2, x * kotak + 2, y * kotak + 2, 0);
        }
    }
}

void pemain1()
{
    // glColor4f(1, 0.5, 0.5, 1.0);
    material(1, 0.5, 0.5, 1.0);
    glPushMatrix();
    glTranslated(0.75 + dx1, 1.15 + dy1, 0.4);
    if (mode != 3)
        glRotatef(-90, 1, 0, 0);
    glScalef(0.25, 0.25, 0.25);

    glutSolidCube(2);
    glPopMatrix();
    // */
}

void pemain2()
{
    // glColor4f(0.5,1,0.5, 1.0);
    material(0.5, 1, 0.5, 1.0);
    glPushMatrix();
    glTranslated(0.75 + dx2, 0.5 + dy2, 0.4);
    if (mode == 3)
        glRotated(90, 1, 0, 0);
    glScaled(0.3, 0.3, 0.3);
    glutSolidTeapot(1);
    glPopMatrix();
}

void observer()
{
    // glColor4f(1,1,1, 1.0);
    material(1, 1, 1, 1.0);

    glPushMatrix();
    glTranslated(0.3 + dx_ob, 0.1 + dy_ob, 0.4);
    glScaled(0.3, 0.1, 0.2); // scaling dulu
    glRotated(spin_observer[0], 1, 0, 0);
    glRotated(spin_observer[1], 0, 1, 0);
    glRotated(spin_observer[2], 0, 0, 1);
    glutSolidCone(1, 2, 100, 100);
    glPopMatrix();
}

void animasi()
{
    if (mode_ob == 1)
        spin_observer[0] += observer_speed;
    else if (mode_ob == 2)
        spin_observer[1] += observer_speed;
    else if (mode_ob == 3)
        spin_observer[2] += observer_speed;
    glutPostRedisplay();
}

void display()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    if (mode == 3)
    {
        gluLookAt(pos[0], pos[1], pos[2],
                  pos[0] + viewdir[0], pos[1] + viewdir[1], pos[2] + viewdir[2],
                  0, 0, 1);
    }
    glTranslated(21, 4.0, 0);
    glRotated(spin[0], 1, 0, 0);
    glRotated(spin[1], 0, 1, 0);
    glRotated(spin[2], 0, 0, 1);
    glTranslated(-21, -4.0, 0);

    ular(13 * kotak + 1, 3 * kotak + 1, 16 * kotak + 1, 1 * kotak + 1);
    ular(4 * kotak + 1, 3 * kotak + 1, 6 * kotak + 1, 1 * kotak + 1);
    ular(5 * kotak + 1, 2 * kotak + 1, 2 * kotak + 1, 0 * kotak + 1);



    tangga(5 * kotak + 1, 0 * kotak + 1, 8 * kotak + 1, 3 * kotak + 1);
    tangga(10 * kotak + 0, 0 * kotak + 1, 13 * kotak + 0, 0 * kotak + 1);
    tangga(15 * kotak + 1, 0 * kotak + 1, 15 * kotak + 1, 3 * kotak + 1);

    pemain1();
    pemain2();
    observer();
    papan();
    if (mode == 2)
    {
        glutIdleFunc(animasi);
    }
    else
    {
        glutIdleFunc(NULL);
    }
    glutSwapBuffers(); // Use glutSwapBuffers for double buffering
    // glFlush(); // Not needed if using glutSwapBuffers
}

int *cek(int dx, int dy)
{
    //Cek kotak ular or tangga
    //Kotak x_gambar-1
    if (dx == 1 * kotak && dy == 0 * kotak)
    {
        dx = 4 * kotak;
        dy = 3 * kotak;
    }
    else if (dx == 8 * kotak && dy == 1 * kotak)
    {
        dx = 4 * kotak;
        dy = 1 * kotak;
    }
    else if (dx == 15 * kotak && dy == 0 * kotak)
    {
        dx = 15 * kotak;
        dy = 3 * kotak;
    }
    else if (dx == 9 * kotak && dy == 2 * kotak)
    {
        dx = 14 * kotak;
        dy = 2 * kotak;
    }
    else if (dx == 20 * kotak && dy == 0 * kotak)
    {
        dx = 17 * kotak;
        dy = 3 * kotak;
    }
    //Ini ular
    else if (dx == 1 * kotak && dy == 3 * kotak)
    {
        dx = 3 * kotak;
        dy = 1 * kotak;
    }
    else if (dx == 14 * kotak && dy == 3 * kotak)
    {
        dx = 9 * kotak;
        dy = 0 * kotak;
    }
    else if (dx == 16 * kotak && dy == 2 * kotak)
    {
        dx = 18 * kotak;
        dy = 0 * kotak;
    }
    else if (dx == 5 * kotak && dy == 3 * kotak)
    {
        dx = 9 * kotak;
        dy = 1 * kotak;
    }
    static int d[2]; // Use static to return a valid pointer to a global/static variable
    d[0] = dx;
    d[1] = dy;
    return d;
}

void input(unsigned char key, int x, int y)
{
    if (key == 'q' || key == 'Q')
    {
        exit(0);
    }
    // mode
    else if (key == 'v' || key == 'V')
    {
        if (mode == 1)
        {
            spin[0] = spin_save[0];
            spin[1] = spin_save[1];
            spin[2] = spin_save[2];
            spin_observer[0] = spin_ob_save[0];
            spin_observer[1] = spin_ob_save[1];
            spin_observer[2] = spin_ob_save[2];
            mode = 2;
        }
        else if (mode == 2)
        {
            spin_save[0] = spin[0];
            spin_save[1] = spin[1];
            spin_save[2] = spin[2];
            spin[0] = 0;
            spin[1] = 0;
            spin[2] = 0;
            spin_ob_save[0] = spin_observer[0];
            spin_ob_save[1] = spin_observer[1];
            spin_ob_save[2] = spin_observer[2];
            spin_observer[0] = 0;
            spin_observer[1] = 0;
            spin_observer[2] = 0;
            mode = 3;
        }
        else
        {
            spin[0] = 0;
            mode = 1;
        }
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        if (mode == 3)
        {
            // Corrected gluPerspective call: specify aspect ratio, near and far clipping planes
            gluPerspective(60.0, (double)glutGet(GLUT_WINDOW_WIDTH) / (double)glutGet(GLUT_WINDOW_HEIGHT), 0.1, 100.0);
        }
        else
        {
            glOrtho(-0.25, 42.25, -0.25, 8.25, -50, 50);
        }
        glMatrixMode(GL_MODELVIEW);
        // cout << "mode = " << mode<<endl;
        glutPostRedisplay();
    }
    //observer
    else if (key == 'd' || key == 'D')
    {
        arah_observer = "kanan";
        if (dx_ob < 41.49)
            dx_ob += 0.1;
        glutPostRedisplay();
    }
    else if (key == 'a' || key == 'A')
    {
        arah_observer = "kiri";
        if (dx_ob > 0.0)
            dx_ob -= 0.1;
        glutPostRedisplay();
    }
    else if (key == 'w' || key == 'W')
    {
        arah_observer = "atas";
        if (dy_ob < 7.69)
            dy_ob += 0.1;
        glutPostRedisplay();
    }
    else if (key == 's' || key == 'S')
    {
        arah_observer = "bawah";
        if (dy_ob > 0.2)
            dy_ob -= 0.1;
        glutPostRedisplay();
    }
    // cahaya
    else if (key == '1') // AMBIENT ON/OFF
    {
        if (amb)
        {
            GLfloat ambient_light[] = {0.0, 0.0, 0.0, 1.0}; // OFF
            glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient_light);
            amb = false;
            cout << "Ambient OFF" << endl;
        }
        else
        {
            GLfloat ambient_light[] = {0.9, 0.9, 0.9, 1.0}; // ON
            glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient_light);
            amb = true;
            cout << "Ambient ON" << endl;
        }
        glutPostRedisplay();
    }
    else if (key == '2') // DIFFUSE ON/OFF
    {
        if (diff)
        {
            glDisable(GL_LIGHT0);
            diff = false;
            cout << "Diffuse OFF" << endl;
        }
        else
        {
            glEnable(GL_LIGHT0);
            diff = true;
            cout << "Diffuse ON" << endl;
        }
        glutPostRedisplay();
    }
    else if (key == '3') // SPECULAR ON/OFF
    {
        if (spec)
        {
            glDisable(GL_LIGHT1);
            spec = false;
            cout << "Specular OFF" << endl;
        }
        else
        {
            glEnable(GL_LIGHT1);
            spec = true;
            cout << "Specular ON" << endl;
        }
        glutPostRedisplay();
    }
    // Play game
    else if (key == 'r' || key == 'R')
    {
        turn = 1;
        menang = false;
        ganjil = 1;
        genap = 0;

        dx1 = 0;
        dy1 = 0;
        dx2 = 0;
        dy2 = 0;

        display();
        cout << "-----------------------------------------------" << endl;
        cout << "----------------GAME RESET---------------------" << endl;
        cout << "-----------------------------------------------" << endl;
        cout << "Tekan [Spasi] untuk bermain" << endl;
    }
    else if (!menang && key == ' ')
    {
        int dadu = rand() % 6 + 1;
        cout << "Giliran Pemain " << turn << endl;
        cout << "Angka Hasil Dadu: " << dadu << endl;
        int *d;
        if (turn == 1)
        {
            for (int i = 0; i < dadu; i++)
            {
                if (dy1 % 4 == 0)
                {
                    if (dx1 == 40)
                    {
                        dy1 += kotak;
                    }
                    else
                    {
                        dx1 += kotak;
                    }
                }
                else if (dy1 % 4 == 2)
                {
                    if (dx1 == 0)
                    {
                        dy1 += kotak;
                    }
                    else
                    {
                        dx1 -= kotak;
                    }
                }

                if (dx1 == 0 && dy1 == 6)
                {
                    cout << "Player 1 menang" << endl;
                    menang = true;
                    break;
                }
                display();
                Sleep(100);
            }
            d = cek(dx1, dy1);
            dx1 = d[0];
            dy1 = d[1];
            turn = 2;
            //Player 2----------------------------------------------------------------------------
        }
        else if (turn == 2)
        {
            for (int i = 0; i < dadu; i++)
            {
                if (dy2 % 4 == 0)
                {
                    if (dx2 == 40)
                    {
                        dy2 += kotak;
                    }
                    else
                    {
                        dx2 += kotak;
                    }
                }
                else if (dy2 % 4 == 2)
                {
                    if (dx2 == 0)
                    {
                        dy2 += kotak;
                    }
                    else
                    {
                        dx2 -= kotak;
                    }
                }

                if (dx2 == 0 && dy2 == 6)
                {
                    cout << "Player 2 menang" << endl;
                    menang = true;
                    break;
                }

                display();
                Sleep(100);
            }
            d = cek(dx2, dy2);
            dx2 = d[0];
            dy2 = d[1];
            turn = 1;
        }
        display();
    }
    // spin
    else if (mode == 2)
    {
        if (key == 'u' || key == 'U')
        {
            spin[2] += 5;
            glutPostRedisplay();
        }
        else if (key == 'o' || key == 'O')
        {
            spin[2] -= 5;
            glutPostRedisplay();
        }
        else if (key == 'j' || key == 'J')
        {
            spin[1] += 5;
            glutPostRedisplay();
        }
        else if (key == 'l' || key == 'L')
        {
            spin[1] -= 5;
            glutPostRedisplay();
        }
        else if (key == 'i' || key == 'I')
        {
            spin[0] -= 5;
            glutPostRedisplay();
        }
        else if (key == 'k' || key == 'K')
        {
            spin[0] += 5;
            glutPostRedisplay();
        }
    }
}

float rot_speed = 0.03f; // kecepatan default putar kanan/kiri

void input_sp(int key, int x, int y)
{
    if (mode == 3)
    {
        if (key == GLUT_KEY_UP)
        {
            pos[0] += 0.5 * viewdir[0];
            pos[1] += 0.5 * viewdir[1];
            pos[2] += 0.5 * viewdir[2];
        }
        else if (key == GLUT_KEY_DOWN)
        {
            pos[0] -= 0.5 * viewdir[0];
            pos[1] -= 0.5 * viewdir[1];
            pos[2] -= 0.5 * viewdir[2];
        }
        else if (key == GLUT_KEY_RIGHT)
        {
            alpha -= rot_speed; // pakai variabel rot_speed
            viewdir[0] = cos(alpha);
            viewdir[1] = sin(alpha);
        }
        else if (key == GLUT_KEY_LEFT)
        {
            alpha += rot_speed; // pakai variabel rot_speed
            viewdir[0] = cos(alpha);
            viewdir[1] = sin(alpha);
        }
        else if (key == GLUT_KEY_PAGE_UP)
        {
            viewdir[2] += 0.1;
        }
        else if (key == GLUT_KEY_PAGE_DOWN)
        {
            viewdir[2] -= 0.1;
        }
        glutPostRedisplay();
    }
}


void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        mode_ob = 1;
    }
    else if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN)
    {
        mode_ob = 2;
    }
    else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {
        mode_ob = 3;
    }
}
void init_cahaya()
{
    //init cahaya dikelompokkan
    GLfloat light_position_diff[] = {-1.0, 1.0, 1.0, 0.0};
    GLfloat diffuse_light[] = {1.0, 1.0, 1.0, 1.0};

    GLfloat light_position_spec[] = {10.0, 10.0, 24.0, 0.0};
    GLfloat specular_light[] = {1.0, 1.0, 1.0, 1.0};

    GLfloat ambient_light[] = {0.9, 0.9, 0.9, 1.0};

    glLightfv(GL_LIGHT0, GL_POSITION, light_position_diff);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
    glLightfv(GL_LIGHT1, GL_POSITION, light_position_spec);
    glLightfv(GL_LIGHT1, GL_SPECULAR, specular_light);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient_light);

    glEnable(GL_LIGHTING);
    //glShadeModel (GL_SMOOTH);

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
}

void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (mode == 3)
    {
        gluPerspective(60.0, (double)w / (double)h, 0.1, 100.0);
    }
    else
    {
        glOrtho(-0.25, 42.25, -0.25, 8.25, -50, 50);
    }
    glMatrixMode(GL_MODELVIEW);
    glutPostRedisplay();
}

void myinit()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-0.25, 42.25, -0.25, 8.25, -50, 50);
    glMatrixMode(GL_MODELVIEW);

    glClearColor(0, 0, 0, 1.0);
    glColor4f(0.0, 0.0, 1.0, 1.0);

    glShadeModel(GL_SMOOTH); // Changed to GL_SMOOTH for better lighting
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    init_cahaya();
    srand(time(NULL));
    cout << "Tekan [Spasi] untuk bermain" << endl;
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // Changed to GLUT_DOUBLE for smoother animation
    glutInitWindowSize(1000, 200);
    //glutInitWindowPosition(100,100);
    glutCreateWindow("UAS Ular Tangga");
    myinit();
    glutKeyboardFunc(input);
    glutSpecialFunc(input_sp);
    glutMouseFunc(mouse);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape); // Added reshape function to handle window resizing
    glutMainLoop();
    return 0;
}
