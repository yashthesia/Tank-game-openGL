#include <stdio.h>
#include "iostream"
#include <openGL/gl.h>
#include <GLUT/glut.h>
#include "math.h"
using namespace std;
//////////////////////////////
//initial variables
int turn  =1;
int Firsttime =1;
int intro = 1;
int introCount = 0;
int winner = 0;
int w = 0;
int h = 0;
int wf= 640;
int hf = 480;
int cloudx = 0;
//////////////////////////////
void Init()
{
    glClearColor(0.0,0.0,0.0,0);
    glColor3f(1.0,0.0,0.0);
    gluOrtho2D(w , wf , h , hf);
    glMatrixMode(GL_MODELVIEW);
}
void introduction(){
        string str ="CGV PROJECT \n 15BCE120 - 15BCE126 -15BCE123\n";
        char *c;
        glPushMatrix();
        glColor3f(0, 1, 0);
        glTranslatef(50, 300,0);
        glScalef(0.15f,0.15f,0);
        
        for (int i=0; i<str.size(); i++)
        {
            glutStrokeCharacter(GLUT_STROKE_ROMAN , str[i]);
        }
        glPopMatrix();
}
class surface{
public:
    int base[480][640];
    void hills(int y1, int y2, int x){
        for(int i = y1,count = 0;i>=y2;i--,count++){
            for (int j = x-count; j<x+count; j++) {
                if (i > 0 && i< 479 && j > 0 && j < 639) {
                    base[i][j] = 1;
                }
            }
        }
    }
    void  intialDraw(){
//        glColor3f(0.737, 0.561, 0.561);
//        glBegin(GL_QUADS);
//            glVertex2f(0,0);
//            glVertex2f(640,0);
//            glVertex2f(640, 200);
//            glVertex2f(0,200);
//        glEnd();
        
        for (int i =0 ; i<480; i++) {
            for (int j = 0; j<640; j++) {
                if (i<200) {
                    base[i][j] = 1;
                }
                else{
                    base[i][j] = 0;
                }
            }
        }
        hills(300, 200, 210);
        hills(250, 100, 450);
        hills(340, 100, 600);
        hills(340, 100, 60);
        
    }

    void drawBase(){
        for (int i =0 ; i<480; i++) {
            for (int j = 0; j<640; j++) {
                if (base[i][j] == 1) {
                    glColor3f(0.737, 0.561, 0.561);
                    glBegin(GL_QUADS);
                        glVertex2f(j+1-2,i+1-2);
                        glVertex2f(j+1-2,i+1);
                        glVertex2f(j+1,i+1);
                        glVertex2f(j+1,i+1-2);
                    glEnd();
                }
                else{
                    
                    glColor3f(0.0, 0.749, 1);
                    glBegin(GL_QUADS);
                    glVertex2f(j+1-2,i+1-2);
                    glVertex2f(j+1-2,i+1);
                    glVertex2f(j+1,i+1);
                    glVertex2f(j+1,i+1-2);
                    glEnd();
                    base[i][j] = 0;
                }
            }
        }
    }
};
void cloud(int x, int y){
    
    for (int i = -20; i<=20; i++) {
        for (int j = -20; j<=20; j++) {
            if ((i)*(i) +(j)*(j) <=400) {
                glColor3f(1, 1, 1);
                glBegin(GL_QUADS);
                glVertex2f(x+j-2,y+i-2);
                glVertex2f(x+j-2,y+i);
                glVertex2f(x+j,y+i);
                glVertex2f(x+j,y+i-2);
                glEnd();
            }
        }
    }
}
void drawcloud(int x, int  y){
    cloud(x+50, y);
    cloud(x+20, y);
    cloud(x+35, y+10);
    cloud(x+25, y-10);
    cloud(x, y);
}

surface mainEnvi;
class tank{
public:
    int health =100;
    int color = 0;
    double angle;
    int x;
    int y;
    tank(){}
    tank(double angle, int x, int y, int color){
        this->x = x;
        this->y = y;
        this->color = color;
        this->angle = angle*3.14/180;
    }
    void drawLine(){
        int x2 =this->x + (int)30*cos(angle);
        int y2 = this->y + (int)30*sin(angle);
        glColor3f(1.0f,color,0.0f);
        glBegin(GL_LINES);
        glVertex2d(x,y);
        glVertex2d(x2,y2);
        glEnd();
    }
    void draw(){
        glColor3f(1.0f,color,0.0f);
        glBegin(GL_POLYGON);
        getNewY();
        glVertex2d(x-5,y);
        glVertex2d(x+5,y);
        glVertex2d(x+10,y+5);
        glVertex2d(x+4,y+5);
        glVertex2d(x+2,y+10);
        glVertex2d(x-2,y+10);
        glVertex2d(x-4,y+5);
        glVertex2d(x-10,y+5);
        glEnd();
        drawLine();
        drawHealth();
    }
    void drawHealth(){
        string str = to_string(health);
        char *c;
        glPushMatrix();
        glColor3f(0, 1, 0);
        glTranslatef(x, y+30,0);
        glScalef(0.09f,0.08f,0);
        
        for (int i=0; i<str.size(); i++)
        {
            glutStrokeCharacter(GLUT_STROKE_ROMAN , str[i]);
        }
        glPopMatrix();
    }
    void changeAngle(double angle){
        this->angle += angle*3.14/180;
    }
    void getNewY(){
        for (int i = 0; i<480; i++) {
            if (mainEnvi.base[i][x] == 0) {
                break;
            }
            y = i;
        }
    }
};
tank tank0(60.0, 200, 200,1);
tank tank1(60.0, 400, 200,0);
tank mainTank[2];
class bullet{
public:
    
