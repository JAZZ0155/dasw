#include <GL/glut.h>
#include <Windows.h>
#include <math.h>
#include <stdio.h>

#define Pi 3.1415926
#define Pr 15			//缩放倍数

class control;

class sine
{
public:
	control *con;
	float x,y;
	float rx,ry;
	int k;			//函数上的样点数
	float *point;
	int length;

public:
	void update();
	void draw();
	sine(control *con,float x,float y);
	~sine(void);
};

class circle
{
public:
	control *con;
	int index;
	int cycletime;
	float r;
	float x,y;
	float rx,ry;
	int k;					//圆上样点的个数
public:
	void update(circle *c);
	void draw();
	circle(control *con,int index,float x,float y);
	~circle(void);
};

class line
{
public:
	control *con;
	float sx,sy;
	float cx,cy;
public:
	void draw();
	void update();
	line(control *con);
	~line(void);
};

class control
{
public:
	float z;
	float r[10];
	int cycletime[10];
	int tc;				//总周期
	int num;			//三角函数项个数
	int t;				//时间
	sine *s;
	circle *c[10];
	line *l;
public:
	void draw();
	void update();
	control(float r[],int cycletime[],int num,int z);
	~control(void);
};

class admin
{
public:
	int num;
	control *cc[10];
public:
	admin(void);
	~admin(void);
	void display();
	void idle();
	void addcontrol(float r[],int cycletime[],int num,int z);
};

//----------------------------------------------------------------

admin a;

void myDisplay()
{
	a.display();
}

void myIdle()
{
	a.idle();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(1200, 700);
	glutCreateWindow("Fourier");

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50, 12/7, 1, 20);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(4, 2, 4, 0, 0, 0, -1, 5, -1);

	float r1[2] = { 6 ,2};
	float r2[1] = { 6 };
	//float r3[1] = { 3 };
	float r4[1] = { 2 };

	int cycletime1[2] = { 30, 10 };
	int cycletime2[1] = { 30 };
	//int cycletime3[1] = { 20 };
	int cycletime4[1] = { 10 };

	int num1 = 2;
	int num2 = 1;
	//int num3 = 1;
	int num4 = 1;

	int z1 = -10;
	int z2 = 0;
	//int z3 = 5;
	int z4 = 10;
	a.addcontrol(r1, cycletime1, num1, z1);
	a.addcontrol(r2, cycletime2, num2, z2);
	//a.addcontrol(r3, cycletime3, num3, z3);
	a.addcontrol(r4, cycletime4, num4, z4);
	glutDisplayFunc(&myDisplay);
	glutIdleFunc(&myIdle);

	glutMainLoop();
return 0;
}

//------------------------------------------------------------------control

control::control(float r[],int cycletime[],int num,int z)
{
	this->t=0;

	this->z=z;

	this->num=num;
	for(int i=0;i<num;i++)
	{
		this->r[i]=r[i];
		this->cycletime[i]=cycletime[i];
	}
	this->tc=cycletime[0];
	
	this->s=new sine(this,5,0);
	this->c[0]=new circle(this,0,25,0);
	for(int i=1;i<num;i++)
	{
		this->c[i]=new circle(this,i,this->c[i-1]->rx,0);
	}
	this->l=new line(this);
}

control::~control(void)
{
}

void control::update()
{
	this->t++;
	//if(this->t==this->tc)
	//	this->t=0;
	
	c[0]->update(NULL);
	for(int i=1;i<num;i++)
	{
		c[i]->update(c[i-1]);
	}
	s->update();
	l->update();
}

void control::draw()
{
	s->draw();
	for(int i=0;i<num;i++)
	{
		c[i]->draw();
	}
	l->draw();
}

//-------------------------------------------------------------------------------circle

circle::circle(control *con,int index,float x,float y)
{
	this->con=con;
	this->index=index;
	this->cycletime=con->cycletime[index];
	this->r=con->r[index];
	this->x=x;
	this->y=y;
	this->rx=this->x+this->r;
	this->ry=this->y;

	this->k=50;
}

circle::~circle(void)
{
}

