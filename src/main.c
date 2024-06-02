#include "main.h"

int main(int argc, char** argv)
{
	loadDatabase();	// 加载数据

	// 输入事件监听
	while (true) {
		showMainMenu();	// 显示主菜单

		char ch = getch();

		switch (ch)
		{
			case '1': {
				showEnterStudentInfoMenu();
			} break;
			case '2': {
				showAllStudentInfo();
			} break;
			case '3': {
				showSearchStudentInfoMenu();
			} break;
			case '4': {
				showModifyStudentInfoMenu();
			} break;
			case '5': {
				showDeleteStudentInfoMenu();
			} break;
			case '6': {
				showMatchHobbiesMenu();
			} break;
			case '7': {
				saveDatabase();		// 保存新数据
				exit(0);
			} break;
			default: break;
		}

		Sleep(10);	// 防止循环过快造成卡顿
	}

	return 0;
}

const char* const parseHobbies(HOBBIESTYPE hobbies)
{
	char* hobbies_s = (char*)malloc(MAX_HOBBIES_LEN * sizeof(char));

	if (hobbies_s == NULL) {
		fprintf(stderr, "\t[内存分配失败！]\n");
		exit(EXIT_FAILURE);
	}

	hobbies_s[0] = '\0'; // 初始化为空字符串

	for (int i = 0; i < HOBBY_MAPPINGS_LEN; i++) {
		if (hobbies & HOBBY_MAPPINGS[i].type) {
			if (hobbies_s[0] != '\0') {
				strcat(hobbies_s, ";");
			}
			strcat(hobbies_s, HOBBY_MAPPINGS[i].chineseName);
		}
	}

	return (const char* const)hobbies_s;
}

void expendStudentsInfo()
{
	int new_size = STUDENTS_COUNT + 1;  // 扩展一格
	STUDENTINFO* new_array = (STUDENTINFO*)malloc(new_size * sizeof(STUDENTINFO));

	if (new_array == NULL) {
		fprintf(stderr, "\t[内存分配失败！]\n");
		exit(EXIT_FAILURE);
	}

	// 复制旧数组内容到新数组
	for (int i = 0; i < STUDENTS_COUNT; i++) {
		new_array[i] = STUDENTS_INFO[i];
	}

	// 释放旧数组内存
	free(STUDENTS_INFO);

	// 更新指针
	STUDENTS_INFO = new_array;
}

void saveDatabase()
{
	FILE* file = fopen("students.dat", "wb");
	if (file == NULL) {
		fprintf(stderr, "无法打开文件保存数据！\n");
		exit(EXIT_FAILURE);
	}

	// 写入学生数量
	fwrite(&STUDENTS_COUNT, sizeof(int), 1, file);

	// 写入学生信息
	for (int i = 0; i < STUDENTS_COUNT; i++) {
		fwrite(&STUDENTS_INFO[i].student_id, sizeof(unsigned long long), 1, file);

		size_t name_len = strlen(STUDENTS_INFO[i].name) + 1;
		size_t gender_len = strlen(STUDENTS_INFO[i].gender) + 1;
		fwrite(&name_len, sizeof(size_t), 1, file);
		fwrite(STUDENTS_INFO[i].name, sizeof(char), name_len, file);

		fwrite(&gender_len, sizeof(size_t), 1, file);
		fwrite(STUDENTS_INFO[i].gender, sizeof(char), gender_len, file);

		fwrite(&STUDENTS_INFO[i].height, sizeof(int), 1, file);
		fwrite(&STUDENTS_INFO[i].weight, sizeof(int), 1, file);
		fwrite(&STUDENTS_INFO[i].hobbies, sizeof(int), 1, file);
	}

	fclose(file);
}

void loadDatabase()
{
	FILE* file = fopen("students.dat", "rb");
	if (file == NULL) {
		fprintf(stderr, "无法打开文件加载数据！\n");
		return;
	}

	// 读取学生数量
	fread(&STUDENTS_COUNT, sizeof(int), 1, file);

	// 分配内存存储学生信息
	STUDENTS_INFO = (STUDENTINFO*)malloc(STUDENTS_COUNT * sizeof(STUDENTINFO));
	if (STUDENTS_INFO == NULL) {
		fprintf(stderr, "\t[内存分配失败！]\n");
		exit(EXIT_FAILURE);
	}

	// 读取学生信息
	for (int i = 0; i < STUDENTS_COUNT; i++) {
		fread(&STUDENTS_INFO[i].student_id, sizeof(unsigned long long), 1, file);

		size_t name_len;
		size_t gender_len;
		fread(&name_len, sizeof(size_t), 1, file);
		STUDENTS_INFO[i].name = (const char*)malloc(name_len * sizeof(char));
		if (STUDENTS_INFO[i].name == NULL) {
			fprintf(stderr, "\t[内存分配失败！]\n");
			exit(EXIT_FAILURE);
		}
		fread((char*)STUDENTS_INFO[i].name, sizeof(char), name_len, file);

		fread(&gender_len, sizeof(size_t), 1, file);
		STUDENTS_INFO[i].gender = (const char*)malloc(gender_len * sizeof(char));
		if (STUDENTS_INFO[i].gender == NULL) {
			fprintf(stderr, "\t[内存分配失败！]\n");
			exit(EXIT_FAILURE);
		}
		fread((char*)STUDENTS_INFO[i].gender, sizeof(char), gender_len, file);

		fread(&STUDENTS_INFO[i].height, sizeof(int), 1, file);
		fread(&STUDENTS_INFO[i].weight, sizeof(int), 1, file);
		fread(&STUDENTS_INFO[i].hobbies, sizeof(int), 1, file);
	}

	fclose(file);
}

