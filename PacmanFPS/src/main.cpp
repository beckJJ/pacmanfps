//     Universidade Federal do Rio Grande do Sul
//             Instituto de Informática
//       Departamento de Informática Aplicada
//
//    INF01047 Fundamentos de Computação Gráfica
//               Prof. Eduardo Gastal
//
//           TRABALHO FINAL - PACMAN FPS
//

// Arquivos "headers" padrões de C podem ser incluídos em um
// programa C++, sendo necessário somente adicionar o caractere
// "c" antes de seu nome, e remover o sufixo ".h". Exemplo:
//    #include <stdio.h> // Em C
//  vira
//    #include <cstdio> // Em C++
//
#include <cmath>
#include <cstdio>
#include <cstdlib>
#define _WIN32_WINNT 0x0500
#include <windows.h>
// Headers abaixo são específicos de C++
#include <map>
#include <stack>
#include <string>
#include <vector>
#include <limits>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <iostream>

// Headers das bibliotecas OpenGL
#include <glad/glad.h>   // Criação de contexto OpenGL 3.3
#include <GLFW/glfw3.h>  // Criação de janelas do sistema operacional

// Headers da biblioteca GLM: criação de matrizes e vetores.
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>

// Headers da biblioteca para carregar modelos obj
#include <tiny_obj_loader.h>

#include <stb_image.h>

// Headers locais, definidos na pasta "include/"
#include "utils.h"
#include "matrices.h"
#include "collisions.h"
#include "structs.h"

#define MAXLIGHTS 500

// Estrutura que representa um modelo geométrico carregado a partir de um
// arquivo ".obj". Veja https://en.wikipedia.org/wiki/Wavefront_.obj_file .
struct ObjModel
{
    tinyobj::attrib_t                 attrib;
    std::vector<tinyobj::shape_t>     shapes;
    std::vector<tinyobj::material_t>  materials;

    // Este construtor lê o modelo de um arquivo utilizando a biblioteca tinyobjloader.
    // Veja: https://github.com/syoyo/tinyobjloader
    ObjModel(const char* filename, const char* basepath = NULL, bool triangulate = true)
    {
        printf("Carregando modelo \"%s\"... ", filename);

        std::string err;
        bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filename, basepath, triangulate);

        if (!err.empty())
            fprintf(stderr, "\n%s\n", err.c_str());

        if (!ret)
            throw std::runtime_error("Erro ao carregar modelo.");

        printf("OK.\n");
    }
};


// Declaração de funções utilizadas para pilha de matrizes de modelagem.
void PushMatrix(glm::mat4 M);
void PopMatrix(glm::mat4& M);

// Declaração de várias funções utilizadas em main().  Essas estão definidas
// logo após a definição de main() neste arquivo.
void BuildTrianglesAndAddToVirtualScene(ObjModel*); // Constrói representação de um ObjModel como malha de triângulos para renderização
void ComputeNormals(ObjModel* model); // Computa normais de um ObjModel, caso não existam.
void LoadShadersFromFiles(); // Carrega os shaders de vértice e fragmento, criando um programa de GPU
void LoadTextureImage(const char* filename); // Função que carrega imagens de textura
void DrawVirtualObject(const char* object_name); // Desenha um objeto armazenado em g_VirtualScene
GLuint LoadShader_Vertex(const char* filename);   // Carrega um vertex shader
GLuint LoadShader_Fragment(const char* filename); // Carrega um fragment shader
void LoadShader(const char* filename, GLuint shader_id); // Função utilizada pelas duas acima
GLuint CreateGpuProgram(GLuint vertex_shader_id, GLuint fragment_shader_id); // Cria um programa de GPU
void PrintObjModelInfo(ObjModel*); // Função para debugging

// Declaração de funções auxiliares para renderizar texto dentro da janela
// OpenGL. Estas funções estão definidas no arquivo "textrendering.cpp".
void TextRendering_Init();
float TextRendering_LineHeight(GLFWwindow* window);
float TextRendering_CharWidth(GLFWwindow* window);
void TextRendering_PrintString(GLFWwindow* window, const std::string &str, float x, float y, float scale = 1.0f);
void TextRendering_PrintMatrix(GLFWwindow* window, glm::mat4 M, float x, float y, float scale = 1.0f);
void TextRendering_PrintVector(GLFWwindow* window, glm::vec4 v, float x, float y, float scale = 1.0f);
void TextRendering_PrintMatrixVectorProduct(GLFWwindow* window, glm::mat4 M, glm::vec4 v, float x, float y, float scale = 1.0f);
void TextRendering_PrintMatrixVectorProductMoreDigits(GLFWwindow* window, glm::mat4 M, glm::vec4 v, float x, float y, float scale = 1.0f);
void TextRendering_PrintMatrixVectorProductDivW(GLFWwindow* window, glm::mat4 M, glm::vec4 v, float x, float y, float scale = 1.0f);
void TextRendering_InitialScreen(GLFWwindow* window);

// Funções abaixo renderizam como texto na janela OpenGL algumas matrizes e
// outras informações do programa. Definidas após main().
void TextRendering_ShowModelViewProjection(GLFWwindow* window, glm::mat4 projection, glm::mat4 view, glm::mat4 model, glm::vec4 p_model);
void TextRendering_ShowEulerAngles(GLFWwindow* window);
void TextRendering_ShowProjection(GLFWwindow* window);
void TextRendering_ShowFramesPerSecond(GLFWwindow* window);
void TextRendering_ShowPositionCamera(GLFWwindow* window);

// Funções callback para comunicação com o sistema operacional e interação do
// usuário. Veja mais comentários nas definições das mesmas, abaixo.
void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
void ErrorCallback(int error, const char* description);
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void CursorPosCallback(GLFWwindow* window, double xpos, double ypos);
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

// Definimos uma estrutura que armazenará dados necessários para renderizar
// cada objeto da cena virtual.
struct SceneObject
{
    std::string  name;                   // Nome do objeto
    size_t       first_index;            // Índice do primeiro vértice dentro do vetor indices[] definido em BuildTrianglesAndAddToVirtualScene()
    size_t       num_indices;            // Número de índices do objeto dentro do vetor indices[] definido em BuildTrianglesAndAddToVirtualScene()
    GLenum       rendering_mode;         // Modo de rasterização (GL_TRIANGLES, GL_TRIANGLE_STRIP, etc.)
    GLuint       vertex_array_object_id; // ID do VAO onde estão armazenados os atributos do modelo
    glm::vec3    bbox_min; // Axis-Aligned Bounding Box do objeto
    glm::vec3    bbox_max;
};

void InitGhost(); // Função que inicializa as posições dos ghosts;
void InitPointsHalls();
void InitPoints();
void InitPowerPoints();

glm::vec4 CalculateGhostPosition(const char* ghost_name); // Função que calcula a nova posição do ghost
float CalculateGhostRotationY(const char* ghost_name); // calcula a rotação em torno do eixo Y que deve ser aplicado no ghost
int GetNextCheckpoint(Ghost ghost); // Função que retorna o checkpoint posterior ao atual do ghost
int GetPreviousCheckpoint(Ghost ghost); // Função que retorna o checkpoint anterior ao atual do ghost
void UpdateGhostWay(const char* ghost_name); // Função que atualiza o sentido que o ghost irá se locomover
void CalculatePowerPointPosition(); // Função que calcula a nova posição de um Power Point
void InsertLightsPositions(int pointer, glm::vec4 position); // Função que insere nova posição de uma lux no array


// Abaixo definimos variáveis globais utilizadas em várias funções do código.

// A cena virtual é uma lista de objetos nomeados, guardados em um dicionário
// (map).  Veja dentro da função BuildTrianglesAndAddToVirtualScene() como que são incluídos
// objetos dentro da variável g_VirtualScene, e veja na função main() como
// estes são acessados.
std::map<std::string, SceneObject> g_VirtualScene;

// Lista de ghosts no mapa
std::map<std::string, Ghost> g_Ghosts;

// Lista de corredores do mapa
std::map<int, PointsHall> g_PointsHalls;

// Lista de Points no mapa
std::map<int, Point> g_Points;

// Lista de power points
std::map<int, Power> g_PowerPoints;

// Pilha que guardará as matrizes de modelagem.
std::stack<glm::mat4>  g_MatrixStack;

// Razão de proporção da janela (largura/altura). Veja função FramebufferSizeCallback().
float g_ScreenRatio = 1.0f;

// Ângulos de Euler que controlam a rotação de um dos cubos da cena virtual
float g_AngleX = 0.0f;
float g_AngleY = 0.0f;
float g_AngleZ = 0.0f;

// "g_LeftMouseButtonPressed = true" se o usuário está com o botão esquerdo do mouse
// pressionado no momento atual. Veja função MouseButtonCallback().
bool g_LeftMouseButtonPressed = false;
bool g_RightMouseButtonPressed = false; // Análogo para botão direito do mouse
bool g_MiddleMouseButtonPressed = false; // Análogo para botão do meio do mouse

bool g_ThirdPerson = false;

// Variáveis que definem a câmera em coordenadas esféricas, controladas pelo
// usuário através do mouse (veja função CursorPosCallback()). A posição
// efetiva da câmera é calculada dentro da função main(), dentro do loop de
// renderização.
float g_CameraTheta = -3.1415f / 2.0f; // Ângulo no plano ZX em relação ao eixo Z
float g_CameraPhi = 0.0f;   // Ângulo em relação ao eixo Y
float g_CameraDistance = 35.0f; // Distância da câmera para a origem

// Variáveis que controlam rotação do antebraço
float g_ForearmAngleZ = 0.0f;
float g_ForearmAngleX = 0.0f;

// Variáveis que controlam translação do torso
float g_TorsoPositionX = 0.0f;
float g_TorsoPositionY = 0.0f;

// Variável que controla o tipo de projeção utilizada: perspectiva ou ortográfica.
bool g_UsePerspectiveProjection = true;

// Variável que controla se o texto informativo será mostrado na tela.
bool g_ShowInfoText = true;

// Variavel que controla o inicio do jogo
bool g_StartGame = false;

// Variaveis que controlam o fim do jogo
bool g_EndGame = false;
bool g_GameWon = false;

// Variáveis que definem um programa de GPU (shaders). Veja função LoadShadersFromFiles().
GLuint vertex_shader_id;
GLuint fragment_shader_id;
GLuint program_id = 0;
GLint model_uniform;
GLint view_uniform;
GLint projection_uniform;
GLint object_id_uniform;
GLint bbox_min_uniform;
GLint bbox_max_uniform;
GLint lightspositions_uniform;
GLint total_lights_uniform;

// Array das posições das luzes a serem carregadas
glm::vec4 lightspositions[MAXLIGHTS];
// Total de luzes a serem carregadas
int total_lights = 0;

bool tecla_W_pressionada = false;
bool tecla_A_pressionada = false;
bool tecla_S_pressionada = false;
bool tecla_D_pressionada = false;
bool tecla_ESPACO_pressionada = false;

float delta_t = 0.0f;
float delta_t_ghosts = 0.0f;

extern glm::vec4 vetor_u;
extern glm::vec4 vetor_w;

// Variáveis globais que armazenam a última posição do cursor do mouse, para
// que possamos calcular quanto que o mouse se movimentou entre dois instantes
// de tempo. Utilizadas no callback CursorPosCallback() abaixo.
double g_LastCursorPosX, g_LastCursorPosY;

// Abaixo definimos as varáveis que efetivamente definem a câmera virtual.
// Veja slides 195-227 e 229-234 do documento Aula_08_Sistemas_de_Coordenadas.pdf.
//glm::vec4 camera_position_c  = glm::vec4(0.0f, 20.0f, g_CameraDistance, 1.0f); // Ponto "c", centro da câmera
glm::vec4 pacman_position_c  = glm::vec4(-13.0f, 1.0f, 0.0f, 1.0f); // Ponto central do pacman
glm::vec4 camera_position_c  = glm::vec4(pacman_position_c.x, pacman_position_c.y, pacman_position_c.z, 1.0f); // Ponto "c", centro da câmera

// Ajuste para posicionar os ghosts nas posições corretas do mapa
glm::vec4 adjust_position_ghosts = glm::vec4(5.0f, 0.0f, 3.5f, 0.0f);

// Tamanho padrão dos ghosts
glm::vec3 ghosts_scale = glm::vec3(1.5f, 1.5f, 1.5f);

// Número de texturas carregadas pela função LoadTextureImage()
GLuint g_NumLoadedTextures = 0;

// Variaveis de rotação para os Power Points
float rotate_x = 3.1415f * 1.5f;
float rotate_z = 0.0f;

