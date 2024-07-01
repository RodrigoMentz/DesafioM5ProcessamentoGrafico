#include "Sprite.h"

Sprite::~Sprite()
{
    // Pede pra OpenGL desalocar os buffers
	glDeleteVertexArrays(1, &VAO);
}

enum Direcoes {
	Baixo = 0,
	Esquerda = 1,
	Cima = 2,
	Direita = 3
};

void Sprite::inicializar(GLuint texID, int nAnimations, int nFrames, glm::vec3 pos, glm::vec3 escala, float angulo, glm::vec3 cor)
{
    this->pos = pos;
    this->escala.x = escala.x / (float) nFrames;
	this->escala.y = escala.y / (float) nAnimations;
    this->angulo = angulo;
	this->texID = texID;
	this->nAnimations = nAnimations;
	this->nFrames = nFrames;

	offsetTex.s = 1.0/ (float) nFrames;
	offsetTex.t = 1.0/ (float) nAnimations; 
	vel = 5.0;
	iAnimation = Direita;
	estaRotacionada = false;
    //Especificação da geometria da sprite (quadrado, 2 triangulos)
    GLfloat vertices[] = {
		//x   y    z    r      g      b      s    t
		-0.5, 0.5, 0.0, cor.r, cor.g, cor.b, 0.0, offsetTex.t, //v0
        -0.5,-0.5, 0.0, cor.r, cor.g, cor.b, 0.0, 0.0, //v1
         0.5, 0.5, 0.0, cor.r, cor.g, cor.b, offsetTex.s, offsetTex.t, //v2
        -0.5,-0.5, 0.0, cor.r, cor.g, cor.b, 0.0, 0.0, //v1
         0.5,-0.5, 0.0, cor.r, cor.g, cor.b, offsetTex.s, 0.0, //v3
         0.5, 0.5, 0.0, cor.r, cor.g, cor.b, offsetTex.s, offsetTex.t  //v2
	};

	GLuint VBO;
	
	glGenBuffers(1, &VBO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);
	
	glBindVertexArray(VAO);

	//Atributo layout 0 - Posição - 3 valores dos 8 que descrevem o vértice
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Atributo layout 1 - Cor - 3 valores dos 8 que descrevem o vértice
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3* sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	//Atributo layout 2 - Coordenada de textura - 2 valores dos 8 que descrevem o vértice
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6* sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0); 

	glBindVertexArray(0);

	FPS = 12.0;
	lastTime = 0.0;

}

void Sprite::atualizar()
{
	float now = glfwGetTime();

	float dt = now - lastTime;

	if (dt >= 1 / FPS)
	{
		iFrame = (iFrame + 1) % nFrames;
		lastTime = now;
	}

	float offsetTexFrameS = iAnimation * offsetTex.s;
	float offsetTexFrameT = iFrame * offsetTex.t;
	shader->setVec2("offsetTex", offsetTexFrameS, offsetTexFrameT);

	glm::mat4 model = glm::mat4(1); //matriz identidade

	model = glm::translate(model, pos);
	model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0, 0.0, 1.0));
	model = glm::scale(model, escala);

	if (this->nAnimations == 4 && iAnimation == Esquerda) {
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));
	}
	if (this->nAnimations == 4 && iAnimation == Baixo) {
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(1.0, 0.0, 0.0));
		estaRotacionada = true;
	}

	//resolver quando boneco vai para baixo e para os lados, para nao ficar virado
	if (this->nAnimations == 4 && iAnimation == Direita && estaRotacionada) {
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(1.0, 0.0, 0.0));
	}

	if (this->nAnimations == 4 && iAnimation == Esquerda && estaRotacionada) {
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(1.0, 0.0, 0.0));
	}

	if (this->nAnimations == 4 && iAnimation == Cima) {
		estaRotacionada = false;
	}

	shader->setMat4("model", glm::value_ptr(model));
}

void Sprite::desenhar()
{
    atualizar();

	glBindTexture(GL_TEXTURE_2D, texID);

    glBindVertexArray(VAO); 

	glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Sprite::moverParaDireita()
{
	if (pos.x >= 780)
	{
		pos.x = 780;
	}
	else
	{
		pos.x += vel;
		if (escala.x < 0.0)
			escala.x = -escala.x;
		iAnimation = Direita;
	}
}

void Sprite::moverParaEsquerda()
{
	if (pos.x <= 20)
	{
		pos.x = 20;
	}
	else
	{
		pos.x -= vel;
		if (escala.x > 0.0)
			escala.x = -escala.x;
		iAnimation = Esquerda;
	}
}

void Sprite::moverParaCima()
{
	if (pos.y >= 350)
	{
		pos.y = 350;
	}
	else
	{
		pos.y += vel;
		if (escala.y < 0.0)
			escala.y = -escala.y;
		iAnimation = Cima;
	}

}

void Sprite::moverParaBaixo()
{
	if (pos.y <= 140)
	{
		pos.y = 140;
	}
	else
	{
		pos.y -= vel;
		if (escala.y > 0.0)
			escala.y = -escala.y;
		iAnimation = Baixo;
	}
}