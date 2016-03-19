#pragma once
#include <glut.h>
#include <string>
#include <vector>

// Realizes the command template for Visualizer.
class VisualizerCommand {
public:

private:

};

// Realizes the class can be taked by Visualizer for
// initializing.
class VisualizerInitializingCommand : public VisualizerCommand {
public:
    VisualizerInitializingCommand() {}

private:

};

// Realizes the class can be taked by Visualizer for
// updating.
class VisualizerUpdatingCommand : public VisualizerCommand {
public:
    VisualizerUpdatingCommand() {}

private:

};

// Transforms string into visualizer command.
class StringParser {
public:
    StringParser(std::string string) : string_(string) {}

    VisualizerInitializingCommand ParseIntoVisualizerInitializingCommand() {
        VisualizerInitializingCommand command;
        // future realization depends on the form of command
    }

    VisualizerUpdatingCommand ParseIntoVisualizerUpdatingCommand() {
        VisualizerUpdatingCommand command;
        // future realization depends on the form of command
    }

private:
    // Splits string by " "
    std::vector<std::string> Split() {
        std::vector<std::string> strings;

        return strings;
    }

    std::string string_;
};

// Realizes the drawer using singleton.
class Visualizer {
public:
    static Visualizer & Instance() {
        static Visualizer visualizer;
        return visualizer;
    }
    static void Initialize(std::string string_command) {
        StringParser parser(string_command);
        //auto command = parser.ParseIntoVisualizerInitializingCommand();
        
    }
    static void Visualize() {
        myGlutInit();
        glutDisplayFunc(DisplayFunc);
        glutMainLoop();
    }
private:
    // Realizes one game field drawn on the computer screen.
    class GameField {

    };

    static std::vector<GameField> game_fields_;

    Visualizer() {}
    ~Visualizer() {}
    Visualizer(const Visualizer & other) {}
    Visualizer & operator=(const Visualizer & other) {}

    static void Update(std::string string_command) {
        StringParser parser(string_command);
        auto command = parser.ParseIntoVisualizerUpdatingCommand();

    }

    static void DisplayFunc() {
        glClear(GL_COLOR_BUFFER_BIT);
        // glColor3f(1.0, 1.0, 1.0);

        glFlush();
    }
    static void myGlutInit() {
        int WinWid = 400;
        int WinHei = 400;
        glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
        glutInitWindowSize(WinWid, WinHei);
        
        //void glutReshapeWindow(int width, int height)// оконный режим
        glutInitWindowPosition(0, 0);
        glutCreateWindow("Window");
        glClearColor(1.0, 1.0, 1.0, 0.0);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);/*выбор режима прорисовки
                                                  полигонов. выбран дефолтный.*/
        glMatrixMode(GL_PROJECTION); //Установить проекцию
        glLoadIdentity();
        // glutFullScreen(); // полноэкранный режим
        glutReshapeWindow(1300, 800);
        // int GameFieldWid = 200;
        // int GameFieldHei = 300;
        // glOrtho(0.0, GameFieldWid, 0.0, GameFieldHei, -1.0, 1.0);
        // glutKeyboardFunc(myGlutKeyboardFunc);
        //glutSpecialFunc(myGlutSpecialFunc);
    }
};