int main(int argc, char* argv[])
{
    // esconder o terminal
  //  ShowWindow(GetConsoleWindow(), SW_HIDE);

    // Inicializamos a biblioteca GLFW, utilizada para criar uma janela do
    // sistema operacional, onde poderemos renderizar com OpenGL.
    int success = glfwInit();
    if (!success)
    {
        fprintf(stderr, "ERROR: glfwInit() failed.\n");
        std::exit(EXIT_FAILURE);
    }

    // Definimos o callback para impressão de erros da GLFW no terminal
    glfwSetErrorCallback(ErrorCallback);

    // Pedimos para utilizar OpenGL versão 3.3 (ou superior)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    // Pedimos para utilizar o perfil "core", isto é, utilizaremos somente as
    // funções modernas de OpenGL.
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Criamos uma janela do sistema operacional, com 800 colunas e 600 linhas
    // de pixels, e com título "INF01047 ...".
    GLFWwindow* window;
    window = glfwCreateWindow(800, 600, "INF01047 - TRABALHO FINAL - PACMAN FPS", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        fprintf(stderr, "ERROR: glfwCreateWindow() failed.\n");
        std::exit(EXIT_FAILURE);
    }

    // Definimos a função de callback que será chamada sempre que o usuário
    // pressionar alguma tecla do teclado ...
    glfwSetKeyCallback(window, KeyCallback);
    // ... ou clicar os botões do mouse ...
    glfwSetMouseButtonCallback(window, MouseButtonCallback);
    // ... ou movimentar o cursor do mouse em cima da janela ...
    glfwSetCursorPosCallback(window, CursorPosCallback);
    // ... ou rolar a "rodinha" do mouse.
    glfwSetScrollCallback(window, ScrollCallback);

    // Indicamos que as chamadas OpenGL deverão renderizar nesta janela
    glfwMakeContextCurrent(window);

    // Carregamento de todas funções definidas por OpenGL 3.3, utilizando a
    // biblioteca GLAD.
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

    // Definimos a função de callback que será chamada sempre que a janela for
    // redimensionada, por consequência alterando o tamanho do "framebuffer"
    // (região de memória onde são armazenados os pixels da imagem).
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
    FramebufferSizeCallback(window, 800, 600); // Forçamos a chamada do callback acima, para definir g_ScreenRatio.

    // Imprimimos no terminal informações sobre a GPU do sistema
    const GLubyte *vendor      = glGetString(GL_VENDOR);
    const GLubyte *renderer    = glGetString(GL_RENDERER);
    const GLubyte *glversion   = glGetString(GL_VERSION);
    const GLubyte *glslversion = glGetString(GL_SHADING_LANGUAGE_VERSION);

    printf("GPU: %s, %s, OpenGL %s, GLSL %s\n", vendor, renderer, glversion, glslversion);

    // Carregamos os shaders de vértices e de fragmentos que serão utilizados
    // para renderização. Veja slides 180-200 do documento Aula_03_Rendering_Pipeline_Grafico.pdf.
    //
    LoadShadersFromFiles();

    // Carregamos duas imagens para serem utilizadas como textura
    LoadTextureImage("../../data/tc-earth_daymap_surface.jpg");      // TextureImage0
    LoadTextureImage("../../data/tc-earth_nightmap_citylights.gif"); // TextureImage1
    LoadTextureImage("../../data/textures/BlackMarble/Black_marble_02/Textures_4K/black_marble_02_4k_baseColor.tga"); // TextureImage2
    //LoadTextureImage("../../data/textures/Bricks/textures3/brick_4/brick_4_diff_2k.jpg"); // TextureImage3
    //LoadTextureImage("../../data/textures/Bricks/textures3/brick_4/brick_4_AO_2k.jpg"); // TextureImage3
    LoadTextureImage("../../data/textures/Bricks/textures3/brick_4/brick_4_spec_2k.jpg"); // TextureImage3
    //LoadTextureImage("../../data/textures/Bricks/textures3/red_brick_03/red_brick_03_diff_2k.jpg"); // TextureImage3

    // Construímos a representação de objetos geométricos através de malhas de triângulos
    ObjModel spheremodel("../../data/sphere.obj");
    ComputeNormals(&spheremodel);
    BuildTrianglesAndAddToVirtualScene(&spheremodel);


    ObjModel planemodel("../../data/plane.obj");
    ComputeNormals(&planemodel);
    BuildTrianglesAndAddToVirtualScene(&planemodel);


    // FONTE: modelo de labirinto baixado adquirido em:
    // https: //sketchfab.com/3d-models/pac-man-9b2fd5bc82ba4212895fd0b753a4df09
    ObjModel mazemodel("../../data/maze/source/TEXTURES/MAZE.obj", "../../data/maze/source/TEXTURES/");
    ComputeNormals(&mazemodel);
    BuildTrianglesAndAddToVirtualScene(&mazemodel);


    // FONTE: modelo de labirinto baixado adquirido em:
    // https: https://www.cgtrader.com/items/3696643/download-page
    ObjModel ghostsmodel("../../data/ghosts.obj", "../../data/");
    ComputeNormals(&ghostsmodel);
    BuildTrianglesAndAddToVirtualScene(&ghostsmodel);


    // FONTE: modelo de estrela baixado adquirido em:
    // https: https://free3d.com/pt/3d-model/star-v1--713680.html
    ObjModel starmodel("../../data/star/21327_Star_v1.obj", "../../data/star/");
    ComputeNormals(&starmodel);
    BuildTrianglesAndAddToVirtualScene(&starmodel);


    InitGhost();
    InitPointsHalls();
    InitPoints();
    InitPowerPoints();

    if ( argc > 1 )
    {
        ObjModel model(argv[1]);
        BuildTrianglesAndAddToVirtualScene(&model);
    }

    // Inicializamos o código para renderização de texto.
    TextRendering_Init();

    // Habilitamos o Z-buffer. Veja slides 104-116 do documento Aula_09_Projecoes.pdf.
    glEnable(GL_DEPTH_TEST);

    // Habilitamos o Backface Culling. Veja slides 23-34 do documento Aula_13_Clipping_and_Culling.pdf.
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    float speed = 5.0f; // Velocidade da câmera
    float prev_time = (float)glfwGetTime();
    float prev_time_ghosts = (float)glfwGetTime();
    float prev_time_bezier = (float)glfwGetTime();

    // Ficamos em loop, renderizando, até que o usuário feche a janela
    while (!glfwWindowShouldClose(window))
    {
        // Aqui executamos as operações de renderização

        // Definimos a cor do "fundo" do framebuffer como branco.  Tal cor é
        // definida como coeficientes RGBA: Red, Green, Blue, Alpha; isto é:
        // Vermelho, Verde, Azul, Alpha (valor de transparência).
        // Conversaremos sobre sistemas de cores nas aulas de Modelos de Iluminação.
        //
        //           R     G     B     A
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        // "Pintamos" todos os pixels do framebuffer com a cor definida acima,
        // e também resetamos todos os pixels do Z-buffer (depth buffer).
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Pedimos para a GPU utilizar o programa de GPU criado acima (contendo
        // os shaders de vértice e fragmentos).
        glUseProgram(program_id);

        // Se não foi clicado Enter para o jogo iniciar
        if (!g_StartGame) {
            TextRendering_InitialScreen(window);
        } else {
            if (g_EndGame) {
                system("pause");
                exit(1);
            }
            // Atualiza delta de tempo
            float current_time = (float)glfwGetTime();
            delta_t = current_time - prev_time;
            prev_time = current_time;

            // Vetor que projeta w no plano X,Z, evitando que a camera oscile de altura
            //glm::vec4 projected_w  = glm::vec4(vetor_w.x, vetor_w.y, vetor_w.z, vetor_w.w); // usado quando se quer andar livremente pelo mapa, podendo subir e descer na altura
            glm::vec4 projected_w  = glm::vec4(vetor_w.x, 0.0f, vetor_w.z, vetor_w.w);    // usado para andar na altura certa no labirinto, não podendo mudar a altura
            projected_w /= norm(projected_w);
            // Realiza movimentação de objetos
            if (tecla_W_pressionada) {
                // Movimenta pacman para frente
                glm::vec4 new_position = pacman_position_c - projected_w * speed * delta_t;

                if (!CollisionMaze(new_position)) {
                    pacman_position_c = new_position;
                }
                if (CollisionTeleport(new_position)) {
                    // teleportar o pacman para o outro lado do labirinto
                    pacman_position_c.z *= -1;
                }
                CollisionPoints(pacman_position_c, &g_Points);
            }

            if (tecla_A_pressionada) {
                // Movimenta pacman para esquerda
                glm::vec4 new_position = pacman_position_c - vetor_u * speed * delta_t;
                if (!CollisionMaze(new_position)) {
                    pacman_position_c = new_position;
                }
                if (CollisionTeleport(new_position)) {
                    // teleportar o pacman para o outro lado do labirinto
                    pacman_position_c.z *= -1;
                }
                CollisionPoints(pacman_position_c, &g_Points);
            }

            if (tecla_S_pressionada) {
                // Movimenta pacman para trás
                glm::vec4 new_position = pacman_position_c + projected_w * speed * delta_t;
                if (!CollisionMaze(new_position)) {
                    pacman_position_c = new_position;
                }
                if (CollisionTeleport(new_position)) {
                    // teleportar o pacman para o outro lado do labirinto
                    pacman_position_c.z *= -1;
                }
                CollisionPoints(pacman_position_c, &g_Points);
            }

            if (tecla_D_pressionada) {
                // Movimenta pacman para direita
                glm::vec4 new_position = pacman_position_c + vetor_u * speed * delta_t;
                if (!CollisionMaze(new_position)) {
                    pacman_position_c = new_position;
                }
                if (CollisionTeleport(new_position)) {
                    // teleportar o pacman para o outro lado do labirinto
                    pacman_position_c.z *= -1;
                }
                CollisionPoints(pacman_position_c, &g_Points);
            }
            if (tecla_ESPACO_pressionada) {
                //Movimenta pacman para cima
                glm::vec4 new_position = pacman_position_c + glm::vec4(0.0f, 1.0f, 0.0f, 0.0f) * speed * delta_t;
                if (!CollisionMaze(new_position) && !CollisionPlanes(new_position)) {
                    pacman_position_c = new_position;
                }
            } else {
                glm::vec4 new_position = pacman_position_c - glm::vec4(0.0f, 1.0f, 0.0f, 0.0f) * speed * delta_t;
                if (!CollisionMaze(new_position) && !CollisionPlanes(new_position)) {
                    pacman_position_c = new_position;
                }
            }

            // Calcula colisões com fantasmas
            if (CollisionGhostsPacman(pacman_position_c, g_Ghosts)) {
                // reseta posição do pacman para posição inicial
                pacman_position_c  = glm::vec4(-13.0f, 1.0f, 0.0f, 1.0f);
            }

            // Calcula as coordenadas de câmera
            glm::vec4 camera_view_vector;

            float v_x = cos(g_CameraPhi) * sin(g_CameraTheta);
            float v_y = sin(g_CameraPhi);
            float v_z = cos(g_CameraPhi) * cos(g_CameraTheta);

            // Se estiver em terceira pessoa
            if (g_ThirdPerson) {
                camera_position_c.x = pacman_position_c.x + g_CameraDistance * v_x;
                camera_position_c.y = g_CameraDistance;
                camera_position_c.z = pacman_position_c.z + g_CameraDistance * v_z;

                camera_view_vector = pacman_position_c - camera_position_c ;
            } else { // se estiver em primeira pessoa
                camera_position_c = pacman_position_c;
                camera_view_vector = glm::vec4(-v_x, v_y, -v_z, 0.0f);
            }

            glm::vec4 camera_up_vector   = glm::vec4(0.0f,1.0f,0.0f,0.0f); // Vetor "up" fixado para apontar para o "céu" (eito Y global)

            // Computamos a matriz "View" utilizando os parâmetros da câmera para
            // definir o sistema de coordenadas da câmera.  Veja slides 2-14, 184-190 e 236-242 do documento Aula_08_Sistemas_de_Coordenadas.pdf.
            glm::mat4 view = Matrix_Camera_View(camera_position_c, camera_view_vector, camera_up_vector);

            // Agora computamos a matriz de Projeção.
            glm::mat4 projection;

            // Note que, no sistema de coordenadas da câmera, os planos near e far
            // estão no sentido negativo! Veja slides 176-204 do documento Aula_09_Projecoes.pdf.
            float nearplane = -0.1f;  // Posição do "near plane"
            float farplane  = -100.0f; // Posição do "far plane"

            if (g_UsePerspectiveProjection)
            {
                // Projeção Perspectiva.
                // Para definição do field of view (FOV), veja slides 205-215 do documento Aula_09_Projecoes.pdf.
                float field_of_view = 3.141592 / 3.0f;
                projection = Matrix_Perspective(field_of_view, g_ScreenRatio, nearplane, farplane);
            }
            else
            {
                // Projeção Ortográfica.
                // Para definição dos valores l, r, b, t ("left", "right", "bottom", "top"),
                // PARA PROJEÇÃO ORTOGRÁFICA veja slides 219-224 do documento Aula_09_Projecoes.pdf.
                // Para simular um "zoom" ortográfico, computamos o valor de "t"
                // utilizando a variável g_CameraDistance.
                float t = 1.5f*g_CameraDistance/2.5f;
                float b = -t;
                float r = t*g_ScreenRatio;
                float l = -r;
                projection = Matrix_Orthographic(l, r, b, t, nearplane, farplane);
            }

            glm::mat4 model = Matrix_Identity(); // Transformação identidade de modelagem

            // Enviamos as matrizes "view" e "projection" para a placa de vídeo
            // (GPU). Veja o arquivo "shader_vertex.glsl", onde estas são
            // efetivamente aplicadas em todos os pontos.
            glUniformMatrix4fv(view_uniform       , 1 , GL_FALSE , glm::value_ptr(view));
            glUniformMatrix4fv(projection_uniform , 1 , GL_FALSE , glm::value_ptr(projection));
            glUniform4fv(lightspositions_uniform, sizeof(lightspositions), glm::value_ptr(lightspositions[0]));
            glUniform1i(total_lights_uniform, total_lights);

            #define SPHERE       0
            #define BUNNY        1
            #define PLANE        2
            #define MAZE         3
            #define GHOST_BLINKY 4
            #define GHOST_CLYDE  5
            #define GHOST_INKY   6
            #define GHOST_PINKY  7
            #define POINT        8
            #define POWER        9


            // Desenhamos o modelo da esfera
            model = Matrix_Translate(pacman_position_c.x,pacman_position_c.y,pacman_position_c.z)
                  * Matrix_Scale(1.5f, 1.5f, 1.5f);
            glUniformMatrix4fv(model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
            glUniform1i(object_id_uniform, SPHERE);
            DrawVirtualObject("sphere");

            // Desenhamos o modelo do plano
            model = Matrix_Translate(0.0f,-1.0f,0.0f)
                  * Matrix_Scale(40.0f, 1.0f, 40.0f);
            glUniformMatrix4fv(model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
            glUniform1i(object_id_uniform, PLANE);
            DrawVirtualObject("plane");

            // Desenhamos o modelo do labirinto
            model = Matrix_Translate(0.0f,-2.0f,0.0f)
                  * Matrix_Scale(15.0f, 25.0f, 15.0f)
                  * Matrix_Rotate_Y(3.1415 * 1.5f);
            glUniformMatrix4fv(model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
            glUniform1i(object_id_uniform, MAZE);
            DrawVirtualObject("maze");

            // Desenhamos os modelos dos ghosts
            float actual_time_ghosts = (float)glfwGetTime();
            delta_t_ghosts = (actual_time_ghosts - prev_time_ghosts) * 6;
            prev_time_ghosts = actual_time_ghosts;


            // Blinky Ghost
            glm::vec4 new_ghost_position = CalculateGhostPosition("Blinky_Sphere.005");
            float new_ghost_rotateY      = CalculateGhostRotationY("Blinky_Sphere.005");
            Ghost ghost = g_Ghosts["Blinky_Sphere.005"];
            ghost.current_position = new_ghost_position;
            g_Ghosts["Blinky_Sphere.005"] = ghost;
            model = Matrix_Translate(new_ghost_position.x, new_ghost_position.y, new_ghost_position.z)
                  * Matrix_Scale(ghosts_scale.x, ghosts_scale.y, ghosts_scale.z)
                  * Matrix_Rotate_Y(new_ghost_rotateY)
                  * Matrix_Translate(ghost.centralization.x, ghost.centralization.y, ghost.centralization.z);

            glUniformMatrix4fv(model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
            glUniform1i(object_id_uniform, GHOST_BLINKY);
            DrawVirtualObject("Blinky_Sphere.005");


            // Clyde Ghost
            new_ghost_position = CalculateGhostPosition("Clyde_Sphere.013");
            new_ghost_rotateY  = CalculateGhostRotationY("Clyde_Sphere.013");
            ghost = g_Ghosts["Clyde_Sphere.013"];
            ghost.current_position = new_ghost_position;
            g_Ghosts["Clyde_Sphere.013"] = ghost;
            model = Matrix_Translate(new_ghost_position.x, new_ghost_position.y, new_ghost_position.z)
                  * Matrix_Scale(ghosts_scale.x, ghosts_scale.y, ghosts_scale.z)
                  * Matrix_Rotate_Y(new_ghost_rotateY)
                  * Matrix_Translate(ghost.centralization.x, ghost.centralization.y, ghost.centralization.z);

            glUniformMatrix4fv(model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
            glUniform1i(object_id_uniform, GHOST_CLYDE);
            DrawVirtualObject("Clyde_Sphere.013");


            // Inky Ghost
            new_ghost_position = CalculateGhostPosition("Inky_Sphere.021");
            new_ghost_rotateY  = CalculateGhostRotationY("Inky_Sphere.021");
            ghost = g_Ghosts["Inky_Sphere.021"];
            ghost.current_position = new_ghost_position;
            g_Ghosts["Inky_Sphere.021"] = ghost;
            model = Matrix_Translate(new_ghost_position.x, new_ghost_position.y, new_ghost_position.z)
                  * Matrix_Scale(ghosts_scale.x, ghosts_scale.y, ghosts_scale.z)
                  * Matrix_Rotate_Y(new_ghost_rotateY)
                  * Matrix_Translate(ghost.centralization.x, ghost.centralization.y, ghost.centralization.z);

            glUniformMatrix4fv(model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
            glUniform1i(object_id_uniform, GHOST_INKY);
            DrawVirtualObject("Inky_Sphere.021");


            // Pinky Ghost
            new_ghost_position = CalculateGhostPosition("Pinky_Sphere.022");
            new_ghost_rotateY  = CalculateGhostRotationY("Pinky_Sphere.022");
            ghost = g_Ghosts["Pinky_Sphere.022"];
            ghost.current_position = new_ghost_position;
            g_Ghosts["Pinky_Sphere.022"] = ghost;
            model = Matrix_Translate(new_ghost_position.x, new_ghost_position.y, new_ghost_position.z)
                  * Matrix_Scale(ghosts_scale.x, ghosts_scale.y, ghosts_scale.z)
                  * Matrix_Rotate_Y(new_ghost_rotateY)
                  * Matrix_Translate(ghost.centralization.x, ghost.centralization.y, ghost.centralization.z);

            glUniformMatrix4fv(model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
            glUniform1i(object_id_uniform, GHOST_PINKY);
            DrawVirtualObject("Pinky_Sphere.022");


            std::map<int, Point>::iterator it  = g_Points.begin();
            std::map<int, Power>::iterator it2 = g_PowerPoints.begin();

            // Points
            int num_not_taken = 0;
            while (it != g_Points.end()) {
                Point point = it->second;

                if (!point.taken) {
                    num_not_taken++;
                    model = Matrix_Translate(point.position.x, point.position.y, point.position.z)
                          * Matrix_Scale(0.25f, 0.25f, 0.25f);

                    glUniformMatrix4fv(model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
                    glUniform1i(object_id_uniform, POINT);
                    DrawVirtualObject("sphere");
                }
                it++;
            }
            if (num_not_taken == 0) {
                g_EndGame = true;
                printf("You won The Game!\n");
            }

            // POWER POINT
            float actual_time_bezier = (float)glfwGetTime();

            if ((actual_time_bezier - prev_time_bezier) >= 0.01f) {
                CalculatePowerPointPosition();
                rotate_z += 0.1f;
                prev_time_bezier = actual_time_bezier;
            }

            while (it2 != g_PowerPoints.end()) {
                Power power = it2->second;

                model = Matrix_Translate(power.actual_position.x, power.actual_position.y, power.actual_position.z)
                      * Matrix_Scale(0.3f, 0.3f, 0.3f)
                      * Matrix_Rotate_X(rotate_x)
                      * Matrix_Rotate_Z(rotate_z);

                glUniformMatrix4fv(model_uniform, 1 , GL_FALSE , glm::value_ptr(model));
                glUniform1i(object_id_uniform, POWER);
                DrawVirtualObject("21327_Star_v1");

                it2++;
            }


            // Imprimimos na tela os ângulos de Euler que controlam a rotação do
            // terceiro cubo.
            //TextRendering_ShowEulerAngles(window);

            TextRendering_ShowPositionCamera(window);

            // Imprimimos na informação sobre a matriz de projeção sendo utilizada.
            TextRendering_ShowProjection(window);

            // Imprimimos na tela informação sobre o número de quadros renderizados
            // por segundo (frames per second).
            TextRendering_ShowFramesPerSecond(window);

            // TextRendering_ShowModelViewProjection(window, projection, view, view, camera_position_c);
        }

        // O framebuffer onde OpenGL executa as operações de renderização não
        // é o mesmo que está sendo mostrado para o usuário, caso contrário
        // seria possível ver artefatos conhecidos como "screen tearing". A
        // chamada abaixo faz a troca dos buffers, mostrando para o usuário
        // tudo que foi renderizado pelas funções acima.
        // Veja o link: Veja o link: https://en.wikipedia.org/w/index.php?title=Multiple_buffering&oldid=793452829#Double_buffering_in_computer_graphics
        glfwSwapBuffers(window);

        // Verificamos com o sistema operacional se houve alguma interação do
        // usuário (teclado, mouse, ...). Caso positivo, as funções de callback
        // definidas anteriormente usando glfwSet*Callback() serão chamadas
        // pela biblioteca GLFW.
        glfwPollEvents();
    }

    // Finalizamos o uso dos recursos do sistema operacional
    glfwTerminate();

    // mostrar o terminal
    ShowWindow(GetConsoleWindow(), SW_SHOW);

    // Fim do programa
    return 0;
}


// Função que inicializa os ghosts e os devidos checkpoints dos mesmos
void InitGhost() {
    Ghost theghost;
    theghost.name = "Blinky_Sphere.005";
    theghost.way  = 1;
    theghost.delta_t = 0.0f;
    theghost.checkpoint = 0;
    theghost.num_points = 18;
    theghost.centralization = glm::vec3(0.6f, 0.0f, -0.8f);
    theghost.positions[0] = glm::vec4(  0.0f,  -3.0f,   0.0f, 1.0f) + adjust_position_ghosts;
    theghost.positions[1] = glm::vec4(  5.0f,  -3.0f,   0.0f, 1.0f) + adjust_position_ghosts;
    theghost.positions[2] = glm::vec4(  5.0f,  -3.0f,   5.0f, 1.0f) + adjust_position_ghosts;
    theghost.positions[3] = glm::vec4(-12.0f,  -3.0f,   5.0f, 1.0f) + adjust_position_ghosts;
    theghost.positions[4] = glm::vec4(-12.0f,  -3.0f,  20.0f, 1.0f) + adjust_position_ghosts;
    theghost.positions[5] = glm::vec4(-17.0f,  -3.0f,  20.0f, 1.0f) + adjust_position_ghosts;

    theghost.positions[6]  = glm::vec4(-17.0f, -3.0f,  16.0f, 1.0f) + adjust_position_ghosts;
    theghost.positions[7]  = glm::vec4(-23.0f, -3.0f,  16.0f, 1.0f) + adjust_position_ghosts;
    theghost.positions[8]  = glm::vec4(-23.0f, -3.0f,  20.0f, 1.0f) + adjust_position_ghosts;
    theghost.positions[9]  = glm::vec4(-28.0f, -3.0f,  20.0f, 1.0f) + adjust_position_ghosts;
    theghost.positions[10] = glm::vec4(-28.0f, -3.0f,  -1.0f, 1.0f) + adjust_position_ghosts;
    theghost.positions[11] = glm::vec4(-23.0f, -3.0f,  -1.0f, 1.0f) + adjust_position_ghosts;

    theghost.positions[12] = glm::vec4(-23.0f, -3.0f,   5.0f, 1.0f) + adjust_position_ghosts;
    theghost.positions[13] = glm::vec4(-17.0f, -3.0f,   5.0f, 1.0f) + adjust_position_ghosts;
    theghost.positions[14] = glm::vec4(-17.0f, -3.0f,  -6.0f, 1.0f) + adjust_position_ghosts;
    theghost.positions[15] = glm::vec4(-12.0f, -3.0f,  -6.0f, 1.0f) + adjust_position_ghosts;
    theghost.positions[16] = glm::vec4(-12.0f, -3.0f, -12.0f, 1.0f) + adjust_position_ghosts;
    theghost.positions[17] = glm::vec4(  5.0f, -3.0f, -12.0f, 1.0f) + adjust_position_ghosts;

    g_Ghosts[theghost.name] = theghost;

    theghost.name = "Clyde_Sphere.013";
    theghost.way  = 1;
    theghost.delta_t = 0.0f;
    theghost.checkpoint = 0;
    theghost.num_points = 20;
    theghost.centralization = glm::vec3(0.6f, 0.0f, 1.73f);
    theghost.positions[0] = glm::vec4( 0.0f,  -3.0f,  -2.5f, 1.0f) + adjust_position_ghosts;
    theghost.positions[1] = glm::vec4( 5.0f,  -3.0f,  -2.5f, 1.0f) + adjust_position_ghosts;
    theghost.positions[2] = glm::vec4( 5.0f,  -3.0f,   5.0f, 1.0f) + adjust_position_ghosts;
    theghost.positions[3] = glm::vec4( 0.0f,  -3.0f,   5.0f, 1.0f) + adjust_position_ghosts;
    theghost.positions[4] = glm::vec4( 0.0f,  -3.0f,  10.0f, 1.0f) + adjust_position_ghosts;
    theghost.positions[5] = glm::vec4(23.0f,  -3.0f,  10.0f, 1.0f) + adjust_position_ghosts;

    theghost.positions[6]  = glm::vec4(23.0f,  -3.0f,   0.0f, 1.0f) + adjust_position_ghosts;
    theghost.positions[7]  = glm::vec4(16.0f,  -3.0f,   0.0f, 1.0f) + adjust_position_ghosts;
    theghost.positions[8]  = glm::vec4(16.0f,  -3.0f,   5.0f, 1.0f) + adjust_position_ghosts;
    theghost.positions[9]  = glm::vec4(10.0f,  -3.0f,   5.0f, 1.0f) + adjust_position_ghosts;
    theghost.positions[10] = glm::vec4(10.0f,  -3.0f,  -1.0f, 1.0f) + adjust_position_ghosts;
    theghost.positions[11] = glm::vec4( 5.0f,  -3.0f,  -1.0f, 1.0f) + adjust_position_ghosts;

    theghost.positions[12] = glm::vec4(  5.0f,  -3.0f,  -12.0f, 1.0f) + adjust_position_ghosts;
    theghost.positions[13] = glm::vec4( -1.0f,  -3.0f,  -12.0f, 1.0f) + adjust_position_ghosts;
    theghost.positions[14] = glm::vec4( -1.0f,  -3.0f,  -17.0f, 1.0f) + adjust_position_ghosts;
    theghost.positions[15] = glm::vec4(-17.0f,  -3.0f,  -17.0f, 1.0f) + adjust_position_ghosts;
    theghost.positions[16] = glm::vec4(-17.0f,  -3.0f,   -6.0f, 1.0f) + adjust_position_ghosts;
    theghost.positions[17] = glm::vec4(-12.0f,  -3.0f,   -6.0f, 1.0f) + adjust_position_ghosts;

    theghost.positions[18] = glm::vec4(-12.0f,  -3.0f,  -12.0f, 1.0f) + adjust_position_ghosts;
    theghost.positions[19] = glm::vec4(  5.0f,  -3.0f,  -12.0f, 1.0f) + adjust_position_ghosts;

    g_Ghosts[theghost.name] = theghost;

    theghost.name = "Inky_Sphere.021";
    theghost.way  = 1;
    theghost.delta_t = 0.0f;
    theghost.checkpoint = 0;
    theghost.num_points = 21;
    theghost.centralization = glm::vec3(0.6f, 0.0f, 4.5f);
    theghost.positions[0] = glm::vec4( 0.0f,  -3.0f,  -5.0f, 1.0f) + adjust_position_ghosts;
    theghost.positions[1] = glm::vec4(10.0f,  -3.0f,  -5.0f, 1.0f) + adjust_position_ghosts;
    theghost.positions[2] = glm::vec4(10.0f,  -3.0f, -11.0f, 1.0f) + adjust_position_ghosts;
    theghost.positions[3] = glm::vec4(16.0f,  -3.0f, -11.0f, 1.0f) + adjust_position_ghosts;
    theghost.positions[4] = glm::vec4(16.0f,  -3.0f,  -6.0f, 1.0f) + adjust_position_ghosts;
    theghost.positions[5] = glm::vec4(23.0f,  -3.0f,  -6.0f, 1.0f) + adjust_position_ghosts;

    theghost.positions[6]  = glm::vec4( 23.0f,  -3.0f,  -27.0f, 1.0f) + adjust_position_ghosts;
    theghost.positions[7]  = glm::vec4( 16.0f,  -3.0f,  -27.0f, 1.0f) + adjust_position_ghosts;
    theghost.positions[8]  = glm::vec4( 16.0f,  -3.0f,  -17.0f, 1.0f) + adjust_position_ghosts;
    theghost.positions[9]  = glm::vec4(-23.0f,  -3.0f,  -17.0f, 1.0f) + adjust_position_ghosts;
    theghost.positions[10] = glm::vec4(-23.0f,  -3.0f,  -27.0f, 1.0f) + adjust_position_ghosts;
    theghost.positions[11] = glm::vec4(-28.0f,  -3.0f,  -27.0f, 1.0f) + adjust_position_ghosts;

    theghost.positions[12] = glm::vec4(-28.0f,  -3.0f,  -6.0f, 1.0f) + adjust_position_ghosts;
    theghost.positions[13] = glm::vec4(-23.0f,  -3.0f,  -6.0f, 1.0f) + adjust_position_ghosts;
    theghost.positions[14] = glm::vec4(-23.0f,  -3.0f, -11.0f, 1.0f) + adjust_position_ghosts;
    theghost.positions[15] = glm::vec4(-17.0f,  -3.0f, -11.0f, 1.0f) + adjust_position_ghosts;
    theghost.positions[16] = glm::vec4(-17.0f,  -3.0f,  -6.0f, 1.0f) + adjust_position_ghosts;
    theghost.positions[17] = glm::vec4(-12.0f,  -3.0f,  -6.0f, 1.0f) + adjust_position_ghosts;

    theghost.positions[18] = glm::vec4(-12.0f,  -3.0f, -12.0f, 1.0f) + adjust_position_ghosts;
    theghost.positions[19] = glm::vec4(  5.0f,  -3.0f, -12.0f, 1.0f) + adjust_position_ghosts;
    theghost.positions[20] = glm::vec4(  5.0f,  -3.0f,  -5.0f, 1.0f) + adjust_position_ghosts;

    g_Ghosts[theghost.name] = theghost;

    theghost.name = "Pinky_Sphere.022";
    theghost.way  = 1;
    theghost.delta_t = 0.0f;
    theghost.checkpoint = 0;
    theghost.num_points = 16;
    theghost.centralization = glm::vec3(0.6f, 0.0f, 7.4f);
    theghost.positions[0] = glm::vec4(  0.0f,  -3.0f,  -7.5f, 1.0f) + adjust_position_ghosts;
    theghost.positions[1] = glm::vec4(  5.0f,  -3.0f,  -7.5f, 1.0f) + adjust_position_ghosts;
    theghost.positions[2] = glm::vec4(  5.0f,  -3.0f, -12.0f, 1.0f) + adjust_position_ghosts;
    theghost.positions[3] = glm::vec4(  0.0f,  -3.0f, -12.0f, 1.0f) + adjust_position_ghosts;
    theghost.positions[4] = glm::vec4(  0.0f,  -3.0f, -17.0f, 1.0f) + adjust_position_ghosts;
    theghost.positions[5] = glm::vec4(-12.0f,  -3.0f, -17.0f, 1.0f) + adjust_position_ghosts;

    theghost.positions[6]  = glm::vec4(-12.0f,  -3.0f, -27.0f, 1.0f) + adjust_position_ghosts;
    theghost.positions[7]  = glm::vec4(-17.0f,  -3.0f, -27.0f, 1.0f) + adjust_position_ghosts;
    theghost.positions[8]  = glm::vec4(-17.0f,  -3.0f, -23.0f, 1.0f) + adjust_position_ghosts;
    theghost.positions[9]  = glm::vec4(-23.0f,  -3.0f, -23.0f, 1.0f) + adjust_position_ghosts;
    theghost.positions[10] = glm::vec4(-23.0f,  -3.0f, -17.0f, 1.0f) + adjust_position_ghosts;
    theghost.positions[11] = glm::vec4(-17.0f,  -3.0f, -17.0f, 1.0f) + adjust_position_ghosts;

    theghost.positions[12] = glm::vec4(-17.0f,  -3.0f,  10.0f, 1.0f) + adjust_position_ghosts;
    theghost.positions[13] = glm::vec4(  0.0f,  -3.0f,  10.0f, 1.0f) + adjust_position_ghosts;
    theghost.positions[14] = glm::vec4(  0.0f,  -3.0f,   5.0f, 1.0f) + adjust_position_ghosts;
    theghost.positions[15] = glm::vec4(  5.0f,  -3.0f,   5.0f, 1.0f) + adjust_position_ghosts;

    g_Ghosts[theghost.name] = theghost;

}

void InitPointsHalls()
{
    PointsHall hall;

    hall.id = 0;
    hall.hall_begin = glm::vec4(21.0f, 0.0f, -23.0f, 1.0f);
    hall.hall_end   = glm::vec4(21.0f, 0.0f,  23.0f, 1.0f);
    g_PointsHalls[hall.id] = hall;

    hall.id = 1;
    hall.hall_begin = glm::vec4(-23.0f, 0.0f, -23.0f, 1.0f);
    hall.hall_end   = glm::vec4(-23.0f, 0.0f,  23.0f, 1.0f);
    g_PointsHalls[hall.id] = hall;

    hall.id = 2;
    hall.hall_begin = glm::vec4(-18.0f, 0.0f, -14.0f, 1.0f);
    hall.hall_end   = glm::vec4( 29.0f, 0.0f, -14.0f, 1.0f);
    g_PointsHalls[hall.id] = hall;

    hall.id = 3;
    hall.hall_begin = glm::vec4(-18.0f, 0.0f, 14.0f, 1.0f);
    hall.hall_end   = glm::vec4( 29.0f, 0.0f, 14.0f, 1.0f);
    g_PointsHalls[hall.id] = hall;

    hall.id = 4;
    hall.hall_begin = glm::vec4(-12.0f, 0.0f, -14.0f, 1.0f);
    hall.hall_end   = glm::vec4(-12.0f, 0.0f,  14.0f, 1.0f);
    g_PointsHalls[hall.id] = hall;

    hall.id = 5;
    hall.hall_begin = glm::vec4(-7.0f, 0.0f, -23.0f, 1.0f);
    hall.hall_end   = glm::vec4(-7.0f, 0.0f,  -2.0f, 1.0f);
    g_PointsHalls[hall.id] = hall;

    hall.id = 6;
    hall.hall_begin = glm::vec4(-7.0f, 0.0f,  3.0f, 1.0f);
    hall.hall_end   = glm::vec4(-7.0f, 0.0f, 24.0f, 1.0f);
    g_PointsHalls[hall.id] = hall;

    hall.id = 7;
    hall.hall_begin = glm::vec4(29.0f, 0.0f, -23.0f, 1.0f);
    hall.hall_end   = glm::vec4(29.0f, 0.0f,  -2.0f, 1.0f);
    g_PointsHalls[hall.id] = hall;

    hall.id = 8;
    hall.hall_begin = glm::vec4(29.0f, 0.0f,  3.0f, 1.0f);
    hall.hall_end   = glm::vec4(29.0f, 0.0f, 24.0f, 1.0f);
    g_PointsHalls[hall.id] = hall;

    hall.id = 9;
    hall.hall_begin = glm::vec4(15.0f, 0.0f, -23.0f, 1.0f);
    hall.hall_end   = glm::vec4(29.0f, 0.0f, -23.0f, 1.0f);
    g_PointsHalls[hall.id] = hall;

    hall.id = 10;
    hall.hall_begin = glm::vec4(15.0f, 0.0f, 23.0f, 1.0f);
    hall.hall_end   = glm::vec4(29.0f, 0.0f, 23.0f, 1.0f);
    g_PointsHalls[hall.id] = hall;

    hall.id = 11;
    hall.hall_begin = glm::vec4(15.0f, 0.0f, -23.0f, 1.0f);
    hall.hall_end   = glm::vec4(15.0f, 0.0f, -14.0f, 1.0f);
    g_PointsHalls[hall.id] = hall;

    hall.id = 12;
    hall.hall_begin = glm::vec4(15.0f, 0.0f, 15.0f, 1.0f);
    hall.hall_end   = glm::vec4(15.0f, 0.0f, 24.0f, 1.0f);
    g_PointsHalls[hall.id] = hall;

    hall.id = 13;
    hall.hall_begin = glm::vec4(-18.0f, 0.0f, -22.0f, 1.0f);
    hall.hall_end   = glm::vec4(-18.0f, 0.0f, -15.0f, 1.0f);
    g_PointsHalls[hall.id] = hall;

    hall.id = 14;
    hall.hall_begin = glm::vec4(-18.0f, 0.0f, 16.0f, 1.0f);
    hall.hall_end   = glm::vec4(-18.0f, 0.0f, 23.0f, 1.0f);
    g_PointsHalls[hall.id] = hall;

    hall.id = 15;
    hall.hall_begin = glm::vec4(-23.0f, 0.0f, -23.0f, 1.0f);
    hall.hall_end   = glm::vec4(-18.0f, 0.0f, -23.0f, 1.0f);
    g_PointsHalls[hall.id] = hall;

    hall.id = 16;
    hall.hall_begin = glm::vec4(-23.0f, 0.0f, 23.0f, 1.0f);
    hall.hall_end   = glm::vec4(-18.0f, 0.0f, 23.0f, 1.0f);
    g_PointsHalls[hall.id] = hall;

    hall.id = 17;
    hall.hall_begin = glm::vec4(23.0f, 0.0f, -3.0f, 1.0f);
    hall.hall_end   = glm::vec4(29.0f, 0.0f, -3.0f, 1.0f);
    g_PointsHalls[hall.id] = hall;

    hall.id = 18;
    hall.hall_begin = glm::vec4(23.0f, 0.0f, 3.0f, 1.0f);
    hall.hall_end   = glm::vec4(29.0f, 0.0f, 3.0f, 1.0f);
    g_PointsHalls[hall.id] = hall;

    hall.id = 19;
    hall.hall_begin = glm::vec4(-13.0f, 0.0f, -23.0f, 1.0f);
    hall.hall_end   = glm::vec4( -8.0f, 0.0f, -23.0f, 1.0f);
    g_PointsHalls[hall.id] = hall;

    hall.id = 20;
    hall.hall_begin = glm::vec4(-13.0f, 0.0f, 23.0f, 1.0f);
    hall.hall_end   = glm::vec4( -8.0f, 0.0f, 23.0f, 1.0f);
    g_PointsHalls[hall.id] = hall;

    hall.id = 21;
    hall.hall_begin = glm::vec4(15.0f, 0.0f, -8.0f, 1.0f);
    hall.hall_end   = glm::vec4(20.0f, 0.0f, -8.0f, 1.0f);
    g_PointsHalls[hall.id] = hall;

    hall.id = 22;
    hall.hall_begin = glm::vec4(15.0f, 0.0f, 8.0f, 1.0f);
    hall.hall_end   = glm::vec4(20.0f, 0.0f, 8.0f, 1.0f);
    g_PointsHalls[hall.id] = hall;

    hall.id = 23;
    hall.hall_begin = glm::vec4(15.0f, 0.0f, -8.0f, 1.0f);
    hall.hall_end   = glm::vec4(15.0f, 0.0f, -2.0f, 1.0f);
    g_PointsHalls[hall.id] = hall;

    hall.id = 24;
    hall.hall_begin = glm::vec4(15.0f, 0.0f, 2.0f, 1.0f);
    hall.hall_end   = glm::vec4(15.0f, 0.0f, 8.0f, 1.0f);
    g_PointsHalls[hall.id] = hall;

    hall.id = 25;
    hall.hall_begin = glm::vec4(-13.0f, 0.0f, -23.0f, 1.0f);
    hall.hall_end   = glm::vec4(-13.0f, 0.0f, -19.0f, 1.0f);
    g_PointsHalls[hall.id] = hall;

    hall.id = 26;
    hall.hall_begin = glm::vec4(-13.0f, 0.0f, 19.0f, 1.0f);
    hall.hall_end   = glm::vec4(-13.0f, 0.0f, 23.0f, 1.0f);
    g_PointsHalls[hall.id] = hall;

    hall.id = 27;
    hall.hall_begin = glm::vec4(-17.0f, 0.0f, -19.0f, 1.0f);
    hall.hall_end   = glm::vec4(-14.0f, 0.0f, -19.0f, 1.0f);
    g_PointsHalls[hall.id] = hall;

    hall.id = 28;
    hall.hall_begin = glm::vec4(-17.0f, 0.0f, 19.0f, 1.0f);
    hall.hall_end   = glm::vec4(-14.0f, 0.0f, 19.0f, 1.0f);
    g_PointsHalls[hall.id] = hall;

    hall.id = 29;
    hall.hall_begin = glm::vec4(-10.0f, 0.0f, -2.0f, 1.0f);
    hall.hall_end   = glm::vec4( -8.0f, 0.0f, -2.0f, 1.0f);
    g_PointsHalls[hall.id] = hall;

    hall.id = 30;
    hall.hall_begin = glm::vec4(-10.0f, 0.0f, 2.0f, 1.0f);
    hall.hall_end   = glm::vec4( -8.0f, 0.0f, 2.0f, 1.0f);
    g_PointsHalls[hall.id] = hall;

    hall.id = 31;
    hall.hall_begin = glm::vec4(-18.0f, 0.0f, -8.0f, 1.0f);
    hall.hall_end   = glm::vec4(-13.0f, 0.0f, -8.0f, 1.0f);
    g_PointsHalls[hall.id] = hall;

    hall.id = 32;
    hall.hall_begin = glm::vec4(-18.0f, 0.0f, 8.0f, 1.0f);
    hall.hall_end   = glm::vec4(-13.0f, 0.0f, 8.0f, 1.0f);
    g_PointsHalls[hall.id] = hall;

    hall.id = 33;
    hall.hall_begin = glm::vec4(-18.0f, 0.0f, -8.0f, 1.0f);
    hall.hall_end   = glm::vec4(-18.0f, 0.0f, -2.0f, 1.0f);
    g_PointsHalls[hall.id] = hall;

    hall.id = 34;
    hall.hall_begin = glm::vec4(-18.0f, 0.0f, 2.0f, 1.0f);
    hall.hall_end   = glm::vec4(-18.0f, 0.0f, 8.0f, 1.0f);
    g_PointsHalls[hall.id] = hall;

    hall.id = 35;
    hall.hall_begin = glm::vec4(-22.0f, 0.0f, -2.0f, 1.0f);
    hall.hall_end   = glm::vec4(-19.0f, 0.0f, -2.0f, 1.0f);
    g_PointsHalls[hall.id] = hall;

    hall.id = 36;
    hall.hall_begin = glm::vec4(-22.0f, 0.0f, 2.0f, 1.0f);
    hall.hall_end   = glm::vec4(-19.0f, 0.0f, 2.0f, 1.0f);
    g_PointsHalls[hall.id] = hall;
}

void InitPoints() {
    Point point;
    float steps       = 2.0f;
    float actual_step = 0.0;
    int actual_id     = 0;
    float point_height = 0.5f;

    std::map<int, PointsHall>::iterator it = g_PointsHalls.begin();

    while (it != g_PointsHalls.end()) {
        float initial_position_x = it->second.hall_begin.x;
        float initial_position_z = it->second.hall_begin.z;

        if (it->second.hall_begin.x != it->second.hall_end.x) {
            while ((initial_position_x + actual_step) <= it->second.hall_end.x) {
                point.id = actual_id;

                point.position = glm::vec4(initial_position_x + actual_step, point_height, initial_position_z, 1.0f);
                point.taken = 0;

                g_Points[point.id] = point;

                //InsertLightsPositions(point.id, point.position);

                actual_step += steps;
                actual_id++;
            }
        } else {
            while ((initial_position_z + actual_step) <= it->second.hall_end.z) {
                point.id = actual_id;

                point.position = glm::vec4(initial_position_x, point_height, initial_position_z + actual_step, 1.0f);
                point.taken = 0;

                g_Points[point.id] = point;

                //InsertLightsPositions(point.id, point.position);

                actual_step += steps;
                actual_id++;
            }
        }

        actual_step = 0.0f;
        it++;
    }
}

void InitPowerPoints()
{
    Power power;

    power.id    = 0;
    power.way   = 0;
    power.t     = 0;
    power.steps = 0.005f;
    power.actual_position = glm::vec4(0.0f,  0.0f,  0.0f, 1.0f);
    power.points[0]       = glm::vec4(-2.50f, -0.25f, 0.0f,  1.0f);
    power.points[1]       = glm::vec4(-1.75f,  2.00f, 0.0f,  1.0f);
    power.points[2]       = glm::vec4(-1.00f, -2.00f, 0.0f,  1.0f);
    power.points[3]       = glm::vec4(-0.25f,  0.25f, 0.0f,  1.0f);

    g_PowerPoints[power.id] = power;

    power.id    = 1;
    power.way   = 0;
    power.t     = 0;
    power.steps = 0.005f;
    power.actual_position = glm::vec4(0.0f,  0.0f,  0.0f, 1.0f);
    power.points[0]       = glm::vec4( 8.50f, -0.25f, 0.0f,  1.0f);
    power.points[1]       = glm::vec4( 9.25f,  2.00f, 0.0f,  1.0f);
    power.points[2]       = glm::vec4(10.00f, -2.00f, 0.0f,  1.0f);
    power.points[3]       = glm::vec4(10.75f,  0.25f, 0.0f,  1.0f);

    g_PowerPoints[power.id] = power;
}

// Função que retorna o checkpoint posterior ao atual do ghost
int GetNextCheckpoint(Ghost ghost) {
    int prox_checkpoint;

    // Se o checkpoint atual do ghost for o ultimo, o proximo checkpoint será o primeiro,
    // fazendo com que o ghost fique num processo circular
    if (ghost.num_points - 1 == ghost.checkpoint)
        prox_checkpoint = 1;
    else
        prox_checkpoint = ghost.checkpoint + 1;

    return prox_checkpoint;
}

// Função que retorna o checkpoint anterior ao atual do ghost
int GetPreviousCheckpoint(Ghost ghost) {
    int prev_checkpoint;

    // Se o checkpoint atual do ghost for o primeiro, o checkpoint anterior será o último,
    // fazendo com que o ghost fique num processo circular
    if (ghost.checkpoint == 0)
        prev_checkpoint = ghost.num_points - 1;
    else
        prev_checkpoint = ghost.checkpoint - 1;

    return prev_checkpoint;
}

// Função que atualiza o sentido de deslocamento do ghost
void UpdateGhostWay(const char* ghost_name) {
    Ghost ghost = g_Ghosts[ghost_name];
    int prox_checkpoint = GetNextCheckpoint(ghost);

    glm::vec4 origin  = ghost.positions[ghost.checkpoint];
    glm::vec4 destiny = ghost.positions[prox_checkpoint];

    if (origin.x != destiny.x) {
        if (destiny.x > origin.x) { // Se o ghost for se locomover no sentido positivo de X
            g_Ghosts[ghost.name].way = 1;
        } else { // Se o ghost for se locomover no sentido negativo de X
            g_Ghosts[ghost.name].way = 3;
        }
    } else if (origin.z != destiny.z) {
        if (destiny.z > origin.z) { // Se o ghost for se locomover no sentido positivo de Z
            g_Ghosts[ghost.name].way = 2;
        } else { // Se o ghost for se locomover no sentido negativo de Z
            g_Ghosts[ghost.name].way = 4;
        }
    }
}

// Função que calcula a próxima posição dos ghosts no mapa
glm::vec4 CalculateGhostPosition(const char* ghost_name)
{
    // Pega todas as informações do ghost
    Ghost ghost = g_Ghosts[ghost_name];
    int prox_checkpoint = GetNextCheckpoint(ghost);


    // calcula a nova posição do ghost
    glm::vec4 origin  = ghost.positions[ghost.checkpoint];
    glm::vec4 destiny = ghost.positions[prox_checkpoint];
    glm::vec4 new_position = origin + ghost.delta_t * (destiny - origin);


    // calcula a distancia entre os checkpoints, a distancia que falta para chegar no proximo checkpoint
    // e a distancia atual para o primeiro checkpoint
    float distance_initial       = sqrt(pow(destiny.x - origin.x, 2) + pow(destiny.y - origin.y, 2) + pow(destiny.z - origin.z, 2));
    float distance_to_destiny    = sqrt(pow(destiny.x - new_position.x, 2) + pow(destiny.y - new_position.y, 2) + pow(destiny.z - new_position.z, 2));
    float dist_new_pos_to_origin = sqrt(pow(new_position.x - origin.x, 2) + pow(new_position.y - origin.y, 2) + pow(new_position.z - origin.z, 2));


    // se chegou no destino ou acabou passando do mesmo, troca de checkpoint para percorrer um novo trecho
    if (distance_to_destiny == 0 || dist_new_pos_to_origin > distance_initial) {
        g_Ghosts[ghost_name].checkpoint = prox_checkpoint;
        g_Ghosts[ghost_name].delta_t = delta_t_ghosts / distance_initial;

        // Atualiza o sentido de deslocamento do ghost
        UpdateGhostWay(ghost_name);
    } else {
        g_Ghosts[ghost_name].delta_t += delta_t_ghosts / distance_initial;
    }

    return glm::vec4(new_position.x,new_position.y,new_position.z, 1.0f);
}

// Função que calcula a rotação no eixo Y que deve ser aplicada no ghost
float CalculateGhostRotationY(const char* ghost_name) {
    Ghost ghost = g_Ghosts[ghost_name];

    float y = 0.0f;

    switch (ghost.way) {
        case 1 : y = 0.0f;           break;
        case 2 : y = 3.1415f * 1.5f; break;
        case 3 : y = 3.1415f;        break;
        case 4 : y = 3.1415f / 2.0f; break;
        default: y = 0.0f;           break;
    }

    return y;
}

void CalculatePowerPointPosition()
{
    std::map<int, Power>::iterator it = g_PowerPoints.begin();

    while (it != g_PowerPoints.end()) {
        Power power = it->second;

        float b03 = pow(1 - power.t, 3);
        float b13 = 3 * power.t * pow(1 - power.t, 2);
        float b23 = 3 * pow(power.t, 2) * (1 - power.t);
        float b33 = pow(power.t, 3);

        glm::vec4 ct = b03 * power.points[0] + b13 * power.points[1] + b23 * power.points[2] + b33 * power.points[3];
        power.actual_position = ct;

        if (power.t >= 1) {
            power.way = 1;
        }
        else if (power.t <= 0) {
            power.way = 0;
        }

        if (power.way == 0) {
            power.t += power.steps;
        } else {
            power.t -= power.steps;
        }

        g_PowerPoints[it->first] = power;
        InsertLightsPositions(power.id, power.actual_position);
        it++;
    }
}

void InsertLightsPositions(int pointer, glm::vec4 position)
{
    if (pointer < MAXLIGHTS) {
        if (pointer >= total_lights)
            total_lights ++;

        lightspositions[pointer] = glm::vec4(position.x, position.y, position.z, position.w);
    }
}

// Função que carrega uma imagem para ser utilizada como textura
void LoadTextureImage(const char* filename)
{
    printf("Carregando imagem \"%s\"... ", filename);

    // Primeiro fazemos a leitura da imagem do disco
    stbi_set_flip_vertically_on_load(true);
    int width;
    int height;
    int channels;
    unsigned char *data = stbi_load(filename, &width, &height, &channels, 3);

    if ( data == NULL )
    {
        fprintf(stderr, "ERROR: Cannot open image file \"%s\".\n", filename);
        std::exit(EXIT_FAILURE);
    }

    printf("OK (%dx%d).\n", width, height);

    // Agora criamos objetos na GPU com OpenGL para armazenar a textura
    GLuint texture_id;
    GLuint sampler_id;
    glGenTextures(1, &texture_id);
    glGenSamplers(1, &sampler_id);

    // Veja slides 95-96 do documento Aula_20_Mapeamento_de_Texturas.pdf
    //glSamplerParameteri(sampler_id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //glSamplerParameteri(sampler_id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glSamplerParameteri(sampler_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glSamplerParameteri(sampler_id, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Parâmetros de amostragem da textura.
    glSamplerParameteri(sampler_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glSamplerParameteri(sampler_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Agora enviamos a imagem lida do disco para a GPU
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);

    GLuint textureunit = g_NumLoadedTextures;
    glActiveTexture(GL_TEXTURE0 + textureunit);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindSampler(textureunit, sampler_id);

    stbi_image_free(data);

    g_NumLoadedTextures += 1;
}

// Função que desenha um objeto armazenado em g_VirtualScene. Veja definição
// dos objetos na função BuildTrianglesAndAddToVirtualScene().
void DrawVirtualObject(const char* object_name)
{
    // "Ligamos" o VAO. Informamos que queremos utilizar os atributos de
    // vértices apontados pelo VAO criado pela função BuildTrianglesAndAddToVirtualScene(). Veja
    // comentários detalhados dentro da definição de BuildTrianglesAndAddToVirtualScene().
    glBindVertexArray(g_VirtualScene[object_name].vertex_array_object_id);

    // Setamos as variáveis "bbox_min" e "bbox_max" do fragment shader
    // com os parâmetros da axis-aligned bounding box (AABB) do modelo.
    glm::vec3 bbox_min = g_VirtualScene[object_name].bbox_min;
    glm::vec3 bbox_max = g_VirtualScene[object_name].bbox_max;
    glUniform4f(bbox_min_uniform, bbox_min.x, bbox_min.y, bbox_min.z, 1.0f);
    glUniform4f(bbox_max_uniform, bbox_max.x, bbox_max.y, bbox_max.z, 1.0f);

    // Pedimos para a GPU rasterizar os vértices dos eixos XYZ
    // apontados pelo VAO como linhas. Veja a definição de
    // g_VirtualScene[""] dentro da função BuildTrianglesAndAddToVirtualScene(), e veja
    // a documentação da função glDrawElements() em
    // http://docs.gl/gl3/glDrawElements.
    //printf("%s - %i \n", object_name, g_VirtualScene[object_name].num_indices);
    glDrawElements(
        g_VirtualScene[object_name].rendering_mode,
        g_VirtualScene[object_name].num_indices,
        GL_UNSIGNED_INT,
        (void*)(g_VirtualScene[object_name].first_index * sizeof(GLuint))
    );

    // "Desligamos" o VAO, evitando assim que operações posteriores venham a
    // alterar o mesmo. Isso evita bugs.
    glBindVertexArray(0);
}

// Função que carrega os shaders de vértices e de fragmentos que serão
// utilizados para renderização. Veja slides 180-200 do documento Aula_03_Rendering_Pipeline_Grafico.pdf.
//
void LoadShadersFromFiles()
{
    // Note que o caminho para os arquivos "shader_vertex.glsl" e
    // "shader_fragment.glsl" estão fixados, sendo que assumimos a existência
    // da seguinte estrutura no sistema de arquivos:
    //
    //    + FCG_Lab_01/
    //    |
    //    +--+ bin/
    //    |  |
    //    |  +--+ Release/  (ou Debug/ ou Linux/)
    //    |     |
    //    |     o-- main.exe
    //    |
    //    +--+ src/
    //       |
    //       o-- shader_vertex.glsl
    //       |
    //       o-- shader_fragment.glsl
    //
    vertex_shader_id = LoadShader_Vertex("../../src/shader_vertex.glsl");
    fragment_shader_id = LoadShader_Fragment("../../src/shader_fragment.glsl");

    // Deletamos o programa de GPU anterior, caso ele exista.
    if ( program_id != 0 )
        glDeleteProgram(program_id);

    // Criamos um programa de GPU utilizando os shaders carregados acima.
    program_id = CreateGpuProgram(vertex_shader_id, fragment_shader_id);

    // Buscamos o endereço das variáveis definidas dentro do Vertex Shader.
    // Utilizaremos estas variáveis para enviar dados para a placa de vídeo
    // (GPU)! Veja arquivo "shader_vertex.glsl" e "shader_fragment.glsl".
    model_uniform           = glGetUniformLocation(program_id, "model"); // Variável da matriz "model"
    view_uniform            = glGetUniformLocation(program_id, "view"); // Variável da matriz "view" em shader_vertex.glsl
    projection_uniform      = glGetUniformLocation(program_id, "projection"); // Variável da matriz "projection" em shader_vertex.glsl
    object_id_uniform       = glGetUniformLocation(program_id, "object_id"); // Variável "object_id" em shader_fragment.glsl
    bbox_min_uniform        = glGetUniformLocation(program_id, "bbox_min");
    bbox_max_uniform        = glGetUniformLocation(program_id, "bbox_max");
    lightspositions_uniform = glGetUniformLocation(program_id, "lightspositions");
    total_lights_uniform    = glGetUniformLocation(program_id, "total_lights");

    // Variáveis em "shader_fragment.glsl" para acesso das imagens de textura
    glUseProgram(program_id);
    glUniform1i(glGetUniformLocation(program_id, "TextureImage0"), 0);
    glUniform1i(glGetUniformLocation(program_id, "TextureImage1"), 1);
    glUniform1i(glGetUniformLocation(program_id, "TextureImage2"), 2);
    glUniform1i(glGetUniformLocation(program_id, "TextureImage3"), 3);
    glUseProgram(0);
}

// Função que pega a matriz M e guarda a mesma no topo da pilha
void PushMatrix(glm::mat4 M)
{
    g_MatrixStack.push(M);
}

// Função que remove a matriz atualmente no topo da pilha e armazena a mesma na variável M
void PopMatrix(glm::mat4& M)
{
    if ( g_MatrixStack.empty() )
    {
        M = Matrix_Identity();
    }
    else
    {
        M = g_MatrixStack.top();
        g_MatrixStack.pop();
    }
}

// Função que computa as normais de um ObjModel, caso elas não tenham sido
// especificadas dentro do arquivo ".obj"
void ComputeNormals(ObjModel* model)
{
    if ( !model->attrib.normals.empty() )
        return;

    // Primeiro computamos as normais para todos os TRIÂNGULOS.
    // Segundo, computamos as normais dos VÉRTICES através do método proposto
    // por Gouraud, onde a normal de cada vértice vai ser a média das normais de
    // todas as faces que compartilham este vértice.

    size_t num_vertices = model->attrib.vertices.size() / 3;

    std::vector<int> num_triangles_per_vertex(num_vertices, 0);
    std::vector<glm::vec4> vertex_normals(num_vertices, glm::vec4(0.0f,0.0f,0.0f,0.0f));

    for (size_t shape = 0; shape < model->shapes.size(); ++shape)
    {
        size_t num_triangles = model->shapes[shape].mesh.num_face_vertices.size();

        for (size_t triangle = 0; triangle < num_triangles; ++triangle)
        {
            assert(model->shapes[shape].mesh.num_face_vertices[triangle] == 3);

            glm::vec4  vertices[3];
            for (size_t vertex = 0; vertex < 3; ++vertex)
            {
                tinyobj::index_t idx = model->shapes[shape].mesh.indices[3*triangle + vertex];
                const float vx = model->attrib.vertices[3*idx.vertex_index + 0];
                const float vy = model->attrib.vertices[3*idx.vertex_index + 1];
                const float vz = model->attrib.vertices[3*idx.vertex_index + 2];
                vertices[vertex] = glm::vec4(vx,vy,vz,1.0);
            }

            const glm::vec4  a = vertices[0];
            const glm::vec4  b = vertices[1];
            const glm::vec4  c = vertices[2];

            // PREENCHA AQUI o cálculo da normal de um triângulo cujos vértices
            // estão nos pontos "a", "b", e "c", definidos no sentido anti-horário.
            const glm::vec4  n = crossproduct(b-a, c-a);

            for (size_t vertex = 0; vertex < 3; ++vertex)
            {
                tinyobj::index_t idx = model->shapes[shape].mesh.indices[3*triangle + vertex];
                num_triangles_per_vertex[idx.vertex_index] += 1;
                vertex_normals[idx.vertex_index] += n;
                model->shapes[shape].mesh.indices[3*triangle + vertex].normal_index = idx.vertex_index;
            }
        }
    }

    model->attrib.normals.resize( 3*num_vertices );

    for (size_t i = 0; i < vertex_normals.size(); ++i)
    {
        glm::vec4 n = vertex_normals[i] / (float)num_triangles_per_vertex[i];
        n /= norm(n);
        model->attrib.normals[3*i + 0] = n.x;
        model->attrib.normals[3*i + 1] = n.y;
        model->attrib.normals[3*i + 2] = n.z;
    }
}

// Constrói triângulos para futura renderização a partir de um ObjModel.
void BuildTrianglesAndAddToVirtualScene(ObjModel* model)
{
    GLuint vertex_array_object_id;
    glGenVertexArrays(1, &vertex_array_object_id);
    glBindVertexArray(vertex_array_object_id);

    std::vector<GLuint> indices;
    std::vector<float>  model_coefficients;
    std::vector<float>  normal_coefficients;
    std::vector<float>  texture_coefficients;

    for (size_t shape = 0; shape < model->shapes.size(); ++shape)
    {
        size_t first_index = indices.size();
        size_t num_triangles = model->shapes[shape].mesh.num_face_vertices.size();

        const float minval = std::numeric_limits<float>::min();
        const float maxval = std::numeric_limits<float>::max();

        glm::vec3 bbox_min = glm::vec3(maxval,maxval,maxval);
        glm::vec3 bbox_max = glm::vec3(minval,minval,minval);

        for (size_t triangle = 0; triangle < num_triangles; ++triangle)
        {
            assert(model->shapes[shape].mesh.num_face_vertices[triangle] == 3);

            for (size_t vertex = 0; vertex < 3; ++vertex)
            {
                tinyobj::index_t idx = model->shapes[shape].mesh.indices[3*triangle + vertex];

                indices.push_back(first_index + 3*triangle + vertex);

                const float vx = model->attrib.vertices[3*idx.vertex_index + 0];
                const float vy = model->attrib.vertices[3*idx.vertex_index + 1];
                const float vz = model->attrib.vertices[3*idx.vertex_index + 2];

                model_coefficients.push_back( vx ); // X
                model_coefficients.push_back( vy ); // Y
                model_coefficients.push_back( vz ); // Z
                model_coefficients.push_back( 1.0f ); // W

                bbox_min.x = std::min(bbox_min.x, vx);
                bbox_min.y = std::min(bbox_min.y, vy);
                bbox_min.z = std::min(bbox_min.z, vz);
                bbox_max.x = std::max(bbox_max.x, vx);
                bbox_max.y = std::max(bbox_max.y, vy);
                bbox_max.z = std::max(bbox_max.z, vz);

                // Inspecionando o código da tinyobjloader, o aluno Bernardo
                // Sulzbach (2017/1) apontou que a maneira correta de testar se
                // existem normais e coordenadas de textura no ObjModel é
                // comparando se o índice retornado é -1. Fazemos isso abaixo.
                if ( idx.normal_index != -1 )
                {
                    const float nx = model->attrib.normals[3*idx.normal_index + 0];
                    const float ny = model->attrib.normals[3*idx.normal_index + 1];
                    const float nz = model->attrib.normals[3*idx.normal_index + 2];
                    normal_coefficients.push_back( nx ); // X
                    normal_coefficients.push_back( ny ); // Y
                    normal_coefficients.push_back( nz ); // Z
                    normal_coefficients.push_back( 0.0f ); // W
                }

                if ( idx.texcoord_index != -1 )
                {
                    const float u = model->attrib.texcoords[2*idx.texcoord_index + 0];
                    const float v = model->attrib.texcoords[2*idx.texcoord_index + 1];
                    texture_coefficients.push_back( u );
                    texture_coefficients.push_back( v );
                }
            }
        }

        size_t last_index = indices.size() - 1;

        SceneObject theobject;
        theobject.name           = model->shapes[shape].name;
        theobject.first_index    = first_index; // Primeiro índice
        theobject.num_indices    = last_index - first_index + 1; // Número de indices
        theobject.rendering_mode = GL_TRIANGLES;       // Índices correspondem ao tipo de rasterização GL_TRIANGLES.
        theobject.vertex_array_object_id = vertex_array_object_id;

        theobject.bbox_min = bbox_min;
        theobject.bbox_max = bbox_max;

        //std::cout << model->shapes[shape].name << std::endl;

        g_VirtualScene[model->shapes[shape].name] = theobject;
    }

    GLuint VBO_model_coefficients_id;
    glGenBuffers(1, &VBO_model_coefficients_id);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_model_coefficients_id);
    glBufferData(GL_ARRAY_BUFFER, model_coefficients.size() * sizeof(float), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, model_coefficients.size() * sizeof(float), model_coefficients.data());
    GLuint location = 0; // "(location = 0)" em "shader_vertex.glsl"
    GLint  number_of_dimensions = 4; // vec4 em "shader_vertex.glsl"
    glVertexAttribPointer(location, number_of_dimensions, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(location);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    if ( !normal_coefficients.empty() )
    {
        GLuint VBO_normal_coefficients_id;
        glGenBuffers(1, &VBO_normal_coefficients_id);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_normal_coefficients_id);
        glBufferData(GL_ARRAY_BUFFER, normal_coefficients.size() * sizeof(float), NULL, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, normal_coefficients.size() * sizeof(float), normal_coefficients.data());
        location = 1; // "(location = 1)" em "shader_vertex.glsl"
        number_of_dimensions = 4; // vec4 em "shader_vertex.glsl"
        glVertexAttribPointer(location, number_of_dimensions, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(location);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    if ( !texture_coefficients.empty() )
    {
        GLuint VBO_texture_coefficients_id;
        glGenBuffers(1, &VBO_texture_coefficients_id);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_texture_coefficients_id);
        glBufferData(GL_ARRAY_BUFFER, texture_coefficients.size() * sizeof(float), NULL, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, texture_coefficients.size() * sizeof(float), texture_coefficients.data());
        location = 2; // "(location = 1)" em "shader_vertex.glsl"
        number_of_dimensions = 2; // vec2 em "shader_vertex.glsl"
        glVertexAttribPointer(location, number_of_dimensions, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(location);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    GLuint indices_id;
    glGenBuffers(1, &indices_id);

    // "Ligamos" o buffer. Note que o tipo agora é GL_ELEMENT_ARRAY_BUFFER.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indices.size() * sizeof(GLuint), indices.data());
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // XXX Errado!
    //

    // "Desligamos" o VAO, evitando assim que operações posteriores venham a
    // alterar o mesmo. Isso evita bugs.
    glBindVertexArray(0);
}

// Carrega um Vertex Shader de um arquivo GLSL. Veja definição de LoadShader() abaixo.
GLuint LoadShader_Vertex(const char* filename)
{
    // Criamos um identificador (ID) para este shader, informando que o mesmo
    // será aplicado nos vértices.
    GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);

    // Carregamos e compilamos o shader
    LoadShader(filename, vertex_shader_id);

    // Retorna o ID gerado acima
    return vertex_shader_id;
}

// Carrega um Fragment Shader de um arquivo GLSL . Veja definição de LoadShader() abaixo.
GLuint LoadShader_Fragment(const char* filename)
{
    // Criamos um identificador (ID) para este shader, informando que o mesmo
    // será aplicado nos fragmentos.
    GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

    // Carregamos e compilamos o shader
    LoadShader(filename, fragment_shader_id);

    // Retorna o ID gerado acima
    return fragment_shader_id;
}

// Função auxilar, utilizada pelas duas funções acima. Carrega código de GPU de
// um arquivo GLSL e faz sua compilação.
void LoadShader(const char* filename, GLuint shader_id)
{
    // Lemos o arquivo de texto indicado pela variável "filename"
    // e colocamos seu conteúdo em memória, apontado pela variável
    // "shader_string".
    std::ifstream file;
    try {
        file.exceptions(std::ifstream::failbit);
        file.open(filename);
    } catch ( std::exception& e ) {
        fprintf(stderr, "ERROR: Cannot open file \"%s\".\n", filename);
        std::exit(EXIT_FAILURE);
    }
    std::stringstream shader;
    shader << file.rdbuf();
    std::string str = shader.str();
    const GLchar* shader_string = str.c_str();
    const GLint   shader_string_length = static_cast<GLint>( str.length() );

    // Define o código do shader GLSL, contido na string "shader_string"
    glShaderSource(shader_id, 1, &shader_string, &shader_string_length);

    // Compila o código do shader GLSL (em tempo de execução)
    glCompileShader(shader_id);

    // Verificamos se ocorreu algum erro ou "warning" durante a compilação
    GLint compiled_ok;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compiled_ok);

    GLint log_length = 0;
    glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_length);

    // Alocamos memória para guardar o log de compilação.
    // A chamada "new" em C++ é equivalente ao "malloc()" do C.
    GLchar* log = new GLchar[log_length];
    glGetShaderInfoLog(shader_id, log_length, &log_length, log);

    // Imprime no terminal qualquer erro ou "warning" de compilação
    if ( log_length != 0 )
    {
        std::string  output;

        if ( !compiled_ok )
        {
            output += "ERROR: OpenGL compilation of \"";
            output += filename;
            output += "\" failed.\n";
            output += "== Start of compilation log\n";
            output += log;
            output += "== End of compilation log\n";
        }
        else
        {
            output += "WARNING: OpenGL compilation of \"";
            output += filename;
            output += "\".\n";
            output += "== Start of compilation log\n";
            output += log;
            output += "== End of compilation log\n";
        }

        fprintf(stderr, "%s", output.c_str());
    }

    // A chamada "delete" em C++ é equivalente ao "free()" do C
    delete [] log;
}

// Esta função cria um programa de GPU, o qual contém obrigatoriamente um
// Vertex Shader e um Fragment Shader.
GLuint CreateGpuProgram(GLuint vertex_shader_id, GLuint fragment_shader_id)
{
    // Criamos um identificador (ID) para este programa de GPU
    GLuint program_id = glCreateProgram();

    // Definição dos dois shaders GLSL que devem ser executados pelo programa
    glAttachShader(program_id, vertex_shader_id);
    glAttachShader(program_id, fragment_shader_id);

    // Linkagem dos shaders acima ao programa
    glLinkProgram(program_id);

    // Verificamos se ocorreu algum erro durante a linkagem
    GLint linked_ok = GL_FALSE;
    glGetProgramiv(program_id, GL_LINK_STATUS, &linked_ok);

    // Imprime no terminal qualquer erro de linkagem
    if ( linked_ok == GL_FALSE )
    {
        GLint log_length = 0;
        glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &log_length);

        // Alocamos memória para guardar o log de compilação.
        // A chamada "new" em C++ é equivalente ao "malloc()" do C.
        GLchar* log = new GLchar[log_length];

        glGetProgramInfoLog(program_id, log_length, &log_length, log);

        std::string output;

        output += "ERROR: OpenGL linking of program failed.\n";
        output += "== Start of link log\n";
        output += log;
        output += "\n== End of link log\n";

        // A chamada "delete" em C++ é equivalente ao "free()" do C
        delete [] log;

        fprintf(stderr, "%s", output.c_str());
    }

    // Os "Shader Objects" podem ser marcados para deleção após serem linkados
    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);

    // Retornamos o ID gerado acima
    return program_id;
}

// Definição da função que será chamada sempre que a janela do sistema
// operacional for redimensionada, por consequência alterando o tamanho do
// "framebuffer" (região de memória onde são armazenados os pixels da imagem).
void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    // Indicamos que queremos renderizar em toda região do framebuffer. A
    // função "glViewport" define o mapeamento das "normalized device
    // coordinates" (NDC) para "pixel coordinates".  Essa é a operação de
    // "Screen Mapping" ou "Viewport Mapping" vista em aula ({+ViewportMapping2+}).
    glViewport(0, 0, width, height);

    // Atualizamos também a razão que define a proporção da janela (largura /
    // altura), a qual será utilizada na definição das matrizes de projeção,
    // tal que não ocorra distorções durante o processo de "Screen Mapping"
    // acima, quando NDC é mapeado para coordenadas de pixels. Veja slides 205-215 do documento Aula_09_Projecoes.pdf.
    //
    // O cast para float é necessário pois números inteiros são arredondados ao
    // serem divididos!
    g_ScreenRatio = (float)width / height;
}

// Função callback chamada sempre que o usuário aperta algum dos botões do mouse
void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        // Se o usuário pressionou o botão esquerdo do mouse, guardamos a
        // posição atual do cursor nas variáveis g_LastCursorPosX e
        // g_LastCursorPosY.  Também, setamos a variável
        // g_LeftMouseButtonPressed como true, para saber que o usuário está
        // com o botão esquerdo pressionado.
        glfwGetCursorPos(window, &g_LastCursorPosX, &g_LastCursorPosY);
        g_LeftMouseButtonPressed = true;
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {
        // Quando o usuário soltar o botão esquerdo do mouse, atualizamos a
        // variável abaixo para false.
        g_LeftMouseButtonPressed = false;
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
        // Se o usuário pressionou o botão esquerdo do mouse, guardamos a
        // posição atual do cursor nas variáveis g_LastCursorPosX e
        // g_LastCursorPosY.  Também, setamos a variável
        // g_RightMouseButtonPressed como true, para saber que o usuário está
        // com o botão esquerdo pressionado.
        glfwGetCursorPos(window, &g_LastCursorPosX, &g_LastCursorPosY);
        g_RightMouseButtonPressed = true;
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
    {
        // Quando o usuário soltar o botão esquerdo do mouse, atualizamos a
        // variável abaixo para false.
        g_RightMouseButtonPressed = false;
    }
    if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS)
    {
        // Se o usuário pressionou o botão esquerdo do mouse, guardamos a
        // posição atual do cursor nas variáveis g_LastCursorPosX e
        // g_LastCursorPosY.  Também, setamos a variável
        // g_MiddleMouseButtonPressed como true, para saber que o usuário está
        // com o botão esquerdo pressionado.
        glfwGetCursorPos(window, &g_LastCursorPosX, &g_LastCursorPosY);
        g_MiddleMouseButtonPressed = true;
    }
    if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_RELEASE)
    {
        // Quando o usuário soltar o botão esquerdo do mouse, atualizamos a
        // variável abaixo para false.
        g_MiddleMouseButtonPressed = false;
    }
}

// Função callback chamada sempre que o usuário movimentar o cursor do mouse em
// cima da janela OpenGL.
void CursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
    // Abaixo executamos o seguinte: caso o botão esquerdo do mouse esteja
    // pressionado, computamos quanto que o mouse se movimento desde o último
    // instante de tempo, e usamos esta movimentação para atualizar os
    // parâmetros que definem a posição da câmera dentro da cena virtual.
    // Assim, temos que o usuário consegue controlar a câmera.

    if (g_LeftMouseButtonPressed)
    {
        // Deslocamento do cursor do mouse em x e y de coordenadas de tela!
        float dx = xpos - g_LastCursorPosX;
        float dy = ypos - g_LastCursorPosY;

        // Atualizamos parâmetros da câmera com os deslocamentos
        g_CameraTheta += 0.01f*dx;
        g_CameraPhi   += 0.01f*dy;

        // Em coordenadas esféricas, o ângulo phi deve ficar entre -pi/2 e +pi/2.
        float phimax = 3.141592f/2;
        float phimin = -phimax;

        if (g_CameraPhi > phimax)
            g_CameraPhi = phimax;

        if (g_CameraPhi < phimin)
            g_CameraPhi = phimin;

        // Atualizamos as variáveis globais para armazenar a posição atual do
        // cursor como sendo a última posição conhecida do cursor.
        g_LastCursorPosX = xpos;
        g_LastCursorPosY = ypos;
    }

    if (g_RightMouseButtonPressed)
    {
        // Deslocamento do cursor do mouse em x e y de coordenadas de tela!
        float dx = xpos - g_LastCursorPosX;
        float dy = ypos - g_LastCursorPosY;

        // Atualizamos parâmetros da antebraço com os deslocamentos
        g_ForearmAngleZ -= 0.01f*dx;
        g_ForearmAngleX += 0.01f*dy;

        // Atualizamos as variáveis globais para armazenar a posição atual do
        // cursor como sendo a última posição conhecida do cursor.
        g_LastCursorPosX = xpos;
        g_LastCursorPosY = ypos;
    }

    if (g_MiddleMouseButtonPressed)
    {
        // Deslocamento do cursor do mouse em x e y de coordenadas de tela!
        float dx = xpos - g_LastCursorPosX;
        float dy = ypos - g_LastCursorPosY;

        // Atualizamos parâmetros da antebraço com os deslocamentos
        g_TorsoPositionX += 0.01f*dx;
        g_TorsoPositionY -= 0.01f*dy;

        // Atualizamos as variáveis globais para armazenar a posição atual do
        // cursor como sendo a última posição conhecida do cursor.
        g_LastCursorPosX = xpos;
        g_LastCursorPosY = ypos;
    }
}

// Função callback chamada sempre que o usuário movimenta a "rodinha" do mouse.
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    // Atualizamos a distância da câmera para a origem utilizando a
    // movimentação da "rodinha", simulando um ZOOM.
    g_CameraDistance -= 0.1f*yoffset;

    // Uma câmera look-at nunca pode estar exatamente "em cima" do ponto para
    // onde ela está olhando, pois isto gera problemas de divisão por zero na
    // definição do sistema de coordenadas da câmera. Isto é, a variável abaixo
    // nunca pode ser zero. Versões anteriores deste código possuíam este bug,
    // o qual foi detectado pelo aluno Vinicius Fraga (2017/2).
    const float verysmallnumber = std::numeric_limits<float>::epsilon();
    if (g_CameraDistance < verysmallnumber)
        g_CameraDistance = verysmallnumber;
}

// Definição da função que será chamada sempre que o usuário pressionar alguma
// tecla do teclado. Veja http://www.glfw.org/docs/latest/input_guide.html#input_key
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mod)
{
    // ================
    // Não modifique este loop! Ele é utilizando para correção automatizada dos
    // laboratórios. Deve ser sempre o primeiro comando desta função KeyCallback().
    for (int i = 0; i < 10; ++i)
        if (key == GLFW_KEY_0 + i && action == GLFW_PRESS && mod == GLFW_MOD_SHIFT)
            std::exit(100 + i);
    // ================

    // Se o usuário pressionar a tecla ESC, fechamos a janela.
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    // O código abaixo implementa a seguinte lógica:
    //   Se apertar tecla X       então g_AngleX += delta;
    //   Se apertar tecla shift+X então g_AngleX -= delta;
    //   Se apertar tecla Y       então g_AngleY += delta;
    //   Se apertar tecla shift+Y então g_AngleY -= delta;
    //   Se apertar tecla Z       então g_AngleZ += delta;
    //   Se apertar tecla shift+Z então g_AngleZ -= delta;

    float delta = 3.141592 / 16; // 22.5 graus, em radianos.

    if (key == GLFW_KEY_X && action == GLFW_PRESS)
    {
        g_AngleX += (mod & GLFW_MOD_SHIFT) ? -delta : delta;
    }

    if (key == GLFW_KEY_Y && action == GLFW_PRESS)
    {
        g_AngleY += (mod & GLFW_MOD_SHIFT) ? -delta : delta;
    }
    if (key == GLFW_KEY_Z && action == GLFW_PRESS)
    {
        g_AngleZ += (mod & GLFW_MOD_SHIFT) ? -delta : delta;
    }

    // Se o usuário apertar a tecla espaço, resetamos os ângulos de Euler para zero.
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
        g_AngleX = 0.0f;
        g_AngleY = 0.0f;
        g_AngleZ = 0.0f;
        g_ForearmAngleX = 0.0f;
        g_ForearmAngleZ = 0.0f;
        g_TorsoPositionX = 0.0f;
        g_TorsoPositionY = 0.0f;
    }

    // Se o usuário apertar a tecla P, utilizamos projeção perspectiva.
    if (key == GLFW_KEY_P && action == GLFW_PRESS)
    {
        g_UsePerspectiveProjection = true;
    }

    // Se o usuário apertar a tecla O, utilizamos projeção ortográfica.
    if (key == GLFW_KEY_O && action == GLFW_PRESS)
    {
        g_UsePerspectiveProjection = false;
    }

    // Se o usuário apertar a tecla C, mudamos de primeira pessoa para terceira e vice-versa.
    if (key == GLFW_KEY_C && action == GLFW_PRESS)
    {
        if (g_ThirdPerson){
            g_ThirdPerson = false;
            //g_CameraDistance = 0.0f;
        } else {
            g_ThirdPerson = true;
            //g_CameraDistance = 2.5f;
        }
    }

    // Se o usuário apertar a tecla H, fazemos um "toggle" do texto informativo mostrado na tela.
    if (key == GLFW_KEY_H && action == GLFW_PRESS)
    {
        g_ShowInfoText = !g_ShowInfoText;
    }

    // Se o usuário apertar a tecla R, recarregamos os shaders dos arquivos "shader_fragment.glsl" e "shader_vertex.glsl".
    if (key == GLFW_KEY_R && action == GLFW_PRESS)
    {
        LoadShadersFromFiles();
        fprintf(stdout,"Shaders recarregados!\n");
        fflush(stdout);
    }

    // Veja https://www.glfw.org/docs/3.3/input_guide.html#input_key
    if (key == GLFW_KEY_W)
    {
        if (action == GLFW_PRESS)
            // Usuário apertou a tecla D, então atualizamos o estado para pressionada
            tecla_W_pressionada = true;

        else if (action == GLFW_RELEASE)
            // Usuário largou a tecla D, então atualizamos o estado para NÃO pressionada
            tecla_W_pressionada = false;

        else if (action == GLFW_REPEAT)
            // Usuário está segurando a tecla D e o sistema operacional está
            // disparando eventos de repetição. Neste caso, não precisamos
            // atualizar o estado da tecla, pois antes de um evento REPEAT
            // necessariamente deve ter ocorrido um evento PRESS.
            ;
    }

    // Veja https://www.glfw.org/docs/3.3/input_guide.html#input_key
    if (key == GLFW_KEY_A)
    {
        if (action == GLFW_PRESS)
            // Usuário apertou a tecla D, então atualizamos o estado para pressionada
            tecla_A_pressionada = true;

        else if (action == GLFW_RELEASE)
            // Usuário largou a tecla D, então atualizamos o estado para NÃO pressionada
            tecla_A_pressionada = false;

        else if (action == GLFW_REPEAT)
            // Usuário está segurando a tecla D e o sistema operacional está
            // disparando eventos de repetição. Neste caso, não precisamos
            // atualizar o estado da tecla, pois antes de um evento REPEAT
            // necessariamente deve ter ocorrido um evento PRESS.
            ;
    }

    // Veja https://www.glfw.org/docs/3.3/input_guide.html#input_key
    if (key == GLFW_KEY_S)
    {
        if (action == GLFW_PRESS)
            // Usuário apertou a tecla D, então atualizamos o estado para pressionada
            tecla_S_pressionada = true;

        else if (action == GLFW_RELEASE)
            // Usuário largou a tecla D, então atualizamos o estado para NÃO pressionada
            tecla_S_pressionada = false;

        else if (action == GLFW_REPEAT)
            // Usuário está segurando a tecla D e o sistema operacional está
            // disparando eventos de repetição. Neste caso, não precisamos
            // atualizar o estado da tecla, pois antes de um evento REPEAT
            // necessariamente deve ter ocorrido um evento PRESS.
            ;
    }

    // Veja https://www.glfw.org/docs/3.3/input_guide.html#input_key
    if (key == GLFW_KEY_D)
    {
        if (action == GLFW_PRESS)
            // Usuário apertou a tecla D, então atualizamos o estado para pressionada
            tecla_D_pressionada = true;

        else if (action == GLFW_RELEASE)
            // Usuário largou a tecla D, então atualizamos o estado para NÃO pressionada
            tecla_D_pressionada = false;

        else if (action == GLFW_REPEAT)
            // Usuário está segurando a tecla D e o sistema operacional está
            // disparando eventos de repetição. Neste caso, não precisamos
            // atualizar o estado da tecla, pois antes de um evento REPEAT
            // necessariamente deve ter ocorrido um evento PRESS.
            ;
    }

    if (key == GLFW_KEY_SPACE) {
        if (action == GLFW_PRESS)
            tecla_ESPACO_pressionada = true;

        else if (action == GLFW_RELEASE)
            tecla_ESPACO_pressionada = false;

        else if (action == GLFW_REPEAT)
            ;
    }

    if (key == GLFW_KEY_ENTER && g_StartGame == false) {
        g_StartGame = true;
    }
}