void printSysTitle()
{
	// 打印标题
	for (int i = 0; i < SYS_TITLE_LEN; i++) {
		printf(SYS_TITLE[i]);

		if (i == SYS_TITLE_LEN - 1) putchar('\n');	// 最后多空一行
	}
}

void showMainMenu()
{
	system("cls");	// 清除旧内容
	printSysTitle();	// 打印标题

	// 打印菜单
	for (int i = 0; i < MAIN_MENU_LEN; i++) {
		printf("\t%d. %s\n", (i + 1), MAIN_MENU[i]);
	}
}

void showEnterStudentInfoMenu()
{
	system("cls");	// 清除旧内容
	printSysTitle();	// 打印标题

	STUDENTINFO stu = { '\0' };		// 用于存储新录入学生信息的临时变量

	printf("\t[请输入学生信息]\n");
	for (int i = 0; i < ENTER_STUDENT_INFO_MENU_LEN; i++) {
		printf("\t=> %s: ", ENTER_STUDENT_INFO_MENU[i]);
		switch (i)
		{
			case 0: {
				int res = scanf("%llu", &(stu.student_id));
			} break;
			case 1: {
				stu.name = (const char*)malloc(NAME_MAX_LENGTH * sizeof(char));
				int res = scanf("%s", (char*)(stu.name));
			} break;
			case 2: {
				stu.gender = (const char*)malloc(GENDER_MAX_LENGTH * sizeof(char));
				int res = scanf("%s", (char*)(stu.gender));
			} break;
			case 3: {
				int res = scanf("%d", &(stu.height));
			} break;
			case 4: {
				int res = scanf("%d", &(stu.weight));
			} break;
			case 5: {
				int res = scanf("%d", &(stu.hobbies));
			} break;
			default:
				exit(EXIT_FAILURE);
				break;
		}
	}

	// 录入
	expendStudentsInfo();
	STUDENTS_INFO[STUDENTS_COUNT++] = stu;	// 注意这里的计算 STUDENTS_COUNT = (STUDENTS_COUNT - 1) + 1
	
	// 给出提示信信息
	printf("\t[正在录入···]\n");
	Sleep(500);
	printf("\t[录入成功！]\n\n");

	system("@echo [Press any key to continue] && pause 1>nul 2>nul");	// 暂停
}

void showAllStudentInfo()
{
	system("cls");	// 清除旧内容
	printSysTitle();	// 打印标题

	// 给出提示信信息
	printf("\t[正在查询···]\n");
	Sleep(500);
	printf("\t[查询成功！]\n");
	Sleep(200);
	
	system("cls");	// 清除旧内容
	printSysTitle();	// 打印标题

	showPartialStudentInfo(STUDENTS_INFO, STUDENTS_COUNT);

	system("@echo [Press any key to continue] && pause 1>nul 2>nul");	// 暂停
}

void showPartialStudentInfo(const STUDENTINFO* stus, const int count)
{
	if (stus == NULL || count == 0)
		return;

	for (int i = 0; i < count; i++) {
		printf(
			"\tNo.%d" 
			"\t%llu"
			"  %s"
			"\t%s"
			"  %d cm"
			"  %d 斤"
			"  %s\n",
			(i + 1),
			stus[i].student_id,
			stus[i].name,
			stus[i].gender,
			stus[i].height,
			stus[i].weight,
			parseHobbies(stus[i].hobbies));
	}
	putchar('\n');
}

