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
#include <glad/glad.h>
//#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm\ext\matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>

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
void testParse();
int getIntToJSON(json o);
void lookConsole();
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
bool cursorIsInCircle(double x, double y);
std::vector<GLfloat> randomColorForLine();


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
void enableACircle(CircleNode* c);
void drawCircleWithShader(GLfloat x, GLfloat y, GLfloat z, GLfloat radius, GLint numberOfSides);


/* - - - - - - - - - - -
|      GLOBAL VARS     |
- - - - - - - - - - - */
std::vector<std::vector<GLfloat>> listPosCercles;
std::map<std::string, CircleNode> circlesPos;
GLfloat CIRCLE_ENABLE_COLOR[] = { 255,0,0 };
GLfloat CIRCLE_DISABLE_COLOR[] = { 0,255,0 };
GLfloat LINE_DISABLE_COLOR[] = { 0,0,255 };
CircleNode* currentEnable;
CircleNode* findingNode;


//LIGTH
void drawCircleShader(float x, float y, float rad, int numberVertices);
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

std::vector<float> linesVertices;
std::vector<float> circleVerticesAll;
std::vector<float> circleShaderAll;
unsigned int VBO, cercleVAO;

/* - - - - - - - - - - - - - - - -
|         BEGIN : MAIN            |
- - - - - - - - - - - - - - - - -*/
int main(void)
{
    srand(time(NULL)); //enable random

    //test
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //test

    GLFWwindow* window;

    // Initialize the library
    if (!glfwInit()) { return -1; }

    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Rusted Engine", NULL, NULL);


    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Make the window's context current & cursor callBack
    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, cursor_position_callback);

    //Load our data from the output file
    testParse();
    // std::cout << "[TEST] ~ " << linesVertices[0] << std::endl;


     //ADD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    std::cout << "[SIZE] > " << linesVertices.size() << std::endl;

    std::vector<unsigned int> blablaColor;

    int j = 0; std::string t_s = "";

    for (int i = 0; i < linesVertices.size(); i++) {
        if ((i + 1) % 3 == 0) {
            blablaColor.push_back(j); j++;
        }

    }

    float verticesT[] = {
         0.157175,-0.516328, 0.0f,  // top right
         0.999634, -0.504914, 0.0f
    };

    unsigned int indices[] = {  // note that we start from 0!
        0, 1  // first Triangle
    };

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(linesVertices[0]) * linesVertices.size(), &linesVertices.front(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(blablaColor[0]) * blablaColor.size(), &blablaColor.front(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {

        //Clear previous elements
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_ACCUM_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_LINES, sizeof(linesVertices[0])* linesVertices.size(), GL_UNSIGNED_INT, 0);

        // rendering
  
        glClear(GL_ACCUM_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        for (auto vecCircle : circlesPos) {
           // drawCircleWithShader(vecCircle.second.position[0], vecCircle.second.position[1], 0, vecCircle.second.position[2], 30);
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

            GLfloat tX;
            GLfloat tY;

            int cpt = 0;
            for (auto node : allnodes) {

                //Tri des noeuds dont le nombre de connections est nulle
                if (node["nb_connections"] != "0") {

                    int init_weight = getIntToJSON(node["weight"]);
                    int norm_weight = log(init_weight);


                    GLfloat rX;// = rand() % MAX_POS_X;
                    GLfloat rY;// = rand() % MAX_POS_Y;

                    tX = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 2)) - 1;
                    tY = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 2)) - 1;
                    
                    rX = tX; rY = tY;

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


                            GLfloat rX = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 2)) - 1;
                            GLfloat rY = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 2)) - 1;

                            //ADD - LINES
                            linesVertices.push_back(tX);
                            linesVertices.push_back(tY);
                            linesVertices.push_back(0.0f);

                            linesVertices.push_back(rX);
                            linesVertices.push_back(rY);
                            linesVertices.push_back(0.0f);
                            //ADD

                            std::vector<GLfloat> v_nodeConnexion;
                            v_nodeConnexion.push_back(rX);
                            v_nodeConnexion.push_back(rY);
                            v_nodeConnexion.push_back(0);
                            cb.position = v_nodeConnexion;
                            cb.label = str_clean_co;
                            circlesPos[str_clean_co] = cb;

                            //Push vertices
                            circleVerticesAll.push_back(rX);
                            circleVerticesAll.push_back(rY);
                            circleVerticesAll.push_back(0);

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

        float calc = pow(x - c.second.position[0], 2) + pow(y - c.second.position[1], 2);
        float rad2 = pow(c.second.position[2], 2);

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



void drawCircleWithShader(GLfloat x, GLfloat y, GLfloat z, GLfloat radius, GLint numberOfSides)
{
    radius = 0.0025;
    int const numberOfVertices = numberOfSides + 2;

    GLfloat twicePi = 2.0f * 3.14159265359;

    std::vector<GLfloat> circleVerticesX(numberOfVertices);
    std::vector<GLfloat> circleVerticesY(numberOfVertices);
    std::vector<GLfloat> circleVerticesZ(numberOfVertices);

    circleVerticesX[0] = x;
    circleVerticesY[0] = y;
    circleVerticesZ[0] = z;

    for (int i = 1; i < numberOfVertices; i++)
    {
        circleVerticesX[i] = x + (radius * cos(i * twicePi / numberOfSides));
        circleVerticesY[i] = y + (radius * sin(i * twicePi / numberOfSides));
        circleVerticesZ[i] = z;
    }


    float* allCircleVertices = new float[numberOfVertices * 3];


    for (int i = 0; i < numberOfVertices; i++)
    {
        circleShaderAll.push_back(circleVerticesX[i]);
        circleShaderAll.push_back(circleVerticesY[i]);
        circleShaderAll.push_back(circleVerticesZ[i]);
    }

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(circleShaderAll[0]) * circleShaderAll.size(), &circleShaderAll.front(), GL_STATIC_DRAW);
    glDrawArrays(GL_TRIANGLE_FAN, 0, sizeof(circleShaderAll[0]) * circleShaderAll.size());

}