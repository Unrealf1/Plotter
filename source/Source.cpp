#include <GL\freeglut.h>
#include <vector>
#include <thread>
#include <iostream>
#include <math.h>
#include <windows.h>
#include <sstream>

using namespace std;

HWND  hWnd = NULL;              // Здесь будет хранится дескриптор окна

struct coord {
	float x, y, z;
};

struct color {
	float R, G, B, A;
};

struct figure {
	int type; //0 - нет 1 - полигон 2 - квадрат 3 - круг 4 - ломаная линия 5 - точка
	vector<coord> top;
	vector<color> colors;
	coord center;
	float rad;
	coord angle;
	float anglesize;
	float width;
};
struct text {
	coord place, center;
	vector<char> word;
	int size; //10 или 24
	color colors;
};
double PI = 3.1415926535897932384626;
double e = 2.71828182845904523536;
int WINDOW_WIDTH, WINDOW_HEIGHT;

vector<vector<char>> History;

vector<figure> figures;
vector<text> words;
int scene = 1;

bool qualilty = 0;
vector<char> QResult;

bool history_open = 0;
bool menu_open = 0;

double Result = 0.0;
vector<char> THEstring;

double GrafParameters[5]; // Частота точек, растяжение в ширину и в высоту. сдвиг влево и вниз цвет точек

int lastbutton = -1;
GLuint base;

vector<char> NUMtoSTR(double D) {
	vector<char> word;
	ostringstream ost;
	ost << Result;
	string s_d = ost.str();
	word.resize(s_d.size());
	for (int i = 0; i < s_d.size(); i++) {
		word[i] = s_d[i];
	}
	return word;
}

long double fact(int n) {
	if (n < 0) {
		return 0;
	}
	long double R = 1;
	for (int i = 2; i <= n; i++) {
		R *= i;
	}
	return R;
} // 2 вариант fact-а

double fact1(int N)
{
	if (N < 0) 
		return 0;

	if (N == 0) 
		return 1;

	else 
		return N * fact1(N - 1);
}

/*GLvoid BuildFont(GLvoid) {  // Построение нашего растрового шрифта
	HFONT  font;
	base = glGenLists(151);  // Выделим место для 96 символов ( НОВОЕ )
	font = CreateFont( 50, 0, 0, 0, 700, false, false, false, ANSI_CHARSET, OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE | DEFAULT_PITCH, "Arial");
	SelectObject(NULL, font);        // Выбрать шрифт, созданный нами ( НОВОЕ )

	wglUseFontBitmaps(NULL, 32, 151, base); // Построить 96 символов начиная с пробела ( НОВОЕ )
	
}
*/
void historyOPCL() {
	float speed = 0.2f;
	if (history_open) {
		while (figures[37].top[2].y < 56) {

			figures[37].top[2].y += speed;
			figures[37].top[3].y += speed;
			for (int i = 0; i < 9; i++) {
				words[25 + i].place.y += speed;
			}
			for (int i = 0; i < 4; i++) {
				figures[38].top[i].y += speed;
				figures[39].top[i].y += speed;
				figures[40].top[i].y += speed;
			}
			Sleep(1);
		}
		for (int i = 0; i < 4; i++) {
			figures[37].colors[i].R = 1.0f;
			figures[37].colors[i].G = 0.0f;
			figures[37].colors[i].B = 0.0f;

		}
		words[25].word = { 92, '/' };
		for (int i = 33; i >= 26; i--) {
			words[i].word.clear();
		}
		history_open = false;
	}
	else {
		int N = History.size();
		for (int i = 0; i < 4; i++) {
			N--;
			if (N >= 0){
				if (History[N].size() != 0) {
					int t = 0;
					words[33 - i].word.push_back('=');
					for (t; History[N][t] != '='; t++) {
						words[33 - i].word.push_back(History[N][t]);
					}
					t++;
					for (t; t < History[N].size(); t++) {
						words[29 - i].word.push_back(History[N][t]);
					}
					words[33 - i].place.x = 35.0f - 5.0f * (words[33 - i].word.size());
				}
		}

		}
		for (int i = 0; i < 4; i++) {
			figures[37].colors[i].R = 0.65f;
			figures[37].colors[i].G = 0.65f;
			figures[37].colors[i].B = 0.65f;
		}
		while (figures[37].top[2].y > -100) {
			figures[37].top[2].y -= speed;
			figures[37].top[3].y -= speed;
			for (int i = 0; i < 9; i++) {
				words[25 + i].place.y -= speed;
			}
			for (int i = 0; i < 4; i++) {
				figures[38].top[i].y -= speed;
				figures[39].top[i].y -= speed;
				figures[40].top[i].y -= speed;
			}
			Sleep(1);
		}
		words[25].word = {'/', 92 };
		history_open = true;
	}
}

void menuOPCL() {
	if (menu_open) {
		while (figures[68].top[0].y < 87) {
			figures[68].top[0].y += 0.001;
			figures[68].top[1].y += 0.001;
			for (int i = 0; i < 4; i++) {
				figures[70].top[i].y += 0.001;
				figures[71].top[i].y += 0.001;

			}
		}
		menu_open = false;
	}
	else {
		while (figures[68].top[0].y > -100) {
			figures[68].top[0].y -= 0.001;
			figures[68].top[1].y -= 0.001;
			for (int i = 0; i < 4; i++) {
				figures[70].top[i].y -= 0.001;
				figures[71].top[i].y -= 0.001;

			}
		}
		menu_open = true;
	}
}

int add(int type, vector<coord> top, vector<color> colors, coord center, float rad, coord angle, float anglesize, float width) {
	figure A;
	A.type = type;
	A.top = top;
	A.colors = colors;
	A.center = center;
	A.rad = rad;
	A.angle = angle;
	A.anglesize = anglesize;
	A.width = width;

	figures.push_back(A);
	return figures.size() - 1;
}

void ReSize(int width, int height) {  // Изменить размер и инициализировать окно GL 
	WINDOW_HEIGHT = height;
	WINDOW_WIDTH = width;
	if (WINDOW_WIDTH < 300) {
		glutReshapeWindow(300, WINDOW_HEIGHT);
		WINDOW_WIDTH = 300;
	}
	if (WINDOW_HEIGHT < 300) {
		glutReshapeWindow(WINDOW_WIDTH, 300);
		WINDOW_HEIGHT = 300;
	}

	if (height == 0) {             // Предотвращение деления на ноль
		height = 1;
	}
	float ratio = width / height * 1.0f;
	// Сброс текущей области вывода 

	glMatrixMode(GL_PROJECTION);            // Выбор матрицы проекций

	glLoadIdentity();              // Сброс матрицы проекции
	glViewport(0, 0, width, height);
	gluPerspective(45.0f, ratio, 0.1f, 100.0f);// Вычисление соотношения геометрических размеров для окна

	//glMatrixMode(GL_MODELVIEW);            // Выбор матрицы вида модели // Из-за этого увеличивает
	cout << "Window size: " << width << ' ' << height << endl;
	glLoadIdentity();              // Сброс матрицы вида модели
}

int addword(coord center, coord place, vector<char> word, int size, color colors) {
	text A;
	A.center = center;
	A.place = place;
	A.word = word;
	A.size = size;
	A.colors = colors;

	words.push_back(A);
	return words.size() - 1;
}

void write(text A) {
	glLoadIdentity();
	glTranslatef(A.center.x, A.center.y, A.center.z);
	glColor4f(A.colors.R, A.colors.G, A.colors.B, A.colors.A);
	glRasterPos3f(A.place.x, A.place.y, A.place.z);
	switch (A.size)
	{
	default:
		break;
	case 10: {
		for (int i = 0; i < A.word.size(); i++) {
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, A.word[i]);
		}
		break;
	}
	case 24: {
		for (int i = 0; i < A.word.size(); i++) {
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, A.word[i]);
		}
		break;
	}
	}
}