void showSearchStudentInfoMenu()
{
	system("cls");	// 清除旧内容
	printSysTitle();	// 打印标题

	for (int i = 0; i < SEARCH_STUDENT_INFO_MENU_LEN; i++) {
		printf("\t%d. %s\n", (i + 1), SEARCH_STUDENT_INFO_MENU[i]);
	}

	// 用于接收查询结果
	const STUDENTINFO* search_results = NULL;	// 信息
	int search_size = 0;						// 个数

	while (true) {
		char ch = getch();

		switch (ch)
		{
			case '1': {
				system("cls");		// 清除旧内容
				printSysTitle();	// 打印标题

				searchStudentInfo(FROM_NAME, &search_results ,&search_size);	// 搜索

				system("cls");		// 清除旧内容
				printSysTitle();	// 打印标题

				if (search_results != NULL) {	// 找到了结果
					showPartialStudentInfo(search_results, search_size);	// 展示搜索结果
				}
				else {
					printf("\t[未找到相关信息]\n\n");
				}

				system("@echo [Press any key to continue] && pause 1>nul 2>nul");	// 暂停
				return;
			} break;
			case '2': {
				system("cls");		// 清除旧内容
				printSysTitle();	// 打印标题

				searchStudentInfo(FROM_STUDENT_ID, &search_results, &search_size);	// 搜索	

				system("cls");		// 清除旧内容
				printSysTitle();	// 打印标题

				if (search_results != NULL) {
					showPartialStudentInfo(search_results, search_size);	// 展示搜索结果
				}
				else {
					printf("\t[未找到相关信息]\n\n");
				}

				system("@echo [Press any key to continue] && pause 1>nul 2>nul");	// 暂停
				return;
			} break;
			case '3': {
				system("cls");		// 清除旧内容
				printSysTitle();	// 打印标题

				searchStudentInfo(FROM_GENDER, &search_results, &search_size);	// 搜索

				system("cls");		// 清除旧内容
				printSysTitle();	// 打印标题

				if (search_results != NULL) {
					showPartialStudentInfo(search_results, search_size);	// 展示搜索结果
				}
				else {
					printf("\t[未找到相关信息]\n\n");
				}

				system("@echo [Press any key to continue] && pause 1>nul 2>nul");	// 暂停
				return;
			} break;
			case '4': {
				return;
			} break;
			default: break;
		}

		Sleep(10);	// 防止循环过快造成卡顿
	}
}

