#include <iostream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>
#include <vector>
#include <random>


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


#pragma warning(disable:4996)

#define windowWidth 1000
#define windowHeight 800

#define Plate_Num 20

#define Mini_Giant_Num 50

#define Stone_Num 5


template <typename T>
const T& clamp(const T& value, const T& minVal, const T& maxVal) {
	return std::max(minVal, std::min(value, maxVal));
}


float vertices[] = { //--- 버텍스 속성: 좌표값(FragPos), 노말값 (Normal)
-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f,0.0f,		0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,0.0f,0.0f,		 0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,0.0f,1.0f, //뒷면
0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,0.0f,1.0f,		-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,1.0f,1.0f,		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,1.0f,0.0f,

-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,0.0f,0.0f,		0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,1.0f,0.0f,		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,1.0f,1.0f, //앞면
0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,	1.0f,1.0f,		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,0.0f,1.0f,		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,0.0f,0.0f,

-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,1.0f,1.0f,		 -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f,0.0f,1.0f,	 -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,0.0f,0.0f, // 왼쪽면
-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,0.0f,0.0f,	 -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f,1.0f,0.0f,			 -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,1.0f,1.0f,

0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,0.0f,1.0f,			 0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,1.0f,1.0f,			0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,1.0f,0.0f, //오른쪽면
0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,1.0f,0.0f,    	0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f,0.0f,		     0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,0.0f,1.0f,

-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,0.0f,0.0f,		0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,1.0f,0.0f,		 0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,1.0f,1.0f, // 바닥
0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,1.0f,1.0f,	  	 -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,0.0f,1.0f,			-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,0.0f,0.0f,

-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,0.0f,1.0f,		 0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,1.0f,1.0f,	   0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,1.0f,0.0f,// 위
0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,	1.0f,0.0f,		 -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,0.0f,0.0f,		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,0.0f,1.0f
};


GLuint shaderID;
GLuint vertexShader;
GLuint fragmentShader;


int width{}, height{};
float box_width_size{}, box_height_size{};
float world_size{ 1.0 };


float camera_x = 0.0f;
float camera_y = 0.2f;
float camera_z = 5.0f;
float camera_y_angle = 0.0f;


glm::vec3 cameraPos = glm::vec3(camera_x, camera_y, camera_z); // 카메라 위치
glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, -1.0f);      // 초기 카메라 바라보는 방향
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

std::default_random_engine dre;
std::uniform_real_distribution<float> uid{ -1.0f,1.0f };

float cameraSpeed = 0.5f;
float cameraRotationSpeed = 2;


glm::vec3 sun_color = { 1.0f, 1.0f, 1.0f };
glm::vec3 light_pos = { 0.0f, 60.0f, 0.0f };
static float sun_angle = 5.0f;



bool P_front{}, P_back{}, P_right{}, P_left{};
float leg_rotate{ 0.0 };
float blend_size{ 1.0 };
float effect_size[10]{ 0.0 };
int rear_effect{ 0 };


bool Throw_mode{};
bool B_Spin_mode{};
bool T_Spin_mode{};

bool HIT{ false };

struct Point3
{
	float x;
	float y;
	float z;
};


struct Player
{
	glm::vec3 pos;
	float jump_value;
	bool jump;

	bool W_Set;
	bool W_Riding;

	glm::vec3 W_start_p;
	glm::vec3 W_middle_p;
	int W_index = 0;
	glm::vec3 W_end_p;

	bool flying; // 와이어에서 내려 공중이면


	float x_shake = 0.0f;
	float y_shake = 0.0f;
	float z_shake = 0.0f;

}player;


struct Mouse
{
	float mx;
	float my;

	float last_mx;
	float last_my;

	float last_dx;
	float last_dy;

	GLfloat V[4][3] =
	{
		{ -0.5f, 0.0f, 0.0f },
		{ 0.5f, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f },
		{ 0.5f, 0.0f, 0.5f }
	};


	GLfloat N[4][3] =
	{
		{ 0.5f, 0.0f, 0.0f },
		{ 0.5f, 0.0f, 0.0f },
		{ 0.5f, 0.0f, 0.0f },
		{ 0.5f, 0.0f, 0.0f },
	};

	GLuint mvao;
	GLuint mvbo_v;
	GLuint mvbo_n;


}M;

struct Mini_Giant
{
	float x;
	float y;
	float z;

	float dx;
	float dz;
	float speed;
	float run_speed;


	float R; // 비틀기 각도
	float angle;
	bool Move = false; // 움직임 시작
	bool Run = false; // 달리기 시작

};

struct Big_Giant {
	glm::vec3 pos = { 40.0f, 15.0f, 50.0f };
	float rad;
	bool hp{ true };
	float dead_rad{ 0.0 };
	bool effect{ false };
}big_giant;

struct Animal_Giant
{
	glm::vec3 pos = { -30.0f, 10.0f, -30.0f };


}animal_giant;


struct Spear
{
	glm::vec3 pos = { 55, 33, 14 };
	glm::vec3 direction;

	float speed;
	bool hp;
	bool shooting;
	float y_value{ 1.0f };
	float dy{ 0.05f };
	float angle{ 0.0 };
	// 라디안을 각도로 변환 (선택사항)
	float angleDegrees = 0.0;
	glm::vec3 result;

}spear;


struct Stone
{
	glm::vec3 pos = { 0.0f, 5.0f, -60.0f };
	glm::vec3 start_pos = { 0.0f, 5.0f, -60.0f };
	glm::vec3 target_pos = { 0.0f, 60.0f, 0.0f };
	glm::vec3 end_pos = { 0.0f, 60.0f, 0.0f };
	int T; //배지어 곡선 매개변수
	int speed;

	std::vector<Point3> route; // 경로
	float rad{ 0.0 };

	bool action = false;
	int Used_stone_num = 0;

}stone;

struct Collision_area {
	glm::vec3 T_hand_R = { 0,0,0 };
	glm::vec3 T_hand_L = { 0,0,0 };
	glm::vec3 T_face = { 40.0f, 27.0f, 49.5f };
}c_area;

static bool Throwing = false;


static std::vector<Point3> wire;

static std::vector<Mini_Giant> Mini_giant(Mini_Giant_Num);
static int Used_MG = 0;


static float plate[Plate_Num][3];
static float void_plate[15][3];

void Mini_Timer(int);
void Throw_Stone_Timer(int);
void T_Attack_Timer(int);
void Spear_Timer(int);
void Hit_Timer(int);

void Mouse_Click(int button, int state, int x, int y);
bool check_pillar_collision();
void check_stone_collision();
bool intersectRayAabb(const glm::vec3& rayOrigin, const glm::vec3& rayDir,
	const glm::vec3& aabbMin, const glm::vec3& aabbMax);



// 베지어 곡선을 계산하는 함수
Point3 computeBezierPoint(float t, const Point3& p0, const Point3& p1, const Point3& p2)
{
	float x = (1 - t) * (1 - t) * p0.x + 2 * (1 - t) * t * p1.x + t * t * p2.x;
	float y = (1 - t) * (1 - t) * p0.y + 2 * (1 - t) * t * p1.y + t * t * p2.y;
	float z = (1 - t) * (1 - t) * p0.z + 2 * (1 - t) * t * p1.z + t * t * p2.z;
	return { x, y, z };
}


void Set_Wire(const glm::vec3& start_p, const glm::vec3& end_p)
{
	wire.clear();

	Point3 start_pos = { start_p.x, start_p.y, start_p.z };
	Point3 end_pos = { end_p.x, end_p.y, end_p.z };
	Point3 middle_pos = { start_p.x, start_p.y, start_p.z };

	player.W_start_p = glm::vec3(start_pos.x, start_pos.y, start_pos.z);
	player.W_middle_p = glm::vec3(middle_pos.x, middle_pos.y, middle_pos.z);
	player.W_end_p = glm::vec3(end_pos.x, end_pos.y, end_pos.z);


	float create_point_num = 0.0001; // 10000개

	for (float i = 0; i <= 1.0; i += create_point_num)
	{
		Point3 temp = computeBezierPoint(i, start_pos, middle_pos, end_pos);
		wire.push_back(temp);
	}
}


//와이어 관련
void Wire_update()
{
	// 새로운 중간점을 계산
	player.W_start_p = player.pos;

	float ratio = static_cast<float>(player.W_index) / 100.0f;

	// 중간점의 x와 z 값을 출발점에서 도착점으로 선형 보간
	float middle_x = player.W_start_p.x + (player.W_end_p.x - player.W_start_p.x) * ratio;
	float middle_y = player.pos.y; //player.W_start_p.y + (player.W_end_p.y - player.W_start_p.y) * ratio;
	float middle_z = player.W_start_p.z + (player.W_end_p.z - player.W_start_p.z) * ratio;

	// W_index를 1씩 증가시킴
	// 비율이 1에 도달하면 end_pos에 도달한 것으로 간주
	if (player.W_index <= 50) {
		player.W_index++;
	}


	// wire 벡터의 크기를 미리 할당하여 재할당 횟수를 최소화
	size_t wire_size = static_cast<size_t>(1.0f / 0.0001 + 1); // 크기 계산
	wire.clear();
	wire.reserve(wire_size);


	Point3 start_pos = { player.W_start_p.x, player.W_start_p.y, player.W_start_p.z };
	Point3 middle_pos = { middle_x, middle_y, middle_z };
	Point3 end_pos = { player.W_end_p.x, player.W_end_p.y, player.W_end_p.z };
	// 베지어 곡선 계산 및 wire 벡터에 추가
	for (float i = 0.0f; i <= 1.0f; i += 0.0001f)
	{

		wire.push_back(computeBezierPoint(i, start_pos, middle_pos, end_pos));
	}
}

bool Follow_Wire(int n) // 0 입력하면 중단 및 초기화
{

	static int W_N = 0;
	static int W_value = 100;

	if (n == 0)
	{
		W_N = 0;
		W_value = 100;
		player.jump = true;
		player.flying = true;
		player.jump_value = 1; // 와이어 종료시 점프 값
		player.W_Set = false;
		player.W_Riding = false;
		player.W_index = 0;
		wire.clear();
		return false;
	}

	W_N += W_value;

	W_value += 50;

	if (W_N < wire.size())
	{
		player.pos.x = wire[W_N].x;
		player.pos.y = wire[W_N].y;
		player.pos.z = wire[W_N].z;
		return true;
	}
	else if (std::abs(player.pos.x - wire.back().x) <= 0.5f
		&& std::abs(player.pos.y - wire.back().y) <= 0.5f
		&& std::abs(player.pos.z - wire.back().z) <= 0.5f)
	{
		W_N = 0;
		W_value = 100;
		player.jump = true;
		player.flying = true;
		player.jump_value = 1; // 와이어 종료시 점프 값
		player.W_Set = false;
		player.W_Riding = false;
		player.W_index = 0;
		wire.clear();
		return false;

	}
	else
	{

		W_N = 0;
		W_value = 100;
		player.jump = true;
		player.flying = true;
		player.jump_value = 1; // 와이어 종료시 점프 값
		player.W_Set = false;
		player.W_Riding = false;
		player.W_index = 0;
		wire.clear();
		return false;
	}

}

void Wire_Timer(int value)
{
	if (player.W_Set && !player.W_Riding)
	{
		Wire_update();
		glutPostRedisplay();
		glutTimerFunc(30, Wire_Timer, 1);
	}
}

void Follow_Wire_Timer(int value)
{
	if (player.W_Riding == false)
		return;

	Follow_Wire(1);

	if (player.W_Riding && player.W_Set) // 와이어가 연결됬고, 날아가는 중이면?
	{
		glutPostRedisplay();
		glutTimerFunc(30, Follow_Wire_Timer, 1);
		return;
	}
	else
	{
		return;
	}

}

// 태양 색 관련
glm::vec3 change_sun_color(float sun_angle) {
	// Normalize sun_angle to the range [0, 360)
	if (HIT) {
		return glm::vec3(1, 0, 0);
	}
	else {
		sun_angle = fmod(sun_angle, 360.0f);

		glm::vec3 color;

		if (270.0f <= sun_angle && sun_angle < 330.0f) {
			// Sky color (e.g., blue)
			color = glm::vec3(0.53f, 0.81f, 0.98f); // Sky blue
		}
		else if (sun_angle >= 330.0f || sun_angle < 30.0f) {
			// White color
			color = glm::vec3(0.6f, 0.6f, 0.7f); // White
		}
		else if (sun_angle >= 30.0f && sun_angle < 90.0f) {
			// Orange color
			color = glm::vec3(1.0f, 0.647f, 0.0f); // Orange
		}
		else {
			// Handle other cases as needed
			// You might want to set a default color here
			color = glm::vec3(0.3f, 0.3f, 0.6f); // Default to white
		}
		return color;
	}

}


