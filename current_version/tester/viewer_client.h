#pragma once
// #pragma comment( lib, "glut32.lib" )
// #pragma comment( lib, "glaux.lib" )

#include "glut.h"
#include "glaux.h"
#include "glu.h"
#include <string>
#include <vector>
#include <thread>
#include <mutex>

class Ship {

} ship;

// Stores current state of game field, can update
// and draw this state.
class CurrentScene {
public:
    // Singleton.
    static CurrentScene & Instance() {
        static CurrentScene scene;
        return scene;
    }

    // Initializes current scene data.
    void Initialize(double block_x, double block_y) {
        std::lock_guard<std::mutex> lock(mutex);
        green_block.SetCoordinates(block_x, block_y);
    }

    // Updates current scene data.
    void Update(double block_x, double block_y) {
        std::lock_guard<std::mutex> lock(mutex);
        green_block.SetCoordinates(block_x, block_y);
    }

    // Function for glutDisplayFunc called for drawing.
    void Draw() {
        std::lock_guard<std::mutex> lock(mutex);
        green_block.Draw();
        DrawObject(ship);
    }

private:
    // Just a green square.
    class GreenBlock {
    public:
        GreenBlock() {}
        GreenBlock(double x, double y) : x_(x), y_(y) {}
        void SetCoordinates(double x, double y) {
            x_ = x;
            y_ = y;
        }
        void Draw() {
            std::cout << "green_block.Draw();" << std::endl;
            glColor3f(0.0, 0.64, 0.0);
            glBegin(GL_POLYGON);
            glVertex3f(x_, y_, 0.0);
            glVertex3f(x_ + 1, y_, 0.0);
            glVertex3f(x_ + 1, y_ + 1, 0.0);
            glVertex3f(x_, y_ + 1, 0.0);
            glEnd();
        }
    private:
        double x_, y_;
    };

    template<class ObjectType>
    void DrawObject(ObjectType object) {
        if (typeid(ObjectType) == typeid(Ship)) {
            // Загрузка картинки
            // GLuint	texture[1];
            /*AUX_RGBImageRec * image;
            image = auxDIBImageLoad("Textures\\ship.bmp");
            glRasterPos2d(-4.5, -3);                    // нижний левый угол
            glPixelZoom(1, 1);
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);         // выравнивание
            glDrawPixels(image->sizeX, image->sizeY, // ширина и высота
                GL_RGB, GL_UNSIGNED_BYTE,      // формат и тип
                image->data);     // сами данные**/
            // Создание текстуры
            /*glGenTextures(1, &texture[0]);
            glBindTexture(GL_TEXTURE_2D, texture[0]);
            //
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            //
            glTexImage2D(GL_TEXTURE_2D, 0, 3, texture1->sizeX, texture1->sizeY, 0,
                GL_RGB, GL_UNSIGNED_BYTE, texture1->data);

            glEnable(GL_TEXTURE_2D);		// Разрешение наложение текстуры

            glBegin(GL_POLYGON);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0, -1.0, 0.0);	// Низ лево
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0, -1.0, 0.0);	// Низ право
            glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0, 1.0, 0.0);	// Верх право
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0, 1.0, 0.0);	// Верх лево
            glEnd();*/
        }
    }

    std::mutex mutex;
    GreenBlock green_block;

    CurrentScene() {}
    ~CurrentScene() {}
    CurrentScene(const CurrentScene & other) = delete;
    CurrentScene & operator=(const CurrentScene & other) = delete;

};


// Realizes the class working with Glut.
class Visualizer {
public:
    // Singleton.
    static Visualizer & Instance() {
        static Visualizer visualizer;
        return visualizer;
    }

    static void Initialize(int32_t block_x, int32_t block_y) {
        // VisualizerCommandParser parser(string_command);
        //auto command = parser.ParseIntoVisualizerInitializingCommand();
        CurrentScene::Instance().Initialize(block_x, block_y);
        InitializeGlut();
    }

    // Starts Glut main loop.
    static void Visualize() {
        glutMainLoop();
    }

    static void UpdateScene(double block_x, double block_y) {
        // VisualizerCommandParser parser(string_command);
        // auto command = parser.ParseIntoVisualizerUpdatingCommand();
        CurrentScene::Instance().Update(block_x, block_y);
        glutPostRedisplay();
        glFlush();
    }

private:
    static const int32_t kMsForFrame = 42; // ~(1000 ms / 24 fps)
    static const int32_t glut_window_width = 300;
    static const int32_t glut_window_height = 300;
    static const int32_t glut_window_position_x = 900;
    static const int32_t glut_window_position_y = 200;

    Visualizer() {}
    ~Visualizer() {}
    Visualizer(const Visualizer & other) = delete;
    Visualizer & operator=(const Visualizer & other) = delete;

    // Function for glutDisplayFunc.
    static void DrawScene() {
        glClear(GL_COLOR_BUFFER_BIT);
        CurrentScene::Instance().Draw();
        glFlush();
    }

    // Function for glutTimerFunc.
    static void GlutTimerFunction(int n_timer) {
        glutTimerFunc(kMsForFrame, GlutTimerFunction, n_timer);
    }

    static void InitializeGlut() {
        std::string glut_window_name = "Window_Name";
        glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
        glutInitWindowSize(glut_window_width, glut_window_height);

        glutInitWindowPosition(glut_window_position_x, glut_window_position_y);
        glutCreateWindow(glut_window_name.c_str());
        glClearColor(0.0, 0.0, 0.0, 0.0); // black
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0.0, 10, 0.0, 10, -1.0, 1.0);
        glutDisplayFunc(DrawScene);
        // glutFullScreen();
        // glutReshapeWindow(300, 300);
        // glutKeyboardFunc(myGlutKeyboardFunc);
        // glutSpecialFunc(myGlutSpecialFunc);
        glutTimerFunc(0, GlutTimerFunction, 0);
    }

};