bool changescene(int type){
	scene = type;

	figures.clear();
	words.clear();

	lastbutton = -1;

	THEstring.clear();
	switch (type)
	{
	default:
		break;
	case 1: {
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		vector<coord> top;
		coord C;
		vector<color> colors;
		color COL;
		coord center;
		coord angle;
		C.x = -100.0f;
		C.y = 60.0f;
		C.z = 3.0f;
		top.push_back(C);

		C.x = 50.0f;
		C.y = 60.0f;
		C.z = 3.0f;
		top.push_back(C);

		C.x = 50.0f;
		C.y = 55.0f;
		C.z = 3.0f;
		top.push_back(C);

		C.x = -100.0f;
		C.y = 55.0f;
		C.z = 3.0f;
		top.push_back(C);

		COL.R = 1.0f;
		COL.G = 0.0f;
		COL.B = 0.0f;
		COL.A = 1.0f;
		colors.push_back(COL);
		colors.push_back(COL);
		colors.push_back(COL);
		colors.push_back(COL);

		center.x = 0;
		center.y = -0.01f;
		center.z = 0;

		angle.x = 0.0;
		angle.y = 0.0;
		angle.z = 0;

		add(1, top, colors, center, 0, angle, 0, 0); // Горизонтальная линия, отделяющая "Экран"

		top.clear();
		colors.clear();

		C.x = 45.0f;
		C.y = 100.0f;
		C.z = 1.0f;
		top.push_back(C);

		C.x = 45.0f;
		C.y = -100.0f;
		C.z = 1.0f;
		top.push_back(C);

		C.x = 50.0f;
		C.y = -100.0f;
		C.z = 1.0f;
		top.push_back(C);

		C.x = 50.0f;
		C.y = 100.0f;
		C.z = 1.0f;
		top.push_back(C);

		COL.R = 1.0f;
		COL.G = 0.0f;
		COL.B = 0.0f;
		COL.A = 1.0f;
		colors.push_back(COL);
		colors.push_back(COL);
		colors.push_back(COL);
		colors.push_back(COL);

		center.x = 0;
		center.y = 0;
		center.z = 0;

		angle.x = 0.0;
		angle.y = 0.0;
		angle.z = 0;

		add(1, top, colors, center, 0, angle, 0, 0); //Вертикальная линия справа

		top.clear();
		colors.clear();

		C.x = -70.0f;
		C.y = 55.0f;
		C.z = 1.0f;
		top.push_back(C);

		C.x = -70.0f;
		C.y = -100.0f;
		C.z = 1.0f;
		top.push_back(C);

		C.x = -73.0f;
		C.y = -100.0f;
		C.z = 1.0f;
		top.push_back(C);

		C.x = -73.0f;
		C.y = 55.0f;
		C.z = 1.0f;
		top.push_back(C);

		COL.R = 1.0f;
		COL.G = 0.0f;
		COL.B = 0.0f;
		COL.A = 1.0f;
		colors.push_back(COL);
		colors.push_back(COL);
		colors.push_back(COL);
		colors.push_back(COL);

		center.x = 0.0f;
		center.y = 0.0f;
		center.z = 0.0f;

		angle.x = 0.0f;
		angle.y = 0.0f;
		angle.z = 0.0f;

		add(1, top, colors, center, 0, angle, 0, 0); // Вертикальная линия, отделяющая левую колонку действий
		for (int i = 1; i <= 6; i++) { // Горизонтальные линии для создания кнопок для операций слева
			top.clear();
			colors.clear();

			C.x = -100.0f;
			C.y = i * 25.83333f - 1.5f - 100;
			C.z = 1.0f;
			top.push_back(C);

			C.x = -100.0f;
			C.y = i * 25.83333f + 1.5f - 100;
			C.z = 1.0f;
			top.push_back(C);

			C.x = -70.0f;
			C.y = i * 25.83333f + 1.5f - 100;
			C.z = 1.0f;
			top.push_back(C);

			C.x = -70.0f;
			C.y = i * 25.83333f - 1.5f - 100;
			C.z = 1.0f;
			top.push_back(C);

			COL.R = 1.0f;
			COL.G = 0.0f;
			COL.B = 0.0f;
			COL.A = 1.0f;
			colors.push_back(COL);
			colors.push_back(COL);
			colors.push_back(COL);
			colors.push_back(COL);

			center.x = 0;
			center.y = 0;
			center.z = 0;

			angle.x = 0.0;
			angle.y = 0.0;
			angle.z = 0;

			add(1, top, colors, center, 0, angle, 0, 0);
		}
		top.clear();
		colors.clear();
		for (int i = 0; i < 6; i++) { //невидимые белые квадраты для нажатия кнопок (9 - 15)
			C.x = -100.0f;
			C.y = i * 25.83333f - 100;
			C.z = 0.2f;
			top.push_back(C);

			C.x = -100.0f;
			C.y = i * 25.83333f + 25.83333f - 100;
			C.z = 0.2f;
			top.push_back(C);

			C.x = -70.0f;
			C.y = i * 25.83333f + 25.83333f - 100;
			C.z = 0.2f;
			top.push_back(C);

			C.x = -70.0f;
			C.y = i * 25.83333f - 100;
			C.z = 0.2f;
			top.push_back(C);

			COL.R = 1.0f;
			COL.G = 1.0f;
			COL.B = 1.0f;
			COL.A = 0.2f;
			colors.push_back(COL);
			colors.push_back(COL);
			colors.push_back(COL);
			colors.push_back(COL);

			center.x = 0;
			center.y = 0;
			center.z = 0;

			angle.x = 0.0;
			angle.y = 0.0;
			angle.z = 0;

			add(0, top, colors, center, 0, angle, 0, 0);
			top.clear();
			colors.clear();
		}
		for (int i = 1; i <= 2; i++) {
			C.x = i * 39.3333f - 73.0f;
			C.y = 55.0f;
			C.z = 1.0f;
			top.push_back(C);

			C.x = i * 39.3333f - 73.0f;
			C.y = -100.0f;
			C.z = 1.0f;
			top.push_back(C);

			C.x = i * 39.3333f - 75.0f;
			C.y = -100.0f;
			C.z = 1.0f;
			top.push_back(C);

			C.x = i * 39.3333f - 75.0f;
			C.y = 55.0f;
			C.z = 1.0f;
			top.push_back(C);

			COL.R = 1.0f;
			COL.G = 0.0f;
			COL.B = 0.0f;
			COL.A = 1.0f;
			colors.push_back(COL);
			colors.push_back(COL);
			colors.push_back(COL);
			colors.push_back(COL);

			center.x = 0.0f;
			center.y = 0.0f;
			center.z = 0.0f;

			angle.x = 0.0f;
			angle.y = 0.0f;
			angle.z = 0.0f;

			add(1, top, colors, center, 0, angle, 0, 0); // разделительная линия для основных кгопок(цифры) вертикальные
			top.clear();
			colors.clear();
		}
		for (int i = 1; i <= 4; i++) {
			C.x = -70.0f;
			C.y = i * 31 - 101.0f;
			C.z = 1.0f;
			top.push_back(C);

			C.x = 45.0f;
			C.y = i * 31 - 101.0f;
			C.z = 1.0f;
			top.push_back(C);

			C.x = 45.0f;
			C.y = i * 31 - 99.0f;
			C.z = 1.0f;
			top.push_back(C);

			C.x = -70.0f;
			C.y = i * 31 - 99.0f;
			C.z = 1.0f;
			top.push_back(C);

			COL.R = 1.0f;
			COL.G = 0.0f;
			COL.B = 0.0f;
			COL.A = 1.0f;
			colors.push_back(COL);
			colors.push_back(COL);
			colors.push_back(COL);
			colors.push_back(COL);

			center.x = 0.0f;
			center.y = 0.0f;
			center.z = 0.0f;

			angle.x = 0.0f;
			angle.y = 0.0f;
			angle.z = 0.0f;

			add(1, top, colors, center, 0, angle, 0, 0); // разделительная линия для основных кгопок(цифры) горизонтальные
			top.clear();
			colors.clear();
		}

		for (int i = 1; i <= 3; i++) { //Невидимые белые квадраты для осн кнопок 21 - 36
			for (int t = 1; t <= 5; t++) {
				C.x = (i - 1) * 39.3333f - 73.0f;
				C.y = (t - 1) * 31 - 99.0f;
				C.z = 0.2f;
				top.push_back(C);

				C.x = (i - 1) * 39.3333f - 73.0f;
				C.y = t * 31 - 99.0f;
				C.z = 0.2f;
				top.push_back(C);

				C.x = i * 39.3333f - 73.0f;
				C.y = t * 31 - 99.0f;
				C.z = 0.2f;
				top.push_back(C);

				C.x = i * 39.3333f - 73.0f;
				C.y = (t - 1) * 31 - 99.0f;
				C.z = 0.2f;
				top.push_back(C);

				COL.R = 1.0f;
				COL.G = 1.0f;
				COL.B = 1.0f;
				COL.A = 0.2f;
				colors.push_back(COL);
				colors.push_back(COL);
				colors.push_back(COL);
				colors.push_back(COL);

				center.x = 0.0f;
				center.y = 0.0f;
				center.z = 0.0f;

				angle.x = 0.0f;
				angle.y = 0.0f;
				angle.z = 0.0f;

				add(0, top, colors, center, 0, angle, 0, 0);
				top.clear();
				colors.clear();
			}
		}
		C.x = -100.0f;
		C.y = 100.0f;
		C.z = 2.9f;
		top.push_back(C);

		C.x = 45.0f;
		C.y = 100.0f;
		C.z = 2.9f;
		top.push_back(C);

		C.x = 45.0f;
		C.y = 58.5f;
		C.z = 2.9f;
		top.push_back(C);

		C.x = -100.0f;
		C.y = 58.5f;
		C.z = 2.9f;
		top.push_back(C);

		COL.R = 0.8f;
		COL.G = 0.8f;
		COL.B = 0.8f;
		COL.A = 0.0f;
		colors.push_back(COL);
		colors.push_back(COL);
		colors.push_back(COL);
		colors.push_back(COL);

		center.x = 0.0f;
		center.y = 0.0f;
		center.z = 0.0f;

		angle.x = 0.0f;
		angle.y = 0.0f;
		angle.z = 0.0f;

		add(1, top, colors, center, 0, angle, 0, 0); // Фон экрана
		top.clear();
		colors.clear();

		C.x = -100.0f;
		C.y = 55.0f;
		C.z = 1.0f;
		top.push_back(C);

		C.x = 45.0f;
		C.y = 55.0f;
		C.z = 1.0f;
		top.push_back(C);

		C.x = 45.0f;
		C.y = 55.5f;
		C.z = 1.0f;
		top.push_back(C);

		C.x = -100.0f;
		C.y = 55.5f;
		C.z = 1.0f;
		top.push_back(C);

		COL.R = 1.0f;
		COL.G = 0.0f;
		COL.B = 0.0f;
		COL.A = 0.0f;
		colors.push_back(COL);
		colors.push_back(COL);
		colors.push_back(COL);
		colors.push_back(COL);

		center.x = 0.0f;
		center.y = 0.0f;
		center.z = 0.0f;

		angle.x = 0.0f;
		angle.y = 0.0f;
		angle.z = 0.0f;

		add(1, top, colors, center, 0, angle, 0, 0); // Фон истории
		top.clear();
		colors.clear();

		C.x = -100.0f;
		C.y = 55.0f;
		C.z = 1.2f;
		top.push_back(C);

		C.x = 45.0f;
		C.y = 55.0f;
		C.z = 1.2f;
		top.push_back(C);

		C.x = 45.0f;
		C.y = 58.0f;
		C.z = 1.2f;
		top.push_back(C);

		C.x = -100.0f;
		C.y = 58.0f;
		C.z = 1.2f;
		top.push_back(C);

		COL.R = 1.0f;
		COL.G = 0.0f;
		COL.B = 0.0f;
		COL.A = 0.0f;
		colors.push_back(COL);
		colors.push_back(COL);
		colors.push_back(COL);
		colors.push_back(COL);

		center.x = 0.0f;
		center.y = 0.0f;
		center.z = 0.0f;

		angle.x = 0.0f;
		angle.y = 0.0f;
		angle.z = 0.0f;

		add(1, top, colors, center, 0, angle, 0, 0); // Красная полоска в истории
		top.clear();
		colors.clear();

		for (int i = 0; i < 2; i++) {
			C.x = -100.0f;
			C.y = -100.0f + 38.25f * (i * 2) + 156;
			C.z = 1.1f;
			top.push_back(C);

			C.x = 45.0f;
			C.y = -100.0f + 38.25f * (i * 2) + 156;
			C.z = 1.1f;
			top.push_back(C);

			C.x = 45.0f;
			C.y = -100.0f + 38.25f * (i * 2 + 1) + 156;
			C.z = 1.1f;
			top.push_back(C);

			C.x = -100.0f;
			C.y = -100.0f + 38.25f * (i * 2 + 1) + 156;
			C.z = 1.1f;
			top.push_back(C);

			COL.R = 0.0f;
			COL.G = 1.0f;
			COL.B = 0.0f;
			COL.A = 0.0f;
			colors.push_back(COL);
			colors.push_back(COL);
			colors.push_back(COL);
			colors.push_back(COL);

			center.x = 0.0f;
			center.y = 0.0f;
			center.z = 0.0f;

			angle.x = 0.0f;
			angle.y = 0.0f;
			angle.z = 0.0f;

			add(1, top, colors, center, 0, angle, 0, 0); // Прямоугольники в истории
			top.clear();
			colors.clear();
		}
		for (int i = 0; i < 9; i++) {
			C.x = 50.0f;
			C.y = i * 23.0f - 100.0f;
			C.z = 1.2f;
			top.push_back(C);

			C.x = 100.0f;
			C.y = i * 23.0f - 100.0f;
			C.z = 1.2f;
			top.push_back(C);

			C.x = 100.0f;
			C.y = i * 23.0f + 3.0f - 100.0f;
			C.z = 1.2f;
			top.push_back(C);

			C.x = 50.0f;
			C.y = i * 23.0f + 3.0f - 100.0f;
			C.z = 1.2f;
			top.push_back(C);

			COL.R = 1.0f;
			COL.G = 0.0f;
			COL.B = 0.0f;
			COL.A = 0.0f;
			colors.push_back(COL);
			colors.push_back(COL);
			colors.push_back(COL);
			colors.push_back(COL);

			center.x = 0.0f;
			center.y = 0.0f;
			center.z = 0.0f;

			angle.x = 0.0f;
			angle.y = 0.0f;
			angle.z = 0.0f;

			add(1, top, colors, center, 0, angle, 0, 0); // Горизонтальные линии для операций справа
			top.clear();
			colors.clear();
		}
		C.x = 73.5f;
		C.y = 87.0f;
		C.z = 1.2f;
		top.push_back(C);

		C.x = 76.5f;
		C.y = 87.0f;
		C.z = 1.2f;
		top.push_back(C);

		C.x = 76.5f;
		C.y = -100.0f;
		C.z = 1.2f;
		top.push_back(C);

		C.x = 73.5f;
		C.y = -100.0f;
		C.z = 1.2f;
		top.push_back(C);

		COL.R = 1.0f;
		COL.G = 0.0f;
		COL.B = 0.0f;
		COL.A = 0.0f;
		colors.push_back(COL);
		colors.push_back(COL);
		colors.push_back(COL);
		colors.push_back(COL);

		center.x = 0.0f;
		center.y = 0.0f;
		center.z = 0.0f;

		angle.x = 0.0f;
		angle.y = 0.0f;
		angle.z = 0.0f;

		add(1, top, colors, center, 0, angle, 0, 0); // Вертикальная линия для операций справа
		top.clear();
		colors.clear();

		for (int i = 0; i < 2; i++) {
			for (int t = 0; t < 8; t++) {
				C.x = 50.0f + 25 * i;
				C.y = -100 + 23 * t + 3;
				C.z = 0.2f;
				top.push_back(C);

				C.x = 74.0f + 25 * i;
				C.y = -100 + 23 * t + 3;
				C.z = 0.2f;
				top.push_back(C);

				C.x = 74.0f + 25 * i;
				C.y = -100 + 23 * t + 23;
				C.z = 0.2f;
				top.push_back(C);

				C.x = 50.0f + 25 * i;
				C.y = -100 + 23 * t + 23;
				C.z = 0.2f;
				top.push_back(C);

				COL.R = 1.0f;
				COL.G = 1.0f;
				COL.B = 1.0f;
				COL.A = 0.0f;
				colors.push_back(COL);
				colors.push_back(COL);
				colors.push_back(COL);
				colors.push_back(COL);

				center.x = 0.0f;
				center.y = 0.0f;
				center.z = 0.0f;

				angle.x = 0.0f;
				angle.y = 0.0f;
				angle.z = 0.0f;

				add(0, top, colors, center, 0, angle, 0, 0); // Невидимые квадраты для операций справа
				top.clear();
				colors.clear();
			}
		}

		C.x = 50;
		C.y = 87;
		C.z = 3.75f;
		top.push_back(C);

		C.x = 100;
		C.y = 87;
		C.z = 3.75f;
		top.push_back(C);

		C.x = 100;
		C.y = 100;
		C.z = 3.75f;
		top.push_back(C);

		C.x = 50;
		C.y = 100;
		C.z = 3.75f;
		top.push_back(C);

		COL.R = 1.0f;
		COL.G = 1.0f;
		COL.B = 1.0f;
		COL.A = 0.0f;
		colors.push_back(COL);
		colors.push_back(COL);
		colors.push_back(COL);
		colors.push_back(COL);

		center.x = 0.0f;
		center.y = 0.0f;
		center.z = 0.0f;

		angle.x = 0.0f;
		angle.y = 0.0f;
		angle.z = 0.0f;

		add(0, top, colors, center, 0, angle, 0, 0); // Невидимый квадрат для меню
		top.clear();
		colors.clear();

		C.x = 50;
		C.y = 87;
		C.z = 3.6f;
		top.push_back(C);

		C.x = 100;
		C.y = 87;
		C.z = 3.6f;
		top.push_back(C);

		C.x = 100;
		C.y = 87;
		C.z = 3.6f;
		top.push_back(C);

		C.x = 50;
		C.y = 87;
		C.z = 3.6f;
		top.push_back(C);

		COL.R = 0.5f;
		COL.G = 0.0f;
		COL.B = 1.0f;
		COL.A = 0.0f;
		colors.push_back(COL);
		colors.push_back(COL);
		colors.push_back(COL);
		colors.push_back(COL);

		center.x = 0.0f;
		center.y = 0.0f;
		center.z = 0.0f;

		angle.x = 0.0f;
		angle.y = 0.0f;
		angle.z = 0.0f;

		add(1, top, colors, center, 0, angle, 0, 0); // Плитка меню
		top.clear();
		colors.clear();

		C.x = 50;
		C.y = 87;
		C.z = 3.7f;
		top.push_back(C);

		C.x = 100;
		C.y = 87;
		C.z = 3.7f;
		top.push_back(C);

		C.x = 100;
		C.y = 100;
		C.z = 3.7f;
		top.push_back(C);

		C.x = 50;
		C.y = 100;
		C.z = 3.7f;
		top.push_back(C);

		COL.R = 0.5f;
		COL.G = 0.5f;
		COL.B = 0.5f;
		COL.A = 1.0f;
		colors.push_back(COL);
		colors.push_back(COL);
		colors.push_back(COL);
		colors.push_back(COL);

		center.x = 0.0f;
		center.y = 0.0f;
		center.z = 0.0f;

		angle.x = 0.0f;
		angle.y = 0.0f;
		angle.z = 0.0f;

		add(1, top, colors, center, 0, angle, 0, 0); // Непрозрачный квадрат для меню
		top.clear();
		colors.clear();
		for (int i = 0; i < 2; i++) {
		C.x = 50;
		C.y = 274 - 20 * i + 1;
		C.z = 3.65f;
		top.push_back(C);

		C.x = 100;
		C.y = 274 - 20 * i + 1;
		C.z = 3.65f;
		top.push_back(C);

		C.x = 100;
		C.y = 274 - 20 * i - 1;
		C.z = 3.65f;
		top.push_back(C);

		C.x = 50;
		C.y = 274 - 20 * i - 1;
		C.z = 3.65f;
		top.push_back(C);

		COL.R = 1.0f;
		COL.G = 1.0f;
		COL.B = 1.0f;
		COL.A = 0.0f;
		colors.push_back(COL);
		colors.push_back(COL);
		colors.push_back(COL);
		colors.push_back(COL);

		center.x = 0.0f;
		center.y = 0.0f;
		center.z = 0.0f;

		angle.x = 0.0f;
		angle.y = 0.0f;
		angle.z = 0.0f;

		add(1, top, colors, center, 0, angle, 0, 0); // Разграничение в меню
		top.clear();
		colors.clear();
	}
		
		coord place; //Добавляю символы
		vector<char> word;
		int size; //10 или 24
		color CLR;

		center.x = 0.0f;
		center.y = 0.0f;
		center.z = 0.0f;

		place.x = -90.0f;
		place.y = -90.0f;
		place.z = 0.4f;

		word = { '+' };
		size = 24;
		CLR.R = 0.0f;
		CLR.G = 0.0f;
		CLR.B = 0.0f;
		CLR.A = 1.0f;

		addword(center, place, word, size, CLR);
		word.clear();


		center.x = 0.0f;
		center.y = 0.0f;
		center.z = 0.0f;

		place.x = -90.0f;
		place.y = -90.0f + 25.83333f;
		place.z = 0.4f;

		word = { '-' };
		size = 24;
		CLR.R = 0.0f;
		CLR.G = 0.0f;
		CLR.B = 0.0f;
		CLR.A = 1.0f;

		addword(center, place, word, size, CLR);
		word.clear();


		center.x = 0.0f;
		center.y = 0.0f;
		center.z = 0.0f;

		place.x = -90.0f;
		place.y = -90.0f + 25.83333f * 2.0f;
		place.z = 0.4f;

		word = { '*' };
		size = 24;
		CLR.R = 0.0f;
		CLR.G = 0.0f;
		CLR.B = 0.0f;
		CLR.A = 1.0f;

		addword(center, place, word, size, CLR);
		word.clear();


		center.x = 0.0f;
		center.y = 0.0f;
		center.z = 0.0f;

		place.x = -90.0f;
		place.y = -90.0f + 25.83333f * 3.0f;
		place.z = 0.4f;

		word = { '/' };
		size = 24;
		CLR.R = 0.0f;
		CLR.G = 0.0f;
		CLR.B = 0.0f;
		CLR.A = 1.0f;

		addword(center, place, word, size, CLR);
		word.clear();


		center.x = 0.0f;
		center.y = 0.0f;
		center.z = 0.0f;

		place.x = -90.0f;
		place.y = -90.0f + 25.83333f * 4.0f;
		place.z = 0.4f;

		word = { '^' };
		size = 24;
		CLR.R = 0.0f;
		CLR.G = 0.0f;
		CLR.B = 0.0f;
		CLR.A = 1.0f;

		addword(center, place, word, size, CLR);
		word.clear();

		center.x = 0.0f;
		center.y = 0.0f;
		center.z = 0.0f;

		place.x = -90.0f;
		place.y = -90.0f + 25.83333f * 5.0f;
		place.z = 0.4f;

		word = { '√' };
		size = 24;
		CLR.R = 0.0f;
		CLR.G = 0.0f;
		CLR.B = 0.0f;
		CLR.A = 1.0f;

		addword(center, place, word, size, CLR);
		word.clear();

		center.x = 0.0f;
		center.y = 0.0f;
		center.z = 0.0f;

		place.x = 19.0f - 73.0f;
		place.y = 15.5f - 100.0f;
		place.z = 0.4f;

		word = { '+' };
		size = 24;
		CLR.R = 0.0f;
		CLR.G = 0.0f;
		CLR.B = 0.0f;
		CLR.A = 1.0f;

		addword(center, place, word, size, CLR);
		word.clear();

		center.x = 0.0f;
		center.y = 0.0f;
		center.z = 0.0f;

		place.x = 19.0f - 73.0f;
		place.y = 12.5f - 100.0f;
		place.z = 0.4f;

		word = { '-' };
		size = 24;
		CLR.R = 0.0f;
		CLR.G = 0.0f;
		CLR.B = 0.0f;
		CLR.A = 1.0f;

		addword(center, place, word, size, CLR);
		word.clear();

		center.x = 0.0f;
		center.y = 0.0f;
		center.z = 0.0f;

		place.x = 18.0f - 73.0f + 2 * 39.3333f;
		place.y = 14.0f - 100.0f;
		place.z = 0.4f;

		word = { '.' };
		size = 24;
		CLR.R = 0.0f;
		CLR.G = 0.0f;
		CLR.B = 0.0f;
		CLR.A = 1.0f;

		addword(center, place, word, size, CLR);
		word.clear();

		center.x = 0.0f;
		center.y = 0.0f;
		center.z = 0.0f;

		place.x = 18.0f - 73.0f + 1.0f * 39.3333f;
		place.y = 14.0f - 100.0f;
		place.z = 0.4f;

		word = { '0' };
		size = 24;
		CLR.R = 0.0f;
		CLR.G = 0.0f;
		CLR.B = 0.0f;
		CLR.A = 1.0f;

		addword(center, place, word, size, CLR);
		word.clear();

		center.x = 0.0f;
		center.y = 0.0f;
		center.z = 0.0f;

		place.x = 18.0f - 73.0f;
		place.y = 14.0f - 100.0f + 1.0f * 31.0f;
		place.z = 0.4f;

		word = { '1' };
		size = 24;
		CLR.R = 0.0f;
		CLR.G = 0.0f;
		CLR.B = 0.0f;
		CLR.A = 1.0f;

		addword(center, place, word, size, CLR);
		word.clear();

		center.x = 0.0f;
		center.y = 0.0f;
		center.z = 0.0f;

		place.x = 18.0f - 73.0f + 1.0f * 39.3333f;
		place.y = 14.0f - 100.0f + 1.0f * 31.0f;
		place.z = 0.4f;

		word = { '2' };
		size = 24;
		CLR.R = 0.0f;
		CLR.G = 0.0f;
		CLR.B = 0.0f;
		CLR.A = 1.0f;

		addword(center, place, word, size, CLR);
		word.clear();
		center.x = 0.0f;
		center.y = 0.0f;
		center.z = 0.0f;

		place.x = 18.0f - 73.0f + 2.0f * 39.3333f;
		place.y = 14.0f - 100.0f + 1.0f * 31.0f;
		place.z = 0.4f;

		word = { '3' };
		size = 24;
		CLR.R = 0.0f;
		CLR.G = 0.0f;
		CLR.B = 0.0f;
		CLR.A = 1.0f;

		addword(center, place, word, size, CLR);
		word.clear();

		center.x = 0.0f;
		center.y = 0.0f;
		center.z = 0.0f;

		place.x = 18.0f - 73.0f + 0.0f * 39.3333f;
		place.y = 14.0f - 100.0f + 2.0f * 31.0f;
		place.z = 0.4f;

		word = { '4' };
		size = 24;
		CLR.R = 0.0f;
		CLR.G = 0.0f;
		CLR.B = 0.0f;
		CLR.A = 1.0f;

		addword(center, place, word, size, CLR);
		word.clear();

		center.x = 0.0f;
		center.y = 0.0f;
		center.z = 0.0f;

		place.x = 18.0f - 73.0f + 1.0f * 39.3333f;
		place.y = 14.0f - 100.0f + 2.0f * 31.0f;
		place.z = 0.4f;

		word = { '5' };
		size = 24;
		CLR.R = 0.0f;
		CLR.G = 0.0f;
		CLR.B = 0.0f;
		CLR.A = 1.0f;

		addword(center, place, word, size, CLR);
		word.clear();
		center.x = 0.0f;
		center.y = 0.0f;
		center.z = 0.0f;

		place.x = 18.0f - 73.0f + 2.0f * 39.3333f;
		place.y = 14.0f - 100.0f + 2.0f * 31.0f;
		place.z = 0.4f;

		word = { '6' };
		size = 24;
		CLR.R = 0.0f;
		CLR.G = 0.0f;
		CLR.B = 0.0f;
		CLR.A = 1.0f;

		addword(center, place, word, size, CLR);
		word.clear();

		center.x = 0.0f;
		center.y = 0.0f;
		center.z = 0.0f;

		place.x = 18.0f - 73.0f + 0.0f * 39.3333f;
		place.y = 14.0f - 100.0f + 3.0f * 31.0f;
		place.z = 0.4f;

		word = { '7' };
		size = 24;
		CLR.R = 0.0f;
		CLR.G = 0.0f;
		CLR.B = 0.0f;
		CLR.A = 1.0f;

		addword(center, place, word, size, CLR);
		word.clear();

		center.x = 0.0f;
		center.y = 0.0f;
		center.z = 0.0f;

		place.x = 18.0f - 73.0f + 1.0f * 39.3333f;
		place.y = 14.0f - 100.0f + 3.0f * 31.0f;
		place.z = 0.4f;

		word = { '8' };
		size = 24;
		CLR.R = 0.0f;
		CLR.G = 0.0f;
		CLR.B = 0.0f;
		CLR.A = 1.0f;

		addword(center, place, word, size, CLR);
		word.clear();
		center.x = 0.0f;
		center.y = 0.0f;
		center.z = 0.0f;

		place.x = 18.0f - 73.0f + 2.0f * 39.3333f;
		place.y = 14.0f - 100.0f + 3.0f * 31.0f;
		place.z = 0.4f;

		word = { '9' };
		size = 24;
		CLR.R = 0.0f;
		CLR.G = 0.0f;
		CLR.B = 0.0f;
		CLR.A = 1.0f;

		addword(center, place, word, size, CLR);
		word.clear();

		center.x = 0.0f;
		center.y = 0.0f;
		center.z = 0.0f;

		place.x = 18.0f - 73.0f + 0.0f * 39.3333f;
		place.y = 14.0f - 100.0f + 4.0f * 31.0f;
		place.z = 0.4f;

		word = { '=' };
		size = 24;
		CLR.R = 0.0f;
		CLR.G = 0.0f;
		CLR.B = 0.0f;
		CLR.A = 1.0f;

		addword(center, place, word, size, CLR);
		word.clear();

		center.x = 0.0f;
		center.y = 0.0f;
		center.z = 0.0f;

		place.x = 18.0f - 73.0f + 1.0f * 39.3333f;
		place.y = 14.0f - 100.0f + 4.0f * 31.0f;
		place.z = 0.4f;

		word = { 'C' };
		size = 24;
		CLR.R = 0.0f;
		CLR.G = 0.0f;
		CLR.B = 0.0f;
		CLR.A = 1.0f;

		addword(center, place, word, size, CLR);
		word.clear();
		center.x = 0.0f;
		center.y = 0.0f;
		center.z = 0.0f;

		place.x = 18.0f - 73.0f + 2.0f * 39.3333f;
		place.y = 14.0f - 100.0f + 4.0f * 31.0f;
		place.z = 0.4f;

		word = { '<', '-' };
		size = 24;
		CLR.R = 0.0f;
		CLR.G = 0.0f;
		CLR.B = 0.0f;
		CLR.A = 1.0f;

		addword(center, place, word, size, CLR);
		word.clear();

		center.x = 0.0f;
		center.y = 0.0f;
		center.z = 0.0f;

		place.x = -90.0f; //!!!!!!!!!!!!
		place.y = 70.0f;
		place.z = 3.0f;

		size = 24;
		CLR.R = 0.0f;
		CLR.G = 1.0f;
		CLR.B = 0.0f;
		CLR.A = 1.0f;
	
		word = { '0' };
		
		addword(center, place, word, size, CLR); // Результат
		word.clear();
		
		center.x = 0.0f;
		center.y = 0.0f;
		center.z = 0.0f;

		place.x = -95.0f; //!!!!!!!!!!!!
		place.y = 85.0f;
		place.z = 3.0f;

		size = 24;
		CLR.R = 1.0f;
		CLR.G = 1.0f;
		CLR.B = 0.0f;
		CLR.A = 1.0f;

		word = { ' ' };

		addword(center, place, word, size, CLR); // Строка ввода
		word.clear();

		center.x = 0.0f;
		center.y = 0.0f;
		center.z = 0.0f;

		place.x = -90.0f; //!!!!!!!!!!!!
		place.y = 70.0f;
		place.z = 3.0f;

		size = 0;
		CLR.R = 0.0f;
		CLR.G = 1.0f;
		CLR.B = 0.0f;
		CLR.A = 1.0f;

		word = { 'W', 'r', 'o', 'n', 'g', ' ', 'f', 'o', 'r', 'm', 'a', 't' };

		addword(center, place, word, size, CLR); // Сообщение об ошибке
		word.clear();

		center.x = 0.0f;
		center.y = 0.0f;
		center.z = 0.0f;

		place.x = -25.0f;
		place.y = 56.0f;
		place.z = 3.5f;

		size = 10;
		CLR.R = 0.0f;
		CLR.G = 1.0f;
		CLR.B = 0.0f;
		CLR.A = 1.0f;

		word = { 92, '/' };

		addword(center, place, word, size, CLR); //Стрелочки для истории
		word.clear();

		for (int i = 1; i <= 4; i++) {
			center.x = 0.0f;
			center.y = 0.0f;
			center.z = 0.0f;

			place.x = -93.0f;
			place.y = 155.0f -100.0f + 38.25f * i - 9.5625f;
			place.z = 1.5f;

			size = 24;
			CLR.R = 1.0f;
			CLR.G = 0.0f;
			CLR.B = 0.0f;
			CLR.A = 1.0f;

			word = { };

			cout << addword(center, place, word, size, CLR) << ' '; //Строки в истории
			word.clear();
		}

		for (int i = 1; i <= 4; i++) {
			center.x = 0.0f;
			center.y = 0.0f;
			center.z = 0.0f;

			place.x = 35.0f;
			place.y = 155.0f -100.0f + 38.25f * i - 28.6875f;
			place.z = 1.5f;

			size = 24;
			CLR.R = 1.0f;
			CLR.G = 0.0f;
			CLR.B = 0.0f;
			CLR.A = 1.0f;

			word = {  };

			addword(center, place, word, size, CLR); //Результаты в истории
			word.clear();
		}
		for (int i = 0; i < 3; i++) {
			center.x = 0.0f;
			center.y = 0.0f;
			center.z = 0.0f;

			place.x = 72.5f;
			place.y = 95.5f - i;
			place.z = 3.8f;

			size = 24;
			CLR.R = 0.0f;
			CLR.G = 0.0f;
			CLR.B = 0.0f;
			CLR.A = 1.0f;

			word = { '_' };

			addword(center, place, word, size, CLR); // Меню
			word.clear();
		}
		center.x = 0.0f;
		center.y = 0.0f;
		center.z = 0.0f;

		place.x = 56.75f;
		place.y = 23.0f * 8.0f - 112.5f;
		place.z = 3.5f;

		size = 24;
		CLR.R = 0.0f;
		CLR.G = 0.0f;
		CLR.B = 0.0f;
		CLR.A = 1.0f;

		word = { 's', 'i', 'n' };

		addword(center, place, word, size, CLR); 
		word.clear();

		center.x = 0.0f;
		center.y = 0.0f;
		center.z = 0.0f;

		place.x = 81.75f;
		place.y = 23.0f * 8.0f - 112.5f;
		place.z = 3.5f;

		size = 24;
		CLR.R = 0.0f;
		CLR.G = 0.0f;
		CLR.B = 0.0f;
		CLR.A = 1.0f;

		word = { 'c', 'o', 's' };

		addword(center, place, word, size, CLR); 
		word.clear();

		center.x = 0.0f;
		center.y = 0.0f;
		center.z = 0.0f;

		place.x = 59.25f;
		place.y = 23.0f * 7.0f - 112.5f;
		place.z = 3.5f;

		size = 24;
		CLR.R = 0.0f;
		CLR.G = 0.0f;
		CLR.B = 0.0f;
		CLR.A = 1.0f;

		word = { 't', 'g' };

		addword(center, place, word, size, CLR);
		word.clear();

		center.x = 0.0f;
		center.y = 0.0f;
		center.z = 0.0f;

		place.x = 81.75f;
		place.y = 23.0f * 7.0f - 112.5f;
		place.z = 3.5f;

		size = 24;
		CLR.R = 0.0f;
		CLR.G = 0.0f;
		CLR.B = 0.0f;
		CLR.A = 1.0f;

		word = { 'l', 'o', 'g' };

		addword(center, place, word, size, CLR);
		word.clear();

		center.x = 0.0f;
		center.y = 0.0f;
		center.z = 0.0f;

		place.x = 59.25f;
		place.y = 23.0f * 6.0f - 112.5f;
		place.z = 3.5f;

		size = 24;
		CLR.R = 0.0f;
		CLR.G = 0.0f;
		CLR.B = 0.0f;
		CLR.A = 1.0f;

		word = { 'l', 'n' };

		addword(center, place, word, size, CLR);
		word.clear();

		center.x = 0.0f;
		center.y = 0.0f;
		center.z = 0.0f;

		place.x = 86.75f;
		place.y = 23.0f * 6.0f - 112.5f;
		place.z = 3.5f;

		size = 24;
		CLR.R = 0.0f;
		CLR.G = 0.0f;
		CLR.B = 0.0f;
		CLR.A = 1.0f;

		word = { '|' };

		addword(center, place, word, size, CLR);
		word.clear();

		center.x = 0.0f;
		center.y = 0.0f;
		center.z = 0.0f;

		place.x = 61.75f;
		place.y = 23.0f * 5.0f - 112.5f;
		place.z = 3.5f;

		size = 24;
		CLR.R = 0.0f;
		CLR.G = 0.0f;
		CLR.B = 0.0f;
		CLR.A = 1.0f;

		word = { '(' };

		addword(center, place, word, size, CLR);
		word.clear();

		center.x = 0.0f;
		center.y = 0.0f;
		center.z = 0.0f;

		place.x = 86.75f;
		place.y = 23.0f * 5.0f - 112.5f;
		place.z = 3.5f;

		size = 24;
		CLR.R = 0.0f;
		CLR.G = 0.0f;
		CLR.B = 0.0f;
		CLR.A = 1.0f;

		word = { ')' };

		addword(center, place, word, size, CLR);
		word.clear();

		center.x = 0.0f;
		center.y = 0.0f;
		center.z = 0.0f;

		place.x = 61.75f;
		place.y = 23.0f * 4.0f - 112.5f;
		place.z = 3.5f;

		size = 24;
		CLR.R = 0.0f;
		CLR.G = 0.0f;
		CLR.B = 0.0f;
		CLR.A = 1.0f;

		word = { 'P' };

		addword(center, place, word, size, CLR);
		word.clear();

		center.x = 0.0f;
		center.y = 0.0f;
		center.z = 0.0f;

		place.x = 86.75f;
		place.y = 23.0f * 4.0f - 112.5f;
		place.z = 3.5f;

		size = 24;
		CLR.R = 0.0f;
		CLR.G = 0.0f;
		CLR.B = 0.0f;
		CLR.A = 1.0f;

		word = { 'e' };

		addword(center, place, word, size, CLR);
		word.clear();

		center.x = 0.0f;
		center.y = 0.0f;
		center.z = 0.0f;

		place.x = 61.75f;
		place.y = 23.0f * 3.0f - 112.5f;
		place.z = 3.5f;

		size = 24;
		CLR.R = 0.0f;
		CLR.G = 0.0f;
		CLR.B = 0.0f;
		CLR.A = 1.0f;

		word = { '!' };

		addword(center, place, word, size, CLR);
		word.clear();

		center.x = 0.0f;
		center.y = 0.0f;
		center.z = 0.0f;

		place.x = 86.75f;
		place.y = 23.0f * 3.0f - 112.5f;
		place.z = 3.5f;

		size = 24;
		CLR.R = 0.0f;
		CLR.G = 0.0f;
		CLR.B = 0.0f;
		CLR.A = 1.0f;

		word = {  };

		addword(center, place, word, size, CLR);
		word.clear();
		/*
		place.x = 0.0f;
		place.y = 0.0f;
		place.z = 0.4f;

		word = { 'H', 'e', 'l', 'l', 'o', 'w', '!' };
		size = 24;
		CLR.R = 1.0f;
		CLR.G = 1.0f;
		CLR.B = 1.0f;
		CLR.A = 1.0f;

		addword(center, place, word, size, CLR);
		word.clear();
		*/
		break;
		}
		case 2: {
			cout << "Scene 2" << endl;
			glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
			vector<coord> top;
			coord C;
			vector<color> colors;
			color COL;
			coord center;
			coord angle;

			C.x = 50;
			C.y = 87;
			C.z = 1.0f;
			top.push_back(C);

			C.x = 100;
			C.y = 87;
			C.z = 1.0f;
			top.push_back(C);

			C.x = 100;
			C.y = 100;
			C.z = 1.0f;
			top.push_back(C);

			C.x = 50;
			C.y = 100;
			C.z = 1.0f;
			top.push_back(C);

			COL.R = 1.0f;
			COL.G = 1.0f;
			COL.B = 1.0f;
			COL.A = 1.0f;
			colors.push_back(COL);
			colors.push_back(COL);
			colors.push_back(COL);
			colors.push_back(COL);

			center.x = 0.0f;
			center.y = 0.0f;
			center.z = 0.0f;

			angle.x = 0.0f;
			angle.y = 0.0f;
			angle.z = 0.0f;

			add(0, top, colors, center, 0, angle, 0, 0); // Невидимый квадрат меню
			top.clear();
			colors.clear();

			C.x = 50;
			C.y = 100;
			C.z = 1.0f;
			top.push_back(C);

			C.x = 46;
			C.y = 100;
			C.z = 1.0f;
			top.push_back(C);

			C.x = 46;
			C.y = -100;
			C.z = 1.0f;
			top.push_back(C);

			C.x = 50;
			C.y = -100;
			C.z = 1.0f;
			top.push_back(C);

			COL.R = 1.0f;
			COL.G = 0.0f;
			COL.B = 0.0f;
			COL.A = 1.0f;
			colors.push_back(COL);
			colors.push_back(COL);
			colors.push_back(COL);
			colors.push_back(COL);

			center.x = 0.0f;
			center.y = 0.0f;
			center.z = 0.0f;

			angle.x = 0.0f;
			angle.y = 0.0f;
			angle.z = 0.0f;

			add(1, top, colors, center, 0, angle, 0, 0); // Вертикальная линия справа
			top.clear();
			colors.clear();

			C.x = 46;
			C.y = -150;
			C.z = 0.1f;
			top.push_back(C);

			C.x = 46;
			C.y = 150;
			C.z = 0.1f;
			top.push_back(C);

			C.x = 150;
			C.y = 150;
			C.z = 0.1f;
			top.push_back(C);

			C.x = 150;
			C.y = -150;
			C.z = 0.1f;
			top.push_back(C);

			COL.R = 0.5f;
			COL.G = 0.5f;
			COL.B = 0.5f;
			COL.A = 1.0f;
			colors.push_back(COL);
			colors.push_back(COL);
			colors.push_back(COL);
			colors.push_back(COL);

			center.x = 0.0f;
			center.y = 0.0f;
			center.z = 0.0f;

			angle.x = 0.0f;
			angle.y = 0.0f;
			angle.z = 0.0f;

			add(1, top, colors, center, 0, angle, 0, 0); // Фон меню 0.5f, 0.5f, 0.5f, 1.0f
			top.clear();
			colors.clear();

			C.x = 50;
			C.y = -60;
			C.z = 1.0f;
			top.push_back(C);

			C.x = -100;
			C.y = -60;
			C.z = 1.0f;
			top.push_back(C);

			C.x = -100;
			C.y = -62;
			C.z = 1.0f;
			top.push_back(C);

			C.x = 50;
			C.y = -62;
			C.z = 1.0f;
			top.push_back(C);

			COL.R = 1.0f;
			COL.G = 0.0f;
			COL.B = 0.0f;
			COL.A = 1.0f;
			colors.push_back(COL);
			colors.push_back(COL);
			colors.push_back(COL);
			colors.push_back(COL);

			center.x = 0.0f;
			center.y = 0.0f;
			center.z = 0.0f;

			angle.x = 0.0f;
			angle.y = 0.0f;
			angle.z = 0.0f;

			add(1, top, colors, center, 0, angle, 0, 0); //Горизонтальная линия, отделяющая основное поле
			top.clear();
			colors.clear();

			C.x = 50;
			C.y = -60;
			C.z = 0.1f;
			top.push_back(C);

			C.x = -100;
			C.y = -60;
			C.z = 0.1f;
			top.push_back(C);

			C.x = -100;
			C.y = -150;
			C.z = 0.1f;
			top.push_back(C);

			C.x = 50;
			C.y = -150;
			C.z = 0.1f;
			top.push_back(C);

			COL.R = 0.5f;
			COL.G = 0.5f;
			COL.B = 0.5f;
			COL.A = 0.5f;
			colors.push_back(COL);
			colors.push_back(COL);
			colors.push_back(COL);
			colors.push_back(COL);

			center.x = 0.0f;
			center.y = 0.0f;
			center.z = 0.0f;

			angle.x = 0.0f;
			angle.y = 0.0f;
			angle.z = 0.0f;

			add(1, top, colors, center, 0, angle, 0, 0); //Фон управления
			top.clear();
			colors.clear();

			C.x = -26.7;
			C.y = -60;
			C.z = 1.1f;
			top.push_back(C);

			C.x = -26.7;
			C.y = 100;
			C.z = 1.1f;
			top.push_back(C);

			C.x = -27.3;
			C.y = 100;
			C.z = 1.1f;
			top.push_back(C);

			C.x = -27.3;
			C.y = -60;
			C.z = 1.1f;
			top.push_back(C);

			COL.R = 0.0f;
			COL.G = 0.0f;
			COL.B = 0.0f;
			COL.A = 1.0f;
			colors.push_back(COL);
			colors.push_back(COL);
			colors.push_back(COL);
			colors.push_back(COL);

			center.x = 0.0f;
			center.y = 0.0f;
			center.z = 0.0f;

			angle.x = 0.0f;
			angle.y = 0.0f;
			angle.z = 0.0f;

			add(1, top, colors, center, 0, angle, 0, 0); // Ось ОХ
			top.clear();
			colors.clear();

			C.x = -100;
			C.y = 20.3f;
			C.z = 1.1f;
			top.push_back(C);

			C.x = 46;
			C.y = 20.3f;
			C.z = 1.1f;
			top.push_back(C);

			C.x = 46;
			C.y = 19.7f;
			C.z = 1.1f;
			top.push_back(C);

			C.x = -100;
			C.y = 19.7f;
			C.z = 1.1f;
			top.push_back(C);

			COL.R = 0.0f;
			COL.G = 0.0f;
			COL.B = 0.0f;
			COL.A = 1.0f;
			colors.push_back(COL);
			colors.push_back(COL);
			colors.push_back(COL);
			colors.push_back(COL);

			center.x = 0.0f;
			center.y = 0.0f;
			center.z = 0.0f;

			angle.x = 0.0f;
			angle.y = 0.0f;
			angle.z = 0.0f;

			add(1, top, colors, center, 0, angle, 0, 0); // Ось ОУ
			top.clear();
			colors.clear();

			C.x = -100;
			C.y = -100;
			C.z = 1.0f;
			top.push_back(C);

			C.x = 46;
			C.y = -100;
			C.z = 1.0f;
			top.push_back(C);

			C.x = 46;
			C.y = -91;
			C.z = 1.0f;
			top.push_back(C);

			C.x = -100;
			C.y = -91;
			C.z = 1.0f;
			top.push_back(C);

			COL.R = 0.7f;
			COL.G = 0.7f;
			COL.B = 0.7f;
			COL.A = 1.0f;
			colors.push_back(COL);
			colors.push_back(COL);
			colors.push_back(COL);
			colors.push_back(COL);

			center.x = 0.0f;
			center.y = 0.0f;
			center.z = 0.0f;

			angle.x = 0.0f;
			angle.y = 0.0f;
			angle.z = 0.0f;

			add(1, top, colors, center, 0, angle, 0, 0); // Место для уравнения
			top.clear();
			colors.clear();

			C.x = 46;
			C.y = -76;
			C.z = 1.0f;
			top.push_back(C);

			C.x = -100;
			C.y = -76;
			C.z = 1.0f;
			top.push_back(C);

			C.x = -100;
			C.y = -77;
			C.z = 1.0f;
			top.push_back(C);

			C.x = 46;
			C.y = -77;
			C.z = 1.0f;
			top.push_back(C);

			COL.R = 1.0f;
			COL.G = 0.0f;
			COL.B = 0.0f;
			COL.A = 1.0f;
			colors.push_back(COL);
			colors.push_back(COL);
			colors.push_back(COL);
			colors.push_back(COL);

			center.x = 0.0f;
			center.y = 0.0f;
			center.z = 0.0f;

			angle.x = 0.0f;
			angle.y = 0.0f;
			angle.z = 0.0f;

			add(1, top, colors, center, 0, angle, 0, 0); //Горизонтальная линия, кнопки управления
			top.clear();
			colors.clear();

			for (int i = 1; i <= 3; i++) {
				C.x = -100 +48.7 * i;
				C.y = -60;
				C.z = 1.0f;
				top.push_back(C);

				C.x = -99 + 48.7 * i;
				C.y = -60;
				C.z = 1.0f;
				top.push_back(C);

				C.x = -99 + 48.7 * i;
				C.y = -91;
				C.z = 1.0f;
				top.push_back(C);

				C.x = -100 + 48.7 * i;
				C.y = -91;
				C.z = 1.0f;
				top.push_back(C);

				COL.R = 1.0f;
				COL.G = 0.0f;
				COL.B = 0.0f;
				COL.A = 1.0f;
				colors.push_back(COL);
				colors.push_back(COL);
				colors.push_back(COL);
				colors.push_back(COL);

				center.x = 0.0f;
				center.y = 0.0f;
				center.z = 0.0f;

				angle.x = 0.0f;
				angle.y = 0.0f;
				angle.z = 0.0f;

				add(1, top, colors, center, 0, angle, 0, 0); //Вертикальные линии, кнопки управления
				top.clear();
				colors.clear();
			}
			for (int i = 0; i < 3; i++) {
				for (int t = 0; t < 2; t++) {
					C.x = -100 + 49.7 * i - i;
					C.y = -62 - 15 * t;
					C.z = 0.1f;
					top.push_back(C);

					C.x = -100 + 49.7 * (i + 1) - i;
					C.y = -62 - 15 * t;
					C.z = 0.1f;
					top.push_back(C);

					C.x = -100 + 49.7 * (i + 1) - i;
					C.y = -62 - 15 * (t + 1);
					C.z = 0.1f;
					top.push_back(C);

					C.x = -100 + 49.7 * i - i;
					C.y = -62 - 15 * (t + 1);
					C.z = 0.1f;
					top.push_back(C);

					COL.R = 1.0f;
					COL.G = 1.0f;
					COL.B = 1.0f;
					COL.A = 1.0f;
					colors.push_back(COL);
					colors.push_back(COL);
					colors.push_back(COL);
					colors.push_back(COL);

					center.x = 0.0f;
					center.y = 0.0f;
					center.z = 0.0f;

					angle.x = 0.0f;
					angle.y = 0.0f;
					angle.z = 0.0f;
					
					add(0, top, colors, center, 0, angle, 0, 0); //Кнопки управления
					top.clear();
					colors.clear();
				}
			}//7+ 20 7-27 yx
			for (int i = -10; i <= 10; i++)
			{
				
				C.x = -28.2;
				C.y = i*7+20;
				C.z = 0.1f;
				top.push_back(C);

				C.x = -25.8;
				C.y = i * 7 + 20;
				C.z = 0.1f;
				top.push_back(C);

				COL.R = 0.0f;
				COL.G = 0.0f;
				COL.B = 0.0f;
				COL.A = 1.0f;
				colors.push_back(COL);
				colors.push_back(COL);
				colors.push_back(COL);
				colors.push_back(COL);

				center.x = 0.0f;
				center.y = 0.0f;
				center.z = 0.0f;

				angle.x = 0.0f;
				angle.y = 0.0f;
				angle.z = 0.0f;

				add(4, top, colors, center, 0, angle, 0, 0.8); //Цена деления y
				top.clear();
				colors.clear();

				C.x = 7*i-27;
				C.y = 21.2;
				C.z = 0.1f;
				top.push_back(C);

				C.x = 7*i-27;
				C.y = 18.8;
				C.z = 0.1f;
				top.push_back(C);

				COL.R = 0.0f;
				COL.G = 0.0f;
				COL.B = 0.0f;
				COL.A = 1.0f;
				colors.push_back(COL);
				colors.push_back(COL);
				colors.push_back(COL);
				colors.push_back(COL);

				center.x = 0.0f;
				center.y = 0.0f;
				center.z = 0.0f;

				angle.x = 0.0f;
				angle.y = 0.0f;
				angle.z = 0.0f;

				add(4, top, colors, center, 0, angle, 0, 0.8); //Цена деления y
				top.clear();
				colors.clear();
			}
			coord place; //Добавляю символы
			vector<char> word;
			int size; //10 или 24
			color CLR;
			for (int i = 0; i < 3; i++) {
				center.x = 0.0f;
				center.y = 0.0f;
				center.z = 0.0f;

				place.x = 72.5f;
				place.y = 95.5f - i;
				place.z = 3.8f;

				size = 24;
				CLR.R = 0.0f;
				CLR.G = 0.0f;
				CLR.B = 0.0f;
				CLR.A = 1.0f;

				word = { '_' };

				addword(center, place, word, size, CLR); // Меню
				word.clear();
			}
			center.x = 0.0f;
			center.y = 0.0f;
			center.z = 0.0f;

			place.x = -99.0f;
			place.y = -99;
			place.z = 3.5f;

			size = 24;
			CLR.R = 0.0f;
			CLR.G = 0.0f;
			CLR.B = 0.0f;
			CLR.A = 1.0f;

			word = {'Y', '='};

			addword(center, place, word, size, CLR); //№3
			word.clear();

			for (int i = -9; i <= 9; i++) {
				if (i != 0) {
					center.x = 0.0f;
					center.y = 0.0f;
					center.z = 0.0f;

					place.x = -27 + i * 7;
					place.y = 15;
					place.z = 3.5f;

					size = 10;
					CLR.R = 0.2f;
					CLR.G = 0.2f;
					CLR.B = 0.2f;
					CLR.A = 1.0f;

					if (i >= 0) {
						word = { (char)(abs(i) + 48)};
					}
					else {
						word = { '-', (char)(abs(i) + 48)};
					}


					addword(center, place, word, size, CLR);
					word.clear();
				}
			}
			center.x = 0.0f;
			center.y = 0.0f;
			center.z = 0.0f;

			place.x = -27 - 70;
			place.y = 15;
			place.z = 3.5f;

			size = 10;
			CLR.R = 0.2f;
			CLR.G = 0.2f;
			CLR.B = 0.2f;
			CLR.A = 1.0f;
			word = { '-', '1', '0' };
			addword(center, place, word, size, CLR);
			word.clear();

			center.x = 0.0f;
			center.y = 0.0f;
			center.z = 0.0f;

			place.x = -27 + 70;
			place.y = 15;
			place.z = 3.5f;

			size = 10;
			CLR.R = 0.2f;
			CLR.G = 0.2f;
			CLR.B = 0.2f;
			CLR.A = 1.0f;
			word = {'1', '0' };
			addword(center, place, word, size, CLR);
			word.clear();

			for (int i = -9; i <= 9; i++) {
				if (i != 0) {
					center.x = 0.0f;
					center.y = 0.0f;
					center.z = 0.0f;

					place.x = -25;
					place.y = 20 + 7 * i;
					place.z = 3.5f;

					size = 10;
					CLR.R = 0.2f;
					CLR.G = 0.2f;
					CLR.B = 0.2f;
					CLR.A = 1.0f;

					if (i >= 0) {
						word = { (char)(abs(i) + 48) };
					}
					else {
						word = { '-', (char)(abs(i) + 48) };
					}


					addword(center, place, word, size, CLR);
					word.clear();
				}
			}
			center.x = 0.0f;
			center.y = 0.0f;
			center.z = 0.0f;

			place.x = -25;
			place.y = -50;
			place.z = 3.5f;

			size = 10;
			CLR.R = 0.2f;
			CLR.G = 0.2f;
			CLR.B = 0.2f;
			CLR.A = 1.0f;
			word = { '-', '1', '0' };
			addword(center, place, word, size, CLR);
			word.clear();

			center.x = 0.0f;
			center.y = 0.0f;
			center.z = 0.0f;

			place.x = -25;
			place.y = 90;
			place.z = 3.5f;

			size = 10;
			CLR.R = 0.2f;
			CLR.G = 0.2f;
			CLR.B = 0.2f;
			CLR.A = 1.0f;
			word = { '1', '0' };
			addword(center, place, word, size, CLR);
			word.clear();

			center.x = 0.0f;
			center.y = 0.0f;
			center.z = 0.0f;

			place.x = 10;
			place.y = -72;
			place.z = 3.5f;

			size = 24;
			CLR.R = 0.0f;
			CLR.G = 0.0f;
			CLR.B = 0.0f;
			CLR.A = 1.0f;
			word = { 'C', 'l', 'e', 'a', 'r' };
			addword(center, place, word, size, CLR);
			word.clear();
		}
	}

	return true;
}

