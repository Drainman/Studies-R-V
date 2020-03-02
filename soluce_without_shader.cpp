/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
|                                                           |
|           Projet : Graphique et Visualisation             |
|                   Corentin TROADEC                        |
|                      Mars 2020                            |
|                                                           |
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


/* - - - - - - - - - - -
|   INCLUDES & LIB     |
- - - - - - - - - - - */

/* OPENGL LIB */
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm\ext\matrix_transform.hpp>

/* STANDARD LIB */
#include <iostream>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>  
#include <math.h>
#include <time.h> 
#include <map>

/* EXT LIB */
#include "nlohmann/json.hpp"


/* - - - - - - - - - - -
|       DEFINES        |
- - - - - - - - - - - */
#define SCREEN_WIDTH 1280 //1280
#define SCREEN_HEIGHT 720 //720

#define MAX_POS_X 1280
#define MAX_POS_Y 720

using json = nlohmann::json;

/* - - - - - - - - - - -
|PROTOTYPE (w! circles)|
- - - - - - - - - - - */
void drawLineColorAndWidth(GLfloat lineVertices[], GLfloat lineColor[], int width);
void drawLine_GLCOLOR(GLfloat lineVertices[], GLfloat lineColor[], int width);
void testParse();
int getIntToJSON(json o);
void drawAllLines();
std::vector<GLfloat> generateRandomPos();
std::vector<GLfloat> randomColorForLine();
void lookConsole();
void drawAllLinesEN_DIS();
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
bool cursorIsInCircle(double x, double y);
void drawLines(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2);



/* - - - - - - - - - - - - - - - -
|       CLASS : CircleNode        |
- - - - - - - - - - - - - - - - -*/
class CircleNode {
public:
    std::vector<GLfloat> position;
    std::vector<CircleNode*> connex_position;
    std::vector<GLfloat> color;
    std::string label;
    bool isEnable;
    bool isConnected;

    CircleNode() { color = randomColorForLine(); isEnable = false; isConnected = false; }
    CircleNode(std::vector<GLfloat> setPos) { position = setPos; color = randomColorForLine(); isEnable = false; isConnected = false; }

    /*
    * @desc : Display this node
    */
    std::string toString() {
        std::string strC = "[NODE-PARENT] > " + label + " (" + std::to_string(position[0]) + ";" + std::to_string(position[1]) + ") - " + std::to_string(position[2]) + "\n";
        strC += "[NODE-CHILDS] : \n";
        for (auto cex : connex_position) {
            strC += "\t * " + cex->label + " > (" + std::to_string(cex->position[0]) + ";" + std::to_string(cex->position[1]) + ") - " + std::to_string(cex->position[2]) + "\n";
        }
        return strC;
    }

    /*
    * @desc Change the state of all the connected node on disconnected to connected.
    */
    void setConnect() {
        for (auto cex : connex_position) { cex->isConnected = true; }
    }

    /*
    * @desc Change the state of all the connected node on connected to disconnected.
    */
    void setDisconnect() {
        for (auto cex : connex_position) { cex->isConnected = false; }
    }
};
/* - - - - - - - - - - - - - - - -
|    END CLASS : CircleNode      |
- - - - - - - - - - - - - - - - -*/




/* - - - - - - - - - - - -
| PROTOTYPE (w circles) |
- - - - - - - - - - - - */
void drawCircle(GLfloat x, GLfloat y, GLfloat z, GLfloat radius, GLint numberOfSides, CircleNode c);
void drawCircleTest(GLfloat x, GLfloat y, GLfloat z, GLfloat radius, GLint numberOfSides, CircleNode c);
void enableACircle(CircleNode* c);



/* - - - - - - - - - - -
|      GLOBAL VARS     |
- - - - - - - - - - - */
std::vector<std::vector<GLfloat>> listPosCercles;
std::map<std::string, CircleNode> circlesPos;
GLfloat CIRCLE_ENABLE_COLOR[] = { 0.96,0.45 ,0.501 };
GLfloat CIRCLE_DISABLE_COLOR[] = { 0.46 ,0.855 ,0.678 };
GLfloat LINE_DISABLE_COLOR[] = { 0,0,255 };
CircleNode* currentEnable;
CircleNode* findingNode;



