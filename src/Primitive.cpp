#include "Primitive.h"

Primitive::Primitive()
{
	s=1;
	t=1;
}


Primitive::~Primitive()
{
}

void Primitive::draw(){
}

void Primitive::initShaders()
{

}

void Primitive::setS(float s){
	this->s=s;
}

void Primitive::setT(float t){
	this->t=t;
}

Ptorus::Ptorus(float inner, float outer, float slices, float stacks)
{
	this->inner=inner;
	this->outer=outer;
	this->slices=slices;
	this->stacks=stacks;
}


Ptorus::~Ptorus()
{
}

void Ptorus::draw(){
	GLfloat vNormal[3];   
	double majorStep = 2.0f*3.14 / slices;   
	double minorStep = 2.0f*3.14 / stacks;   
	int i, j;   

	for (i=0; i<slices; ++i)    
	{   
		double a0 = i * majorStep;   
		double a1 = a0 + majorStep;   
		GLfloat x0 = (GLfloat) cos(a0);   
		GLfloat y0 = (GLfloat) sin(a0);   
		GLfloat x1 = (GLfloat) cos(a1);   
		GLfloat y1 = (GLfloat) sin(a1);

		glBegin(GL_TRIANGLE_STRIP);   
		for (j=0; j<=stacks; ++j)    
		{   
			double b = j * minorStep;   
			GLfloat c = (GLfloat) cos(b);   
			GLfloat r = outer * c + inner;   
			GLfloat z = outer * (GLfloat) sin(b);   

			// First point   
			glTexCoord2f((float)(i)/(float)(slices), (float)(j)/(float)(stacks));   
			vNormal[0] = x0*c;
			vNormal[1] = y0*c;
			vNormal[2] = z/inner;
			///normalizar
			double magnitude2=sqrt(vNormal[0]*vNormal[0]+vNormal[1]*vNormal[1]+vNormal[2]*vNormal[2]);
			vNormal[0] /=magnitude2;
			vNormal[1] /=magnitude2;
			vNormal[2] /= magnitude2;

			glNormal3fv(vNormal);   
			glVertex3f(x0*r, y0*r, z);   

			glTexCoord2f((float)(i+1)/(float)(slices), (float)(j)/(float)(stacks));   
			vNormal[0] = x1*c;   
			vNormal[1] = y1*c;   
			vNormal[2] = z/inner;   
			glNormal3fv(vNormal);   
			glVertex3f(x1*r, y1*r, z);   
		}   
		glEnd();   
	}
}

Prectangle::Prectangle(float x1, float y1, float x2, float y2)
{
	this->x1=x1;
	this->y1=y1;
	this->x2=x2;
	this->y2=y2;
	calcValues();
}

Prectangle::~Prectangle()
{
}

void Prectangle::calcValues(){
	x3=x2;
	y3=y1;
	x4=x1;
	y4=y2;
}

void Prectangle::draw(){
	glPushMatrix();
	glNormal3d(0,0,1);
	glBegin(GL_QUADS);
	glTexCoord2f(0,0);
	glVertex3d(x1,y1,0);
	glTexCoord2f(s,0);
	glVertex3d(x3,y3,0);
	glTexCoord2f(s,t);
	glVertex3d(x2,y2,0);
	glTexCoord2f(0,t);
	glVertex3d(x4,y4,0);
	glEnd();
	glPopMatrix();
}


Pcylinder::Pcylinder(float base, float top, float height, float slices, float stacks)
{
	this->base=base;
	this->top=top;
	this->height=height;
	this->slices=slices;
	this->stacks=stacks;
}


Pcylinder::~Pcylinder(void)
{
}

void Pcylinder::drawCircle(float radius){
	double delta = 2*pi/slices;

	glPushMatrix();
	glRotated(-90,1,0,0);
	glBegin(GL_POLYGON);
	for(unsigned int i =0; i <= slices; i++)
	{
		glTexCoord2d(0.5 + (cos(i*delta)/2), 0.5 + (sin(i*delta)/2));//tirar caso nao fique bem
		glNormal3d(sin(i*delta/2),0,cos(i*delta/2));
		glNormal3d(sin(i*delta),0,cos(i*delta));
		glVertex3d(radius*sin(i*delta),0,radius*cos(i*delta));
	}
	glEnd();
	glPopMatrix();
}

void Pcylinder::draw(){
	GLUquadricObj *qObj = gluNewQuadric();
	gluQuadricNormals(qObj, GLU_SMOOTH);
	gluQuadricTexture(qObj, GL_TRUE);
	glEnable(GL_TEXTURE_2D);
	//base
	drawCircle(base);
	//cylinder
	gluCylinder(qObj, base, top, height, slices, stacks);
	//top
	glPushMatrix();
	glTranslated(0,0,height);
	glRotated(180,1,0,0);
	drawCircle(top);
	glPopMatrix();
}

Psphere::Psphere(float radius, float slices, float stacks)
{
	this->radius=radius;
	this->slices=slices;
	this->stacks=stacks;
}


Psphere::~Psphere(void)
{
}