bool check() {
	if (THEstring.size() == 0) {
		return 0;
	}
	if (THEstring.size() == 1 && (THEstring[0] < 48 || THEstring[0] > 57) && THEstring[0] != 'P' && THEstring[0] != 'e' && THEstring[0] != 'X') {
		return 0;
	}
	char prev = ' ';
	int left, right, module;
	bool sign = 0, l_scb=0;
	left = 0;
	right = 0;
	module = 0;
	for (int i = 0; i < THEstring.size(); i++) {
		if (THEstring[i] == '|') {
			module++;
		}
		if (THEstring[i] == '.') {
			if (i == 0 || i == THEstring.size() - 1) {
				return 0;
			}
			if (THEstring[i - 1] < 48 || THEstring[i - 1] > 57 || THEstring[i + 1] < 48 || THEstring[i + 1] > 57) {
				return 0;
			}
		}
		if (THEstring[i] == '(') {
			left++;
			sign = 0;
			
		}
		if (THEstring[i] == ')') {
			if (left == 0) {
				return 0;
			}
			if (prev == '(') return 0;
			right++;
			sign = 0;
		}
		if (THEstring[i] == '+' || THEstring[i] == '-' || THEstring[i] == '*' || THEstring[i] == '/' || THEstring[i] == '^') {
			if (sign == 1) {
				return 0;
			}
			else {
				sign = 1;
			}
		}
		if (THEstring[i] >= 48 && THEstring[i] <= 57 || THEstring[i] == 'e' || THEstring[i] == 'P' || THEstring[i] == 'X' || THEstring[i] == '(' || THEstring[i] == ')') {
			sign = 0;
		}
		prev = THEstring[i];
	}
	if (module % 2 > 0) {
		return 0;
	}
	if (left != right) {
		return 0;
	}
	if ((THEstring[THEstring.size() - 1] < 48 || THEstring[THEstring.size() - 1] > 57) && THEstring[THEstring.size() - 1] != 'e' && THEstring[THEstring.size() - 1] != 'P' && THEstring[THEstring.size() - 1] != ')' && THEstring[THEstring.size() - 1] != '|' && THEstring[THEstring.size() - 1] != '!' && THEstring[THEstring.size() - 1] != 'X') {
		return 0;
	}
	return 1;
}