void searchStudentInfo(const enum SEARCH_TYPE stype,const STUDENTINFO** stus, int* stus_count)
{
	// 用于存储找到的结果
	*stus_count = 0;	// 数量（可能不止查找到一个结果）
	*stus = NULL;		// 信息

	switch (stype)
	{
		case FROM_NAME: {	// 按姓名查找
			// 记录待查找的信息
			char* name_buffer = (char*)malloc(NAME_MAX_LENGTH * sizeof(char));

			if (name_buffer == NULL) {
				fprintf(stderr, "内存分配失败！!\n");
				exit(EXIT_FAILURE);
			}

			printf("\t请输入姓名: ");
			int res = scanf("%s", name_buffer);

			// 给出提示信息
			printf("\t[正在查询···]\n");	
			Sleep(200);

			// 遍历所有学生信息，进行查找
			for (int i = 0; i < STUDENTS_COUNT; i++) {
				if (strcmp(STUDENTS_INFO[i].name, name_buffer) == 0) {	// 向 *stus 中存储新查找到的结果
					STUDENTINFO* new_stus = (STUDENTINFO*)malloc((size_t)(*stus_count + 1) * sizeof(STUDENTINFO));

					if (new_stus == NULL) {
						fprintf(stderr, "内存分配失败！!\n");
						exit(EXIT_FAILURE);
					}

					// 复制已搜索的内容
					for (int i = 0; i < *stus_count; i++) {
						new_stus[i].student_id = (*stus)[i].student_id;
						new_stus[i].height = (*stus)[i].height;
						new_stus[i].weight = (*stus)[i].weight;
						new_stus[i].hobbies = (*stus)[i].hobbies;
						new_stus[i].name = (const char*)malloc(NAME_MAX_LENGTH * sizeof(char));
						new_stus[i].gender = (const char*)malloc(GENDER_MAX_LENGTH * sizeof(char));
						if (new_stus[i].name == NULL || new_stus[i].gender == NULL) {
							fprintf(stderr, "\t[内存分配失败！]\n");
							exit(EXIT_FAILURE);
						}
						strcpy((char*)new_stus[i].name, (*stus)[i].name);
						strcpy((char*)new_stus[i].gender, (*stus)[i].gender);
					}

					// 复制进 new_stu
					// 注意这里为了避免返回的 *stus 直接操作源数据 STUDENTS_INFO，
					// 故而采用复制的方式，
					// 而不是直接给指针赋值（否则返回的结果是可以直接操作源数据的，这是非常危险的行为）
					new_stus[*stus_count].student_id = STUDENTS_INFO[i].student_id;
					new_stus[*stus_count].height = STUDENTS_INFO[i].height;
					new_stus[*stus_count].weight = STUDENTS_INFO[i].weight;
					new_stus[*stus_count].hobbies = STUDENTS_INFO[i].hobbies;

					// 这两个属性的复制很特殊
					new_stus[*stus_count].name = (const char*)malloc(NAME_MAX_LENGTH * sizeof(char));
					new_stus[*stus_count].gender = (const char*)malloc(GENDER_MAX_LENGTH * sizeof(char));
					if (new_stus[*stus_count].name == NULL || new_stus[*stus_count].gender == NULL) {
						fprintf(stderr, "\t[内存分配失败！]\n");
						exit(EXIT_FAILURE);
					}
					strcpy((char*)new_stus[*stus_count].name, STUDENTS_INFO[i].name);
					strcpy((char*)new_stus[*stus_count].gender, STUDENTS_INFO[i].gender);

					*stus_count += 1;	// 更新数量变化

					// 释放旧数组内存
					free((void*)*stus);

					// 更新指针
					*stus = (const STUDENTINFO*)new_stus;
				}
			}

			free(name_buffer);	// 记得释放
		} break;
		case FROM_STUDENT_ID: {	// 按学号查找
			// 记录待查找的信息
			unsigned long long id_buffer;

			printf("\t请输入学号: ");
			int res = scanf("%llu", &id_buffer);

			// 给出提示信息
			printf("\t[正在查询···]\n");
			Sleep(200);

			// 遍历所有学生信息，进行查找
			for (int i = 0; i < STUDENTS_COUNT; i++) {
				if (STUDENTS_INFO[i].student_id == id_buffer) {	// 向 *stus 中存储新查找到的结果
					STUDENTINFO* new_stus = (STUDENTINFO*)malloc((size_t)(*stus_count + 1) * sizeof(STUDENTINFO));

					if (new_stus == NULL) {
						fprintf(stderr, "内存分配失败！!\n");
						exit(EXIT_FAILURE);
					}

					// 复制已搜索的内容
					for (int i = 0; i < *stus_count; i++) {
						new_stus[i].student_id = (*stus)[i].student_id;
						new_stus[i].height = (*stus)[i].height;
						new_stus[i].weight = (*stus)[i].weight;
						new_stus[i].hobbies = (*stus)[i].hobbies;
						new_stus[i].name = (const char*)malloc(NAME_MAX_LENGTH * sizeof(char));
						new_stus[i].gender = (const char*)malloc(GENDER_MAX_LENGTH * sizeof(char));
						if (new_stus[i].name == NULL || new_stus[i].gender == NULL) {
							fprintf(stderr, "\t[内存分配失败！]\n");
							exit(EXIT_FAILURE);
						}
						strcpy((char*)new_stus[i].name, (*stus)[i].name);
						strcpy((char*)new_stus[i].gender, (*stus)[i].gender);
					}

					// 复制进 new_stu
					// 注意这里为了避免返回的 *stus 直接操作源数据 STUDENTS_INFO，
					// 故而采用复制的方式，
					// 而不是直接给指针赋值（否则返回的结果是可以直接操作源数据的，这是非常危险的行为）
					new_stus[*stus_count].student_id = STUDENTS_INFO[i].student_id;
					new_stus[*stus_count].height = STUDENTS_INFO[i].height;
					new_stus[*stus_count].weight = STUDENTS_INFO[i].weight;
					new_stus[*stus_count].hobbies = STUDENTS_INFO[i].hobbies;

					// 这两个属性的复制很特殊
					new_stus[*stus_count].name = (const char*)malloc(NAME_MAX_LENGTH * sizeof(char));
					new_stus[*stus_count].gender = (const char*)malloc(GENDER_MAX_LENGTH * sizeof(char));
					if (new_stus[*stus_count].name == NULL || new_stus[*stus_count].gender == NULL) {
						fprintf(stderr, "\t[内存分配失败！]\n");
						exit(EXIT_FAILURE);
					}
					strcpy((char*)new_stus[*stus_count].name, STUDENTS_INFO[i].name);
					strcpy((char*)new_stus[*stus_count].gender, STUDENTS_INFO[i].gender);

					*stus_count += 1;	// 更新数量变化

					// 释放旧数组内存
					free((void*)*stus);

					// 更新指针
					*stus = (const STUDENTINFO*)new_stus;
				}
			}
		} break;
		case FROM_GENDER: {	// 按性别查找
			// 记录待查找的信息
			char* gender_buffer = (char*)malloc(GENDER_MAX_LENGTH * sizeof(char));

			if (gender_buffer == NULL) {
				fprintf(stderr, "内存分配失败！!\n");
				exit(EXIT_FAILURE);
			}

			printf("\t请输入性别: ");
			int res = scanf("%s", gender_buffer);

			// 给出提示信息
			printf("\t[正在查询···]\n");
			Sleep(200);

			// 遍历所有学生信息，进行查找
			for (int i = 0; i < STUDENTS_COUNT; i++) {
				if (strcmp(STUDENTS_INFO[i].gender, gender_buffer) == 0) {	// 向 *stus 中存储新查找到的结果
					STUDENTINFO* new_stus = (STUDENTINFO*)malloc((size_t)(*stus_count + 1) * sizeof(STUDENTINFO));

					if (new_stus == NULL) {
						fprintf(stderr, "内存分配失败！!\n");
						exit(EXIT_FAILURE);
					}

					// 复制已搜索的内容
					for (int i = 0; i < *stus_count; i++) {
						new_stus[i].student_id = (*stus)[i].student_id;
						new_stus[i].height = (*stus)[i].height;
						new_stus[i].weight = (*stus)[i].weight;
						new_stus[i].hobbies = (*stus)[i].hobbies;
						new_stus[i].name = (const char*)malloc(NAME_MAX_LENGTH * sizeof(char));
						new_stus[i].gender = (const char*)malloc(GENDER_MAX_LENGTH * sizeof(char));
						if (new_stus[i].name == NULL || new_stus[i].gender == NULL) {
							fprintf(stderr, "\t[内存分配失败！]\n");
							exit(EXIT_FAILURE);
						}
						strcpy((char*)new_stus[i].name, (*stus)[i].name);
						strcpy((char*)new_stus[i].gender, (*stus)[i].gender);
					}

					// 复制进 new_stu
					// 注意这里为了避免返回的 *stus 直接操作源数据 STUDENTS_INFO，
					// 故而采用复制的方式，
					// 而不是直接给指针赋值（否则返回的结果是可以直接操作源数据的，这是非常危险的行为）
					new_stus[*stus_count].student_id = STUDENTS_INFO[i].student_id;
					new_stus[*stus_count].height = STUDENTS_INFO[i].height;
					new_stus[*stus_count].weight = STUDENTS_INFO[i].weight;
					new_stus[*stus_count].hobbies = STUDENTS_INFO[i].hobbies;

					// 这两个属性的复制很特殊
					new_stus[*stus_count].name = (const char*)malloc(NAME_MAX_LENGTH * sizeof(char));
					new_stus[*stus_count].gender = (const char*)malloc(GENDER_MAX_LENGTH * sizeof(char));
					if (new_stus[*stus_count].name == NULL || new_stus[*stus_count].gender == NULL) {
						fprintf(stderr, "\t[内存分配失败！]\n");
						exit(EXIT_FAILURE);
					}
					strcpy((char*)new_stus[*stus_count].name, STUDENTS_INFO[i].name);
					strcpy((char*)new_stus[*stus_count].gender, STUDENTS_INFO[i].gender);

					*stus_count += 1;	// 更新数量变化

					// 释放旧数组内存
					free((void*)*stus);

					// 更新指针
					*stus = (const STUDENTINFO*)new_stus;
				}
			}

			free(gender_buffer);	// 记得释放
		} break;
		default:
			exit(EXIT_FAILURE);
			break;
	}
}