/* - - - - - - - - - - - - - - - -
|         BEGIN : MAIN            |
- - - - - - - - - - - - - - - - -*/
int main(void)
{
    srand(time(NULL)); //enable random

    GLFWwindow* window;

    // Initialize the library
    if (!glfwInit()) { return -1; }

    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Rusted Engine", NULL, NULL);


    if (!window) {
        glfwTerminate();
        return -1;
    }

    // Make the window's context current & cursor callBack
    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, cursor_position_callback);

    //Init all the parameter about our IHM
    glViewport(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT, 0, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //Load our data from the output file
    testParse();

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {

        //Clear previous elements
        glClear(GL_COLOR_BUFFER_BIT | GL_ACCUM_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // rendering
        drawAllLinesEN_DIS();
        glClear(GL_ACCUM_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        for (auto vecCircle : circlesPos) {
            drawCircleTest(vecCircle.second.position[0], vecCircle.second.position[1], 0, vecCircle.second.position[2], 30, vecCircle.second);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();

        //lookConsole(); // <----- UNCOMMENT IF YOU WANT USE THE CONSOLE MOD (not recommanded)
    }
    //end loop

    glfwTerminate();
    return 0;
}
/* - - - - - - - - - - - - - - - -
|          END : MAIN            |
- - - - - - - - - - - - - - - - -*/



void testParse() {

    std::ifstream i("output.json");
    if (i.fail()) { std::cout << "[ERROR] ~ Cannot load source file." << std::endl; }
    else {
        std::cout << "[SUCCESS] ~ Loading source file is ok." << std::endl;

        try {
            json j;
            i >> j;
            json allnodes;
            allnodes = j["nodes"];

            int cpt = 0;
            for (auto node : allnodes) {

                //Tri des noeuds dont le nombre de connections est nulle
                if (node["nb_connections"] != "0") {

                    int init_weight = getIntToJSON(node["weight"]);
                    int norm_weight = log(init_weight);


                    GLfloat rX = rand() % MAX_POS_X;
                    GLfloat rY = rand() % MAX_POS_Y;
                    std::vector<GLfloat> thisNodeCircle;
                    thisNodeCircle.push_back(rX);
                    thisNodeCircle.push_back(rY);
                    thisNodeCircle.push_back(norm_weight);
                    listPosCercles.push_back(thisNodeCircle);

                    std::string str_label = node["label"].dump();
                    std::string str_label_clean = str_label.substr(1, str_label.size() - 2);

                    if (circlesPos.find(str_label_clean) == circlesPos.end()) {
                        std::cout << "[CREATION] ~ " << str_label_clean << std::endl;
                        CircleNode c;
                        c.position = thisNodeCircle;
                        c.label = str_label;
                        circlesPos[str_label_clean] = c;
                    }
                    else {
                        circlesPos[str_label_clean].position[2] = norm_weight;
                    }


                    //Parcours les liste de connexions
                    for (auto connex : node["list_connections"]) {

                        std::string str_co = connex.dump();
                        std::string str_clean_co = str_co.substr(1, str_co.size() - 2);


                        if (circlesPos.find(str_clean_co) == circlesPos.end()) {
                            //N'existe pas
                            std::cout << "[CREATION] ~ " << str_clean_co << std::endl;
                            CircleNode cb;
                            GLfloat rX = rand() % SCREEN_WIDTH;
                            GLfloat rY = rand() % SCREEN_HEIGHT;
                            std::vector<GLfloat> v_nodeConnexion;
                            v_nodeConnexion.push_back(rX);
                            v_nodeConnexion.push_back(rY);
                            v_nodeConnexion.push_back(0);
                            cb.position = v_nodeConnexion;
                            cb.label = str_clean_co;
                            circlesPos[str_clean_co] = cb;
                        }

                        //Désactiver les doublons
                        circlesPos[str_clean_co].connex_position.push_back(&circlesPos[str_label_clean]);
                        //circlesPos[str_label_clean].connex_position.push_back(&circlesPos[str_clean_co]);
                    }
                    cpt++;
                }
            }

            currentEnable = &(circlesPos.begin()->second);
            std::cout << "[INFO] ~ Nombre de noeuds à placer : " << std::to_string(cpt) << std::endl;
            std::cout << "[SUCCESS] ~ End of data loading." << std::endl;

        }
        catch (std::exception & e) { std::cerr << e.what() << std::endl; }
    }

}

std::vector<GLfloat> generateRandomPos() {
    GLfloat rX = rand() % SCREEN_WIDTH;
    GLfloat rY = rand() % SCREEN_HEIGHT;
    std::vector<GLfloat> randv;
    randv.push_back(rX);
    randv.push_back(rY);
    return randv;
}

int getIntToJSON(json o) {
    std::string str = o.dump();
    std::string str_clean = str.substr(1, str.size() - 2);
    int init_weight = stoi(str_clean);
    return init_weight;
}


void drawCircleTest(GLfloat x, GLfloat y, GLfloat z, GLfloat radius, GLint numberOfSides, CircleNode c)
{
    int const numberOfVertices = numberOfSides + 2;

    GLfloat twicePi = 2.0f * 3.14159265359;

    std::vector<GLfloat> circleVerticesX(numberOfVertices);
    std::vector<GLfloat> circleVerticesY(numberOfVertices);
    std::vector<GLfloat> circleVerticesZ(numberOfVertices);

    circleVerticesX[0] = x;
    circleVerticesY[0] = y;
    circleVerticesZ[0] = z;
    static const GLfloat c_color[] = { 0.9,0.9,0.9 };

    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

    glBegin(GL_TRIANGLE_FAN);


    for (int i = 1; i < numberOfVertices; i++)
    {
        circleVerticesX[i] = x + (radius * cos(i * twicePi / numberOfSides));
        circleVerticesY[i] = y + (radius * sin(i * twicePi / numberOfSides));
        circleVerticesZ[i] = z;
        glVertex3f(circleVerticesX[i], circleVerticesY[i], circleVerticesZ[i]);
        // glColor3f(c.color[0], c.color[1], c.color[2]);
        if (c.isEnable)
            glColor3fv(CIRCLE_ENABLE_COLOR);
        else if (c.isConnected)
            glColor3f(c.color[0], c.color[1], c.color[2]);
        else
            glColor3fv(CIRCLE_DISABLE_COLOR);
    }

    glEnd();
}


void drawLineColorAndWidth(GLfloat lineVertices[], GLfloat lineColor[], int width) {


    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_LINE_STIPPLE);
    glPushAttrib(GL_LINE_BIT);
    glLineWidth(width);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, lineVertices);

    glColorPointer(3, GL_UNSIGNED_BYTE, sizeof(lineColor), lineColor);
    glEnableClientState(GL_COLOR_ARRAY);
    //glColor3f(255, 12, 255);

    glDrawArrays(GL_LINES, 0, 2);
    glDisableClientState(GL_VERTEX_ARRAY);
    glPopAttrib();
    glDisable(GL_LINE_STIPPLE);
    glDisable(GL_LINE_SMOOTH);
    glDisableClientState(GL_COLOR_ARRAY);
}

void drawLine_GLCOLOR(GLfloat lineVertices[], GLfloat lineColor[], int width) {
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_LINE_STIPPLE);
    glPushAttrib(GL_LINE_BIT);
    glLineWidth(width);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, lineVertices);

    glColorPointer(3, GL_FLOAT, 0, lineColor);
    glEnableClientState(GL_COLOR_ARRAY);
    //glColor3f(255, 12, 255);

    glDrawArrays(GL_LINES, 0, 2);
    glDisableClientState(GL_VERTEX_ARRAY);
    glPopAttrib();
    glDisable(GL_LINE_STIPPLE);
    glDisable(GL_LINE_SMOOTH);
    glDisableClientState(GL_COLOR_ARRAY);
}