vector<char> swapLR(vector<char> A) {
	cout << "swapping" << endl;
	for (int i = 0; i < A.size() / 2; i++) {
		swap(A[i], A[A.size() - i - 1]);
	}
	return A;
}

vector<char> LONGcalculate(vector<char> A, vector<char> B, char c) {
	bool A_minus = 0;
	bool B_minus = 0;
	if (A[0] == '-') {
		A_minus = 1;
		vector<char> A_time;
		for (int i = 1; i < A.size(); i++) {
			A_time.push_back(A[i]);
		}
		A = A_time;
	}
	if (B[0] == '-') {
		A_minus = 1;
		vector<char> B_time;
		for (int i = 1; i < B.size(); i++) {
			B_time.push_back(B[i]);
		}
		B = B_time;
	}

	A = swapLR(A);
	B = swapLR(B);
	bool size;
	if (A.size() > B.size()) {
		size = 0;
	}
	else {
		size = 1;
	}
	vector<char> TimeRES;
	switch (c)
	{
	default:
		break;
	case '+': {
		if (!A_minus && !B_minus) {
		int dop = 0;
		if (size) {
			for (int i = 0; i < B.size(); i++) {
				int a, b;
				if (i < A.size()) {
					a = A[i] - 48;
				}
				else {
					a = 0;
				}
				b = B[i] - 48;
				b += dop;
				dop = (a + b) / 10;
				TimeRES.push_back((a + b) % 10 + 48);
			}
		}
		else {
			for (int i = 0; i < A.size(); i++) {
				int a, b;
				if (i < B.size()) {
					b = B[i] - 48;
				}
				else {
					b = 0;
				}
				a = A[i] - 48;
				b += dop;
				dop = (a + b) / 10;
				TimeRES.push_back((a + b) % 10 + 48);
			}
		}
	}
	}
	}
	TimeRES = swapLR(TimeRES);
	cout << "time result is " << endl;
	for (int i = 0; i < TimeRES.size(); i++) {
		cout << TimeRES[i];
	}
	cout << endl;
	return TimeRES;
}