// Definimos o callback para impressão de erros da GLFW no terminal
void ErrorCallback(int error, const char* description)
{
    fprintf(stderr, "ERROR: GLFW: %s\n", description);
}

// Esta função recebe um vértice com coordenadas de modelo p_model e passa o
// mesmo por todos os sistemas de coordenadas armazenados nas matrizes model,
// view, e projection; e escreve na tela as matrizes e pontos resultantes
// dessas transformações.
void TextRendering_ShowModelViewProjection(
    GLFWwindow* window,
    glm::mat4 projection,
    glm::mat4 view,
    glm::mat4 model,
    glm::vec4 p_model
)
{
    if ( !g_ShowInfoText )
        return;

    glm::vec4 p_world = model*p_model;
    glm::vec4 p_camera = view*p_world;
    glm::vec4 p_clip = projection*p_camera;
    glm::vec4 p_ndc = p_clip / p_clip.w;

    float pad = TextRendering_LineHeight(window);

    TextRendering_PrintString(window, " Model matrix             Model     In World Coords.", -1.0f, 1.0f-pad, 1.0f);
    TextRendering_PrintMatrixVectorProduct(window, model, p_model, -1.0f, 1.0f-2*pad, 1.0f);

    TextRendering_PrintString(window, "                                        |  ", -1.0f, 1.0f-6*pad, 1.0f);
    TextRendering_PrintString(window, "                            .-----------'  ", -1.0f, 1.0f-7*pad, 1.0f);
    TextRendering_PrintString(window, "                            V              ", -1.0f, 1.0f-8*pad, 1.0f);

    TextRendering_PrintString(window, " View matrix              World     In Camera Coords.", -1.0f, 1.0f-9*pad, 1.0f);
    TextRendering_PrintMatrixVectorProduct(window, view, p_world, -1.0f, 1.0f-10*pad, 1.0f);

    TextRendering_PrintString(window, "                                        |  ", -1.0f, 1.0f-14*pad, 1.0f);
    TextRendering_PrintString(window, "                            .-----------'  ", -1.0f, 1.0f-15*pad, 1.0f);
    TextRendering_PrintString(window, "                            V              ", -1.0f, 1.0f-16*pad, 1.0f);

    TextRendering_PrintString(window, " Projection matrix        Camera                    In NDC", -1.0f, 1.0f-17*pad, 1.0f);
    TextRendering_PrintMatrixVectorProductDivW(window, projection, p_camera, -1.0f, 1.0f-18*pad, 1.0f);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    glm::vec2 a = glm::vec2(-1, -1);
    glm::vec2 b = glm::vec2(+1, +1);
    glm::vec2 p = glm::vec2( 0,  0);
    glm::vec2 q = glm::vec2(width, height);

    glm::mat4 viewport_mapping = Matrix(
        (q.x - p.x)/(b.x-a.x), 0.0f, 0.0f, (b.x*p.x - a.x*q.x)/(b.x-a.x),
        0.0f, (q.y - p.y)/(b.y-a.y), 0.0f, (b.y*p.y - a.y*q.y)/(b.y-a.y),
        0.0f , 0.0f , 1.0f , 0.0f ,
        0.0f , 0.0f , 0.0f , 1.0f
    );

    TextRendering_PrintString(window, "                                                       |  ", -1.0f, 1.0f-22*pad, 1.0f);
    TextRendering_PrintString(window, "                            .--------------------------'  ", -1.0f, 1.0f-23*pad, 1.0f);
    TextRendering_PrintString(window, "                            V                           ", -1.0f, 1.0f-24*pad, 1.0f);

    TextRendering_PrintString(window, " Viewport matrix           NDC      In Pixel Coords.", -1.0f, 1.0f-25*pad, 1.0f);
    TextRendering_PrintMatrixVectorProductMoreDigits(window, viewport_mapping, p_ndc, -1.0f, 1.0f-26*pad, 1.0f);
}