void Psphere::draw(){
	GLUquadricObj *qObj = gluNewQuadric();
	gluQuadricNormals(qObj, GLU_SMOOTH);
	gluQuadricTexture(qObj, GL_TRUE);
	glEnable(GL_TEXTURE_2D);
	gluSphere(qObj,radius,slices,stacks);
}

Ptriangle::Ptriangle(float x1,float y1,float z1, float x2,float y2, float z2, float x3, float y3,float z3){
	this->x1 =x1;
	this->y1 =y1;
	this->z1 =z1;
	this->x2 =x2;
	this->y2 =y2;
	this->z2 =z2;
	this->x3 =x3;
	this->y3 =y3;
	this->z3 =z3;
	calcValues();
}

Ptriangle::~Ptriangle(){
}

void Ptriangle::calcValues(){
	float a = sqrtf( pow((x1 - x3),2) + pow((y1 - y3),2) + pow((z1 - z3),2));
	float b = sqrtf( pow((x2 - x1),2) + pow((y2 - y1),2) + pow((z2 - z1),2));
	float c = sqrtf( pow((x3 - x2),2) + pow((y3 - y2),2) + pow((z3 - z2),2));

	float cosy=(pow(a,2)+pow(b,2)-pow(c,2))/(2*a*b);
	float cosa=(-pow(a,2)+pow(b,2)+pow(c,2))/(2*b*c);
	float cosb=(pow(a,2)-pow(b,2)+pow(c,2))/(2*a*c);

	p0[0]=c-a*cosb;
	p0[1]=a*sqrt(1-pow(cosb,2));
	p1[0]=0;
	p1[1]=0;
	p2[0]=c;
	p2[1]=0;
	calculateNormal();
}

void Ptriangle::calculateNormal() {
	float ab1,ab2,ab3,ac1,ac2,ac3;

	ab1 = x2 - x1;
	ab2 = y2 - y1;
	ab3 = z2 - z1;

	ac1 = x3 - x1;
	ac2 = y3 - y1;
	ac3 = z3 - z1;

	n1 = (ab2 * ac3) - (ab3 * ac2);
	n2 = (ac1 * ab3) - (ab1 * ac3);
	n3 = (ab1 * ac2) - (ac1 * ab2);
}

void Ptriangle::draw(){

	glPushMatrix();
	glNormal3d(n1,n2,n3);
	glBegin(GL_TRIANGLES);
	glTexCoord2d(p0[0],p0[1]);
	glVertex3d(x1,y1,z1);
	glTexCoord2d(p1[0],p1[1]);
	glVertex3d(x2,y2,z2);
	glTexCoord2d(p2[0],p2[1]);
	glVertex3d(x3,y3,z3);
	glEnd();
	glPopMatrix();
}

Pplane::Pplane( int parts )
{
	this->parts=parts;
}

float Pplane::getParts()
{
	return parts;
}

void Pplane::draw()
{ 
	GLfloat grid[4][3] = {
		{-0.5,0.0, -0.5}, {0.5, 0.0, -0.5},
		{-0.5, 0.0, 0.5}, {0.5, 0.0, 0.5}};
		GLfloat grid2[4][3] = {
			{0.0, 1.0, 0.0}, {0.0, 1.0, 0.0},
			{0.0, 1.0, 0.0}, {0.0, 1.0, 0.0}};
			GLfloat grid3[4][2] = {
				{0.0, t}, {s,t}, 
				{0.0, 0.0}, {s, 0.0}};

				glMap2f(GL_MAP2_VERTEX_3, 0.0, 1.0, 3, 2,0.0, 1.0, 6, 2, &grid[0][0]);
				glMap2f(GL_MAP2_NORMAL, 0.0, 1.0, 3, 2, 0.0, 1.0, 6, 2, &grid2[0][0]);
				glMap2f(GL_MAP2_TEXTURE_COORD_2, 0.0, 1.0, 2, 2,0.0, 1.0, 4, 2,  &grid3[0][0]);

				glEnable(GL_MAP2_VERTEX_3);
				glEnable(GL_MAP2_NORMAL);
				glEnable(GL_MAP2_TEXTURE_COORD_2);

				glMapGrid2f(parts, 0.0,1.0, parts, 0.0,1.0); 
				glEvalMesh2(GL_FILL, 0,parts, 0,parts);
}


Ppatch::Ppatch(int order, int partsU, int partsV, std::string compute){
	this->order=order;
	this->partsU=partsU;
	this->partsV=partsV;
	this->compute=compute;
}

void Ppatch::addElementToGrid(int i, int j, float value){
	if(order==1){
		grid_1[i][j]=value;
	}else if(order==2){
		grid_2[i][j]=value;
	}else if(order==3){
		grid_3[i][j]=value;
	}
}