void showModifyStudentInfoMenu() {
	system("cls");		// 清除旧内容
	printSysTitle();	// 打印标题

	unsigned long long search_key_from_id = 0LLU;

	// 只有一行，所以直接这么写
	// 需要扩展以后好改
	printf("\t%s: ", MODIFY_STUDENT_INFO_MENU[0]);

	int res = scanf("%llu", &search_key_from_id);

	// 遍历所有学生信息，进行查找
	for (int i = 0; i < STUDENTS_COUNT; i++) {
		if (STUDENTS_INFO[i].student_id == search_key_from_id) {	// 理论上学号就不应该有重复
			showModifyStudentInfoSubMenu(i);
			return;	// 防止有重复学号引起的矛盾（所以只会修改第一个找到的数据）
		}
	}

	// 程序进行到这证明没找到
	system("cls");		// 清除旧内容
	printSysTitle();	// 打印标题
	printf("\t[未找到对应的学生信息！]\n\n");
	system("@echo [Press any key to continue] && pause 1>nul 2>nul");	// 暂停
}

void showModifyStudentInfoSubMenu(int index) 
{
	system("cls");		// 清除旧内容
	printSysTitle();	// 打印标题

	for (int i = 0; i < MODIFY_STUDENT_INFO_SUB_MENU_LEN; i++) {
		printf("\t%d. %s\n", (i + 1), MODIFY_STUDENT_INFO_SUB_MENU[i]);
	}

	while (true) {
		char ch = getch();

		switch (ch)
		{
			case '1': {
				system("cls");		// 清除旧内容
				printSysTitle();	// 打印标题

				char* name_buffer = (char*)malloc(NAME_MAX_LENGTH * sizeof(char));
				
				if (name_buffer == NULL) {
					fprintf(stderr, "内存分配失败！!\n");
					exit(EXIT_FAILURE);
				}
				
				printf("\t请输入新的姓名: ");
				int res = scanf("%s", name_buffer);

				// 释放旧内存区域
				free((void*)(STUDENTS_INFO[index].name));

				// 直接指向新址内存
				STUDENTS_INFO[index].name = (const char*)name_buffer;

				printf("\t[成功修改学生 '%llu' 的姓名为 '%s']\n\n", STUDENTS_INFO[index].student_id, STUDENTS_INFO[index].name);

				system("@echo [Press any key to continue] && pause 1>nul 2>nul");	// 暂停
				return;
			} break;
			case '2': {
				system("cls");		// 清除旧内容
				printSysTitle();	// 打印标题

				char* gender_buffer = (char*)malloc(GENDER_MAX_LENGTH * sizeof(char));

				if (gender_buffer == NULL) {
					fprintf(stderr, "内存分配失败！!\n");
					exit(EXIT_FAILURE);
				}

				printf("\t请输入新的性别: ");
				int res = scanf("%s", gender_buffer);

				// 释放旧内存区域
				free((void*)(STUDENTS_INFO[index].gender));

				// 直接指向新址内存
				STUDENTS_INFO[index].gender = (const char*)gender_buffer;

				printf("\t[成功修改学生 '%llu' 的性别为 '%s']\n\n", STUDENTS_INFO[index].student_id, STUDENTS_INFO[index].gender);

				system("@echo [Press any key to continue] && pause 1>nul 2>nul");	// 暂停
				return;
			} break;
			case '3': {
				system("cls");		// 清除旧内容
				printSysTitle();	// 打印标题

				int height_buffer = -1;

				printf("\t请输入新的身高(cm): ");
				int res = scanf("%d", &height_buffer);

				STUDENTS_INFO[index].height = (height_buffer > 0 ? height_buffer : -1);	// 让 -1 代表修改的非法数据（注意源数据的非法数据可以是任何值）

				printf("\t[成功修改学生 '%llu' 的身高为 '%d cm']\n\n", STUDENTS_INFO[index].student_id, STUDENTS_INFO[index].height);

				system("@echo [Press any key to continue] && pause 1>nul 2>nul");	// 暂停
				return;
			} break;
			case '4': {
				system("cls");		// 清除旧内容
				printSysTitle();	// 打印标题

				int weight_buffer = -1;

				printf("\t请输入新的体重(斤): ");
				int res = scanf("%d", &weight_buffer);

				STUDENTS_INFO[index].weight = (weight_buffer > 0 ? weight_buffer : -1);	// 让 -1 代表修改的非法数据（注意源数据的非法数据可以是任何值）

				printf("\t[成功修改学生 '%llu' 的体重为 '%d 斤']\n\n", STUDENTS_INFO[index].student_id, STUDENTS_INFO[index].weight);

				system("@echo [Press any key to continue] && pause 1>nul 2>nul");	// 暂停
				return;
			} break;
			case '5': {
				system("cls");		// 清除旧内容
				printSysTitle();	// 打印标题

				bool* selected_combination = (bool*)malloc(HOBBY_MAPPINGS_LEN * sizeof(bool));
				
				if (selected_combination == NULL) {
					fprintf(stderr, "\t[内存分配失败！]\n");
					exit(EXIT_FAILURE);
				}

				// 初始化新数组
				memset(selected_combination, 0, HOBBY_MAPPINGS_LEN * sizeof(bool));

				bool shouldExitWhile = false;
				while (true)
				{
					printf("\t[请选择爱好]\n");
					for (int i = 0; i < HOBBY_MAPPINGS_LEN; i++) {
						printf("\t(%c) %d. %s\n", (selected_combination[i] ? '*' : ' '), (i + 1), HOBBY_MAPPINGS[i].chineseName);
					}
					printf("\t[并按回车确定]\n");

					char ch = getch();

					switch (ch)
					{
						case '1': {
							if (selected_combination[0]) {
								selected_combination[0] = false;
							}
							else {
								selected_combination[0] = true;
							}
						} break;
						case '2': {
							if (*(selected_combination + 1)) {
								*(selected_combination + 1) = false;
							}
							else {
								*(selected_combination + 1) = true;
							}
						} break;
						case '3': {
							if (*(selected_combination + 2)) {
								*(selected_combination + 2) = false;
							}
							else {
								*(selected_combination + 2) = true;
							}
						} break;
						case '4': {
							if (*(selected_combination + 3)) {
								*(selected_combination + 3) = false;
							}
							else {
								*(selected_combination + 3) = true;
							}
						} break;
						case '5': {
							if (*(selected_combination + 4)) {
								*(selected_combination + 4) = false;
							}
							else {
								*(selected_combination + 4) = true;
							}
						} break;
						case VK_RETURN: {	// 回车监听（本质上 VK_RETURN = 0x0D），而 VK_RETURN 定义在 Windows API 中
							shouldExitWhile = true;
						} break;
						default: break;
					}

					system("cls");		// 清除旧内容
					printSysTitle();	// 打印标题

					if (shouldExitWhile) {	// 在这里才能退出循环
						STUDENTS_INFO[index].hobbies = 0;
						for (int i = 0; i < HOBBY_MAPPINGS_LEN; i++) {
							if (*(selected_combination + i)) {
								switch (i) {
									case 0:
										STUDENTS_INFO[index].hobbies |= READING;
										break;
									case 1:
										STUDENTS_INFO[index].hobbies |= TRAVELING;
										break;
									case 2:
										STUDENTS_INFO[index].hobbies |= COOKING;
										break;
									case 3:
										STUDENTS_INFO[index].hobbies |= PHOTOGRAPHY;
										break;
									case 4:
										STUDENTS_INFO[index].hobbies |= GARDENING;
										break;
									default:break;
								}
							}
						}

						printf("\t[已经修改学生 '%llu' 的爱好为 '%s']\n\n", STUDENTS_INFO[index].student_id, parseHobbies(STUDENTS_INFO[index].hobbies));
						break;
					}
				}

				free(selected_combination);	// 记得释放

				system("@echo [Press any key to continue] && pause 1>nul 2>nul");	// 暂停
				return;
			} break;
			case '6': {
				return;
			} break;
			default:break;
		}

		Sleep(10);	// 防止循环过快造成卡顿
	}
}

