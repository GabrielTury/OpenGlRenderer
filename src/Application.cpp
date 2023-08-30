#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include<iostream>

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

    glfwTerminate();
    return 0;
}