bool Qcount(vector<char> A) {
	cout << "Start is" << endl;
	for (int i = 0; i < A.size(); i++) {
		cout << A[i];
	}
	cout << endl;
	if (!check) {
		return 0;
	}
	for (int i = 0; i < A.size(); i++) {
		if (A[i] == '(') {
			int t = i + 1;
			int skob = 0;
			vector<char> B;
			for (t; skob == 0 && A[t] != ')'; t++) {
				B.push_back(A[i]);
				if (A[t] == '(') {
					skob++;
				}
				if (A[t] == ')') {
					skob--;
				}
			}
			Qcount(B);
			B.clear();
			for (int j = 0; j <= i; j++) {
				B.push_back(A[j]);
			}
			for (int j = 0; j < QResult.size(); j++) {
				B.push_back(QResult[j]);
			}
			for (t; t < A.size(); t++) {
				B.push_back(A[t]);
			}
			A = B;
		}
	}
	
	vector<vector<char>> main;
	vector<char> Numb;
	for (int i = 0; i < A.size(); i++) {
		if (A[i] >= 48 && A[i] <= 57) {
			cout << "found numb" << endl;
			Numb.push_back(A[i]);
		}
		else {
			cout << "Numb size is " << Numb.size() << endl << "operation is " << A[i] << endl;
			if (Numb.size() != 0) {
				main.push_back(Numb);
				Numb.clear();
			}
			Numb.push_back(A[i]);
			main.push_back(Numb);
			Numb.clear();
		}
	}
	main.push_back(Numb);
	Numb.clear();

	cout << "MAIN: " << endl;
	for (int i = 0; i < main.size(); i++) {
		for (int t = 0; t < main[i].size(); t++) {
			cout << main[i][t];
		}
		cout << ' ';
	}
	cout << endl;
	for (int i = 0; i < main.size(); i++) {
		if (main[i][0] == '^') {
			int R = i + 1;
			int L = i - 1;
			bool TRY = false;
			while (!TRY) {
				TRY = true;
				if (main[L][0] == ' ') {
					L--;
					TRY = false;
				}
				if (main[R][0] == ' ') {
					R++;
					TRY = false;
				}
			}
			main[i] = LONGcalculate(main[L], main[R], '^');
			main[R].clear();
			main[R].push_back(' ');
			main[L].clear();
			main[L].push_back(' ');

		}
	}
	for (int i = 0; i < main.size(); i++) {
		if (main[i][0] == '*' || main[i][0] == '/') {
			int R = i + 1;
			int L = i - 1;
			bool TRY = false;
			while (!TRY) {
				TRY = true;
				if (main[L][0] == ' ') {
					L--;
					TRY = false;
				}
				if (main[R][0] == ' ') {
					R++;
					TRY = false;
				}
			}
			main[i] = LONGcalculate(main[L], main[R], main[i][0]);
			main[R].clear();
			main[R].push_back(' ');
			main[L].clear();
			main[L].push_back(' ');

		}
	}

	for (int i = 0; i < main.size(); i++) {
		if (main[i][0] == '+' || main[i][0] == '-') {
			int R = i + 1;
			int L = i - 1;
			bool TRY = false;
			while (!TRY) {
				TRY = true;
				if (main[L][0] == ' ') {
					cout << "I did it!" << endl;
					L--;
					TRY = false;
				}
				if (main[R][0] == ' ') {
					R++;
					TRY = false;
				}
			}
			main[i] = LONGcalculate(main[L], main[R], main[i][0]);
			main[R].clear();
			main[R].push_back(' ');
			main[L].clear();
			main[L].push_back(' ');

		}
	}

	for (int i = 0; i < main.size(); i++) {
		if (main[i][0] != ' ') {
			QResult = main[i];
		}
	}
	cout << "Result is " << endl;
	for (int i = 0; i < QResult.size(); i++) {
		cout << QResult[i];
	}
	cout << endl;
	return 1;
}

