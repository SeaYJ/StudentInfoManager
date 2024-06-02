#include <stdio.h>
#include <stdbool.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>


static const int NAME_MAX_LENGTH = 32;
static const int GENDER_MAX_LENGTH = 32;
static const int HOBBIES_MAX_LENGTH = 255;


typedef enum {	// ���ö���
	READING = 1,
	TRAVELING = 2,
	COOKING = 4,
	PHOTOGRAPHY = 8,
	GARDENING = 16
} HOBBIESTYPE;


typedef struct {	// ���ñ���ֵ��Ӧ�ĺ��� MAP ����
	HOBBIESTYPE type;
	const char* chineseName;
} HOBBYMAPPING;


HOBBYMAPPING HOBBY_MAPPINGS[] = {	// ���ñ�����Ӧ�ĺ���
	{READING, "�Ķ�"},
	{TRAVELING, "����"},
	{COOKING, "���"},
	{PHOTOGRAPHY, "��Ӱ"},
	{GARDENING, "԰��"}
};
static const int HOBBY_MAPPINGS_LEN = sizeof(HOBBY_MAPPINGS) / sizeof(HOBBY_MAPPINGS[0]);
static const int MAX_HOBBIES_LEN = 64;	// ��ʾ���а�����Ҫ�������洢
const char* const parseHobbies(HOBBIESTYPE hobbies);

typedef struct {	// ѧ����Ϣ
	unsigned long long student_id;
	const char* name;
	const char* gender;
	int height;
	int weight;
	HOBBIESTYPE hobbies;
} STUDENTINFO;
static STUDENTINFO* STUDENTS_INFO = NULL;	// �������ѧ����Ϣ
static int STUDENTS_COUNT = 0;				// ����ѧ������
void expendStudentsInfo();					// ��չ������ÿ��������������һ��λ�ã���ע��������Сʼ���� STUDENTS_COUNT + 1 Ϊ��׼
void saveDatabase();						// �����µ�ѧ������
void loadDatabase();						// �����ѱ����ѧ������



static const char* const SYS_TITLE[] = {
"   _____  ______   __  __    ____    _   __    ______   ____           _______  __   _____\n",
"  / ___/ /_  __/  / / / /   /  _/   / | / /   / ____/  / __ \\         / ___/\\ \\/ /  / ___/\n",
"  \\__ \\   / /    / / / /    / /    /  |/ /   / /_     / / / /         \\__ \\  \\  /   \\__ \\\n",
" ___/ /  / /    / /_/ /   _/ /    / /|  /   / __/    / /_/ /         ___/ /  / /   ___/ /\n",
"/____/  /_/     \\____/   /___/   /_/ |_/   /_/       \\____/         /____/  /_/   /____/\n"
};
static const int SYS_TITLE_LEN = sizeof(SYS_TITLE) / sizeof(SYS_TITLE[0]);
void printSysTitle();


static const char* const MAIN_MENU[] = {	// ���˵�
	"¼��ѧ����Ϣ",
	"�鿴ѧ����Ϣ",
	"����ѧ����Ϣ",
	"�޸�ѧ����Ϣ",
	"ɾ��ѧ����Ϣ",
	"ƥ�䰮��",
	"�˳�ϵͳ"
};
static const int MAIN_MENU_LEN = sizeof(MAIN_MENU) / sizeof(MAIN_MENU[0]);
void showMainMenu();


static const char* const ENTER_STUDENT_INFO_MENU[] = {	// ��¼��ѧ����Ϣ���˵�
	"ѧ��",
	"����",
	"�Ա�",
	"���",
	"����",
	"����"
};
static const int ENTER_STUDENT_INFO_MENU_LEN = sizeof(ENTER_STUDENT_INFO_MENU) / sizeof(ENTER_STUDENT_INFO_MENU[0]);
void showEnterStudentInfoMenu();


void showAllStudentInfo();
void showPartialStudentInfo(const STUDENTINFO* stus, const int count);	// չʾ����ѧ����Ϣ


static const char* const SEARCH_STUDENT_INFO_MENU[] = {
	"����������",
	"��ѧ�Ų���",
	"���Ա����",
	"������һҳ"
};
static const int SEARCH_STUDENT_INFO_MENU_LEN = sizeof(SEARCH_STUDENT_INFO_MENU) / sizeof(SEARCH_STUDENT_INFO_MENU[0]);
void showSearchStudentInfoMenu();
enum SEARCH_TYPE {
	FROM_NAME = 0x0521,
	FROM_STUDENT_ID,
	FROM_GENDER
};	// ������������
void searchStudentInfo(const enum SEARCH_TYPE stype, const STUDENTINFO** stus, int* stus_count);	// ����������


static const char* const MODIFY_STUDENT_INFO_MENU[] = {
	"������ѧ��"
};
static const int MODIFY_STUDENT_INFO_MENU_LEN = sizeof(MODIFY_STUDENT_INFO_MENU) / sizeof(MODIFY_STUDENT_INFO_MENU[0]);
static const char* const MODIFY_STUDENT_INFO_SUB_MENU[] = {
	"����",
	"�Ա�",
	"���",
	"����",
	"����",
	"����"
};
static const int MODIFY_STUDENT_INFO_SUB_MENU_LEN = sizeof(MODIFY_STUDENT_INFO_SUB_MENU) / sizeof(MODIFY_STUDENT_INFO_SUB_MENU[0]);
void showModifyStudentInfoMenu();
void showModifyStudentInfoSubMenu(int index);	// ���������ָ����޸ĵ��Ǹ�ѧ�������� STUDENTS_INFO �е�λ��


static const char* const DELETE_STUDENT_INFO_MENU[] = {
	"������ѧ��"
};
static const int DELETE_STUDENT_INFO_MENU_LEN = sizeof(DELETE_STUDENT_INFO_MENU) / sizeof(DELETE_STUDENT_INFO_MENU[0]);
void showDeleteStudentInfoMenu();
void deleteStudentInfo(int index);


static const char* const MATCH_HOBBIES_MENU[] = {
	"��ѡ�񰮺�(����һ��)"
};
static const int MATCH_HOBBIES_MENU_LEN = sizeof(MATCH_HOBBIES_MENU) / sizeof(MATCH_HOBBIES_MENU[0]);
void showMatchHobbiesMenu();
void matchHobbies(HOBBIESTYPE htype);