// 돌 관련
void Make_stone_route(Stone& S, glm::vec3 target)
{
	S.T = 0; //1000개
	S.action = true;
	S.speed = 20;
	S.route.clear();

	S.start_pos = animal_giant.pos;
	S.start_pos.y += 20;
	S.end_pos = target;


	Point3 start_pos = { S.start_pos.x, S.start_pos.y, S.start_pos.z };
	Point3 end_pos = { S.end_pos.x, S.end_pos.y, S.end_pos.z };

	Point3 middle_pos;
	middle_pos.x = start_pos.x + end_pos.x;
	middle_pos.z = start_pos.z + end_pos.z;
	middle_pos.y = start_pos.y + end_pos.y;


	float create_point_num = 0.001; // 1000개

	for (float i = 0; i <= 1.0; i += create_point_num)
	{
		Point3 temp = computeBezierPoint(i, start_pos, middle_pos, end_pos);
		S.route.push_back(temp);
	}
}


bool Stone_update(Stone& S)
{
	if (S.T <= 200)
	{
		S.speed = 20;
	}
	else if (S.T <= 600)
	{
		S.speed -= 1;
	}
	else
	{
		S.speed += 1;
	}
	S.speed = std::max(S.speed, 10);
	S.speed = std::min(S.speed, 20);


	S.T += S.speed;

	S.T = std::min(S.T, 999);
	S.rad += 2;

	S.pos = { S.route[S.T].x, S.route[S.T].y, S.route[S.T].z };
	//std::cout << "N:" << S.T << " - " << "s.x: " << S.route[S.T].x << ", " << "s.y: " << S.route[S.T].y << ", " << "s.z: " << S.route[S.T].z << std::endl;

	if (S.T == 999)
	{
		S.action = false;
		return false;
	}
	return true;
}


void Stone_timer(int value)
{
	bool result = Stone_update(stone);

	check_stone_collision();
	if (result) // 돌이 아직 끝까지 안간 경우
	{
		glutPostRedisplay();
		glutTimerFunc(30, Stone_timer, 1);
	}

}




const GLchar* vertexShaderSource = "#version 330 core \n"
"layout(location = 0) in vec3 vPos;\n"
"layout(location = 1) in vec3 vNormal;\n"
"layout(location = 2) in vec2 vTexCoord;\n"
"out vec2 TexCoord;\n"
"out vec3 FragPos;\n" //--- 객체의 위치값을 프래그먼트 세이더로 보낸다.
"out vec3 Normal;\n" //--- 노멀값을 프래그먼트 세이더로 보낸다.
"uniform mat4 model;\n" //--- 모델링 변환값
"uniform mat4 view;\n" //--- 뷰잉 변환값
"uniform mat4 projection;\n" //--- 투영 변환값
"void main()\n"
"{\n"
"gl_Position = projection * view * model * vec4(vPos, 1.0);\n"
"FragPos = vec3(model * vec4(vPos, 1.0));\n"
"Normal =  mat3(transpose(inverse(model))) *vNormal;\n"
"TexCoord = vTexCoord;\n"
"}\n";


const GLchar* fragmentShaderSource = "#version 330 core\n"
"in vec3 FragPos;\n" //--- 노멀값을 계산하기 위해 객체의 위치값을 버텍스 세이더에서 받아온다.
"in vec3 Normal;\n"
"in vec2 TexCoord;\n"
"out vec4 FragColor;\n"
"uniform vec3 lightPos;\n" //--- 조명의 위치
"uniform vec3 lightColor;\n" //--- 조명의 색
"uniform vec3 objectColor;\n"
"uniform vec3 viewPos;\n"
"uniform vec3 ambientLight;\n"
"uniform sampler2D texture1;\n"
"uniform float blend_size;\n"
"void main()\n"
"{\n"
"vec3 ambientLight = vec3(0.8);\n" //--- 주변 조명 세기
"vec3 ambient = ambientLight * lightColor;\n" //--- 주변 조명 값
"vec3 normalVector = normalize(Normal);\n"
"vec3 lightDir = normalize(lightPos - FragPos);\n" //--- 표면과 조명의 위치로 조명의 방향을 결정한다.
"float diffuseLight = max(dot(normalVector, lightDir), 0.0);\n" //--- N과 L의 내적 값으로 강도 조절: 음수 방지
"vec3 diffuse = diffuseLight * lightColor;\n" //--- 산란 반사 조명값: 산란반사값 * 조명색상값
"int shininess = 128;\n" //--- 광택 계수
"vec3 viewDir = normalize(viewPos - FragPos);\n" //--- 관찰자의 방향
"vec3 reflectDir = reflect(-lightDir, normalVector);\n" //--- 반사 방향: reflect 함수 - 입사 벡터의 반사 방향 계산
"float specularLight = max(dot(viewDir, reflectDir), 0.0);\n" //--- V와 R의 내적값으로 강도 조절: 음수 방지
"specularLight = pow(specularLight, shininess);\n" //--- shininess 승을 해주어 하이라이트를 만들어준다.
"vec3 specular = specularLight * lightColor;\n" //--- 거울 반사 조명값: 거울반사값 * 조명색상값
"vec3 result = (ambient + diffuse + specular) * objectColor;\n" //--- 최종 조명 설정된 픽셀 색상: (주변+산란반사+거울반사조명)*객체 색상
"FragColor = vec4 (result, blend_size);\n"
"FragColor = texture(texture1, TexCoord) * FragColor;\n"
"}\0";


GLuint VAO, VBO;
GLuint Spere_VAO;
GLuint Spere_VBO_position;
GLuint Spere_VBO_normal;
GLuint Spere_VBO_color;

GLuint Stone_VAO;
GLuint Stone_VBO_position;
GLuint Stone_VBO_normal;
GLuint Stone_VBO_color;

GLuint Titan_VAO;
GLuint Titan_VBO_position;
GLuint Titan_VBO_normal;
GLuint Titan_VBO_color;


GLuint B_Titan_VAO;
GLuint B_Titan_VBO_position;
GLuint B_Titan_VBO_normal;
GLuint B_Titan_VBO_color;


std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
std::vector< glm::vec3 > temp_vertices;
std::vector< glm::vec2 > temp_uvs;
std::vector< glm::vec3 > temp_normals;
std::vector< glm::vec3 > outvertex, outnormal;
std::vector< glm::vec2 > outuv;

float sumX = 0.0, sumY = 0.0, sumZ = 0.0;
float aveX, aveY, aveZ;
float scaleX, scaleY, scaleZ;
float minX = 0.0, minY = 0.0, minZ = 0.0;
float maxX = 0.0, maxY = 0.0, maxZ = 0.0;