// Escrevemos na tela os ângulos de Euler definidos nas variáveis globais
// g_AngleX, g_AngleY, e g_AngleZ.
void TextRendering_ShowEulerAngles(GLFWwindow* window)
{
    if ( !g_ShowInfoText )
        return;

    float pad = TextRendering_LineHeight(window);

    char buffer[80];
    snprintf(buffer, 80, "Euler Angles rotation matrix = Z(%.2f)*Y(%.2f)*X(%.2f)\n", g_AngleZ, g_AngleY, g_AngleX);

    TextRendering_PrintString(window, buffer, -1.0f+pad/10, -1.0f+2*pad/10, 1.0f);
}

void TextRendering_ShowPositionCamera(GLFWwindow* window)
{
    if (!g_ShowInfoText) {
        return;
    }

    float pad = TextRendering_LineHeight(window);

    char buffer[80];
    snprintf(buffer, 80, "Pacman position: X = %.2f Y = %.2f Z = %.2f\n", pacman_position_c.x, pacman_position_c.y, pacman_position_c.z);

    TextRendering_PrintString(window, buffer, -1.0f+pad/10, -1.0f+2*pad/10, 1.0f);
}

// Escrevemos na tela qual matriz de projeção está sendo utilizada.
void TextRendering_ShowProjection(GLFWwindow* window)
{
    if ( !g_ShowInfoText )
        return;

    float lineheight = TextRendering_LineHeight(window);
    float charwidth = TextRendering_CharWidth(window);

    if ( g_UsePerspectiveProjection )
        TextRendering_PrintString(window, "Perspective", 1.0f-13*charwidth, -1.0f+2*lineheight/10, 1.0f);
    else
        TextRendering_PrintString(window, "Orthographic", 1.0f-13*charwidth, -1.0f+2*lineheight/10, 1.0f);
}

