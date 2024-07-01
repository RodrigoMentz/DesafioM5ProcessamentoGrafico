#pragma once

#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

class Sprite
{
    public:
    Sprite() {}
    ~Sprite();

    void inicializar(GLuint texID, int nAnimations = 1, int nFrames = 1, glm::vec3 pos=glm::vec3(0), glm::vec3 escala = glm::vec3(1.0, 1.0, 1.0), float angulo = 0.0, glm::vec3 cor = glm::vec3(1.0, 1.0, 0.0));
    void atualizar();
    void desenhar();
    void moverParaDireita();
    void moverParaEsquerda();
    void moverParaCima();
    void moverParaBaixo();

    inline void setShader(Shader *shader) { this->shader = shader; }
    inline void setAngulo(float angulo) { this->angulo = angulo; }
    protected:

    GLuint VAO;
    GLuint texID;

     glm::vec3 pos, escala;
    float angulo;

    int nAnimations, nFrames, iAnimation, iFrame;
    glm::vec2 offsetTex; //ds e dt 

    Shader *shader;
    float vel;
    bool estaRotacionada;
    float lastTime;
    float FPS;
};