double createNUMB(vector<char> A) {
	A.push_back('.');
	double Bnumb = 0.0;
	double Snumb = 0.0;
	vector<char> Big;
	vector<char> Small;
	int T = 0;
	for (T; A[T] != '.'; T++) {
			Big.push_back(A[T]);
	}
	T++;
	for (T; T < A.size() - 1; T++) {
		Small.push_back(A[T]);
	}
	for (int i = 0; i < Big.size(); i++) {
		Bnumb += (Big[i] - 48) * pow(10, Big.size() - i - 1);
	}
	for (int i = Small.size() - 1; i >= 0; i--) {
		Snumb += 1.0 * (Small[i] - 48) * pow(0.1, i + 1);
	}
	//cout << "NEWNUMB = " << Bnumb << Snumb << endl;
	return Bnumb + Snumb;
}

void reroll(vector<char> & A)
{

	char prev = A[0];
	for (int i = 1; i < A.size(); i++)
	{
		vector<char> B;
		if (prev >= 48 && prev <= 57 && A[i] == '(')
		{
			for (int t = 0; t < i; t++) B.push_back(A[t]);
			B.push_back('*');
			for (int t = i; t < A.size(); t++) B.push_back(A[t]);
			A = B;
		}
		if (A[i] >= 48 && A[i] <= 57 && prev == ')')
		{
			for (int t = 0; t < i; t++) B.push_back(A[t]);
			B.push_back('*');
			for (int t = i; t < A.size(); t++) B.push_back(A[t]);
			A = B;
		}
		prev = A[i];
	}
}

double calculate(double A, double B, char C) {
	switch (C) {
	default:
		break;
	case '+': {
		cout << "doing +" << endl;
		return A + B;

		break;
	}
	case '-': {
		cout << "doing -" << endl;
		return A - B;
		break;
	}
	case '*': {
		cout << "doing *" << endl;
		return A * B;
		break;
	}
	case '/': {
		cout << "doing /" << endl;
		return A / B;
		break;
	}
	case '^': {
		cout << "doing ^" << endl;
		return pow(A, B);
		break;
	}

	}
}