void showDeleteStudentInfoMenu()
{
	system("cls");		// 清除旧内容
	printSysTitle();	// 打印标题

	unsigned long long search_key_from_id = 0LLU;

	// 只有一行，所以直接这么写
	// 需要扩展以后好改
	printf("\t%s: ", DELETE_STUDENT_INFO_MENU[0]);

	int res = scanf("%llu", &search_key_from_id);

	// 遍历所有学生信息，进行查找
	for (int i = 0; i < STUDENTS_COUNT; i++) {
		if (STUDENTS_INFO[i].student_id == search_key_from_id) {	// 理论上学号就不应该有重复
			deleteStudentInfo(i);
			printf("\t[已删除对应的学生信息！]\n\n");
			system("@echo [Press any key to continue] && pause 1>nul 2>nul");	// 暂停
			return;	// 防止有重复学号引起的矛盾（所以只会删除第一个找到的数据）
		}
	}

	// 程序进行到这证明没找到
	system("cls");		// 清除旧内容
	printSysTitle();	// 打印标题
	printf("\t[未找到对应的学生信息！]\n\n");
	system("@echo [Press any key to continue] && pause 1>nul 2>nul");	// 暂停
}

void deleteStudentInfo(int index)
{
	// 检查索引值是否合法
	if (index < 0 || index >= STUDENTS_COUNT) {
		fprintf(stderr, "\t[索引值不合法！]\n");
		return;
	}

	// 如果只有一个元素，直接释放并将指针设为 NULL
	if (STUDENTS_COUNT == 1) {
		free(STUDENTS_INFO);
		STUDENTS_INFO = NULL;
		STUDENTS_COUNT = 0;
		return;
	}

	// 新数组大小减一
	int new_size = STUDENTS_COUNT - 1;
	STUDENTINFO* new_array = (STUDENTINFO*)malloc(new_size * sizeof(STUDENTINFO));

	if (new_array == NULL) {
		fprintf(stderr, "\t[内存分配失败！]\n");
		exit(EXIT_FAILURE);
	}

	// 这里可以理解为“双指针算法”
	// 复制旧数组内容到新数组，跳过要删除的元素
	for (int i = 0, j = 0; i < STUDENTS_COUNT; i++) {
		if (i != index) {
			// 注意这种写法，可以避免缓冲区溢出问题，也就是 C6386 警告
			*(new_array + j++) = STUDENTS_INFO[i];
		}
	}

	// 释放要删除元素的内存
	free((void*)STUDENTS_INFO[index].name);
	free((void*)STUDENTS_INFO[index].gender);

	// 释放旧数组内存
	free(STUDENTS_INFO);

	// 更新指针和计数器
	STUDENTS_INFO = new_array;
	STUDENTS_COUNT = new_size;
}