// Escrevemos na tela o número de quadros renderizados por segundo (frames per
// second).
void TextRendering_ShowFramesPerSecond(GLFWwindow* window)
{
    if ( !g_ShowInfoText )
        return;

    // Variáveis estáticas (static) mantém seus valores entre chamadas
    // subsequentes da função!
    static float old_seconds = (float)glfwGetTime();
    static int   ellapsed_frames = 0;
    static char  buffer[20] = "?? fps";
    static int   numchars = 7;

    ellapsed_frames += 1;

    // Recuperamos o número de segundos que passou desde a execução do programa
    float seconds = (float)glfwGetTime();

    // Número de segundos desde o último cálculo do fps
    float ellapsed_seconds = seconds - old_seconds;

    if ( ellapsed_seconds > 1.0f )
    {
        numchars = snprintf(buffer, 20, "%.2f fps", ellapsed_frames / ellapsed_seconds);

        old_seconds = seconds;
        ellapsed_frames = 0;
    }

    float lineheight = TextRendering_LineHeight(window);
    float charwidth = TextRendering_CharWidth(window);

    TextRendering_PrintString(window, buffer, 1.0f-(numchars + 1)*charwidth, 1.0f-lineheight, 1.0f);
}

// Função para debugging: imprime no terminal todas informações de um modelo
// geométrico carregado de um arquivo ".obj".
// Veja: https://github.com/syoyo/tinyobjloader/blob/22883def8db9ef1f3ffb9b404318e7dd25fdbb51/loader_example.cc#L98
void PrintObjModelInfo(ObjModel* model)
{
  const tinyobj::attrib_t                & attrib    = model->attrib;
  const std::vector<tinyobj::shape_t>    & shapes    = model->shapes;
  const std::vector<tinyobj::material_t> & materials = model->materials;

  printf("# of vertices  : %d\n", (int)(attrib.vertices.size() / 3));
  printf("# of normals   : %d\n", (int)(attrib.normals.size() / 3));
  printf("# of texcoords : %d\n", (int)(attrib.texcoords.size() / 2));
  printf("# of shapes    : %d\n", (int)shapes.size());
  printf("# of materials : %d\n", (int)materials.size());

  for (size_t v = 0; v < attrib.vertices.size() / 3; v++) {
    printf("  v[%ld] = (%f, %f, %f)\n", static_cast<long>(v),
           static_cast<const double>(attrib.vertices[3 * v + 0]),
           static_cast<const double>(attrib.vertices[3 * v + 1]),
           static_cast<const double>(attrib.vertices[3 * v + 2]));
  }

  for (size_t v = 0; v < attrib.normals.size() / 3; v++) {
    printf("  n[%ld] = (%f, %f, %f)\n", static_cast<long>(v),
           static_cast<const double>(attrib.normals[3 * v + 0]),
           static_cast<const double>(attrib.normals[3 * v + 1]),
           static_cast<const double>(attrib.normals[3 * v + 2]));
  }

  for (size_t v = 0; v < attrib.texcoords.size() / 2; v++) {
    printf("  uv[%ld] = (%f, %f)\n", static_cast<long>(v),
           static_cast<const double>(attrib.texcoords[2 * v + 0]),
           static_cast<const double>(attrib.texcoords[2 * v + 1]));
  }

  // For each shape
  for (size_t i = 0; i < shapes.size(); i++) {
    printf("shape[%ld].name = %s\n", static_cast<long>(i),
           shapes[i].name.c_str());
    printf("Size of shape[%ld].indices: %lu\n", static_cast<long>(i),
           static_cast<unsigned long>(shapes[i].mesh.indices.size()));

    size_t index_offset = 0;

    assert(shapes[i].mesh.num_face_vertices.size() ==
           shapes[i].mesh.material_ids.size());

    printf("shape[%ld].num_faces: %lu\n", static_cast<long>(i),
           static_cast<unsigned long>(shapes[i].mesh.num_face_vertices.size()));

    // For each face
    for (size_t f = 0; f < shapes[i].mesh.num_face_vertices.size(); f++) {
      size_t fnum = shapes[i].mesh.num_face_vertices[f];

      printf("  face[%ld].fnum = %ld\n", static_cast<long>(f),
             static_cast<unsigned long>(fnum));

      // For each vertex in the face
      for (size_t v = 0; v < fnum; v++) {
        tinyobj::index_t idx = shapes[i].mesh.indices[index_offset + v];
        printf("    face[%ld].v[%ld].idx = %d/%d/%d\n", static_cast<long>(f),
               static_cast<long>(v), idx.vertex_index, idx.normal_index,
               idx.texcoord_index);
      }

      printf("  face[%ld].material_id = %d\n", static_cast<long>(f),
             shapes[i].mesh.material_ids[f]);

      index_offset += fnum;
    }

    printf("shape[%ld].num_tags: %lu\n", static_cast<long>(i),
           static_cast<unsigned long>(shapes[i].mesh.tags.size()));
    for (size_t t = 0; t < shapes[i].mesh.tags.size(); t++) {
      printf("  tag[%ld] = %s ", static_cast<long>(t),
             shapes[i].mesh.tags[t].name.c_str());
      printf(" ints: [");
      for (size_t j = 0; j < shapes[i].mesh.tags[t].intValues.size(); ++j) {
        printf("%ld", static_cast<long>(shapes[i].mesh.tags[t].intValues[j]));
        if (j < (shapes[i].mesh.tags[t].intValues.size() - 1)) {
          printf(", ");
        }
      }
      printf("]");

      printf(" floats: [");
      for (size_t j = 0; j < shapes[i].mesh.tags[t].floatValues.size(); ++j) {
        printf("%f", static_cast<const double>(
                         shapes[i].mesh.tags[t].floatValues[j]));
        if (j < (shapes[i].mesh.tags[t].floatValues.size() - 1)) {
          printf(", ");
        }
      }
      printf("]");

      printf(" strings: [");
      for (size_t j = 0; j < shapes[i].mesh.tags[t].stringValues.size(); ++j) {
        printf("%s", shapes[i].mesh.tags[t].stringValues[j].c_str());
        if (j < (shapes[i].mesh.tags[t].stringValues.size() - 1)) {
          printf(", ");
        }
      }
      printf("]");
      printf("\n");
    }
  }

  for (size_t i = 0; i < materials.size(); i++) {
    printf("material[%ld].name = %s\n", static_cast<long>(i),
           materials[i].name.c_str());
    printf("  material.Ka = (%f, %f ,%f)\n",
           static_cast<const double>(materials[i].ambient[0]),
           static_cast<const double>(materials[i].ambient[1]),
           static_cast<const double>(materials[i].ambient[2]));
    printf("  material.Kd = (%f, %f ,%f)\n",
           static_cast<const double>(materials[i].diffuse[0]),
           static_cast<const double>(materials[i].diffuse[1]),
           static_cast<const double>(materials[i].diffuse[2]));
    printf("  material.Ks = (%f, %f ,%f)\n",
           static_cast<const double>(materials[i].specular[0]),
           static_cast<const double>(materials[i].specular[1]),
           static_cast<const double>(materials[i].specular[2]));
    printf("  material.Tr = (%f, %f ,%f)\n",
           static_cast<const double>(materials[i].transmittance[0]),
           static_cast<const double>(materials[i].transmittance[1]),
           static_cast<const double>(materials[i].transmittance[2]));
    printf("  material.Ke = (%f, %f ,%f)\n",
           static_cast<const double>(materials[i].emission[0]),
           static_cast<const double>(materials[i].emission[1]),
           static_cast<const double>(materials[i].emission[2]));
    printf("  material.Ns = %f\n",
           static_cast<const double>(materials[i].shininess));
    printf("  material.Ni = %f\n", static_cast<const double>(materials[i].ior));
    printf("  material.dissolve = %f\n",
           static_cast<const double>(materials[i].dissolve));
    printf("  material.illum = %d\n", materials[i].illum);
    printf("  material.map_Ka = %s\n", materials[i].ambient_texname.c_str());
    printf("  material.map_Kd = %s\n", materials[i].diffuse_texname.c_str());
    printf("  material.map_Ks = %s\n", materials[i].specular_texname.c_str());
    printf("  material.map_Ns = %s\n",
           materials[i].specular_highlight_texname.c_str());
    printf("  material.map_bump = %s\n", materials[i].bump_texname.c_str());
    printf("  material.map_d = %s\n", materials[i].alpha_texname.c_str());
    printf("  material.disp = %s\n", materials[i].displacement_texname.c_str());
    printf("  <<PBR>>\n");
    printf("  material.Pr     = %f\n", materials[i].roughness);
    printf("  material.Pm     = %f\n", materials[i].metallic);
    printf("  material.Ps     = %f\n", materials[i].sheen);
    printf("  material.Pc     = %f\n", materials[i].clearcoat_thickness);
    printf("  material.Pcr    = %f\n", materials[i].clearcoat_thickness);
    printf("  material.aniso  = %f\n", materials[i].anisotropy);
    printf("  material.anisor = %f\n", materials[i].anisotropy_rotation);
    printf("  material.map_Ke = %s\n", materials[i].emissive_texname.c_str());
    printf("  material.map_Pr = %s\n", materials[i].roughness_texname.c_str());
    printf("  material.map_Pm = %s\n", materials[i].metallic_texname.c_str());
    printf("  material.map_Ps = %s\n", materials[i].sheen_texname.c_str());
    printf("  material.norm   = %s\n", materials[i].normal_texname.c_str());
    std::map<std::string, std::string>::const_iterator it(
        materials[i].unknown_parameter.begin());
    std::map<std::string, std::string>::const_iterator itEnd(
        materials[i].unknown_parameter.end());

    for (; it != itEnd; it++) {
      printf("  material.%s = %s\n", it->first.c_str(), it->second.c_str());
    }
    printf("\n");
  }
}

void TextRendering_InitialScreen(GLFWwindow* window){
    TextRendering_PrintString(window, "       PACMAN       ", -0.28f,   0.1f, 2.0f);
    TextRendering_PrintString(window, "Press Enter to Start", -0.28f,  -0.1f, 2.0f);

    TextRendering_PrintString(window, "Alunos: Pedro Company Beck    ", -1.0f,  -0.8f, 1.0f);
    TextRendering_PrintString(window, "        Wagner Gomes Ferreira ", -1.0f,  -0.9f, 1.0f);
}

// set makeprg=cd\ ..\ &&\ make\ run\ >/dev/null
// vim: set spell spelllang=pt_br :