int loadObj_normalize_center(const char* filename)
{
	FILE* objFile;

	fopen_s(&objFile, filename, "rb");

	if (objFile == NULL) {
		printf("Impossible to open the file !\n");
		return false;
	}
	while (1) {

		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(objFile, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.
		if (strcmp(lineHeader, "v") == 0) {
			glm::vec3 vertex;
			fscanf(objFile, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);

			if (vertex.x < minX) minX = vertex.x;
			if (vertex.y < minY) minY = vertex.y;
			if (vertex.z < minZ) minZ = vertex.z;
			if (vertex.x > maxX) maxX = vertex.x;
			if (vertex.y > maxY) maxY = vertex.y;
			if (vertex.z > maxZ) maxZ = vertex.z;
			sumX += vertex.x;
			sumY += vertex.y;
			sumZ += vertex.z;

			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			glm::vec2 uv;
			fscanf(objFile, "%f %f\n", &uv.x, &uv.y);
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec3 normal;
			fscanf(objFile, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(objFile, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				printf("File can't be read by our simple parser : ( Try exporting with other options\n");
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
	}

	std::cout << "minX: " << minX << " minY: " << minY << " minZ: " << minZ << std::endl;
	std::cout << "maxX: " << maxX << " maxY: " << maxY << " maxZ: " << maxZ << std::endl;

	aveX = sumX / vertexIndices.size();
	aveY = sumY / vertexIndices.size();
	aveZ = sumZ / vertexIndices.size();
	scaleX = maxX - minX;
	scaleY = maxY - minY;
	scaleZ = maxZ - minZ;

	glm::vec3 temp;

	std::cout << "aveX: " << aveX << " aveY: " << aveY << " aveZ: " << aveZ << std::endl;

	std::cout << "scaleX: " << scaleX << " scaleY: " << scaleY << " scaleZ: " << scaleZ << std::endl;

	for (unsigned int i = 0; i < vertexIndices.size(); i++) {
		unsigned int vertexIndex = vertexIndices[i];
		temp = temp_vertices[vertexIndex - 1];
		temp.x = temp.x - minX;
		temp.y = temp.y - minY;
		temp.z = temp.z - minZ;

		temp.x = ((temp.x * 2.0f) / scaleX) - 1.0f;
		temp.y = ((temp.y * 2.0f) / scaleY) - 1.0f;
		temp.z = ((temp.z * 2.0f) / scaleZ) - 1.0f;

		outvertex.push_back(temp);
		//glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		//outvertex.push_back(vertex);
	}
	for (unsigned int i = 0; i < uvIndices.size(); i++) {
		unsigned int uvIndex = uvIndices[i];
		glm::vec2 vertex = temp_uvs[uvIndex - 1];
		outuv.push_back(vertex);
	}
	for (unsigned int i = 0; i < normalIndices.size(); i++) {
		unsigned int normalIndex = normalIndices[i];
		glm::vec3 vertex = temp_normals[normalIndex - 1];
		outnormal.push_back(vertex);
	}

	return outvertex.size();
}

std::vector< unsigned int > T_vertexIndices, T_uvIndices, T_normalIndices;
std::vector< glm::vec3 > T_temp_vertices;
std::vector< glm::vec2 > T_temp_uvs;
std::vector< glm::vec3 > T_temp_normals;
std::vector< glm::vec3 > T_outvertex, T_outnormal;
std::vector< glm::vec2 > T_outuv;

int T_loadObj_normalize_center(const char* filename)
{
	float sumX = 0.0, sumY = 0.0, sumZ = 0.0;
	float aveX, aveY, aveZ;
	float scaleX, scaleY, scaleZ;
	float minX = 0.0, minY = 0.0, minZ = 0.0;
	float maxX = 0.0, maxY = 0.0, maxZ = 0.0;

	FILE* objFile;

	fopen_s(&objFile, filename, "rb");

	if (objFile == NULL) {
		printf("Impossible to open the file !\n");
		return false;
	}
	while (1) {

		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(objFile, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.
		if (strcmp(lineHeader, "v") == 0) {
			glm::vec3 vertex;
			fscanf(objFile, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);

			if (vertex.x < minX) minX = vertex.x;
			if (vertex.y < minY) minY = vertex.y;
			if (vertex.z < minZ) minZ = vertex.z;
			if (vertex.x > maxX) maxX = vertex.x;
			if (vertex.y > maxY) maxY = vertex.y;
			if (vertex.z > maxZ) maxZ = vertex.z;
			sumX += vertex.x;
			sumY += vertex.y;
			sumZ += vertex.z;

			T_temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			glm::vec2 uv;
			fscanf(objFile, "%f %f\n", &uv.x, &uv.y);
			T_temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec3 normal;
			fscanf(objFile, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			T_temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(objFile, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				printf("File can't be read by our simple parser : ( Try exporting with other options\n");
				return false;
			}
			T_vertexIndices.push_back(vertexIndex[0]);
			T_vertexIndices.push_back(vertexIndex[1]);
			T_vertexIndices.push_back(vertexIndex[2]);
			T_uvIndices.push_back(uvIndex[0]);
			T_uvIndices.push_back(uvIndex[1]);
			T_uvIndices.push_back(uvIndex[2]);
			T_normalIndices.push_back(normalIndex[0]);
			T_normalIndices.push_back(normalIndex[1]);
			T_normalIndices.push_back(normalIndex[2]);
		}
	}

	std::cout << "minX: " << minX << " minY: " << minY << " minZ: " << minZ << std::endl;
	std::cout << "maxX: " << maxX << " maxY: " << maxY << " maxZ: " << maxZ << std::endl;

	aveX = sumX / T_vertexIndices.size();
	aveY = sumY / T_vertexIndices.size();
	aveZ = sumZ / T_vertexIndices.size();
	scaleX = maxX - minX;
	scaleY = maxY - minY;
	scaleZ = maxZ - minZ;

	glm::vec3 temp;

	std::cout << "aveX: " << aveX << " aveY: " << aveY << " aveZ: " << aveZ << std::endl;

	std::cout << "scaleX: " << scaleX << " scaleY: " << scaleY << " scaleZ: " << scaleZ << std::endl;

	for (unsigned int i = 0; i < T_vertexIndices.size(); i++) {
		unsigned int vertexIndex = T_vertexIndices[i];
		temp = T_temp_vertices[vertexIndex - 1];
		temp.x = temp.x - minX;
		temp.y = temp.y - minY;
		temp.z = temp.z - minZ;

		temp.x = ((temp.x * 2.0f) / scaleX) - 1.0f;
		temp.y = ((temp.y * 2.0f) / scaleY) - 1.0f;
		temp.z = ((temp.z * 2.0f) / scaleZ) - 1.0f;

		T_outvertex.push_back(temp);
		//glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		//outvertex.push_back(vertex);
	}
	for (unsigned int i = 0; i < T_uvIndices.size(); i++) {
		unsigned int uvIndex = T_uvIndices[i];
		glm::vec2 vertex = T_temp_uvs[uvIndex - 1];
		T_outuv.push_back(vertex);
	}
	for (unsigned int i = 0; i < T_normalIndices.size(); i++) {
		unsigned int normalIndex = T_normalIndices[i];
		glm::vec3 vertex = T_temp_normals[normalIndex - 1];
		T_outnormal.push_back(vertex);
	}

	return T_outvertex.size();
}

std::vector< unsigned int > B_vertexIndices, B_uvIndices, B_normalIndices;
std::vector< glm::vec3 > B_temp_vertices;
std::vector< glm::vec2 > B_temp_uvs;
std::vector< glm::vec3 > B_temp_normals;
std::vector< glm::vec3 > B_outvertex, B_outnormal;
std::vector< glm::vec2 > B_outuv;

int B_loadObj_normalize_center(const char* filename)
{
	float sumX = 0.0, sumY = 0.0, sumZ = 0.0;
	float aveX, aveY, aveZ;
	float scaleX, scaleY, scaleZ;
	float minX = 0.0, minY = 0.0, minZ = 0.0;
	float maxX = 0.0, maxY = 0.0, maxZ = 0.0;

	FILE* objFile;

	fopen_s(&objFile, filename, "rb");

	if (objFile == NULL) {
		printf("Impossible to open the file !\n");
		return false;
	}
	while (1) {

		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(objFile, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.
		if (strcmp(lineHeader, "v") == 0) {
			glm::vec3 vertex;
			fscanf(objFile, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);

			if (vertex.x < minX) minX = vertex.x;
			if (vertex.y < minY) minY = vertex.y;
			if (vertex.z < minZ) minZ = vertex.z;
			if (vertex.x > maxX) maxX = vertex.x;
			if (vertex.y > maxY) maxY = vertex.y;
			if (vertex.z > maxZ) maxZ = vertex.z;
			sumX += vertex.x;
			sumY += vertex.y;
			sumZ += vertex.z;

			B_temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			glm::vec2 uv;
			fscanf(objFile, "%f %f\n", &uv.x, &uv.y);
			B_temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec3 normal;
			fscanf(objFile, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			B_temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(objFile, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				printf("File can't be read by our simple parser : ( Try exporting with other options\n");
				return false;
			}
			B_vertexIndices.push_back(vertexIndex[0]);
			B_vertexIndices.push_back(vertexIndex[1]);
			B_vertexIndices.push_back(vertexIndex[2]);
			B_uvIndices.push_back(uvIndex[0]);
			B_uvIndices.push_back(uvIndex[1]);
			B_uvIndices.push_back(uvIndex[2]);
			B_normalIndices.push_back(normalIndex[0]);
			B_normalIndices.push_back(normalIndex[1]);
			B_normalIndices.push_back(normalIndex[2]);
		}
	}

	std::cout << "minX: " << minX << " minY: " << minY << " minZ: " << minZ << std::endl;
	std::cout << "maxX: " << maxX << " maxY: " << maxY << " maxZ: " << maxZ << std::endl;

	aveX = sumX / B_vertexIndices.size();
	aveY = sumY / B_vertexIndices.size();
	aveZ = sumZ / B_vertexIndices.size();
	scaleX = maxX - minX;
	scaleY = maxY - minY;
	scaleZ = maxZ - minZ;

	glm::vec3 temp;

	std::cout << "aveX: " << aveX << " aveY: " << aveY << " aveZ: " << aveZ << std::endl;

	std::cout << "scaleX: " << scaleX << " scaleY: " << scaleY << " scaleZ: " << scaleZ << std::endl;

	for (unsigned int i = 0; i < B_vertexIndices.size(); i++) {
		unsigned int vertexIndex = B_vertexIndices[i];
		temp = B_temp_vertices[vertexIndex - 1];
		temp.x = temp.x - minX;
		temp.y = temp.y - minY;
		temp.z = temp.z - minZ;

		temp.x = ((temp.x * 2.0f) / scaleX) - 1.0f;
		temp.y = ((temp.y * 2.0f) / scaleY) - 1.0f;
		temp.z = ((temp.z * 2.0f) / scaleZ) - 1.0f;

		B_outvertex.push_back(temp);
		//glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		//outvertex.push_back(vertex);
	}
	for (unsigned int i = 0; i < B_uvIndices.size(); i++) {
		unsigned int uvIndex = B_uvIndices[i];
		glm::vec2 vertex = B_temp_uvs[uvIndex - 1];
		B_outuv.push_back(vertex);
	}
	for (unsigned int i = 0; i < B_normalIndices.size(); i++) {
		unsigned int normalIndex = B_normalIndices[i];
		glm::vec3 vertex = B_temp_normals[normalIndex - 1];
		B_outnormal.push_back(vertex);
	}

	return B_outvertex.size();
}
std::vector< unsigned int > S_vertexIndices, S_uvIndices, S_normalIndices;
std::vector< glm::vec3 > S_temp_vertices;
std::vector< glm::vec2 > S_temp_uvs;
std::vector< glm::vec3 > S_temp_normals;
std::vector< glm::vec3 > S_outvertex, S_outnormal;
std::vector< glm::vec2 > S_outuv;

int S_loadObj_normalize_center(const char* filename)
{
	float sumX = 0.0, sumY = 0.0, sumZ = 0.0;
	float aveX, aveY, aveZ;
	float scaleX, scaleY, scaleZ;
	float minX = 0.0, minY = 0.0, minZ = 0.0;
	float maxX = 0.0, maxY = 0.0, maxZ = 0.0;

	FILE* objFile;

	fopen_s(&objFile, filename, "rb");

	if (objFile == NULL) {
		printf("Impossible to open the file !\n");
		return false;
	}
	while (1) {

		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(objFile, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.
		if (strcmp(lineHeader, "v") == 0) {
			glm::vec3 vertex;
			fscanf(objFile, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);

			if (vertex.x < minX) minX = vertex.x;
			if (vertex.y < minY) minY = vertex.y;
			if (vertex.z < minZ) minZ = vertex.z;
			if (vertex.x > maxX) maxX = vertex.x;
			if (vertex.y > maxY) maxY = vertex.y;
			if (vertex.z > maxZ) maxZ = vertex.z;
			sumX += vertex.x;
			sumY += vertex.y;
			sumZ += vertex.z;

			S_temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			glm::vec2 uv;
			fscanf(objFile, "%f %f\n", &uv.x, &uv.y);
			S_temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec3 normal;
			fscanf(objFile, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			S_temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(objFile, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				printf("File can't be read by our simple parser : ( Try exporting with other options\n");
				return false;
			}
			S_vertexIndices.push_back(vertexIndex[0]);
			S_vertexIndices.push_back(vertexIndex[1]);
			S_vertexIndices.push_back(vertexIndex[2]);
			S_uvIndices.push_back(uvIndex[0]);
			S_uvIndices.push_back(uvIndex[1]);
			S_uvIndices.push_back(uvIndex[2]);
			S_normalIndices.push_back(normalIndex[0]);
			S_normalIndices.push_back(normalIndex[1]);
			S_normalIndices.push_back(normalIndex[2]);
		}
	}

	std::cout << "minX: " << minX << " minY: " << minY << " minZ: " << minZ << std::endl;
	std::cout << "maxX: " << maxX << " maxY: " << maxY << " maxZ: " << maxZ << std::endl;

	aveX = sumX / S_vertexIndices.size();
	aveY = sumY / S_vertexIndices.size();
	aveZ = sumZ / S_vertexIndices.size();
	scaleX = maxX - minX;
	scaleY = maxY - minY;
	scaleZ = maxZ - minZ;

	glm::vec3 temp;

	std::cout << "aveX: " << aveX << " aveY: " << aveY << " aveZ: " << aveZ << std::endl;

	std::cout << "scaleX: " << scaleX << " scaleY: " << scaleY << " scaleZ: " << scaleZ << std::endl;

	for (unsigned int i = 0; i < S_vertexIndices.size(); i++) {
		unsigned int vertexIndex = S_vertexIndices[i];
		temp = S_temp_vertices[vertexIndex - 1];
		temp.x = temp.x - minX;
		temp.y = temp.y - minY;
		temp.z = temp.z - minZ;

		temp.x = ((temp.x * 2.0f) / scaleX) - 1.0f;
		temp.y = ((temp.y * 2.0f) / scaleY) - 1.0f;
		temp.z = ((temp.z * 2.0f) / scaleZ) - 1.0f;

		S_outvertex.push_back(temp);
		//glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		//outvertex.push_back(vertex);
	}
	for (unsigned int i = 0; i < S_uvIndices.size(); i++) {
		unsigned int uvIndex = S_uvIndices[i];
		glm::vec2 vertex = S_temp_uvs[uvIndex - 1];
		S_outuv.push_back(vertex);
	}
	for (unsigned int i = 0; i < S_normalIndices.size(); i++) {
		unsigned int normalIndex = S_normalIndices[i];
		glm::vec3 vertex = S_temp_normals[normalIndex - 1];
		S_outnormal.push_back(vertex);
	}

	return S_outvertex.size();
}
void make_vertexShaders(GLuint& vertexShader)
{
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		std::cerr << "ERROR : vertex shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}
}
void make_fragmentShaders(GLuint& fragmentShader)
{
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		std::cerr << "ERROR : fragment shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}
}

GLuint make_shaderProgram(GLuint& vertexShader, GLuint& fragmentShader)
{
	GLuint ShaderProgramID;

	ShaderProgramID = glCreateProgram();
	glAttachShader(ShaderProgramID, vertexShader);
	glAttachShader(ShaderProgramID, fragmentShader);
	glLinkProgram(ShaderProgramID);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	GLint result;
	GLchar errorLog[512];

	glGetProgramiv(ShaderProgramID, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(ShaderProgramID, 512, NULL, errorLog);
		std::cerr << "ERROR: shader program 연결 실패\n" << errorLog << std::endl;
		exit(-1);
	}

	glUseProgram(ShaderProgramID);

	return ShaderProgramID;
}

void Init()
{
	loadObj_normalize_center("sphere.obj");

	//// 5.1. VAO 객체 생성 및 바인딩
	glGenVertexArrays(1, &Spere_VAO);
	glGenBuffers(1, &Spere_VBO_position);
	glGenBuffers(1, &Spere_VBO_normal);
	glGenBuffers(1, &Spere_VBO_color);

	// 2 triangles for quad floor
	glBindVertexArray(Spere_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, Spere_VBO_position);
	glBufferData(GL_ARRAY_BUFFER, outvertex.size() * sizeof(glm::vec3), &outvertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, Spere_VBO_normal);
	glBufferData(GL_ARRAY_BUFFER, outnormal.size() * sizeof(glm::vec3), &outnormal[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(1);

	//============================초대형 거인=======================
	T_loadObj_normalize_center("ColossalTitan.obj");

	//// 5.1. VAO 객체 생성 및 바인딩
	glGenVertexArrays(1, &Titan_VAO);
	glGenBuffers(1, &Titan_VBO_position);
	glGenBuffers(1, &Titan_VBO_normal);
	glGenBuffers(1, &Titan_VBO_color);

	// 2 triangles for quad floor
	glBindVertexArray(Titan_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, Titan_VBO_position);
	glBufferData(GL_ARRAY_BUFFER, T_outvertex.size() * sizeof(glm::vec3), &T_outvertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, Titan_VBO_normal);
	glBufferData(GL_ARRAY_BUFFER, T_outnormal.size() * sizeof(glm::vec3), &T_outnormal[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(1);

	GLuint Titan_VBO_uv;
	glGenBuffers(1, &Titan_VBO_uv);
	glBindBuffer(GL_ARRAY_BUFFER, Titan_VBO_uv);
	glBufferData(GL_ARRAY_BUFFER, T_outuv.size() * sizeof(glm::vec2), &T_outuv[0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
	glEnableVertexAttribArray(2);

	//============================짐승 거인=======================
	B_loadObj_normalize_center("Beast_Titan.obj");

	//// 5.1. VAO 객체 생성 및 바인딩
	glGenVertexArrays(1, &B_Titan_VAO);
	glGenBuffers(1, &B_Titan_VBO_position);
	glGenBuffers(1, &B_Titan_VBO_normal);
	glGenBuffers(1, &B_Titan_VBO_color);

	// 2 triangles for quad floor
	glBindVertexArray(B_Titan_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, B_Titan_VBO_position);
	glBufferData(GL_ARRAY_BUFFER, B_outvertex.size() * sizeof(glm::vec3), &B_outvertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, B_Titan_VBO_normal);
	glBufferData(GL_ARRAY_BUFFER, B_outnormal.size() * sizeof(glm::vec3), &B_outnormal[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(1);

	GLuint B_Titan_VBO_uv;
	glGenBuffers(1, &B_Titan_VBO_uv);
	glBindBuffer(GL_ARRAY_BUFFER, B_Titan_VBO_uv);
	glBufferData(GL_ARRAY_BUFFER, B_outuv.size() * sizeof(glm::vec2), &B_outuv[0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
	glEnableVertexAttribArray(2);

	//============================큐브=============================

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); //--- 위치 속성
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); //--- 노말 속성
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); //--- 텍스처 좌표 속성
	glEnableVertexAttribArray(2);

	//============================바위======================
	S_loadObj_normalize_center("Stone.obj");

	//// 5.1. VAO 객체 생성 및 바인딩
	glGenVertexArrays(1, &Stone_VAO);
	glGenBuffers(1, &Stone_VBO_position);
	glGenBuffers(1, &Stone_VBO_normal);
	glGenBuffers(1, &Stone_VBO_color);

	// 2 triangles for quad floor
	glBindVertexArray(Stone_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, Stone_VBO_position);
	glBufferData(GL_ARRAY_BUFFER, S_outvertex.size() * sizeof(glm::vec3), &S_outvertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, Stone_VBO_normal);
	glBufferData(GL_ARRAY_BUFFER, S_outnormal.size() * sizeof(glm::vec3), &S_outnormal[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(1);

	GLuint Stone_VBO_uv;
	glGenBuffers(1, &Stone_VBO_uv);
	glBindBuffer(GL_ARRAY_BUFFER, Stone_VBO_uv);
	glBufferData(GL_ARRAY_BUFFER, S_outuv.size() * sizeof(glm::vec2), &S_outuv[0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
	glEnableVertexAttribArray(2);



	for (int i = 0; i < 11; i++)
	{
		plate[i][0] = 60 - (i * 5.0f);
		plate[i][1] = 12.5f;
		plate[i][2] = 57.5f;
	}


	for (int i = 11; i < Plate_Num; i++)
	{
		plate[i][0] = 57.5f;
		plate[i][1] = 30.0f;
		plate[i][2] = 60 - ((i - 10) * 5.0f);
	}


	for (int i = 0; i < 5; i++) // -60 // 60
	{
		void_plate[i][0] = -10 - rand() % 40;
		void_plate[i][1] = rand() % 5 + 15;
		void_plate[i][2] = 10 + rand() % 40;
	}
	for (int i = 5; i < 10; i++) // -60 // -60
	{
		void_plate[i][0] = -10 - rand() % 40;
		void_plate[i][1] = rand() % 5 + 15;
		void_plate[i][2] = -10 - rand() % 40;
	}
	for (int i = 10; i < 15; i++) // 60 // -60
	{
		void_plate[i][0] = 10 + rand() % 40;
		void_plate[i][1] = rand() % 5 + 15;
		void_plate[i][2] = -10 - rand() % 40;
	}


	//==========================미니 거인============================
	for (int i = 0; i < Mini_Giant_Num; i++)
	{
		Mini_giant[i].x = 5.0f; // x좌표
		Mini_giant[i].y = 0.0f;
		Mini_giant[i].z = 5.0f;
		Mini_giant[i].speed = 0.25f;
		Mini_giant[i].run_speed = 0.5f;
		Mini_giant[i].Move = false;
		Mini_giant[i].Run = false;

	}

	//===========================플레이어 초기 좌표=====================
	player.pos = glm::vec3(10.0f, 0.0f, 40.0f);
	player.jump = false;
	player.W_Set = false;
	player.W_Riding = false;
	player.flying = false;

	//================뇌창 정보============
	spear.hp = false;
	spear.shooting = false;

}

unsigned int texture[13];
int widthImage, heightImage, numberOfChannel;
void InitTexture()
{
	glGenTextures(13, texture);

	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("white.png", &widthImage, &heightImage, &numberOfChannel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	//glTexImage2D(GL_TEXTURE_2D, 0, 3, widthImage, heightImage, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	int tLocation = glGetUniformLocation(shaderID, "texture1");
	glUniform1i(tLocation, 0);
	stbi_image_free(data);


	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true);
	data = stbi_load("작은거인.png", &widthImage, &heightImage, &numberOfChannel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	tLocation = glGetUniformLocation(shaderID, "texture1");
	glUniform1i(tLocation, 0);
	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true);
	data = stbi_load("background_1.png", &widthImage, &heightImage, &numberOfChannel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	tLocation = glGetUniformLocation(shaderID, "texture1");
	glUniform1i(tLocation, 0);
	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true);
	data = stbi_load("background_sky.png", &widthImage, &heightImage, &numberOfChannel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	tLocation = glGetUniformLocation(shaderID, "texture1");
	glUniform1i(tLocation, 0);
	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, texture[4]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true);
	data = stbi_load("stone2.png", &widthImage, &heightImage, &numberOfChannel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, widthImage, heightImage, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	tLocation = glGetUniformLocation(shaderID, "texture1");
	glUniform1i(tLocation, 0);
	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, texture[5]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true);
	data = stbi_load("body.png", &widthImage, &heightImage, &numberOfChannel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	tLocation = glGetUniformLocation(shaderID, "texture1");
	glUniform1i(tLocation, 0);
	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, texture[6]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true);
	data = stbi_load("face.png", &widthImage, &heightImage, &numberOfChannel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	tLocation = glGetUniformLocation(shaderID, "texture1");
	glUniform1i(tLocation, 0);
	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, texture[7]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true);
	data = stbi_load("arm.png", &widthImage, &heightImage, &numberOfChannel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	tLocation = glGetUniformLocation(shaderID, "texture1");
	glUniform1i(tLocation, 0);
	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, texture[8]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true);
	data = stbi_load("Hair_BM.png", &widthImage, &heightImage, &numberOfChannel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	tLocation = glGetUniformLocation(shaderID, "texture1");
	glUniform1i(tLocation, 0);
	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, texture[9]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true);
	data = stbi_load("Skin1_BM.png", &widthImage, &heightImage, &numberOfChannel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	tLocation = glGetUniformLocation(shaderID, "texture1");
	glUniform1i(tLocation, 0);
	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, texture[10]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true);
	data = stbi_load("Skin2_BM.png", &widthImage, &heightImage, &numberOfChannel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	tLocation = glGetUniformLocation(shaderID, "texture1");
	glUniform1i(tLocation, 0);
	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, texture[11]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true);
	data = stbi_load("Skin3_BM.png", &widthImage, &heightImage, &numberOfChannel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	tLocation = glGetUniformLocation(shaderID, "texture1");
	glUniform1i(tLocation, 0);
	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, texture[12]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true);
	data = stbi_load("stone_tex.png", &widthImage, &heightImage, &numberOfChannel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	tLocation = glGetUniformLocation(shaderID, "texture1");
	glUniform1i(tLocation, 0);
	stbi_image_free(data);
}
void windowToNDC(int mouseX, int mouseY, float& ndcX, float& ndcY)
{
	// 윈도우 좌표를 NDC 좌표로 변환
	ndcX = (2.0f * mouseX) / windowWidth - 1.0f;
	ndcY = 1.0f - (2.0f * mouseY) / windowHeight;
}
void drawScene()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glUseProgram(shaderID);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	int lightPosLocation = glGetUniformLocation(shaderID, "lightPos");
	int objColorLocation = glGetUniformLocation(shaderID, "objectColor"); //--- object Color값 전달: (1.0, 0.5, 0.3)의 색
	int objviewLocation = glGetUniformLocation(shaderID, "viewPos");
	int b_size = glGetUniformLocation(shaderID, "blend_size");
	int lightColorLocation = glGetUniformLocation(shaderID, "lightColor");
	int ambientLightLocation = glGetUniformLocation(shaderID, "ambientLight");

	glUniform3f(ambientLightLocation, 1.0, 1.0, 1.0);
	//sun_color = change_sun_color(sun_angle);
	//glUniform3f(lightColorLocation, sun_color.x, sun_color.y, sun_color.z);
	//glUniform3f(lightPosLocation, light_pos.x, light_pos.y, light_pos.z);
	glUniform1f(b_size, blend_size);


	cameraPos = player.pos;
	cameraPos.y = player.pos.y + 0.5f;

	glBindTexture(GL_TEXTURE_2D, texture[0]);


	glm::mat4 view = glm::mat4(1.0f);

	glm::vec3 temp_camera_D;
	temp_camera_D.x = cameraDirection.x + player.x_shake;
	temp_camera_D.y = cameraDirection.y + player.y_shake;
	temp_camera_D.z = cameraDirection.z + player.z_shake;

	view = glm::lookAt(cameraPos, cameraPos + temp_camera_D, cameraUp);

	unsigned int viewLocation = glGetUniformLocation(shaderID, "view"); //--- 뷰잉 변환 설정
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);


	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), (float)600 / (float)600, 0.1f, 200.0f);
	unsigned int projectionLocation = glGetUniformLocation(shaderID, "projection");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);



	unsigned int Model_Transform = glGetUniformLocation(shaderID, "model"); // 안 움직이는 부분
	glm::mat4 transfrom = glm::mat4(1.0f);									//사용할 변환 행렬


	//============================배경===================================

	glBindTexture(GL_TEXTURE_2D, texture[3]);
	transfrom = glm::mat4(1.0f); // 큐브에 사용할 변환 행렬
	transfrom = glm::translate(transfrom, glm::vec3(0.0, 60.0f, 0.0));
	transfrom = glm::scale(transfrom, glm::vec3(120.0f, 120.0f, 120.0f));
	glUniformMatrix4fv(Model_Transform, 1, GL_FALSE, glm::value_ptr(transfrom));
	glUniform3f(objColorLocation, 1.0, 1.0, 1.0);
	glDrawArrays(GL_TRIANGLES, 30, 6);

	glBindTexture(GL_TEXTURE_2D, texture[2]);
	transfrom = glm::mat4(1.0f); // 큐브에 사용할 변환 행렬
	transfrom = glm::translate(transfrom, glm::vec3(0.0, 60.0f, 0.0));
	transfrom = glm::scale(transfrom, glm::vec3(120.0f, 120.0f, 120.0f));
	glUniformMatrix4fv(Model_Transform, 1, GL_FALSE, glm::value_ptr(transfrom));
	glUniform3f(objColorLocation, 1.0, 1.0, 1.0);
	glDrawArrays(GL_TRIANGLES, 18, 6);

	glBindTexture(GL_TEXTURE_2D, texture[2]);
	transfrom = glm::mat4(1.0f); // 큐브에 사용할 변환 행렬
	transfrom = glm::translate(transfrom, glm::vec3(0.0, 60.0f, 0.0));
	transfrom = glm::scale(transfrom, glm::vec3(120.0f, 120.0f, 120.0f));
	glUniformMatrix4fv(Model_Transform, 1, GL_FALSE, glm::value_ptr(transfrom));
	glUniform3f(objColorLocation, 1.0, 1.0, 1.0);
	glDrawArrays(GL_TRIANGLES, 12, 6);

	glBindTexture(GL_TEXTURE_2D, texture[2]);
	transfrom = glm::mat4(1.0f); // 큐브에 사용할 변환 행렬
	transfrom = glm::translate(transfrom, glm::vec3(0.0, 60.0f, 0.0));
	transfrom = glm::scale(transfrom, glm::vec3(120.0f, 120.0f, 120.0f));
	glUniformMatrix4fv(Model_Transform, 1, GL_FALSE, glm::value_ptr(transfrom));
	glUniform3f(objColorLocation, 1.0, 1.0, 1.0);
	glDrawArrays(GL_TRIANGLES, 6, 6);

	glBindTexture(GL_TEXTURE_2D, texture[2]);
	transfrom = glm::mat4(1.0f); // 큐브에 사용할 변환 행렬
	transfrom = glm::translate(transfrom, glm::vec3(0.0, 60.0f, 0.0));
	transfrom = glm::scale(transfrom, glm::vec3(120.0f, 120.0f, 120.0f));
	glUniformMatrix4fv(Model_Transform, 1, GL_FALSE, glm::value_ptr(transfrom));
	glUniform3f(objColorLocation, 1.0, 1.0, 1.0);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindTexture(GL_TEXTURE_2D, texture[4]);
	glUniform3f(objColorLocation, 1.0, 1.0, 1.0);
	for (int i = 0; i < 120; ++i) {
		for (int j = 0; j < 120; ++j) {
			transfrom = glm::mat4(1.0f); // 큐브에 사용할 변환 행렬
			transfrom = glm::translate(transfrom, glm::vec3(-60 + i + 0.5, 0.5f, -60 + j + 0.5));
			transfrom = glm::scale(transfrom, glm::vec3(1.0f, 1.0f, 1.0f));
			glUniformMatrix4fv(Model_Transform, 1, GL_FALSE, glm::value_ptr(transfrom));
			glDrawArrays(GL_TRIANGLES, 24, 6);
		}
	}

	//==========================태양===================================


	transfrom = glm::mat4(1.0f);
	transfrom = glm::translate(transfrom, glm::vec3(light_pos.x, light_pos.y, light_pos.z));
	transfrom = glm::scale(transfrom, glm::vec3(1, 1, 1));
	glUniformMatrix4fv(Model_Transform, 1, GL_FALSE, glm::value_ptr(transfrom));


	glBindVertexArray(Spere_VAO);
	glUniform3f(objColorLocation, 5.0, 4.5, 0.3);
	glDrawArrays(GL_TRIANGLES, 0, 960);


	//============================발판====================================

	transfrom = glm::mat4(1.0f); // 큐브에 사용할 변환 행렬
	glBindVertexArray(VAO);
	glUniform3f(objColorLocation, 0.3, 0.3, 0.3);
	for (int i = 0; i < Plate_Num; i++)
	{
		transfrom = glm::mat4(1.0f);
		transfrom = glm::translate(transfrom, glm::vec3(plate[i][0], plate[i][1], plate[i][2]));
		transfrom = glm::rotate(transfrom, glm::radians(0.0f), glm::vec3(0.0, 1.0, 0.0));
		transfrom = glm::scale(transfrom, glm::vec3(5.0f, 2.0f, 5.0f));
		glUniformMatrix4fv(Model_Transform, 1, GL_FALSE, glm::value_ptr(transfrom));
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	for (int i = 0; i < 15; i++)
	{
		transfrom = glm::mat4(1.0f);
		transfrom = glm::translate(transfrom, glm::vec3(void_plate[i][0], void_plate[i][1], void_plate[i][2]));
		transfrom = glm::rotate(transfrom, glm::radians(0.0f), glm::vec3(0.0, 1.0, 0.0));
		transfrom = glm::scale(transfrom, glm::vec3(5.0f, 2.0f, 5.0f));
		glUniformMatrix4fv(Model_Transform, 1, GL_FALSE, glm::value_ptr(transfrom));
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	//==========================플레이어 위치==============================

	transfrom = glm::mat4(1.0f);

	transfrom = glm::translate(transfrom, glm::vec3(player.pos.x, player.pos.y, player.pos.z));
	transfrom = glm::scale(transfrom, glm::vec3(0.1f, 0.1f, 0.1f));
	glUniformMatrix4fv(Model_Transform, 1, GL_FALSE, glm::value_ptr(transfrom));

	glBindVertexArray(Spere_VAO);
	glUniform3f(objColorLocation, 0.0, 0.0, 1.0);
	glDrawArrays(GL_TRIANGLES, 0, 960);

	//==========================큐브===================================

	//transfrom = glm::mat4(1.0f); // 큐브에 사용할 변환 행렬
	//glUniformMatrix4fv(Model_Transform, 1, GL_FALSE, glm::value_ptr(transfrom));

	//glBindVertexArray(VAO);
	//glUniform3f(objColorLocation, 1.0, 1.0, 0.0);
	//glDrawArrays(GL_TRIANGLES, 0, 36);

	//==========================초대형거인===================================
	glUniform3f(ambientLightLocation, 1.0, 1.0, 1.0);
	glBindVertexArray(Titan_VAO);

	transfrom = glm::mat4(1.0f); // 큐브에 사용할 변환 행렬
	if (big_giant.hp) {
		transfrom = glm::translate(transfrom, glm::vec3(big_giant.pos.x, big_giant.pos.y, big_giant.pos.z));
		transfrom = glm::rotate(transfrom, glm::radians(big_giant.rad), glm::vec3(0.0, 1.0, 0.0));
	}
	else {
		transfrom = glm::translate(transfrom, glm::vec3(big_giant.pos.x, big_giant.pos.y - 14, big_giant.pos.z));
		transfrom = glm::rotate(transfrom, glm::radians(big_giant.dead_rad), glm::vec3(1.0, 0.0, 0.0));
		transfrom = glm::translate(transfrom, glm::vec3(0, 14, 0));
	}

	transfrom = glm::scale(transfrom, glm::vec3(10.0f, 15.0f, 2.4f));
	glUniformMatrix4fv(Model_Transform, 1, GL_FALSE, glm::value_ptr(transfrom));


	glUniform3f(objColorLocation, 1.0, 1.0, 1.0);
	// body
	glBindTexture(GL_TEXTURE_2D, texture[5]);
	glDrawArrays(GL_TRIANGLES, 0, 58329);

	// arm
	glBindTexture(GL_TEXTURE_2D, texture[7]);
	glDrawArrays(GL_TRIANGLES, 58329, 100329);

	// face
	glBindTexture(GL_TEXTURE_2D, texture[6]);
	glDrawArrays(GL_TRIANGLES, 158658, 56956);



	//test
	/*glBindVertexArray(Spere_VAO);
	glUniform3f(objColorLocation, 0.0, 0.0, 1.0);

	transfrom = glm::mat4(1.0f);
	transfrom = glm::translate(transfrom, glm::vec3(big_giant.pos.x, big_giant.pos.y + 12.0f, big_giant.pos.z - 0.5f));
	transfrom = glm::scale(transfrom, glm::vec3(1, 1, 1));
	glUniformMatrix4fv(Model_Transform, 1, GL_FALSE, glm::value_ptr(transfrom));

	glDrawArrays(GL_TRIANGLES, 0, 960);*/

	//============================와이어===============================
	//glUniform3f(ambientLightLocation, 0.8, 0.8, 0.8);
	glBindVertexArray(Spere_VAO);
	glUniform3f(objColorLocation, 0.6, 0.3, 0.0);

	int length = wire.size();
	for (int i = 0; i < length; i++)
	{
		if (i % 2 == 0)
			glUniform3f(objColorLocation, 0.6, 0.3, 0.0);
		else
			glUniform3f(objColorLocation, 0.5, 0.1, 0.0);

		transfrom = glm::mat4(1.0f); //사용할 변환 행렬
		transfrom = glm::translate(transfrom, glm::vec3(wire[i].x, wire[i].y, wire[i].z));
		transfrom = glm::scale(transfrom, glm::vec3(0.01f, 0.01f, 0.01f));
		glUniformMatrix4fv(Model_Transform, 1, GL_FALSE, glm::value_ptr(transfrom));

		glDrawArrays(GL_TRIANGLES, 0, 960);
	}

	//glUniform3f(ambientLightLocation, 0.3, 0.3, 0.3);

	//============================짐승 거인===================================

	glUniform3f(ambientLightLocation, 1.0, 1.0, 1.0);
	glUniform3f(objColorLocation, 1, 1, 1);

	glBindVertexArray(B_Titan_VAO);
	transfrom = glm::mat4(1.0f);
	transfrom = glm::translate(transfrom, glm::vec3(animal_giant.pos.x, animal_giant.pos.y, animal_giant.pos.z));
	transfrom = glm::rotate(transfrom, glm::radians(-135.0f), glm::vec3(0.0, 1.0, 0.0));
	transfrom = glm::scale(transfrom, glm::vec3(10.0f, 10.0f, 2.0f));
	glUniformMatrix4fv(Model_Transform, 1, GL_FALSE, glm::value_ptr(transfrom));
	glBindTexture(GL_TEXTURE_2D, texture[11]);
	glDrawArrays(GL_TRIANGLES, 0, 5100);
	glBindTexture(GL_TEXTURE_2D, texture[10]);
	glDrawArrays(GL_TRIANGLES, 5100, 26100);
	glBindTexture(GL_TEXTURE_2D, texture[9]);
	glDrawArrays(GL_TRIANGLES, 31200, 12300);
	glBindTexture(GL_TEXTURE_2D, texture[8]);
	glDrawArrays(GL_TRIANGLES, 43500, 36600);
	glBindTexture(GL_TEXTURE_2D, texture[10]);
	glDrawArrays(GL_TRIANGLES, 80100, 36000);
	glBindTexture(GL_TEXTURE_2D, texture[11]);
	glDrawArrays(GL_TRIANGLES, 116100, 57999);

	//============돌================
	glUniform3f(ambientLightLocation, 1, 1, 1);
	glBindVertexArray(Spere_VAO);


	if (stone.action)
	{
		int length = stone.route.size();
		for (int i = 0; i < length; i++)
		{
			if (i % 2 == 0)
				glUniform3f(objColorLocation, 0.6, 0.3, 0.0);
			else
				glUniform3f(objColorLocation, 0.5, 0.1, 0.0);

			transfrom = glm::mat4(1.0f); //사용할 변환 행렬
			transfrom = glm::translate(transfrom, glm::vec3(stone.route[i].x, stone.route[i].y, stone.route[i].z));
			transfrom = glm::scale(transfrom, glm::vec3(0.1f, 0.1f, 0.1f));
			glUniformMatrix4fv(Model_Transform, 1, GL_FALSE, glm::value_ptr(transfrom));

			//glDrawArrays(GL_TRIANGLES, 0, 960);
		}
		glBindVertexArray(Stone_VAO);
		glBindTexture(GL_TEXTURE_2D, texture[12]);
		transfrom = glm::mat4(1.0f);
		transfrom = glm::translate(transfrom, glm::vec3(stone.pos.x, stone.pos.y, stone.pos.z));
		transfrom = glm::rotate(transfrom, glm::radians(stone.rad), glm::vec3(0.0, 1.0, 0.0));
		transfrom = glm::rotate(transfrom, glm::radians(stone.rad), glm::vec3(1.0, 0.0, 0.0));
		transfrom = glm::scale(transfrom, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(Model_Transform, 1, GL_FALSE, glm::value_ptr(transfrom));

		//glBindVertexArray(Spere_VAO);
		glUniform3f(objColorLocation, 0.9, 0.8, 0.8);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, S_outvertex.size());
		glDrawArrays(GL_TRIANGLE_FAN, 0, S_outvertex.size());
		glDrawArrays(GL_TRIANGLES, 0, S_outvertex.size());
	}

	glBindVertexArray(Stone_VAO);
	glBindTexture(GL_TEXTURE_2D, texture[12]);
	int stone_stack_num = Stone_Num - stone.Used_stone_num;
	if (stone_stack_num > 0)
	{
		for (int i = 0; i < stone_stack_num; i++)
		{
			transfrom = glm::mat4(1.0f);
			transfrom = glm::rotate(transfrom, glm::radians(-130.0f), glm::vec3(0.0, 1.0, 0.0));
			transfrom = glm::translate(transfrom, glm::vec3((i - 2) * 3.0f, 0.0f, 40.0f));
			transfrom = glm::rotate(transfrom, glm::radians(-145.0f), glm::vec3(0.0, 1.0, 0.0));
			transfrom = glm::scale(transfrom, glm::vec3(2.0f, 2.0f, 2.0f));
			glUniformMatrix4fv(Model_Transform, 1, GL_FALSE, glm::value_ptr(transfrom));
			glDrawArrays(GL_TRIANGLE_STRIP, 0, S_outvertex.size());
			glDrawArrays(GL_TRIANGLE_FAN, 0, S_outvertex.size());
			glDrawArrays(GL_TRIANGLES, 0, S_outvertex.size());
		}

	}


	//==========================뇌창===================================


	glUniform3f(objColorLocation, 1.0, 1.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glBindVertexArray(Spere_VAO);
	transfrom = glm::mat4(1.0f);

	if (spear.hp == false)
	{

		transfrom = glm::translate(transfrom, glm::vec3(1.0f, 0.0f, 0.0f));
		transfrom = glm::translate(transfrom, glm::vec3(spear.pos.x, spear.pos.y + spear.y_value, spear.pos.z));
		transfrom = glm::rotate(transfrom, glm::radians(spear.angle), glm::vec3(0.0f, 1.0f, 0.0f));
		transfrom = glm::rotate(transfrom, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		transfrom = glm::scale(transfrom, glm::vec3(0.1f, 0.1f, 2));
		transfrom = glm::translate(transfrom, glm::vec3(0, -0.5, 0));
		glUniformMatrix4fv(Model_Transform, 1, GL_FALSE, glm::value_ptr(transfrom));

	}
	else
	{

		float dotProduct;

		// 벡터 크기 계산
		float vMagnitude;
		float wMagnitude;

		// 각도 계산 (라디안)
		float angleRad;





		if (spear.shooting)
		{

			transfrom = glm::translate(transfrom, glm::vec3(spear.pos.x, spear.pos.y, spear.pos.z));

		}
		else
		{
			spear.result = glm::cross(glm::vec3(0, 0, 1), cameraDirection);
			dotProduct = glm::dot(glm::vec3(0, 0, 1), cameraDirection);

			// 벡터 크기 계산
			vMagnitude = glm::length(glm::vec3(0, 0, 1));
			wMagnitude = glm::length(cameraDirection);

			// 각도 계산 (라디안)
			angleRad = std::acos(dotProduct / (vMagnitude * wMagnitude));

			// 라디안을 각도로 변환 (선택사항)
			spear.angleDegrees = glm::degrees(angleRad);

			transfrom = glm::translate(transfrom, glm::vec3(player.pos.x, player.pos.y, player.pos.z));
			glm::vec3 result2 = glm::cross(glm::vec3(0, 1, 0), cameraDirection);
			glm::mat4 translateMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(result2.x / 3, result2.y / 3, result2.z / 3));

			transfrom *= translateMatrix;

		}
		transfrom = glm::rotate(transfrom, glm::radians(spear.angleDegrees), spear.result);
		transfrom = glm::scale(transfrom, glm::vec3(0.1f, 0.1f, 2));
		transfrom = glm::translate(transfrom, glm::vec3(0, -0.5, 0));

		glUniformMatrix4fv(Model_Transform, 1, GL_FALSE, glm::value_ptr(transfrom));
	}




	glDrawArrays(GL_TRIANGLES, 0, 960);

	//============================미니 거인===================================
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glUniform3f(ambientLightLocation, 1, 1, 1);
	glBindVertexArray(VAO);
	transfrom = glm::mat4(1.0f); // 큐브에 사용할 변환 행렬

	glUniform3f(objColorLocation, 1, 1, 1);




	for (int i = 0; i < (Mini_Giant_Num - Used_MG) / 2; i++)
	{
		for (int k = 0; k < 2; k++)
		{
			transfrom = glm::mat4(1.0f);
			transfrom = glm::rotate(transfrom, glm::radians(-145.0f), glm::vec3(0.0, 1.0, 0.0));
			transfrom = glm::translate(transfrom, glm::vec3((i - 5) * 1.0f, 2.6, k * 1.0f));
			transfrom = glm::rotate(transfrom, glm::radians(-145.0f), glm::vec3(0.0, 1.0, 0.0));
			transfrom = glm::scale(transfrom, glm::vec3(1.0f, 5.0f, 1.0f));
			glUniformMatrix4fv(Model_Transform, 1, GL_FALSE, glm::value_ptr(transfrom));
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
	}


	for (int i = 0; i < Mini_Giant_Num; i++)
	{
		if (Mini_giant[i].Move)
		{
			transfrom = glm::mat4(1.0f);
			transfrom = glm::translate(transfrom, glm::vec3(Mini_giant[i].x, 2.6, Mini_giant[i].z));
			transfrom = glm::rotate(transfrom, glm::radians(Mini_giant[i].angle), glm::vec3(0.0, 1.0, 0.0));
			transfrom = glm::scale(transfrom, glm::vec3(1.0f, 5.0f, 1.0f));
			glUniformMatrix4fv(Model_Transform, 1, GL_FALSE, glm::value_ptr(transfrom));
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}

	}

	//============================이펙트===============================
	if (T_Spin_mode) {
		glBindTexture(GL_TEXTURE_2D, texture[6]);
		blend_size = 0.3;
		glUniform1f(b_size, blend_size);
		glBindVertexArray(Spere_VAO);
		glUniform3f(objColorLocation, 1.0, 0.7, 0.7);
		for (int i = 10; i > 0; --i) {
			transfrom = glm::mat4(1.0f);
			transfrom = glm::translate(transfrom, glm::vec3(big_giant.pos.x, big_giant.pos.y, big_giant.pos.z));
			transfrom = glm::scale(transfrom, glm::vec3(effect_size[i] * 0.5, effect_size[i], effect_size[i] * 0.6));
			glUniformMatrix4fv(Model_Transform, 1, GL_FALSE, glm::value_ptr(transfrom));

			glDrawArrays(GL_TRIANGLES, 0, outvertex.size() * 1);
		}
		blend_size = 1.0;
		glUniform1f(b_size, blend_size);
		glDisable(GL_BLEND);
	}
	//============================폭발 이펙트==============================
	//glBindTexture(GL_TEXTURE_2D, texture[6]);

	//glBindVertexArray(Spere_VAO);
	//glUniform3f(objColorLocation, 1.0, 0.0, 0.0);

	//blend_size = 0.05f * i;
	//glUniform1f(b_size, blend_size);
	//transfrom = glm::mat4(1.0f);
	//transfrom = glm::translate(transfrom, glm::vec3(big_giant.pos.x, big_giant.pos.y + 12.0f, big_giant.pos.z - 0.5f));
	//transfrom = glm::scale(transfrom, glm::vec3(i * 0.5, i * 0.5, i * 0.5));
	//glUniformMatrix4fv(Model_Transform, 1, GL_FALSE, glm::value_ptr(transfrom));

	//glDrawArrays(GL_TRIANGLES, 0, outvertex.size() * 1);

	//blend_size = 1.0;
	//glUniform1f(b_size, blend_size);
	//glDisable(GL_BLEND);
	//
//============================화면 중앙===================================

	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glBindVertexArray(Spere_VAO);
	glUniform3f(objColorLocation, 1.0, 0.2, 0.2);

	glm::vec3 result = glm::cross(glm::vec3(0, 1, 0), cameraDirection);
	float dotProduct = glm::dot(glm::vec3(0, 1, 0), cameraDirection);

	// 벡터 크기 계산
	float vMagnitude = glm::length(glm::vec3(0, 1, 0));
	float wMagnitude = glm::length(cameraDirection);

	// 각도 계산 (라디안)
	float angleRad = std::acos(dotProduct / (vMagnitude * wMagnitude));

	// 라디안을 각도로 변환 (선택사항)
	float angleDegrees = glm::degrees(angleRad);
	transfrom = glm::mat4(1.0f);
	transfrom = glm::translate(transfrom, glm::vec3((player.pos.x + cameraDirection.x) * 1,
		(player.pos.y + cameraDirection.y + 0.5) * 1, (player.pos.z + cameraDirection.z) * 1));
	transfrom = glm::rotate(transfrom, glm::radians(angleDegrees), result);
	transfrom = glm::scale(transfrom, glm::vec3(0.005f, 0.005f, 0.005f));
	glUniformMatrix4fv(Model_Transform, 1, GL_FALSE, glm::value_ptr(transfrom));

	glDrawArrays(GL_TRIANGLES, 0, 960);

	//============================충돌 체크===================================

	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glBindVertexArray(VAO);
	glUniform3f(objColorLocation, 1.0, 0.5, 0.5);

	transfrom = glm::mat4(1.0f);
	transfrom = glm::translate(transfrom, glm::vec3(100, 100, 100));

	//std::cout << c_area.T_hand_R.x << " " << c_area.T_hand_R.y << " " << c_area.T_hand_R.z << '\n';

	transfrom = glm::scale(transfrom, glm::vec3(1, 1, 1));
	glUniformMatrix4fv(Model_Transform, 1, GL_FALSE, glm::value_ptr(transfrom));

	glDrawArrays(GL_TRIANGLES, 0, 36);


	glutSwapBuffers();

}


GLvoid Keyboard(unsigned char key, int x10, int y)
{
	switch (key)
	{
	case '1':
		player.x_shake = uid(dre);
		player.y_shake = 1;
		player.z_shake = uid(dre);
		glutTimerFunc(30, Hit_Timer, 1);


		break;

	case '3':
		if (Used_MG < Mini_Giant_Num)
		{
			Used_MG += 1;
			Mini_giant[Used_MG - 1].Move = true;
		}
		break;

	case 'w':
		P_front = true;
		break;

	case 's':
		P_back = true;
		break;

	case 'a':
		P_left = true;
		break;

	case 'd':
		P_right = true;
		break;

	case 'c':
		spear.pos = cameraPos;
		spear.hp = true;
		//printf("%f\n", player.pos.x);
		//printf("%f\n", player.pos.y);
		//printf("%f\n", player.pos.z);
		break;

	case 'x':
		spear.shooting = true;
		spear.direction = glm::normalize(cameraDirection);
		break;

	case 'h':

		if (T_Spin_mode)
			T_Spin_mode = false;
		else {
			T_Spin_mode = true;
			glutTimerFunc(30, T_Attack_Timer, 1);
		}
		break;
	case 'q':
		exit(0);
		break;


	case 32: //SPACE_BAR
	{
		if (player.W_Riding) // 이미 와이어 타는중이면
		{
			Follow_Wire(0); // 와이어 타는 중이었다면 내리기
		}
		else
		{
			player.jump_value = 3;
			player.jump = true;
		}
	}
	break;


	case 'e':
	{
		glm::vec3 aabbMin = glm::vec3(60.0f, 0.0f, -60.0f);
		glm::vec3 aabbMax = glm::vec3(60.0f, 120.0f, 60.0f);
		glm::vec3 rayOrigin = glm::vec3(cameraPos.x, cameraPos.y, cameraPos.z);
		glm::vec3 rayDir = glm::vec3(cameraDirection.x, cameraDirection.y, cameraDirection.z);
		bool check1 = intersectRayAabb(rayOrigin, rayDir, aabbMin, aabbMax);

		aabbMin = glm::vec3(60.0f, 120.0f, 60.0f);
		aabbMax = glm::vec3(-60.0f, 0.0f, 60.0f);
		rayOrigin = glm::vec3(cameraPos.x, cameraPos.y, cameraPos.z);
		rayDir = glm::vec3(cameraDirection.x, cameraDirection.y, cameraDirection.z);
		bool check2 = intersectRayAabb(rayOrigin, rayDir, aabbMin, aabbMax);

		if (not check1 and not check2)
		{
			break;
		}
		if (player.W_Riding) // 이미 와이어 타는중이면 막기
		{
			break;
		}

		glm::vec3 Meet_Point;
		player.W_Set = !player.W_Set;
		if (player.W_Set)
		{
			if (check1) {
				float distanceToTarget = 60.0 - cameraPos.x;
				float scaleFactor = distanceToTarget / cameraDirection.x;
				player.W_index = 0;
				Meet_Point = { 60.0, cameraPos.y + cameraDirection.y * scaleFactor, cameraPos.z + cameraDirection.z * scaleFactor };
				Set_Wire(player.pos, Meet_Point);
				glutTimerFunc(30, Wire_Timer, 1);
			}
			if (check2) {
				float distanceToTarget = 60.0 - cameraPos.z;
				float scaleFactor = distanceToTarget / cameraDirection.z;
				player.W_index = 0;
				Meet_Point = { cameraPos.x + cameraDirection.x * scaleFactor, cameraPos.y + cameraDirection.y * scaleFactor, 60.0 };
				Set_Wire(player.pos, Meet_Point);
				glutTimerFunc(30, Wire_Timer, 1);
			}

		}
		else
		{
			wire.clear();
		}
	}
	break;


	case 'f':
		if (player.W_Set && !player.W_Riding)
		{
			player.W_Riding = true;
			glutTimerFunc(30, Follow_Wire_Timer, 1);
		}
		break;


	default:
		break;

	}

	glutPostRedisplay();

}


GLvoid Up_Keyboard(unsigned char key, int x10, int y)
{
	switch (key)
	{
	case 'w':
		P_front = false;
		break;
	case 's':
		P_back = false;
		break;
	case 'a':
		P_left = false;
		break;
	case 'd':
		P_right = false;
		break;
	}
	glutPostRedisplay();
}


void specialKeys(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP:
		// 위쪽 방향키 처리
		glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(cameraRotationSpeed), glm::normalize(glm::cross(cameraDirection, cameraUp)));
		glm::vec4 rotatedDirection = rotationMatrix * glm::vec4(cameraDirection, 0.0f);
		cameraDirection = glm::vec3(rotatedDirection);
		break;
	case GLUT_KEY_DOWN:
		// 아래쪽 방향키 처리
	{
		glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(-cameraRotationSpeed), glm::normalize(glm::cross(cameraDirection, cameraUp)));
		glm::vec4 rotatedDirection = rotationMatrix * glm::vec4(cameraDirection, 0.0f);
		cameraDirection = glm::vec3(rotatedDirection);
	}
	break;
	case GLUT_KEY_LEFT:
		// 왼쪽 방향키 처리
	{
		glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(cameraRotationSpeed), cameraUp);
		glm::vec4 rotatedDirection = rotationMatrix * glm::vec4(cameraDirection, 0.0f);
		cameraDirection = glm::vec3(rotatedDirection);
	}
	break;
	case GLUT_KEY_RIGHT:
		// 오른쪽 방향키 처리
	{
		glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(-cameraRotationSpeed), cameraUp);
		glm::vec4 rotatedDirection = rotationMatrix * glm::vec4(cameraDirection, 0.0f);
		cameraDirection = glm::vec3(rotatedDirection);
	}
	break;
	}


	glutPostRedisplay();
}


bool Mouse_area_out(float mx, float my)
{

	if (mx > 0.05f || mx < -0.05f)
	{
		return true;
	}
	if (my > 0.05f || my < -0.05f)
	{
		return true;
	}
	return false;
}


void Mouse_Click(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		spear.shooting = true;
		spear.direction = glm::normalize(cameraDirection);
	}
	glutPostRedisplay();
}


void Mouse_Move(int wmx, int wmy)
{
	float nx, ny;
	windowToNDC(wmx, wmy, nx, ny);
	M.mx = nx; M.my = ny;

	float dx = abs(M.mx - M.last_mx);
	float dy = abs(M.my - M.last_my);
	if (M.last_dx != 0)  // 마우스가 화면 중앙 근처에 있는 경우 처리 x
	{
		if (M.mx > M.last_mx) // 마우스를 오른쪽으로 이동 //M.mx > M.last_mx
		{
			glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(-cameraRotationSpeed), cameraUp);
			glm::vec4 rotatedDirection = rotationMatrix * glm::vec4(cameraDirection, 0.0f);
			cameraDirection = glm::vec3(rotatedDirection);
		}
		else if (M.mx < M.last_mx)// 마우스를 왼쪽으로 이동
		{
			glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(cameraRotationSpeed), cameraUp);
			glm::vec4 rotatedDirection = rotationMatrix * glm::vec4(cameraDirection, 0.0f);
			cameraDirection = glm::vec3(rotatedDirection);
		}
	}
	if (M.last_dy != 0) //abs(dy - M.last_dy) > 0.05f
	{
		if (M.my > M.last_my) // 마우스를 위로 이동
		{
			if (cameraDirection.y < 0.7) {
				glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(cameraRotationSpeed), glm::normalize(glm::cross(cameraDirection, cameraUp)));
				glm::vec4 rotatedDirection = rotationMatrix * glm::vec4(cameraDirection, 0.0f);
				cameraDirection = glm::vec3(rotatedDirection);
			}
		}
		else if (M.my < M.last_my)// 마우스를 아래로 이동
		{
			if (cameraDirection.y > -0.7) {
				glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(-cameraRotationSpeed), glm::normalize(glm::cross(cameraDirection, cameraUp)));
				glm::vec4 rotatedDirection = rotationMatrix * glm::vec4(cameraDirection, 0.0f);
				//rotatedDirection.y = std::min(rotatedDirection.y, 0.1f);
				cameraDirection = glm::vec3(rotatedDirection);
			}
		}

	}


	M.last_mx = M.mx;
	M.last_my = M.my;

	M.last_dx = dx;
	M.last_dy = dy;

	if (Mouse_area_out(M.mx, M.my)) //마우스가 화면 중앙 영역에서 나갔을 경우에, dx를 0으로 초기화하고 화면 중앙으로 마우스 이동
	{
		//std::cout << "Mouse_Reset" << std::endl;
		M.last_dx = 0;
		M.last_dy = 0;
		glutWarpPointer(glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_HEIGHT) / 2);

	}
	glutPostRedisplay();
}
//void Mouse_Click(int)
float calculateDistance(const glm::vec3& p1, const glm::vec3& p2) {
	return glm::length(p2 - p1);
}

// 두 점이 일정 거리보다 가까운지 여부를 판별하는 함수
bool arePointsClose(const glm::vec3& p1, const glm::vec3& p2, float distanceThreshold) {
	float distance = calculateDistance(p1, p2);
	return distance <= distanceThreshold;
}

bool intersectRayAabb(const glm::vec3& rayOrigin, const glm::vec3& rayDir,
	const glm::vec3& aabbMin, const glm::vec3& aabbMax)
{
	// t 값의 초기화 (t: parameter along the ray)
	float tMin = (aabbMin.x - rayOrigin.x) / rayDir.x;
	float tMax = (aabbMax.x - rayOrigin.x) / rayDir.x;

	// tMin과 tMax의 순서를 조정
	if (tMin > tMax) {
		std::swap(tMin, tMax);
	}

	// y 방향에 대한 t 값 계산
	float tYMin = (aabbMin.y - rayOrigin.y) / rayDir.y;
	float tYMax = (aabbMax.y - rayOrigin.y) / rayDir.y;

	if (tYMin > tYMax) {
		std::swap(tYMin, tYMax);
	}

	// x 방향과 y 방향의 교차 여부 판단
	if ((tMin > tYMax) || (tYMin > tMax)) {
		return false;
	}

	// x 방향의 t 값 업데이트
	if (tYMin > tMin) {
		tMin = tYMin;
	}

	if (tYMax < tMax) {
		tMax = tYMax;
	}

	// z 방향에 대한 t 값 계산
	float tZMin = (aabbMin.z - rayOrigin.z) / rayDir.z;
	float tZMax = (aabbMax.z - rayOrigin.z) / rayDir.z;

	if (tZMin > tZMax) {
		std::swap(tZMin, tZMax);
	}

	// x, y, z 방향 모두에서의 교차 여부 판단
	if ((tMin > tZMax) || (tZMin > tMax)) {
		return false;
	}

	// z 방향의 t 값 업데이트
	if (tZMin > tMin) {
		tMin = tZMin;
	}

	// tMax는 ray와 AABB가 만나는 지점의 parameter 값이 됨
	// 따라서 교차 지점 계산
	//intersectionPoint = rayOrigin + tMax * rayDir;

	return true;
}

void check_island_collision()
{
	bool xCollision = false;
	bool yCollision = false;
	bool Collision = false;
	float y_height = 0.0f;
	for (int i = 0; i < Plate_Num; i++) //플레이트 크기: 가로 세로:-2.5 ~ 2.5, 높이: -1.0f ~1.0f 
	{
		xCollision = (-2.5f + plate[i][0] <= player.pos.x && player.pos.x <= plate[i][0] + 2.5f)
			&& (-2.5f + plate[i][2] <= player.pos.z && player.pos.z <= plate[i][2] + 2.5f);

		yCollision = (-2.0f + plate[i][1] <= player.pos.y && player.pos.y <= plate[i][1] + 2.0f);

		if (xCollision && yCollision)
		{
			Collision = true;
			y_height = plate[i][1];
			break;
		}
		else
		{
			xCollision = false;
			yCollision = false;
		}
	}

	if (Collision) {
		// 플레이어가 플레이트 위에 올라갔을 때의 처리
		if (player.jump_value < 0)// 이러면 위에서 내려오면서 밟는 상황
		{
			player.jump = false;
			player.flying = false;
			player.jump_value = 0;
			player.pos.y = y_height + 3.0f;
		}
		else if (player.jump_value > 0) //이러면 밑에서 올라오면서 부딪힌 상황
		{
			player.jump_value = 0;
			player.pos.y = y_height - 3.0f;
		}
	}


}
void Player_move()
{
	glm::vec3 localCameraDirection = glm::normalize(glm::vec3(cameraDirection.x, 0.0f, cameraDirection.z));

	if (P_front) {
		player.pos.x += cameraSpeed * localCameraDirection.x;
		player.pos.z += cameraSpeed * localCameraDirection.z;
	}
	if (P_back) {
		player.pos.x -= cameraSpeed * localCameraDirection.x;
		player.pos.z -= cameraSpeed * localCameraDirection.z;
	}
	if (P_right) {
		glm::vec3 rightVec = glm::normalize(glm::cross(localCameraDirection, glm::vec3(0.0f, 1.0f, 0.0f)));
		player.pos.x += cameraSpeed * rightVec.x;
		player.pos.z += cameraSpeed * rightVec.z;
	}
	if (P_left) {
		glm::vec3 leftVec = glm::normalize(glm::cross(localCameraDirection, glm::vec3(0.0f, 1.0f, 0.0f)));
		player.pos.x -= cameraSpeed * leftVec.x;
		player.pos.z -= cameraSpeed * leftVec.z;
	}


	if (player.jump || player.flying)
	{
		player.pos.y += player.jump_value;

		if (player.flying)
		{
			player.jump_value -= 0.1f;
		}
		else
		{
			player.jump_value -= 0.25f;
		}

		if (player.pos.y <= 0.0f)
		{
			player.pos.y = 0.0f;
			player.jump = false;
			player.flying = false;
		}
		check_island_collision();
	}

	player.pos.x = clamp(player.pos.x, 0.0f, 59.0f);
	player.pos.z = clamp(player.pos.z, 0.0f, 59.0f);



	//if (check_pillar_collision()) // 기둥과 부딪혔다면 이동 x
	//{
	//	if (P_front) {
	//		player.pos.x -= cameraSpeed * localCameraDirection.x;
	//		player.pos.z -= cameraSpeed * localCameraDirection.z;
	//	}
	//	if (P_back) {
	//		player.pos.x += cameraSpeed * localCameraDirection.x;
	//		player.pos.z += cameraSpeed * localCameraDirection.z;
	//	}
	//	if (P_right) {
	//		glm::vec3 rightVec = glm::normalize(glm::cross(localCameraDirection, glm::vec3(0.0f, 1.0f, 0.0f)));
	//		player.pos.x -= cameraSpeed * rightVec.x;
	//		player.pos.z -= cameraSpeed * rightVec.z;
	//	}
	//	if (P_left) {
	//		glm::vec3 leftVec = glm::normalize(glm::cross(localCameraDirection, glm::vec3(0.0f, 1.0f, 0.0f)));
	//		player.pos.x += cameraSpeed * leftVec.x;
	//		player.pos.z += cameraSpeed * leftVec.z;
	//	}

	//	return;
	//}

}
void Timer(int value)
{
	Player_move();


	effect_size[rear_effect] += 0.5;
	if (effect_size[rear_effect] >= 17.0) {
		rear_effect += 1;
		if (rear_effect == 10) {
			for (int i = 0; i < 10; ++i) {
				effect_size[i] = 0;
			}
			rear_effect = 0;
		}
	}


	if (player.pos.y >= 10.0f && (Throw_mode == false))
	{
		Throw_mode = true;
		glutTimerFunc(5000, Throw_Stone_Timer, 1);
	}

	glutPostRedisplay();
	glutTimerFunc(30, Timer, 1);
}
void Check_Run(Mini_Giant& m, const Player& player) {
	float dx = player.pos.x - m.x;
	float dz = player.pos.z - m.z;

	if (pow(dx, 2) + pow(dz, 2) <= pow(20.0f, 2)) {
		m.Run = true;
	}
}
void check_M_giant_collision() {

	float distanceThreshold = 0.5f;
	bool areClose{ false };
	for (int i = 0; i < Mini_Giant_Num; i++)
	{
		if (Mini_giant[i].Move)
		{
			glm::vec3 M_pos = glm::vec3(Mini_giant[i].x, 0.0f, Mini_giant[i].z);
			areClose = arePointsClose(M_pos, player.pos, distanceThreshold);
			if (areClose) {
				std::cout << "미니거인 충돌" << '\n';
				if (HIT != true)
				{
					player.x_shake = uid(dre);
					player.y_shake = 1;
					player.z_shake = uid(dre);
					glutTimerFunc(30, Hit_Timer, 1);
				}
			}
		}

	}
}
void Mini_Timer(int value)
{

	for (int i = 0; i < Mini_Giant_Num; i++)
	{
		if (Mini_giant[i].Move)
		{
			Check_Run(Mini_giant[i], player);
			if (Mini_giant[i].Run == false)
			{
				Mini_giant[i].dx = player.pos.x - Mini_giant[i].x;
				Mini_giant[i].dz = player.pos.z - Mini_giant[i].z;
				float magnitude = sqrt(Mini_giant[i].dx * Mini_giant[i].dx + Mini_giant[i].dz * Mini_giant[i].dz);

				// 벡터를 정규화
				if (magnitude != 0) {
					Mini_giant[i].dx /= magnitude;
					Mini_giant[i].dz /= magnitude;

				}
				Mini_giant[i].angle = atan2(Mini_giant[i].dz, Mini_giant[i].dx);
				Mini_giant[i].x += Mini_giant[i].dx * Mini_giant[i].speed;
				Mini_giant[i].z += Mini_giant[i].dz * Mini_giant[i].speed;
			}



			if (Mini_giant[i].Run)
			{
				Mini_giant[i].x += Mini_giant[i].dx * Mini_giant[i].run_speed;
				Mini_giant[i].z += Mini_giant[i].dz * Mini_giant[i].run_speed;
			}


		}
	}
	check_M_giant_collision();

	glutPostRedisplay();
	glutTimerFunc(30, Mini_Timer, 1);
}
void Sun_Timer(int value)
{


	//sun_angle = fmod(sun_angle - 1.0f + 360.0f, 360.0f);
	if (sun_angle >= 360.0f)
		sun_angle = 0.0f;
	else
		sun_angle += 0.5f;

	glm::mat4 transfrom = glm::mat4(1.0f);
	transfrom = glm::rotate(transfrom, glm::radians(0.5f), glm::vec3(0.0f, 0.0f, 1.0f));
	glm::vec3 temp_pos = transfrom * glm::vec4(light_pos.x, light_pos.y, light_pos.z, 0.0f);
	light_pos = temp_pos;
	int lightPosLocation = glGetUniformLocation(shaderID, "lightPos");
	glUniform3f(lightPosLocation, light_pos.x, light_pos.y, light_pos.z);


	sun_color = change_sun_color(sun_angle);
	int lightColorLocation = glGetUniformLocation(shaderID, "lightColor");
	glUniform3f(lightColorLocation, sun_color.x, sun_color.y, sun_color.z);


	glutPostRedisplay();
	glutTimerFunc(30, Sun_Timer, 1);
}
void Summon_Timer(int value)
{
	if (Used_MG < Mini_Giant_Num)
	{
		Used_MG += 1;
		Mini_giant[Used_MG - 1].Move = true;
		glutPostRedisplay();
		glutTimerFunc(5000, Summon_Timer, 1);
	}
}
void T_dead_Timer(int value)
{
	if (not big_giant.hp)
	{
		if (big_giant.dead_rad <= -90)
			big_giant.dead_rad = -90;
		else
			big_giant.dead_rad -= 1;
		glutPostRedisplay();
		glutTimerFunc(30, T_dead_Timer, 1);
	}
}
void check_spear_collision()
{
	float distanceThreshold = 2.5f;
	bool areClose = arePointsClose(c_area.T_face, spear.pos, distanceThreshold);

	if (areClose) {
		std::cout << "뇌창 맟춤" << '\n';
		if (big_giant.hp) {
			spear.hp = false;
			spear.shooting = false;
			spear.pos = { 55, 33, 14 };
			big_giant.hp = false;
			glutTimerFunc(30, T_dead_Timer, 1);
		}
	}
}

void Spear_Timer(int value)
{

	if (spear.hp && spear.shooting)
	{
		spear.pos += spear.direction;

		if (spear.pos.x <= -60.0f || spear.pos.x >= 60.0f ||
			spear.pos.y <= -60.0f || spear.pos.y >= 60.0f ||
			spear.pos.z <= -60.0f || spear.pos.z >= 60.0f)
		{
			spear.hp = false;
			spear.shooting = false;

			spear.pos = { 58, 33, 14 };
		}
		check_spear_collision();

	}
	else if (spear.hp)
	{
		float spear_angle_x = cameraDirection.x;
		float spear_angle_z = cameraDirection.z;
		float magnitude = sqrt(spear_angle_x * spear_angle_x + spear_angle_z * spear_angle_z);


		if (magnitude != 0) {
			spear_angle_x /= magnitude;
			spear_angle_z /= magnitude;
		}
		spear.angle = atan2(spear_angle_x, spear_angle_z);
		spear.angle = glm::degrees(spear.angle);
		//std::cout << spear.angle << std::endl;


		spear.pos.x = player.pos.x;
		spear.pos.y = player.pos.y;
		spear.pos.z = player.pos.z;
	}
	else
	{
		spear.angle += 1.0;

		spear.y_value += spear.dy;

		if (spear.y_value <= 0.0f || spear.y_value >= 2.0f)
			spear.dy *= -1;

		if (arePointsClose(player.pos, spear.pos, 1.0f))
		{
			spear.hp = true;
		}
	}


	glutPostRedisplay();
	glutTimerFunc(30, Spear_Timer, 1);
}
void check_stone_collision() {

	float distanceThreshold = 2.0f;
	bool areClose_R = arePointsClose(stone.pos, player.pos, distanceThreshold);

	if (areClose_R) {
		std::cout << "stone 충돌" << '\n';
		if (HIT != true)
		{
			player.x_shake = uid(dre);
			player.y_shake = 1;
			player.z_shake = uid(dre);
			glutTimerFunc(30, Hit_Timer, 1);
		}
	}

}
void Throw_Stone_Timer(int value)
{

	if (stone.action == false && (stone.Used_stone_num < Stone_Num))
	{
		stone.action = true;
		Make_stone_route(stone, player.pos);
		glutTimerFunc(30, Stone_timer, 1);
	}

	if (stone.Used_stone_num != Stone_Num)
	{
		stone.Used_stone_num += 1;
		glutPostRedisplay();
		glutTimerFunc(5000, Throw_Stone_Timer, 1);
	}
	else if (not T_Spin_mode)
	{
		T_Spin_mode = true;
		glutPostRedisplay();
		glutTimerFunc(30, T_Attack_Timer, 1);
	}


}

void check_T_giant_collision() {
	glm::mat4 transfrom = glm::mat4(1.0f);
	transfrom = glm::translate(transfrom, glm::vec3(big_giant.pos.x, big_giant.pos.y, big_giant.pos.z));
	transfrom = glm::rotate(transfrom, glm::radians(big_giant.rad), glm::vec3(0.0, 1.0, 0.0));
	transfrom = glm::translate(transfrom, glm::vec3(-9, 0, -0.5));
	glm::vec3 temp_pos = transfrom * glm::vec4(0, 0, 0, 1.0f);
	c_area.T_hand_R = temp_pos;
	float distanceThreshold = 1.5f;
	bool areClose_R = arePointsClose(c_area.T_hand_R, player.pos, distanceThreshold);

	if (areClose_R) {
		std::cout << "오른팔 충돌" << '\n';
		if (HIT != true)
		{
			player.x_shake = uid(dre);
			player.y_shake = 1;
			player.z_shake = uid(dre);
			glutTimerFunc(30, Hit_Timer, 1);
		}

	}

	transfrom = glm::mat4(1.0f);
	transfrom = glm::translate(transfrom, glm::vec3(big_giant.pos.x, big_giant.pos.y, big_giant.pos.z));
	transfrom = glm::rotate(transfrom, glm::radians(big_giant.rad), glm::vec3(0.0, 1.0, 0.0));
	transfrom = glm::translate(transfrom, glm::vec3(9, 0, -0.5));
	temp_pos = transfrom * glm::vec4(0, 0, 0, 1.0f);
	c_area.T_hand_L = temp_pos;
	distanceThreshold = 1.5f;
	bool areClose_L = arePointsClose(c_area.T_hand_L, player.pos, distanceThreshold);

	if (areClose_L) {
		std::cout << "왼팔 충돌" << '\n';
		if (HIT != true)
		{
			player.x_shake = uid(dre);
			player.y_shake = 1;
			player.z_shake = uid(dre);
			glutTimerFunc(30, Hit_Timer, 1);
		}
	}

}
void T_Attack_Timer(int value)
{
	if (T_Spin_mode)
	{

		big_giant.rad += 2;
		//big_giant.rad -= 2;
		check_T_giant_collision();
		if (big_giant.rad >= 360.0f)
		{
			big_giant.rad = 0;
			T_Spin_mode = false;
		}
		glutPostRedisplay();
		glutTimerFunc(30, T_Attack_Timer, 1);
	}



}


void Hit_Timer(int value)
{

	player.x_shake *= 0.9f;
	player.y_shake *= 0.9f;
	player.z_shake *= 0.9f;
	HIT = true;


	if (glm::abs(player.x_shake) < 0.01f && glm::abs(player.y_shake) < 0.01f && glm::abs(player.z_shake) < 0.01f)
	{
		HIT = false;
		return;
	}
	glutPostRedisplay();
	glutTimerFunc(30, Hit_Timer, 1);
}


int main(int argc, char** argv)
{
	std::srand(static_cast<unsigned int>(std::time(nullptr)));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(300, 100);
	glutInitWindowSize(windowWidth, windowHeight);
	glutCreateWindow("CG_T5");
	glutSetCursor(GLUT_CURSOR_NONE);
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";

	Init();
	InitTexture();
	make_vertexShaders(vertexShader);
	make_fragmentShaders(fragmentShader);
	shaderID = make_shaderProgram(vertexShader, fragmentShader);
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(Keyboard);
	glutKeyboardUpFunc(Up_Keyboard);
	glutSpecialFunc(specialKeys);
	glutTimerFunc(30, Timer, 1);
	glutPassiveMotionFunc(Mouse_Move);
	glutMouseFunc(Mouse_Click);

	glutTimerFunc(30, Mini_Timer, 1);
	glutTimerFunc(30, Sun_Timer, 1);
	glutTimerFunc(5000, Summon_Timer, 1);
	glutTimerFunc(30, Spear_Timer, 1);

	glutMainLoop();

}