void showMatchHobbiesMenu()
{
	system("cls");		// 清除旧内容
	printSysTitle();	// 打印标题

	// 只有一行，所以直接这么写
	// 需要扩展以后好改
	printf("\t[%s]\n", DELETE_STUDENT_INFO_MENU[0]);

	for (int i = 0; i < HOBBY_MAPPINGS_LEN; i++) {
		printf("\t%d. %s\n", (i + 1), HOBBY_MAPPINGS[i].chineseName);
	}

	while (true) {
		char ch = getch();

		switch (ch)
		{
			case '1': {
				matchHobbies(READING);

				system("@echo [Press any key to continue] && pause 1>nul 2>nul");	// 暂停
				return;
			} break;
			case '2': {
				matchHobbies(TRAVELING);

				system("@echo [Press any key to continue] && pause 1>nul 2>nul");	// 暂停
				return;
			} break;
			case '3': {
				matchHobbies(COOKING);

				system("@echo [Press any key to continue] && pause 1>nul 2>nul");	// 暂停
				return;
			} break;
			case '4': {
				matchHobbies(PHOTOGRAPHY);

				system("@echo [Press any key to continue] && pause 1>nul 2>nul");	// 暂停
				return;
			} break;
			case '5': {
				matchHobbies(GARDENING);

				system("@echo [Press any key to continue] && pause 1>nul 2>nul");	// 暂停
				return;
			} break;
			default: break;
		}
	}
}

