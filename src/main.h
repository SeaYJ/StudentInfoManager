#include <stdio.h>
#include <stdbool.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>


static const int NAME_MAX_LENGTH = 32;
static const int GENDER_MAX_LENGTH = 32;
static const int HOBBIES_MAX_LENGTH = 255;


typedef enum {	// 爱好定义
	READING = 1,
	TRAVELING = 2,
	COOKING = 4,
	PHOTOGRAPHY = 8,
	GARDENING = 16
} HOBBIESTYPE;


typedef struct {	// 爱好变量值对应的含义 MAP 定义
	HOBBIESTYPE type;
	const char* chineseName;
} HOBBYMAPPING;


HOBBYMAPPING HOBBY_MAPPINGS[] = {	// 爱好变量对应的含义
	{READING, "阅读"},
	{TRAVELING, "旅行"},
	{COOKING, "烹饪"},
	{PHOTOGRAPHY, "摄影"},
	{GARDENING, "园艺"}
};
static const int HOBBY_MAPPINGS_LEN = sizeof(HOBBY_MAPPINGS) / sizeof(HOBBY_MAPPINGS[0]);
static const int MAX_HOBBIES_LEN = 64;	// 表示所有爱好需要多大数组存储
const char* const parseHobbies(HOBBIESTYPE hobbies);

typedef struct {	// 学生信息
	unsigned long long student_id;
	const char* name;
	const char* gender;
	int height;
	int weight;
	HOBBIESTYPE hobbies;
} STUDENTINFO;
static STUDENTINFO* STUDENTS_INFO = NULL;	// 存放所有学生信息
static int STUDENTS_COUNT = 0;				// 已有学生个数
void expendStudentsInfo();					// 扩展函数（每次新增都需扩大一个位置）；注意新增大小始终以 STUDENTS_COUNT + 1 为标准
void saveDatabase();						// 保存新的学生数据
void loadDatabase();						// 导入已保存的学生数据



static const char* const SYS_TITLE[] = {
"   _____  ______   __  __    ____    _   __    ______   ____           _______  __   _____\n",
"  / ___/ /_  __/  / / / /   /  _/   / | / /   / ____/  / __ \\         / ___/\\ \\/ /  / ___/\n",
"  \\__ \\   / /    / / / /    / /    /  |/ /   / /_     / / / /         \\__ \\  \\  /   \\__ \\\n",
" ___/ /  / /    / /_/ /   _/ /    / /|  /   / __/    / /_/ /         ___/ /  / /   ___/ /\n",
"/____/  /_/     \\____/   /___/   /_/ |_/   /_/       \\____/         /____/  /_/   /____/\n"
};
static const int SYS_TITLE_LEN = sizeof(SYS_TITLE) / sizeof(SYS_TITLE[0]);
void printSysTitle();


static const char* const MAIN_MENU[] = {	// 主菜单
	"录入学生信息",
	"查看学生信息",
	"查找学生信息",
	"修改学生信息",
	"删除学生信息",
	"匹配爱好",
	"退出系统"
};
static const int MAIN_MENU_LEN = sizeof(MAIN_MENU) / sizeof(MAIN_MENU[0]);
void showMainMenu();


static const char* const ENTER_STUDENT_INFO_MENU[] = {	// “录入学生信息”菜单
	"学号",
	"姓名",
	"性别",
	"身高",
	"体重",
	"爱好"
};
static const int ENTER_STUDENT_INFO_MENU_LEN = sizeof(ENTER_STUDENT_INFO_MENU) / sizeof(ENTER_STUDENT_INFO_MENU[0]);
void showEnterStudentInfoMenu();


void showAllStudentInfo();
void showPartialStudentInfo(const STUDENTINFO* stus, const int count);	// 展示部分学生信息


static const char* const SEARCH_STUDENT_INFO_MENU[] = {
	"按姓名查找",
	"按学号查找",
	"按性别查找",
	"返回上一页"
};
static const int SEARCH_STUDENT_INFO_MENU_LEN = sizeof(SEARCH_STUDENT_INFO_MENU) / sizeof(SEARCH_STUDENT_INFO_MENU[0]);
void showSearchStudentInfoMenu();
enum SEARCH_TYPE {
	FROM_NAME = 0x0521,
	FROM_STUDENT_ID,
	FROM_GENDER
};	// 定义所搜类型
void searchStudentInfo(const enum SEARCH_TYPE stype, const STUDENTINFO** stus, int* stus_count);	// 按类型搜索


static const char* const MODIFY_STUDENT_INFO_MENU[] = {
	"请输入学号"
};
static const int MODIFY_STUDENT_INFO_MENU_LEN = sizeof(MODIFY_STUDENT_INFO_MENU) / sizeof(MODIFY_STUDENT_INFO_MENU[0]);
static const char* const MODIFY_STUDENT_INFO_SUB_MENU[] = {
	"姓名",
	"性别",
	"身高",
	"体重",
	"爱好",
	"返回"
};
static const int MODIFY_STUDENT_INFO_SUB_MENU_LEN = sizeof(MODIFY_STUDENT_INFO_SUB_MENU) / sizeof(MODIFY_STUDENT_INFO_SUB_MENU[0]);
void showModifyStudentInfoMenu();
void showModifyStudentInfoSubMenu(int index);	// 这个索引是指向待修改的那个学生在数据 STUDENTS_INFO 中的位置


static const char* const DELETE_STUDENT_INFO_MENU[] = {
	"请输入学号"
};
static const int DELETE_STUDENT_INFO_MENU_LEN = sizeof(DELETE_STUDENT_INFO_MENU) / sizeof(DELETE_STUDENT_INFO_MENU[0]);
void showDeleteStudentInfoMenu();
void deleteStudentInfo(int index);


static const char* const MATCH_HOBBIES_MENU[] = {
	"请选择爱好(仅限一个)"
};
static const int MATCH_HOBBIES_MENU_LEN = sizeof(MATCH_HOBBIES_MENU) / sizeof(MATCH_HOBBIES_MENU[0]);
void showMatchHobbiesMenu();
void matchHobbies(HOBBIESTYPE htype);