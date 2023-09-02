#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include<iostream>

#include <fstream> //biblioteca para utilizar outros arquivos
#include <string>
#include <sstream>

struct ShaderProgramSource //struct eh um jeito de agrupar variaveis em um único tipo, podendo acessa-las individualmente depois
{
    std::string VertexSource;
    std::string FragmnentSource;
};

static ShaderProgramSource ParseShader(const std::string& filepath) //Vai transformar o arquivo  com os shaders em strings, separando o vertex do fragment
{
    std::ifstream stream(filepath); //da input no arquivo do filepath

    enum class ShaderType //enum que define os valores de cada shader
    {
        NONE = -1, VERTEX =0, FRAGMENT = 1
    };

    std::string line; //linha que vai ser lida
    std::stringstream ss[2]; //onde vao armazenar as linhas stringstream é uma sequencia de strings juntas em 1
    ShaderType type = ShaderType::NONE; //seta pro default
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos) //se achar um #shader e não estiver no fim da linha (o npos significa que está no fim da linha)
        {
            if (line.find("vertex") != std::string::npos) //se achar um vertex e não estiver no fim da linhas
            {
                //set vertex mode
                type = ShaderType::VERTEX; //transforma o tipo em vertex
            }
            else if (line.find("fragment") != std::string::npos) // se achar um fragment transforma o tipo em fragment
            {
                //set fragment mode
                type = ShaderType::FRAGMENT;
            }
        }
        else
        {
            ss[(int)type] << line << '\n'; //ss[(int)type] serve para ver em qual stringstream a linha vai ser adicionada, 
                                           //se for VERTEX (VERTEX é igual a 0 como está no enum), vai adicionar a linha atual  ao ss[0] , assim organiza os  e separa em vertex e fragment shaders 
        }
    }
    return { ss[0].str(), ss[1].str() }; // retorna o struct, dando o valor de ss[0] (VERTEX) a string VertexSource, e o valor de ss[1] a FragmentSource
}

static unsigned int CompileShader(unsigned int type, const std::string& source) //compila o shader e verifica se tem algum erro
{
    unsigned int id = glCreateShader(type);

    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    //Error Handling
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id,length, &length, message);
        std::cout << "Failed to compile" <<(type == GL_VERTEX_SHADER ? "vertex": "fragment") << "Shader!" << std::endl;
        std::cout << message << std::endl;

        glDeleteShader(id);
        return 0;
    }

    return id; //retorna um id para identifica o programa
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) //Pega os codigos que foram feitos em glsl como string(ver implementação)                                                                                                     //
{                                                                                                    //e cria um programa opengl com eles, essencialmente junta o vertex com o fragment shader
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs); 
    glLinkProgram(program);
    glValidateProgram(program); // junta o fragment e vertex shader, cria um programa com eles, e valida esse programa (4 linhas de cima)

    glDeleteShader(vs); 
    glDeleteShader(fs); //essas 2 linhas deletam o shader pois ja foram criados e juntados em um programa

    return program; // retorna o programa do shader
}


int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;
    


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    //Starta a library glew que tem ponteiros para as funções do openGl que estão nos drives da placa de video
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Error, no valid openGl Context.";
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

    float positions[6] =
    {
        -0.5f,-0.5f,
         0.0f, 0.5f,
         0.5f, -0.5f
    };

    unsigned int buffer;
    glGenBuffers(1, &buffer /* o & faz passar o endereço de memória do buffer ao invés do valor do int */);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);//Bind buffer deixa o buffer selecionado como "ativo" e eh oq vai ser usado pelo openGL, funciona como as layers do photoshop, o programa soh altera a layer selecionada
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float),positions,GL_STATIC_DRAW); /*seta o buffer para ter as informações dos vertices do array triangleVertex e seta como statico
                                                                                      e avisa que vai usa-lo para criar algo (desenhar)*/

    glEnableVertexAttribArray(0); //habilita a atribuição do array (linha abaixo)
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);//Essa linha informa para o openGl, como deve ler o array de vertices criadoe bindado(bound)
                                                                          //fala em qual indice deve começar no caso 0 (o primeiro)
                                                                          //O numero de componentes por vertice (no caso 2 floats por vertice
                                                                          //O tipo dos componentes do array
                                                                          //Se ele deve ser normalizado ou n (floats ja são normalizados)
                                                                          //Quantos bytes (unidades de memória) ele deve pular até ler o próximo componente sem ser a posição (no caso o tamanho é de 2 floats)
                                                                          //Em qual index começam os componentes(atributos) que deve ler

    //std::string vertexShader =                                            //
    //    "#version 330 core\n"                                             //
    //    "\n"                                                              //
    //    "layout(location = 0) in vec4 position;\n"                        //
    //    "\n"                                                              //
    //    "void main()\n"                                                   //
    //    "{\n"                                                             //
    //    "   gl_Position = position;\n"                                    //
    //    "}\n";                                                            //código do shader, para o vertex

    //std::string fragmentShader =                                          //
    //    "#version 330 core\n"                                             //
    //    "\n"                                                              //
    //    "layout(location = 0) out vec4 color;\n"                          //
    //    "\n"                                                              //
    //    "void main()\n"                                                   //
    //    "{\n"                                                             //
    //    "   color = vec4(1.0, 0.0, 0.0, 1.0);\n"                          //
    //    "}\n";                                                            //código do shader, para o fragment

    ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");
    unsigned int shader = CreateShader(source.VertexSource, source.FragmnentSource); // cria o programa de shader levando esseas 2 códigos de shader como strings
    glUseProgram(shader); // usa esse programa para exibir na tela

    /* Loop until the user closes the window (Como o Update da unity e o Tick da Unreal)*/
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);
        /*//Legacy openGl
        glBegin(GL_TRIANGLES);
        glVertex2f(-0.5f,-0.5f);
        glVertex2f(0.0f, 0.5f);
        glVertex2f(0.5f, -0.5f);
        glEnd();*/


        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shader); // deleta o programa depois do uso (uso sendo a tela fechar)

    glfwTerminate();
    return 0;
}