void matchHobbies(HOBBIESTYPE htype)
{
	system("cls");		// 清除旧内容
	printSysTitle();	// 打印标题

	// 包含所有的搜寻结果
	int search_count = 0;
	const STUDENTINFO* search_results = (const STUDENTINFO*)malloc(search_count * sizeof(STUDENTINFO));

	if (search_results == NULL) {
		fprintf(stderr, "\t[内存分配失败！]\n");
		exit(EXIT_FAILURE);
	}

	// 搜索包含某个爱好的所有学生
	for (int i = 0; i < STUDENTS_COUNT; i++) {
		if (STUDENTS_INFO[i].hobbies & htype) {
			STUDENTINFO* new_array = (STUDENTINFO*)malloc((search_count + 1) * sizeof(STUDENTINFO));

			if (new_array == NULL) {
				fprintf(stderr, "\t[内存分配失败！]\n");
				exit(EXIT_FAILURE);
			}

			// 复制已搜索的内容
			for (int i = 0; i < search_count; i++) {
				new_array[i].student_id = search_results[i].student_id;
				new_array[i].height = search_results[i].height;
				new_array[i].weight = search_results[i].weight;
				new_array[i].hobbies = search_results[i].hobbies;
				new_array[i].name = (const char*)malloc(NAME_MAX_LENGTH * sizeof(char));
				new_array[i].gender = (const char*)malloc(GENDER_MAX_LENGTH * sizeof(char));
				if (new_array[i].name == NULL || new_array[i].gender == NULL) {
					fprintf(stderr, "\t[内存分配失败！]\n");
					exit(EXIT_FAILURE);
				}
				strcpy((char*)new_array[i].name, search_results[i].name);
				strcpy((char*)new_array[i].gender, search_results[i].gender);
			}

			// 添加新搜索到的结果
			new_array[search_count].student_id = STUDENTS_INFO[i].student_id;
			new_array[search_count].height = STUDENTS_INFO[i].height;
			new_array[search_count].weight = STUDENTS_INFO[i].weight;
			new_array[search_count].hobbies = STUDENTS_INFO[i].hobbies;
			new_array[search_count].name = (const char*)malloc(NAME_MAX_LENGTH * sizeof(char));
			new_array[search_count].gender = (const char*)malloc(GENDER_MAX_LENGTH * sizeof(char));
			if (new_array[search_count].name == NULL || new_array[search_count].gender == NULL) {
				fprintf(stderr, "\t[内存分配失败！]\n");
				exit(EXIT_FAILURE);
			}
			strcpy((char*)new_array[search_count].name, STUDENTS_INFO[i].name);
			strcpy((char*)new_array[search_count].gender, STUDENTS_INFO[i].gender);

			search_count += 1;	// 更新数量变化

			// 释放旧内存
			free((void*)search_results);

			// 更新指针
			search_results = (const STUDENTINFO*)new_array;
		}
	}

	if (search_results) {
		printf("\t[已找到具有 '%s' 兴趣的学生！]\n", parseHobbies(htype));
		showPartialStudentInfo(search_results, search_count);
	}
	else {
		printf("\t[找不到具有 '%s' 兴趣的学生！]\n\n", parseHobbies(htype));
	}
}
