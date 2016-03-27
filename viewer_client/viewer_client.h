#pragma once
#include <glut.h>
#include <string>
#include <vector>
#include <thread>
#include <mutex>

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
        std::cout << "green_block.Draw();" << std::endl;
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
    class VisualizerCommandParser {
    public:
        VisualizerCommandParser(std::string string) : string_(string) {}

        VisualizerInitializingCommand ParseIntoVisualizerInitializingCommand() {
            VisualizerInitializingCommand command;
            // future realization depends on the form of command
        }

        VisualizerUpdatingCommand ParseIntoVisualizerUpdatingCommand() {
            VisualizerUpdatingCommand command;
            // future realization depends on the form of command
        }

    private:
        // Splits string by delimiter
        std::vector<std::string> Split(char delimiter) {
            std::vector<std::string> strings;

            return strings;
        }

        std::string string_;
    };

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