void Ppatch::draw(){
	int font;
	glGetIntegerv(GL_FRONT_FACE, &font);

	glEnable(GL_CW);
	glEnable(GL_AUTO_NORMAL);

	if(order==1){
		GLfloat grid_1_2[4][2] = {
			{0.0, t}, {s,t},
			{0.0, 0.0}, {s, 0.0}};

			glMap2f(GL_MAP2_VERTEX_3, 0.0, 1.0, 3, 2 ,0.0, 1.0, 6, 2, &grid_1[0][0]);
			glMap2f(GL_MAP2_TEXTURE_COORD_2, 0.0, 1.0, 2, 2,0.0, 1.0, 4, 2,  &grid_1_2[0][0]);
	}else if(order==2){
		GLfloat grid_2_2[9][2] = {
			{0.0,t}, {s/2, t},{s,t},
			{0.0,t/2},{s/2, t/2},{s, t/2},
			{0.0,0.0},{s/2,0.0},{s,0.0}};

			glMap2f(GL_MAP2_VERTEX_3, 0.0, 1.0, 3, 3 ,0.0, 1.0, 9, 3, &grid_2[0][0]);
			glMap2f(GL_MAP2_TEXTURE_COORD_2, 0.0, 1.0, 2, 3 ,0.0, 1.0, 6, 3,  &grid_2_2[0][0]);
	}else if(order==3){
		GLfloat grid_3_3[16][2] = {
			{0.0,t}, {s/3, t},{2*s/3,t},{s,t},
			{0.0,2*t/3},{s/3,2*t/3},{2*s/3,2*t/3},{s,2*t/3},
			{0.0,t/3},{s/3,t/3},{2*s/3,t/3},{s,t/3},
			{0.0,0.0},{s/3,0.0},{2*s/3,0.0},{s,0.0}};

			glMap2f(GL_MAP2_VERTEX_3, 0.0, 1.0, 3, 4 ,0.0, 1.0, 12, 4, &grid_3[0][0]);
			glMap2f(GL_MAP2_TEXTURE_COORD_2, 0.0, 1.0, 2, 4 ,0.0, 1.0, 8, 4,  &grid_3_3[0][0]);
	}

	glEnable(GL_MAP2_VERTEX_3);
	glEnable(GL_MAP2_NORMAL);
	glEnable(GL_MAP2_TEXTURE_COORD_2);

	glMapGrid2f(partsU, 0.0,1.0, partsV, 0.0,1.0);

	if(compute=="line"){
		glEvalMesh2(GL_LINE, 0,partsU, 0,partsV);
	}else if(compute=="point"){
		glEvalMesh2(GL_POINT, 0,partsU, 0,partsV);
	}else if(compute=="fill"){
		glEvalMesh2(GL_FILL, 0,partsU, 0,partsV);
	}
	glEnable(font);
}

Pwaterline::Pwaterline(void)
{

}

Pwaterline::Pwaterline( std::string heightmap, std::string texturemap, std::string fragmentshader, std::string vertexshader ){	
	stag = new Pcylinder(2,2,2,50,50);
	plano= new Pplane(100);
	this->heightmap=heightmap;
	this->texturemap=texturemap;
	this->fragmentshader=fragmentshader;
	this->vertexshader=vertexshader;
	firstTime=true;
	lastTime=0;

}

void Pwaterline::setHeightMap( std::string height )
{
	this->heightmap=height;
}

std::string Pwaterline::getHeightMap()
{
	return this->heightmap;
}


std::string Pwaterline::getTextureMap()
{
	return this->texturemap;
}

void Pwaterline::setTextureMap( std::string texture )
{
	this->texturemap=texture;
}

std::string Pwaterline::getFragmentShader()
{
	return this->fragmentshader;
}

void Pwaterline::setFragmentShader( std::string fragment )
{
	this->fragmentshader=fragment;
}

std::string Pwaterline::getVertexShader()
{
	return this->vertexshader;
}

void Pwaterline::setVertexShader( std::string vertex )
{
	this->vertexshader=vertex;
}

void Pwaterline::draw()
{
	if(firstTime){
		//Path dos ficheiros
		init(vertexshader.c_str(),fragmentshader.c_str());
		CGFshader::bind();//tratar dos shaders

		//inicia as texturas
		baseTexture=new CGFtexture(texturemap);
		secTexture=new CGFtexture(heightmap);

		timer=0;//tempo actual
		timerL = glGetUniformLocation(id(), "timer");//tempo a ser usado no .vert

		//identifica a textura (GL_TEXTURE0 e 1)
		baseImageLoc = glGetUniformLocation(id(), "textImage");
		glUniform1i(baseImageLoc, 0);

		secImageLoc = glGetUniformLocation(id(), "mapImage");
		glUniform1i(secImageLoc, 1);

		firstTime=false;
	}else{
		CGFshader::bind();
		glUniform1f(timerL, timer);//update para o .vert
	}

	glActiveTexture(GL_TEXTURE0);//activa a textura com o id

	baseTexture->apply();

	glActiveTexture(GL_TEXTURE1);

	secTexture->apply();

	glActiveTexture(GL_TEXTURE0);

	plano->draw();

	CGFshader::unbind();
}

void Pwaterline::update(unsigned long systm){
	if(timer==0){
		lastTime=systm;
		timer=0.01;
	}
	if(systm-lastTime>=100){
		timer+=0.05;
		lastTime=systm;
	}
}