void drawAllLines() {

    for (auto pos1 : circlesPos) {

        for (auto pos2 : pos1.second.connex_position) {

            GLfloat lineVertices[] = {
                pos1.second.position[0] , pos1.second.position[1], 0,
                pos2->position[0], pos2->position[1],0
            };


            std::vector<GLfloat> rcolor;
            if (pos1.second.position[2] > pos2->position[2]) {
                rcolor = pos1.second.color;
            }
            else { rcolor = pos2->color; }

            rcolor = pos1.second.color;

            GLfloat lineColor[] = {
                rcolor[0],rcolor[1],rcolor[2],
                rcolor[0],rcolor[1],rcolor[2],
                rcolor[0],rcolor[1],rcolor[2]
            };

            drawLine_GLCOLOR(lineVertices, lineColor, 1);
        }

    }
}



void drawAllLinesEN_DIS() {

    for (auto pos1 : circlesPos) {

        for (auto pos2 : pos1.second.connex_position) {

            //Positions de la ligne
            GLfloat lineVertices[] = {
                pos1.second.position[0] , pos1.second.position[1], 0,
                pos2->position[0], pos2->position[1],0
            };


            std::vector<GLfloat> rcolor;
            //If one of the two connected node is enable, color the line in red
            if (pos1.second.isEnable || pos2->isEnable) {
                rcolor.push_back(CIRCLE_ENABLE_COLOR[0]);
                rcolor.push_back(CIRCLE_ENABLE_COLOR[1]);
                rcolor.push_back(CIRCLE_ENABLE_COLOR[2]);
            }
            //Else color line in the default value
            else {
                rcolor.push_back(LINE_DISABLE_COLOR[0]);
                rcolor.push_back(LINE_DISABLE_COLOR[1]);
                rcolor.push_back(LINE_DISABLE_COLOR[2]);
            }

            GLfloat lineColor[] = {
                rcolor[0],rcolor[1],rcolor[2],
                rcolor[0],rcolor[1],rcolor[2],
                rcolor[0],rcolor[1],rcolor[2]
            };
            //Draw the indididual line
            drawLine_GLCOLOR(lineVertices, lineColor, 1);
        }
    }
}