bool count(vector<char> A) {
	/*cout << "MAIN: ";
	for (int i = 0; i < A.size(); i++) {
		cout << A[i] << ' ';
	}
	cout << endl;*/
	reroll(A);
	/*cout << "rer: ";
	for (int i = 0; i < A.size(); i++) {
		cout << A[i] << ' ';
	}
	cout << endl;*/
	if (!check()) {
		cout << "ERR: wrong format" << endl;
		return 0;
	}
	/*for (int i = 0; i < A.size(); i++) {
		if (A[i] == 'P') {
			vector<char> B;
			for (int t = 0; t < i; t++) {
				B.push_back(A[t]);
			}

			
		}
	}*/
	for (int i = 0; i < A.size(); i++) {
		if (A[i] == 's' && A[i + 1] == 'q') {
			vector<char> B;
			vector<char> C;
			for (int t = 0; t != i; t++) {
				B.push_back(A[t]);
			}
			int j = i;
			int skob = 0;
			while (A[j] != '(') {
				j++;
			}
			j++;
			for (j; A[j] != ')' || skob > 0; j++) {
				if (A[j] == '(') {
					skob++;
				}
				if (A[j] == ')') {
					skob--;
				}
				C.push_back(A[j]);
			}
			j++;
			count(C);
			if (Result >= 0) {
				Result = calculate(Result, 0.5, '^');
			}
			else {
				return 0;
			}
			ostringstream ost;
			ost << Result;
			string s_d = ost.str();
			for (int j = 0; j < s_d.size(); j++) {
				B.push_back(s_d[j]);
			}
			for (j; j < A.size(); j++) {
				B.push_back(A[j]);
			}
			A = B;
		}
	}
	
	for (int i = 0; i < A.size(); i++) {
		if (A[i] == 's' && A[i + 1] == 'i') {
			vector<char> B;
			vector<char> C;
			for (int t = 0; t != i; t++) {
				B.push_back(A[t]);
			}
			int j = i;
			int skob = 0;
			while (A[j] != '(') {
				j++;
			}
			j++;
			for (j; A[j] != ')' || skob > 0; j++) {
				if (A[j] == '(') {
					skob++;
				}
				if (A[j] == ')') {
					skob--;
				}
				C.push_back(A[j]);
			}
			j++;
			count(C);
			Result = sin(Result*PI/180);
			ostringstream ost;
			ost << Result;
			string s_d = ost.str();
			for (int j = 0; j < s_d.size(); j++) {
				B.push_back(s_d[j]);
			}
			for (j; j < A.size(); j++) {
				B.push_back(A[j]);
			}
			A = B;
		}
	}
	for (int i = 0; i < A.size(); i++) {
		if (A[i] == 'c' && A[i + 1] == 'o') {
			vector<char> B;
			vector<char> C;
			cout << "IM here!" << endl << endl;
			for (int t = 0; t != i; t++) {
				//cout << A[t] << ' ';
				B.push_back(A[t]);
			}
			//cout << endl << endl;
			int j = i;
			while (A[j] != '(') {
				j++;
			}
			j++;
			int skob = 0;
			for (j; A[j] != ')' || skob > 0; j++) {
				if (A[j] == '(')
					skob++;

				if (A[j] == ')') {
					skob--;
				}
				C.push_back(A[j]);
			}
			j++;
			count(C);
			Result = cos(Result*PI / 180);
			ostringstream ost;
			ost << Result;
			string s_d = ost.str();
			for (int j = 0; j < s_d.size(); j++) {
				B.push_back(s_d[j]);
			}
			for (j; j < A.size(); j++) {
				B.push_back(A[j]);
			}
			A = B;
		}
	}
	for (int i = 0; i < A.size(); i++) {
		if (A[i] == 't' && A[i + 1] == 'g') {
			vector<char> B;
			vector<char> C;
			for (int t = 0; t != i; t++) {
				B.push_back(A[t]);
			}
			int j = i;
			while (A[j] != '(') {
				j++;
			}
			j++;
			int skob = 0;
			for (j; A[j] != ')' || skob > 0; j++) {
				if (A[j] == '(')
					skob++;

				if (A[j] == ')') {
					skob--;
				}
				C.push_back(A[j]);
			}
			j++;
			count(C);
			Result = sin(Result*PI / 180)/ cos(Result*PI / 180);
			ostringstream ost;
			ost << Result;
			string s_d = ost.str();
			for (int j = 0; j < s_d.size(); j++) {
				B.push_back(s_d[j]);
			}
			for (j; j < A.size(); j++) {
				B.push_back(A[j]);
			}
			A = B;
		}
	}
	for (int i = 0; i < A.size(); i++) {
		if (A[i] == 'l' && A[i + 1] == 'n') {
			vector<char> B;
			vector<char> C;
			for (int t = 0; t != i; t++) {
				B.push_back(A[t]);
			}
			int j = i;
			int skob = 0;
			while (A[j] != '(') {
				j++;
			}
			j++;
			for (j; A[j] != ')' || skob > 0; j++) {
				if (A[j] == '(') {
					skob++;
				}
				if (A[j] == ')') {
					skob--;
				}
				C.push_back(A[j]);
			}
			j++;
			count(C);
			if (Result > 0) {
				Result = log(Result);
			}
			else {
				return 0;
			}
			ostringstream ost;
			ost << Result;
			string s_d = ost.str();
			for (int j = 0; j < s_d.size(); j++) {
				B.push_back(s_d[j]);
			}
			for (j; j < A.size(); j++) {
				B.push_back(A[j]);
			}
			A = B;
		}
	}
	for (int i = 0; i < A.size(); i++) {
		if (A[i] == '-' && A[i + 1] >= 48 && A[i + 1] <= 57) {
			A[i] = 'm';
			//cout << "Detected ' - '" << endl;
		}
	}
	//cout << "MAIN2: ";
	//for (int i = 0; i < A.size(); i++) {
		//cout << A[i] << ' ';
	//}
	//cout << endl;
	vector<int> links;
	
	for (int i = 0; i < A.size(); i++) {
		if (A[i] == ')') {
			int left, right, LEFT;
			right = i;
			left = right;
			while (A[left] != '(') {
				left--;
			}
			//cout << "LEFT: " << left << endl;
			//cout << "RIGHT: " << right << endl;
			LEFT = left;
			left++;
			vector<char> Astring;
			for (left; left < right; left++) {
				Astring.push_back(A[left]);
			}

			A[LEFT] = ' ';
			A[right] = ' ';
			count(Astring);
			left = LEFT;
			/*for (left; left <= right; left++) {
				A[left] = ' ';
			}*/
			vector<char> B;
			int k = 0;
			//cout << "Creating B  ";
			while (k < left) {
				B.push_back(A[k]);
				//cout << A[k] << ' ';
				k++;
			}
			k = right + 1;
			ostringstream ost;
			ost << Result;
			string s_d = ost.str();
			for (int j = 0; j < s_d.size(); j++) {
				//cout << s_d[j] << ' ';
				B.push_back(s_d[j]);
			}
			while (k < A.size()) {
				//cout << A[k] << ' ';
				B.push_back(A[k]);
				k++;
			}
			count(B);
			return 1;
		}
	}
	for (int i = 1; i < A.size(); i++) {
		if (A[i] == '!' && ((A[i - 1] >= 48 && A[i - 1] <= 57) || A[i - 1] == 'P' || A[i - 1] == 'e')) {
			vector<char> B;
			int j = i - 1;
			while (A[j] >= 48 && A[j] <= 57) {
				j--;
				if (j == -1) {
					break;
				}
			}
			j++;
			int Left = j;
			while (j < i) {
				B.push_back(A[j]);
				j++;
			}
			double D;
			if (A[i - 1] == 'P' || A[i - 1] == 'e') {
				if (A[i - 1] == 'P') {
					D = PI;
				}

				else {
					D = e;
				}
			}
			else {
				D = createNUMB(B);
			}
			
			B.clear();
			D = fact(D);
			//cout << "Factorial is " << D << endl;

			vector<char> word;
			ostringstream ost;
			ost << D;
			string s_d = ost.str();
			word.resize(s_d.size());
			for (int i = 0; i < s_d.size(); i++) {
				word[i] = s_d[i];
			}
			for (int t = 0; t < Left - 1; t++) {
				//cout << 't' << ' ' << t << endl << 'L' << ' ' << Left << endl;
				B.push_back(A[t]);
			}
			for (int t = 0; t < word.size(); t++) {
				B.push_back(word[t]);
			}
			for (int t = i + 1; t < A.size(); t++) {
				//cout << 't' << ' ' << t << endl << 'S' << ' ' << A.size() << endl;
				B.push_back(A[t]);
			}
			A = B;
		}
	}
	//cout << "MAIN3: ";
	//for (int i = 0; i < A.size(); i++) {
		//cout << A[i] << ' ';
	//}
	//cout << endl;
	for (int i = 0; i < A.size(); i++) {
		if (A[i] == '|') {
			int left, right, LEFT;
			right = i;
			left = i;
			right = i + 1;
			while (A[right] != '|') {
				right++;
			}
			//cout << "LEFT: " << left << endl;
			//cout << "RIGHT: " << right << endl;
			LEFT = left;
			left++;
			vector<char> Astring;
			for (left; left < right; left++) {
				Astring.push_back(A[left]);
			}

			A[LEFT] = ' ';
			A[right] = ' ';
			count(Astring);
			if (Result < 0) {
				Result = -Result;
			}
			left = LEFT;
			/*for (left; left <= right; left++) {
			A[left] = ' ';
			}*/
			vector<char> B;
			int k = 0;
			//cout << "Creating B  ";
			while (k < left) {
				B.push_back(A[k]);
				//cout << A[k] << ' ';
				k++;
			}
			k = right + 1;
			ostringstream ost;
			ost << Result;
			string s_d = ost.str();
			for (int j = 0; j < s_d.size(); j++) {
				//cout << s_d[j] << ' ';
				B.push_back(s_d[j]);
			}
			while (k < A.size()) {
				//cout << A[k] << ' ';
				B.push_back(A[k]);
				k++;
			}
			count(B);
			return 1;
		}
	}
	vector<char> operations;
	vector<double> operands;
	bool minus = false;
	for (int i = 0; i < A.size(); i += 0) {
		vector<char> number;
		//cout << i << endl;
		if (A[i] == 'P') {
			operands.push_back(PI);
		}
		if (A[i] == 'e') {
			operands.push_back(e);
		}
		if ((A[i] >= 48 && A[i] <= 57) || A[i] == '.') {
			int t = i;
			while (t < A.size() && ((A[t] >= 48 && A[t] <= 57) || A[t] == '.')) {
				//cout << "letter " << A[t] << endl;
				number.push_back(A[t]);
				//cout << "doing t++" << endl;
				t++;
				if (t == A.size()) {
					t--;
					break;
				}
			}
			double D;
			if (!minus) {
				D = createNUMB(number);
			}
			else {
				D = 0  - createNUMB(number);
			}
			//cout << "D is: " << D << endl << "minus is: " << minus << endl;
			minus = false;
			number.clear();
			operands.push_back(D);
			//cout << i << " - i " << endl << t << "- t" << endl;

			i = t;
		}
		if (A[i] == 'm') {
			//cout << "Detected ' m '" << endl;
			minus = true;
			if (i != 0) {
				if (A[i - 1] <= 57 && A[i - 1] >= 48) {
					operations.push_back('+');
				}
			}
		}
		if (A[i] == '+' || A[i] == '-' || A[i] == '*' || A[i] == '/' || A[i] == '^') {
			//cout << "+letter " << A[i] << endl;
			if (i == 0) {
				operands.push_back(Result);
			}
			operations.push_back(A[i]);
		}
		i++;
	}

	if (operands.size() == 1 && operations.size() == 0) {
		Result = operands[0];
		cout << endl << "RESULT: " << Result << endl;
		return 1;
	}
	
	vector<int> priority;
	for (int i = 0; i < operations.size(); i++) {
		if (operations[i] == '^') {
			priority.push_back(i);
		}
	}
	for (int i = 0; i < operations.size(); i++) {
		if (operations[i] == '*' || operations[i] == '/') {
			priority.push_back(i);
		}
	}
	for (int i = 0; i < operations.size(); i++) {
		if (operations[i] == '+' || operations[i] == '-') {
			priority.push_back(i);
		}
	}
	if (operations.size() == 1 && operands.size() == 1) {
		Result = calculate(Result, operands[0], operations[0]);
		cout << "Result: " << Result << endl;
		return 1;
	}
	//cout << "OPERATIONS: ";
	for (int i = 0; i < operations.size(); i++) {
		//cout << operations[i] << ' ';
	}
	//cout << endl;
	//cout << "OPERANDS: ";
	for (int i = 0; i < operands.size(); i++) {
		//cout << operands[i] << ' ';
	}
	//cout << endl;
	//cout << "PRIORITY: ";
	for (int i = 0; i < priority.size(); i++) {
		//cout << priority[i] << ' ';
	}
	//cout << endl;
	//cout << "links init: ";
	links.resize(operands.size());
	for (int i = 0; i < links.size(); i++) {
		links[i] = -1; //cout << "* ";
	}
	//cout << endl;
	for (int i = 0; i < operations.size(); i++) {
		int I = priority[i];
		int A = I;
		int B = I + 1;
		while (links[A] != -1 || links[B] != -1) {
			if (links[A] != -1) {
				A = links[A];
			}
			if (links[B] != -1) {
				B = links[B];
			}
		}
		if (operands[B] == 0 && operations[I] == '/') {
			return false;
		}
		if (operands[A] < 0 && operations[I] == '^' && (operands[B] != int(operands[B]))) {
			return false;
		}
		operands[A] = calculate(operands[A], operands[B], operations[I]);
		
		//cout << "time result " << operands[A] << endl;
		Result = operands[A];
		links[B] = A;

		//cout << "LINKS: ";
		for (int i = 0; i < links.size(); i++) {
			//cout << links[i] << ' ';
		}
		//cout << endl;
	}

	cout << endl << "RESULT: " << Result << endl;
	A.clear();
	return 1;
}
void Display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for (int i = 0; i < figures.size(); i++) {
		glLoadIdentity();
		switch (figures[i].type)
		{
		default:
			break;
		case 0:
			break;
		case 1:
			glTranslatef(figures[i].center.x, figures[i].center.y, figures[i].center.z);
			glRotatef(figures[i].anglesize, figures[i].angle.x, figures[i].angle.y, figures[i].angle.z);
			glBegin(GL_POLYGON);
			for (int t = 0; t < figures[i].top.size(); t++) {
				glColor4f(figures[i].colors[t].R, figures[i].colors[t].G, figures[i].colors[t].B, figures[i].colors[t].A);
				glVertex3f(figures[i].top[t].x, figures[i].top[t].y, figures[i].top[t].z);
			}
			glEnd();
			break;
		case 2:

			break;
		case 3:

			break;
		case 4:
			glTranslatef(figures[i].center.x, figures[i].center.y, figures[i].center.z);
			glRotatef(figures[i].anglesize, figures[i].angle.x, figures[i].angle.y, figures[i].angle.z);
			glLineWidth(figures[i].width);
			glBegin(GL_LINE_STRIP);
			for (int t = 0; t < figures[i].top.size(); t++) {
				glColor4f(figures[i].colors[t].R, figures[i].colors[t].G, figures[i].colors[t].B, figures[i].colors[t].A);
				glVertex3f(figures[i].top[t].x, figures[i].top[t].y, figures[i].top[t].z);
			}
			glEnd();
			break;
		case 5:
			glTranslatef(figures[i].center.x, figures[i].center.y, figures[i].center.z);
			glRotatef(figures[i].anglesize, figures[i].angle.x, figures[i].angle.y, figures[i].angle.z);
			glLineWidth(figures[i].width);
			glBegin(GL_POINTS);
			glColor4f(figures[i].colors[0].R, figures[i].colors[0].G, figures[i].colors[0].B, figures[i].colors[0].A);
			glVertex3f(figures[i].top[0].x, figures[i].top[0].y, figures[i].top[0].z);
			glEnd();
			break;
		}
	}
	for (int i = 0; i < words.size(); i++) {
		write(words[i]);
	}


	switch (scene)
	{
	default:
		break;
	case 1: {
		if (THEstring.size() < 1.0 * 30 * WINDOW_WIDTH / 600) {
			words[23].size = 24;
		}
		else {

			words[23].size = 10;
		}
	}
	}


	glutSwapBuffers();

}
bool createequation(char I) {
	if (I == 8) {
		if (THEstring.size() != 0) {
			THEstring.pop_back();
		}
	}
	if (I == 127) {
		THEstring.clear();
		Result = 0;
		words[3].word = { '0' };
		words[3].size = 24;
	}
	if (I != 13) {
		if ((I >= 48 && I <= 57) || I == '+' || I == '-' || I == '*' || I == '/' || I == '(' || I == ')' || I == '^' || I == '.' || I == 'S' || I == '|' || I == 's' || I == 'c' || I == 't' || I == '!' || I == 'P' || I == 'e' || I == 'X' || I == 'x' || I == 'l') {
			if (I == 'x') {
				I = 'X';
			}
			switch (I)
			{
			default: {
				THEstring.push_back(I);
				cout << "Pushed " << I << endl;
				break;
			}
			case 'S': {
				THEstring.push_back('s');
				THEstring.push_back('q');
				THEstring.push_back('r');
				THEstring.push_back('t');
				THEstring.push_back('(');
				cout << "Pushed " << "sqrt(" << endl;
				break;
			}
			case 's': {
				THEstring.push_back('s');
				THEstring.push_back('i');
				THEstring.push_back('n');
				THEstring.push_back('(');
				cout << "Pushed " << "sin(" << endl;
				break;
			}
			case 'c': {
				THEstring.push_back('c');
				THEstring.push_back('o');
				THEstring.push_back('s');
				THEstring.push_back('(');
				cout << "Pushed " << "cos(" << endl;
				break;
			}
			case 't': {
				THEstring.push_back('t');
				THEstring.push_back('g');
				THEstring.push_back('(');
				cout << "Pushed " << "tg(" << endl;
				break;
			}
			case 'l': {
				THEstring.push_back('l');
				THEstring.push_back('n');
				THEstring.push_back('(');
				cout << "Pushed " << "ln(" << endl;
				break;
			}
			case '^': {
				THEstring.push_back('^');
				THEstring.push_back('(');
				cout << "Pushed " << "^(" << endl;
			}
			}
		}
	}
	else {
		if (!qualilty) {
			if (check()) {
				double X = -10.6;
				int counting = 0;
				vector<coord> top;
				coord C;
				vector<color> colors;
				color COL;
				coord center;
				coord angle;

				COL.R = 0.0f + sin(GrafParameters[5]) * cos(GrafParameters[5]);
				COL.G = 0.0f + cos(GrafParameters[5]);
				COL.B = 0.0f + sin(GrafParameters[5]);
				COL.A = 1.0f;
				colors.push_back(COL);

				center.x = 0.0f;
				center.y = 0.0f;
				center.z = 0.0f;

				angle.x = 0.0f;
				angle.y = 0.0f;
				angle.z = 0.0f;

				int ID = add(4, top, colors, center, 0, angle, 0, 0);

				while (X <= 10.42) {
					vector<char> word;
					ostringstream ost;
					ost << X;
					string s_d = ost.str();
					word.resize(s_d.size());
					vector<char> B;
					vector<char> temp;
					B = THEstring;
					temp = B;
					for (int i = 0; i < s_d.size(); i++) {
						word[i] = s_d[i];
					}
					bool  findX = true;
					while(findX){
						findX = false;
						for (int i = 0; i < temp.size() && !findX; i++) {
							if (B[i] == 'X') {
								B.clear();
								
								for (int t = 0; t < i; t++) {
									B.push_back(temp[t]);
								}
								B.push_back('(');
								for (int t = 0; t < word.size(); t++) {
									B.push_back(word[t]);
								}
								B.push_back(')');
								for (int t = i + 1; t < temp.size(); t++) {
									B.push_back(temp[t]);
								}
								//cout << "B: ";
								//for (int k = 0; k < B.size(); k++) {
								//	cout << B[k] << ' ';
								//}
								//cout << endl;
								//cout << "temp: ";
								//for (int k = 0; k < temp.size(); k++) {
								//	cout << temp[k] << ' ';
								//}
								//cout << endl;
								findX = true;
								temp = B;
							}
						}
				}
						if (count(B)) {
							cout << "X / Y = " << X << '/' << Result << endl;
							C.x = X * 7 - 27;
							C.y = Result * 7 + 20;
							C.z = 0.0f;
								figures[ID].top.push_back(C);
								figures[ID].colors.push_back(COL);
								counting++;
						}
						else {
							cout << "Error" << endl;
							if (figures[ID].top.size() != 0) {
							COL.R = 0.0f + sin(GrafParameters[5]) * cos(GrafParameters[5]);
							COL.G = 0.0f + cos(GrafParameters[5]);
							COL.B = 0.0f + sin(GrafParameters[5]);
							COL.A = 0.0f;
							colors.push_back(COL);

							center.x = 0.0f;
							center.y = 0.0f;
							center.z = 0.0f;

							angle.x = 0.0f;
							angle.y = 0.0f;
							angle.z = 0.0f;

							int ID = add(4, top, colors, center, 0, angle, 0, 0);
						}

						}
					X += GrafParameters[0];
					if (counting == 50) {
						Display();
						counting = 0;
					}
				}
				top.clear();
				colors.clear();
				GrafParameters[5] += 150;
				cout << "done" << endl;
			}
			else {
				cout << "Wrong format" << endl;
			}
			/*else {
				words[22].size = 0;
				words[24].word = { 'W', 'r', 'o', 'n', 'g', ' ', 'f', 'o', 'r', 'm', 'a', 't' };
				words[24].size = 24;
				THEstring.clear();
				cout << "Done" << endl;
			}*/
		}
		else {
			Qcount(THEstring);
		}
	}

	if (THEstring.size() > 42 * 1.0 * WINDOW_WIDTH / 600) {
		THEstring.pop_back();
		//words[3].size = 0;
		//words[24].word = { 'L', 'i', 'm', 'i', 't', ' ', 'r', 'e', 'a', 'c', 'h', 'e', 'd' };
		//words[24].size = 24;
	}

	words[3].word.clear();
	words[3].word.push_back('Y');
	words[3].word.push_back('=');
	for (int i = 0; i < THEstring.size(); i++) {
		words[3].word.push_back(THEstring[i]);
	}
	return 1;
}

bool createstring(char I) {
	
	if (I == 'h' || I == 'р') {
		thread thr(historyOPCL);
		thr.detach();
	}
	if (I == 8) {
		if (THEstring.size() != 0) {
			THEstring.pop_back();
		}
	}
	if (I == 127) {
		THEstring.clear();
		Result = 0;
		words[22].word = {'0'};
		words[22].size = 24;
		words[24].size = 0;
	}
	if (I != '=' && I != 13) {
		if ((I >= 48 && I <= 57) || I == '+' || I == '-' || I == '*' || I == '/' || I == '(' || I == ')' || I ==  '^' || I == '.' || I == 'S' || I == '|' || I == 's' || I == 'c' || I == 't' || I == '!' || I == 'P' || I == 'e' || I == 'l') {
			switch (I)
			{
			default: {
				THEstring.push_back(I);
				cout << "Pushed " << I << endl;
				break; 
			}
			case 'S': {
				THEstring.push_back('s');
				THEstring.push_back('q');
				THEstring.push_back('r');
				THEstring.push_back('t');
				THEstring.push_back('(');
				cout << "Pushed " << "sqrt(" << endl;
				break;
			}
			case 's': {
				THEstring.push_back('s');
				THEstring.push_back('i');
				THEstring.push_back('n');
				THEstring.push_back('(');
				cout << "Pushed " << "sin(" << endl;
				break;
			}
			case 'c': {
				THEstring.push_back('c');
				THEstring.push_back('o');
				THEstring.push_back('s');
				THEstring.push_back('(');
				cout << "Pushed " << "cos(" << endl;
				break;
			}
			case 't': {
				THEstring.push_back('t');
				THEstring.push_back('g');
				THEstring.push_back('(');
				cout << "Pushed " << "tg(" << endl;
				break;
			}
			case 'l': {
				THEstring.push_back('l');
				THEstring.push_back('n');
				THEstring.push_back('(');
				cout << "Pushed " << "ln(" << endl;
				break;
			}
			}
		}
	}
	else {
		if (!qualilty) {
		if (count(THEstring)) {
			vector<char> word;
			ostringstream ost;
			ost << Result;
			string s_d = ost.str();
			cout << endl << s_d << "Here it is" << endl;
			word.resize(s_d.size());
			for (int i = 0; i < s_d.size(); i++) {
				word[i] = s_d[i];
			}
			words[22].word = word;
			words[22].size = 24;
			words[24].size = 0;

			word.push_back('=');
			for (int i = 0; i < THEstring.size(); i++) {
				word.push_back(THEstring[i]);
			}
			History.push_back(word);

			THEstring.clear();
			cout << "Done" << endl;
		}
		else {
			words[22].size = 0;
			words[24].word = { 'W', 'r', 'o', 'n', 'g', ' ', 'f', 'o', 'r', 'm', 'a', 't' };
			words[24].size = 24;
			THEstring.clear();
			cout << "Done" << endl;
		}
		}
		else {
			Qcount(THEstring);
		}
	}

	if (THEstring.size() > 42 * 1.0 * WINDOW_WIDTH / 600) {
		THEstring.pop_back();
		words[22].size = 0;
		words[24].word = { 'L', 'i', 'm', 'i', 't', ' ', 'r', 'e', 'a', 'c', 'h', 'e', 'd' };
		words[24].size = 24;
	}
	
	
	words[23].word = THEstring;
	return 1;
}