void circle::draw()
{
	glBegin(GL_LINE_LOOP);
	glColor3f(1.0f,1.0f,1.0f);
	for(int i=0;i<k;i++)
	{
		glVertex3f((this->x+this->r*cos(2*Pi/k*i))/Pr,(this->y+this->r*sin(2*Pi/k*i))/Pr,this->con->z/Pr);
	}
	glEnd();
	glColor3f(1.0f,1.0f,0.0f);
	glBegin(GL_LINES);
	glVertex3f(this->x/Pr,this->y/Pr,this->con->z/Pr);
	glVertex3f(this->rx/Pr,this->ry/Pr,this->con->z/Pr);
	glEnd();
	glColor3f(1.0,1.0,1.0);
}

void circle::update(circle *c)
{
	if(c!=NULL)
	{
		this->x=c->rx;
		this->y=c->ry;
	}
	this->rx=this->x+this->r*cos(2*Pi*this->con->t/this->cycletime);
	this->ry=this->y+this->r*sin(2*Pi*this->con->t/this->cycletime);
}

//----------------------------------------------------------------------line

line::line(control *con)
{
	this->con=con;
	this->sx=con->s->rx;
	this->sy=con->s->ry;
	this->cx=con->c[con->num-1]->rx;
	this->cy=con->c[con->num-1]->ry;
}

line::~line(void)
{
}

void line::update()
{
	this->sx=con->s->rx;
	this->sy=con->s->ry;
	this->cx=con->c[con->num-1]->rx;
	this->cy=con->c[con->num-1]->ry;
}

void line::draw()
{
	//glColor3f(1.0f,0.0f,0.0f);					//红色x轴
	//glBegin(GL_LINES);
	//glVertex3f(-10,0,0);
	//glVertex3f(10,0,0);
	//glEnd();

	//glColor3f(0.0f,1.0f,0.0f);					//蓝色y轴
	//glBegin(GL_LINES);
	//glVertex3f(0,10,0);
	//glVertex3f(0,-10,0);
	//glEnd();

	//glColor3f(0.0f,0.0f,1.0f);					//绿色z轴
	//glBegin(GL_LINES);
	//glVertex3f(0,0,10);
	//glVertex3f(0,0,-10);
	//glEnd();

	glColor3f(0.0f,1.0f,1.0f);					//青线
	glBegin(GL_LINES);
	glVertex3f(cx/Pr,cy/Pr,this->con->z/Pr);
	glVertex3f(sx/Pr,sy/Pr,this->con->z/Pr);
	glEnd();
	glColor3f(1.0f,1.0f,1.0f);
}

//--------------------------------------------------------------sine

sine::sine(control *con,float x,float y)
{
	//this->length=2*Pi*k/con->tc;
	this->length=100;
	this->point=new float[length];

	this->con=con;
	this->x=x;
	this->y=y;
	this->rx=x;
	this->ry=y;

	for(int i=0;i<length;i++)
	{
		point[i]=this->ry;
	}
}

sine::~sine(void)
{
}

void sine::draw()
{
	glBegin(GL_LINE_STRIP);
	for(int i=0;i<length;i++)
	{
		//glColor3f(0.4f+(float)(con->t%12)/20,0.8f,(120-8*con->z)/255);
		//glColor3f(0.0f,0.0f,(120-6*con->z)/255);
		glColor3f(0.6f,fabs(this->point[i])*0.2f,fabs(this->con->z-10.0)*0.05f);
		//glColor3f(0.5f,0.5f,(120-6*con->z)/255);
		glVertex3f((this->x-i)/Pr,this->point[i]/Pr,this->con->z/Pr);
	}
	glEnd();
}

void sine::update()
{
	float temp=0;
	for(int j=0;j<con->num;j++)
	{
		temp+=this->con->c[j]->ry-this->con->c[j]->y;
	}
	this->ry=this->y+temp;
	for(int j=this->length-1;j>0;j--)
	{
		this->point[j]=this->point[j-1];
	}
	this->point[0]=this->ry;
}

//-------------------------------------------------------------------

admin::admin(void)
{
	this->num=0;
}

admin::~admin(void)
{
}

void admin::display()
{
	glEnable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT);
	for(int i=0;i<this->num;i++)
	{
		this->cc[i]->draw();
	}
	glFlush();
}

void admin::idle()
{
	for(int i=0;i<this->num;i++)
	{
		this->cc[i]->update();
	}
	this->display();
	Sleep(100);
}

void admin::addcontrol(float r[],int cycletime[],int num,int z)
{
	this->cc[this->num]=new control(r,cycletime,num,z);
	this->num++;
}