std::vector<GLfloat> randomColorForLine() {
    GLfloat R = rand() % 100;
    GLfloat G = rand() % 100;
    GLfloat B = rand() % 100;

    std::vector<GLfloat> randColor;
    for (int i = 0; i < 3; i++) {
        randColor.push_back(R / 100);
        randColor.push_back(G / 100);
        randColor.push_back(B / 100);
    }
    return randColor;
}

void lookConsole() {
    std::string myString;
    std::cout << "[INPUT] > ";
    std::getline(std::cin, myString);

    if (circlesPos.find(myString) != circlesPos.end()) {
        //Disable old circle
        std::cout << "[DISABLE] > Enable displaying circle : " << currentEnable->label << std::endl;
        currentEnable->isEnable = false;
        currentEnable->setDisconnect();

        //Enable new circle
        std::cout << "[ENALBE] > Enable displaying circle : " << myString << std::endl;
        std::cout << circlesPos[myString].toString() << std::endl;
        circlesPos[myString].isEnable = true;
        circlesPos[myString].setConnect();
        currentEnable = &circlesPos[myString];
    }

    else if (myString == "exit")
        exit(0);

    else
        std::cout << "[WARNING] > Node \"" << myString << "\" doesn't exist in the model." << std::endl;
}


static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    //change the y position to look
    ypos = abs(ypos - SCREEN_HEIGHT);

    try {
        if (cursorIsInCircle(xpos, ypos))
            enableACircle(findingNode);
    }
    catch (...) {
        std::cout << "[ERROR] ~ Cannot display this node." << std::endl;
    }

}


bool cursorIsInCircle(double x, double y) {

    bool find = false;
    CircleNode* findCircle;

    for (auto c : circlesPos) {

        int calc = pow(x - c.second.position[0], 2) + pow(y - c.second.position[1], 2);
        int rad2 = pow(c.second.position[2], 2);

        if (calc <= rad2) {
            find = true;
            findingNode = &circlesPos[c.second.label];
            break;
        }
    }

    return find;
}

void enableACircle(CircleNode* c) {
    //Disable old circle
    std::cout << "[DISABLE] > Enable displaying circle : " << currentEnable->label << std::endl;
    currentEnable->isEnable = false;
    currentEnable->setDisconnect();

    //Enable new circle
    std::cout << "[ENALBE] > Enable displaying circle : " << c->label << std::endl;
    std::cout << c->toString() << std::endl;
    c->isEnable = true;
    c->setConnect();
    currentEnable = c;
}