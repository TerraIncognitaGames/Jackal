#pragma once
#include <glut.h>

class Visualizer {
public:
    Visualizer() {}
    void Initialize(const std::string & params) {}
    void Reset() {}
    
    void Visualize() {
        myGlutInit();
        glutDisplayFunc(DisplayFunc);
        glutMainLoop();
    }
private:
    /* There is some shit below */

    static void DisplayFunc() {
        glClear(GL_COLOR_BUFFER_BIT);
        glColor3f(1.0, 0.0, 1.0); //fiol
        glFlush();
    }
    void myGlutInit() {
        int WinWid = 400;
        int WinHei = 400;
        glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
        glutInitWindowSize(WinWid, WinHei);
        glutInitWindowPosition(900, 0);
        glutCreateWindow("Home");
        glClearColor(1.0, 0.0, 1.0, 0.0);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);/*выбор режима прорисовки
                                                  полигонов. выбран дефолтный.*/
        glMatrixMode(GL_PROJECTION); //Установить проекцию
        glLoadIdentity();
        int GameFieldWid = 200;
        int GameFieldHei = 300;
        glOrtho(0.0, GameFieldWid, 0.0, GameFieldHei, -1.0, 1.0);
        //glutKeyboardFunc(myGlutKeyboardFunc);
        //glutSpecialFunc(myGlutSpecialFunc);
    }
};