    int visible = 0;
    int time =0 ;
    double velocity;
    double g;
    double angle;
    int initx, inity;
    int x;
    int y;
    bullet(int v,double gravity, double a,int xp, int yp){
        velocity  = v;
        g = gravity;
        angle = a*3.14/180;
        this->x = xp;
        this->y = yp;
        inity = y;
        initx = x;
    
    }
    void bulletdraw(){
        glColor3f(0, 0, 0);
        glBegin(GL_QUADS);
        glVertex2d(x-1,y-1);
        glVertex2d(x-1,y+1);
        glVertex2d(x+1,y+1);
        glVertex2d(x+1,y-1);
        glEnd();
    }
    void draw(){
        if (x <0 || x>= 640 || y <0 || y>=480) {
            cout << "end bullet" << endl;
            visible = 0;
        }
        if (mainEnvi.base[y][x] == 1) {
            visible = 0;
            cout<<"yeah" <<endl;
        
            for (int ix = -30; ix<=30; ix++) {
                for (int iy = -30; iy<=30; iy++) {
                    if ((ix)*(ix)/2 +(iy)*(iy) <=400) {
                            mainEnvi.base[y+iy][x+ix]=0;
                    }
                }
            }
           
        }
        int tx = mainTank[turn].x;
        int ty = mainTank[turn].y;
        //cout << tx <<":"<<ty <<"-----"<<x<<":"<<y << endl;
        if (x < tx+10 && x>tx-10 && y > ty-10 && y < ty+10) {
            visible = 0;
            mainTank[turn].health -= 10;
            if (mainTank[turn].health == 0) {
                winner = 1;
            }
            cout << turn << " ::::: " << mainTank[turn].health << endl;
        }else{
            bulletdraw();
        }
    }
    void setthings(int px, int py, double pangle){
        initx = px;
        inity = py;
        angle = pangle;
        visible = 1;
        time = 0;
    }

    void nextPosition(int time){
        x = initx+ velocity*time*cos(angle);
        y = inity+ velocity*sin(angle)*time - (0.5*g*time*time);
    }
};
bullet bull(10,0.7,0,0,0);

void keypressed(int key , int x, int y){
    switch (key) {
        case GLUT_KEY_RIGHT:
            mainTank[turn].changeAngle(-2);
            break;
        case GLUT_KEY_LEFT:
            mainTank[turn].changeAngle(+2);
            break;
        case 'a':
            if (mainEnvi.base[mainTank[turn].x-4][mainTank[turn].y+10]==0) {
                mainTank[turn].x -=4;
            }
            break;
        case 's':
            if (mainEnvi.base[mainTank[turn].x][mainTank[turn].y+10]==0) {
                mainTank[turn].x +=4;
            }
            break;
        case 'q':
            bull.setthings(mainTank[turn].x, mainTank[turn].y, mainTank[turn].angle);
            turn = 1-turn;
            break;
        case 'r':
            w = 100;
            wf = 600;
            h = 100;
            hf = 400;
            glPushMatrix();
            float mat [300][500][3];
            glReadPixels(100, 100, 600, 400, GL_RGB, GL_FLOAT, &mat);
            glScalef(1.5f,1.5f,0.0f);
            glPopMatrix();
        default:
            break;
    }
    
}
void winnerDisplay(){
    string str ="winner : Player"+to_string(1+(1-turn));
    char *c;
    glPushMatrix();
    glColor3f(1, 1, 1);
    glTranslatef(200, 300,0);
    glScalef(0.25f,0.25f,0);
    
    for (int i=0; i<str.size(); i++)
    {
        glutStrokeCharacter(GLUT_STROKE_ROMAN , str[i]);
    }
    glPopMatrix();
}
void display(void){
    glClear(GL_COLOR_BUFFER_BIT);
    glPushMatrix();
    if (intro) {
        introduction();
        introCount++;
        if (introCount == 80) {
            intro = 0;
        }
    }
    else if(winner){
        winnerDisplay();
    }
    else{
        if (Firsttime) {
            mainEnvi.intialDraw();
            Firsttime = 0;
        }
        else{
            mainEnvi.drawBase();
        }
        drawcloud(cloudx, 350);
        drawcloud(100+0.5*cloudx, 350);
        drawcloud(200+2*cloudx, 300);
        mainTank[0].draw();
        mainTank[1].draw();
        if (bull.visible==1) {
            bull.time = (bull.time+1)%(640);
            bull.nextPosition(bull.time);
            bull.draw();
        }
        
    }
    glFlush();
}
void update(int value) {
    cloudx = (cloudx+4)%500;
    glutPostRedisplay();
    
    glutTimerFunc(50, update, 0);
    
}
int main(int argc, char **argv)
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(0,0);
    glutInitWindowSize(640,480);
    glutCreateWindow("tank game");
    Init();
    mainTank[0] = tank0;
    mainTank[1] = tank1;
    glutSpecialFunc(keypressed);
    glutDisplayFunc(display);
    glutTimerFunc(200, update, 0);
    glutMainLoop();
}