int findbutton(float x, float y) {
	x += 100;
	y -= 100;
	x = x * 600 / WINDOW_WIDTH;
	y = y* 600 / WINDOW_HEIGHT;
	x -= 100;
	y += 100;
	switch (scene)
	{
	default:
		break;
	case 1:{
		if ((x >= -27.0f && x <= -22.0f) && (y >= 55.0f && y <= 58.0f) && !history_open) {
			return -2;
		}
		if ((x >= -27.0f && x <= -22.0f) && (y >= -100.0f && y <= -97.0f) && history_open) {
			return -3;
		}
		if (!history_open) {
			for (int i = 1; i <= 6; i++) {
				if (x <= -73.0f && x >= -100.0f && y <= 54.99998f) { //проверка на кнопки знаков
					if (y <= i * 25.83333f - 100) {
						return i + 8;
					}
				}
			}
			if (history_open) {

			}
			if ((x > -73.0f &&  x < 45.0f) && (y >= -100.0f && y <= 55.0f)) {
				for (int i = 1; i <= 3; i++) {
					for (int t = 0; t <= 5; t++) {
						if (x < i * 39.3333f - 73.0f && y < t * 31 - 101.0f) {
							return (i - 1) * 5 + t + 20;
						}
					}
				}
			}
		}
		if (x > 50 && x < 100 && y < 100 && y > 87) {
			return 67;
		}
		if (!menu_open) {
			if (x > 50 && x < 75) {
				for (int i = 1; i <= 8; i++) {
					if (y < 23 * i - 97) {
						return 50 + i;
					}
				}
			}
			if (x > 75 && x < 100) {
				for (int i = 1; i <= 8; i++) {
					if (y < 23 * i - 97) {
						return 58 + i;
					}
				}
			}
		}
		
	}
	case 2: {
		if (x < 100 && x > 50 && y < 100 && y > 87) {
			return 0;
		}

		for (int i = 1; i <= 3 && y < -62; i++) {
				for (int t = 1; t <= 2; t++){
					if (x < -100 + 49.7 * i - 2 * i && y > -62 - 15 * t) {
						return 11 + t + 2 * (i - 1);
					}
				}
			}
		if (x < 46) {
			
		}
	}
}
	return -1;
}
void normalbutton(int i) {

	Sleep(60);
	for (int t = 0; t <= 4; t++) {
		figures[i].type = 0;
	}
}

bool pressbutton2(char I){
	createequation(I);
	return 1;
}
bool pressbutton1(char I) {
	createstring(I);
	switch (I) // 9 - 14
	{
		default:
			break;
		case '+':
		{
			figures[9].type = 1;
			thread thr(normalbutton, 9);
			thr.detach();
			break;
		}
		case '-':
		{
			figures[10].type = 1;
			thread thr(normalbutton, 10);
			thr.detach();
			break;
		}
		case '*':
		{
			figures[11].type = 1;
			thread thr(normalbutton, 11);
			thr.detach();
			break;
		}
		case '/':
		{
			figures[12].type = 1;
			thread thr(normalbutton, 12);
			thr.detach();
			break;
		}
		case '^':
		{
			figures[13].type = 1;
			thread thr(normalbutton, 13);
			thr.detach();
			break;
		}
		case '√':	// Так вообще можно?
		{
			figures[14].type = 1;
			thread thr(normalbutton, 14); 
			thr.detach();
			break;
		}
		case '.': 
		{
			figures[31].type = 1;
			thread thr(normalbutton, 31);
			thr.detach();
			break;
		}
		case '0':
		{
			figures[26].type = 1;
			thread thr(normalbutton, 26);
			thr.detach();
			break;
		}
		case '=': {
			figures[25].type = 1;
			thread thr(normalbutton, 25);
			thr.detach();
			break; 
		}
		case 127: {
			figures[30].type = 1;
			thread thr(normalbutton, 30);
			thr.detach();
			break;
		}
		case 8: {
			figures[35].type = 1;
			thread thr(normalbutton, 35);
			thr.detach();
			break;
		}
		case 13: {
			figures[25].type = 1;
			thread thr(normalbutton, 25);
			thr.detach();
			break;
		}
		case 's': {
			figures[58].type = 1;
			thread thr(normalbutton, 58);
			thr.detach();
			break;
		}
		case 'c': {
			figures[66].type = 1;
			thread thr(normalbutton, 66);
			thr.detach();
			break;
		}
		case 't': {
			figures[57].type = 1;
			thread thr(normalbutton, 57);
			thr.detach();
			break;
		}
		case '(': {
			figures[55].type = 1;
			thread thr(normalbutton, 55);
			thr.detach();
			break;
		}
		case ')': {
			figures[63].type = 1;
			thread thr(normalbutton, 63);
			thr.detach();
			break;
		}
		case '|': {
			figures[64].type = 1;
			thread thr(normalbutton, 64);
			thr.detach();
			break;
		}
		case '!': {
			figures[53].type = 1;
			thread thr(normalbutton, 53);
			thr.detach();
			break;
		}
		case 'e': {
			figures[62].type = 1;
			thread thr(normalbutton, 62);
			thr.detach();
			break;
		}
		case 'P': {
			figures[54].type = 1;
			thread thr(normalbutton, 54);
			thr.detach();
			break;
		}

		{
			
		}
	}
	int k = 0;
	for (int i = 1; k < 3; i += 3) {
		k++;
		if (I == i + 48) {
			figures[21 + k].type = 1;
			thread thr(normalbutton, 21 + k);
			thr.detach();
			break;
		}
	}
	k = 0;
	for (int i = 2; k < 3; i += 3) {
		k++;
		if (I == i + 48) {
			figures[26 + k].type = 1;
			thread thr(normalbutton, 26 + k);
			thr.detach();
			break;
		}
	}
	k = 0;
	for (int i = 3; k < 3; i += 3) {
		k++;
		if (I == i + 48) {
			figures[31 + k].type = 1;
			thread thr(normalbutton, 31 + k);
			thr.detach();
			break;
		}
	}
	return true;
}
void key(unsigned char key, int x, int y) {
/*	switch (key)
	{
	default:
		break;
	case '+':
	{
		pressbutton('+');
		break;
	}
		case ''
	}*/
	switch (scene)
	{
	default:
		break;
	case 1: {
		pressbutton1(key);
		break;
	}
	case 2: {
		pressbutton2(key);
		break;
	}
	}
	
}
void mouse2(int button, int state, int x, int y) {
	int TYPE = findbutton((x - 300) / 3, -(y - 300) / 3);
	cout << x << ' ' << y << endl << "Found button " << TYPE << endl;
	switch (button) {
	default:
		break;
	case 0: {
		if (state == 0) {
			if (TYPE != -1 && TYPE < 0) {

			}
			if (TYPE >= 0) {
				lastbutton = TYPE;
				for (int i = 0; i < 4; i++) {
					figures[lastbutton].colors[i].R = 1.0;
					figures[lastbutton].colors[i].G = 1.0;
					figures[lastbutton].colors[i].B = 1.0;
				}
				figures[lastbutton].type = 1;
				cout << "BUTTON NUMBER: " << lastbutton << endl;
				switch (lastbutton) {
				default:
					break;
				case 0: {
					changescene(1);
				}
				case 11: {
					break;
				}
				case 12: {
					break;
				}
				case 13: {
					break;
				}
				case 14: {
					break;
				}
				case 15: {
					break;
				}
				case 16: {
					changescene(2);
					break;
				}
				}
			}
		}
		else {
			if (lastbutton != -1) {
				normalbutton(lastbutton);
				lastbutton = -1;
			}
		}
	}
	}
}
void mouse1(int button, int state, int x, int y) {

	int TYPE = findbutton((x - 300) / 3, -(y - 300) / 3);
	cout << x << ' ' << y << endl << "Found button " << TYPE << endl;
	switch (button)
	{
	default:
		break;
	case 0: {
		if (state == 0) {
			if (TYPE < 0 && TYPE != -1) {
				switch (TYPE)
				{
				default:
					break;
				case -2: {
					if (!history_open) {
						thread thr(historyOPCL);
						thr.detach();
					}
				case -3: {
					if (history_open) {
						thread thr(historyOPCL);
						thr.detach();
					}
				}
				}
				}
			}
			if (TYPE >= 0) {
				//if (lastbutton != TYPE) {
					lastbutton = TYPE;
					for (int i = 0; i < 4; i++) {
						figures[lastbutton].colors[i].R = 1.0;
						figures[lastbutton].colors[i].G = 1.0;
						figures[lastbutton].colors[i].B = 1.0;
					}
					figures[lastbutton].type = 1;
					cout << "BUTTON NUMBER: " << lastbutton << endl;
					switch (lastbutton)
					{
					default:
						break;
					case 24: {
						createstring('7');
						break;
					}
					case 23: {
						createstring('4');
						break;
					}
					case 22: {
						createstring('1');
						break;
					}
					case 29: {
						createstring('8');
						break;
					}
					case 28: {
						createstring('5');
						break;
					}
					case 27: {
						createstring('2');
						break;
					}
					case 26: {
						createstring('0');
						break;
					}
					case 34: {
						createstring('9');
						break;
					}
					case 33: {
						createstring('6');
						break;
					}
					case 32: {
						createstring('3');
						break;
					}
					case 9: {
						createstring('+');
						break;
					}
					case 10: {
						createstring('-');
						break;
					}
					case 11: {
						createstring('*');
						break;
					}
					case 12: {
						createstring('/');
						break;
					}
					case 13: {
						createstring('^');
						break;
					}
					case 14: {
						createstring('S'); //корень
						break;
					}
					case 25: {
						createstring('=');
						break;
					}
					case 30: {
						createstring((char)127); //C
						break;
					}
					case 35: {
						createstring(8); //Backspace
						break;
					}
					case 21: {
						createstring(' '); // +-
						break;
					}
					case 31: {
						createstring('.');
						break;
					}
					case 51: {
						createstring(' ');
						break;
					}
					case 52: {
						createstring(' ');
						break;
					}
					case 53: {
						createstring('!');
						break;
					}
					case 54: {
						createstring('P');
						break;
					}
					case 55: {
						createstring('(');
						break;
					}
					case 56: {
						createstring('l');
						break;
					}
					case 57: {
						createstring('t');
						break;
					}
					case 58: {
						createstring('s');
						break;
					}
					case 59: {
						createstring(' ');
						break;
					}
					case 60: {
						createstring(' ');
						break;
					}
					case 61: {
						createstring(' ');
						break;
					}
					case 62: {
						createstring('e');
						break;
					}
					case 63: {
						createstring(')');
						break;
					}
					case 64: {
						createstring('|');
						break;
					}
					case 65: {
						createstring(' ');
						break;
					}
					case 66: {
						createstring('c');
						break;
						
					}
					case 67: {
						changescene(2);
						//thread A (menuOPCL);
						//A.detach();
						
					}
					}
					//createstring(lastbutton);//
				}
				
				
			//}
		}
		else {
			if (lastbutton != -1) {
				normalbutton(lastbutton);
				lastbutton = -1;
			}
		}
	}
	}
}

void mouseACT(int x, int y) {

}


void mousePAS1(int x, int y) {
	int TYPE = findbutton((x - 300) / 3, -(y - 300) / 3);
	if (TYPE >= 0) {
		if (lastbutton != TYPE) {
			if (lastbutton != -1) {
				figures[lastbutton].type = 0;
				for (int i = 0; i < 4; i++) {
					figures[lastbutton].colors[i].R = 1.0;
					figures[lastbutton].colors[i].G = 1.0;
					figures[lastbutton].colors[i].B = 1.0;
				}
			}
			}
			lastbutton = TYPE;
			for (int i = 0; i < 4; i++) {
				figures[lastbutton].colors[i].R = 0.7;
				figures[lastbutton].colors[i].G = 0.7;
				figures[lastbutton].colors[i].B = 0.7;
			}
			figures[lastbutton].type = 1;
		}
	
	else {
		if (lastbutton != -1) {
			figures[lastbutton].type = 0;
			for (int i = 0; i < 4; i++) {
				figures[lastbutton].colors[i].R = 1.0;
				figures[lastbutton].colors[i].G = 1.0;
				figures[lastbutton].colors[i].B = 1.0;
			}
		}
	}
}
void mouse(int button, int state, int x, int y) {
	switch (scene) {
	default:
		break;
	case 1: {
		mouse1(button, state, x, y);
		break;
	}
	case 2: {
		mouse2(button, state, x, y);
		break;
	}
	}
}
void mousePAS(int x, int y) {
	mousePAS1(x, y);
}


int main(int argc, char** argv) {
	changescene(1); // 1 - первая(главная сцена)
	GrafParameters[0] = 0.05;
	GrafParameters[1] = 0.5;
	GrafParameters[2] = 0.5;
	GrafParameters[3] = 0;
	GrafParameters[4] = 0;
	GrafParameters[5] = 30;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(600, 600);
	glutCreateWindow("Калькулятор");
	hWnd = FindWindow(NULL, "Калькулятор");
	ShowWindow(hWnd, SW_SHOW);              // Показать окно
	SetForegroundWindow(hWnd);              // Слегка повысим приоритет
	SetFocus(hWnd);                // Установить фокус клавиатуры на наше окно

	glShadeModel(GL_SMOOTH);            // Разрешить плавное цветовое сглаживание

	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);          // Очистка экрана в черный цвет
	glOrtho(-100.0, 100.0, -100.0, 100.0, -100.0, 100.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glClearDepth(1.0f);              // Разрешить очистку буфера глубины
	glEnable(GL_DEPTH_TEST);            // Разрешить тест глубины
	glDepthFunc(GL_LEQUAL);            // Тип теста глубины

	glutDisplayFunc(Display);
	glutIdleFunc(Display);
	glutKeyboardFunc(key);
	glutReshapeFunc(ReSize);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseACT);
	glutPassiveMotionFunc(mousePAS);
	glutMainLoop();
}