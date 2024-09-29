#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 9 //数独的边长
#define MIN_NUM 17 //数独要想保证有唯一解，最少的初始数字个数就是17，这是经过反复实验观察得到的
int boolCount;  //布尔变元数量
int clauseCount;  //子句数量
char fileName[100]; //文件名

//十字链表结构体
typedef struct SATNode {
    int data;  //数据域
    SATNode *next;
} SATNode;

typedef struct SATList {
    SATNode *head;  //表头
    SATList *next;
} SATList;

//函数声明
bool addClause(SATList *cnf, SATList *&root);
bool baseDPLL(SATList *&cnf, int *&value, int *&count);
void CopyClause(SATList *&a, SATList *b);
void compute_literal_num(SATList *cnf, int *count);
int *copy(int *a, int n);
void copy(int a[N][N], int b[N][N]);
void destroyClause(SATList *&cnf);
bool evaluateClause(SATNode *cnf, int v[]);
bool emptyClause(SATList *cnf);
void diySudoku(int board[N][N]);
void Delete(SATList *&cnf, SATList *&pre, SATList *&List, int *count);
void Delete(SATList *&cnf, int literal, int *count);
void fillBoard(int board[N][N]);
bool isUnitClause(SATNode *cnf);
bool isSafe(int board[N][N], int row, int col, int num);
void init(int board[N][N]);
void init(int *&arr, int e = 0);
void mySolveSudoku(int a[N][N], int board[N][N]);
int optimizeDPLL(SATList *&cnf, int value[]);
void print(SATList *CNFList);
void printBoard(int board[N][N]);
int ReadFile(SATList *&cnf);
void removeClause(SATList *&cnf, SATList *&root);
void removeNode(SATNode *&cnf, SATNode *&head);
void removeNumbers(int board[N][N], int count);
int SolvePuzzle(int chess[]);
bool solveSudoku(int board[N][N]);
bool WriteFile(int result, double time, int value[]);

//函数定义
/*
函数名称: main
接受参数: void
函数功能: 主函数
返回值: int
 */
int main(void) {
    srand(time_t(NULL));
    SATList* CNFList = NULL;
    int board[N][N];
    clock_t start, finish;  //设置时间变量
    double time;
    int op = 1, i, result;
    int* value, *count; //保存结果，每个变元出现的数量
    while (op) {
        system("cls");
        printf("\n\n		   功能菜单 \n");
        printf("--------------------------------------------\n");
        printf("1.读取cnf文件	         2.遍历输出每个子句\n");
        printf("3.基础DPLL求解算例       4.优化DPLL求解算例\n");
        printf("5.数独游戏\n");
        printf("0.退出\n");
        printf("--------------------------------------------\n");
        printf("	请选择你的操作[0~6]:");
        scanf("%d", &op);
        switch (op) {
            case 1:
                printf("输入要读取的cnf文件:");
                scanf("%s", fileName);
                ReadFile(CNFList);
                getchar();
                getchar();
                break;
            case 2:
                if (CNFList == NULL)
                    printf("未导入文件\n");
                else {
                    print(CNFList);
                }
                getchar();
                getchar();
                break;
            case 3:
                if (CNFList == NULL)
                    printf("未导入文件\n");
                else {
                    init(value, 1);
                    init(count);
                    compute_literal_num(CNFList, count);
                    start = clock();  //计时开始;
                    result = baseDPLL(CNFList, value, count);
                    finish = clock();    //结束
                    printf("求解结果：%d\n", result);
                    if (result == 1) {
                        for (i = 1; i <= boolCount; i++) {
                            if (value[i] == 1)
                                printf("%d ", i);
                            else
                                printf("%d ", -i);
                        }
                        printf("\n");
                    }
                    time = (double)(finish - start) / CLOCKS_PER_SEC * 1000; //计算运行时间
                    printf("start=%ld,finish=%ld,运行时间=%lf ms\n", start, finish, 1.0 * time); //输出运行时间
                    WriteFile(result, time, value);
                    destroyClause(CNFList);
                    printf("文件保存成功\n");
                }
                getchar();
                getchar();
                break;
            case 4:
                if (CNFList == NULL)
                    printf("未导入文件\n");
                else {
                    value = (int *)malloc(sizeof(int) * (boolCount + 1));
                    for (i = 1; i <= boolCount; i++)
                        value[i] = 1;  //初始化，均赋为1
                    start = clock();  //计时开始;
                    result = optimizeDPLL(CNFList, value);
                    finish = clock();    //结束
                    printf("求解结果：%d\n", result);
                    if (result == 1) {
                        for (i = 1; i <= boolCount; i++) {
                            if (value[i] == 1)
                                printf("%d ", i);
                            else
                                printf("%d ", -i);
                        }
                        printf("\n");
                    }
                    time = (double)(finish - start) / CLOCKS_PER_SEC;//计算运行时间
                    printf("运行时间=%lfms\n", time * 1000);//输出运行时间
                    if (WriteFile(result, time, value) == 1)
                        printf("结果已保存至同名文件.res\n");
                    else
                        printf("结果保存失败\n");
                }
                getchar();
                getchar();
                break;
            case 5:
                init(board);
                fillBoard(board);
                int a[N][N];
                copy(a, board);
                int C = rand() % (N *N - MIN_NUM);
                removeNumbers(board, C); // Remove 40 numbers
                printf("初始棋盘：\n");
                printBoard(board);
                printf("\n\n		   功能菜单 \n");
                printf("--------------------------------------------\n");
                printf("1.电脑自动求解\n");
                printf("2.玩家手动求解\n");
                printf("--------------------------------------------\n");
                printf("	请选择你的操作[1~2]:");
                scanf("%d", &i);
                if (i == 1) {
                    if (solveSudoku(board)) {
                        printf("最终棋盘：\n");
                        printBoard(board);
                    } else {
                        printf("No solution exists\n");
                    }
                } else if (i == 2) {
                    mySolveSudoku(a, board);
                } else {
                    printf("输入错误");
                }
                getchar();
                getchar();
                break;
        }
    }
    printf("欢迎下次使用！");
    return 0;
}

/*
函数名称: ReadFile
接受参数: SATList*&
函数功能: 用文件指针fp打开用户指定的文件，并读取文件内容保存到给定参数中，读取成功返回1，失败返回0
返回值: int
*/
int ReadFile(SATList *&cnf) {
    FILE* fp = fopen(fileName, "r"); //打开文件
    char ch;
    int number;
    SATList* lp;
    SATNode* tp;
    if (!fp) {
        printf("文件打开失败!\n");
        return 0;
    }
    while ((ch = getc(fp)) == 'c') {
        while ((ch = getc(fp)) != '\n')
            continue;           //弃去一整行
    }   //运行到此，已经读取了字符p
    getc(fp);
    getc(fp);
    getc(fp);
    getc(fp); //弃去cnf三个字母
    fscanf(fp, "%d", &boolCount);    //p后的第1个数值是布尔变元数量
    fscanf(fp, "%d", &clauseCount);  //p后的第2个数值是子句数量
    cnf = (SATList *)malloc(sizeof(SATList));
    cnf->next = NULL;
    cnf->head = (SATNode *)malloc(sizeof(SATNode));
    cnf->head->next = NULL;
    lp = cnf;
    tp = cnf->head;

    //创建数量为clauseCount的子句, i为计数器
    for (int i = 0; i < clauseCount; i++, lp = lp->next, tp = lp->head) {
        fscanf(fp, "%d", &number);
        for (; number != 0; tp = tp->next) {
            tp->data = number;  //数据域赋值
            tp->next = (SATNode *)malloc(sizeof(SATNode)); //开辟新结点
            fscanf(fp, "%d", &number);
            if (number == 0)
                tp->next = NULL;
        }
        lp->next = (SATList *)malloc(sizeof(SATList)); //开辟新表
        lp->next->head = (SATNode *)malloc(sizeof(SATNode));
        if (i == clauseCount - 1) {
            lp->next = NULL;
            break;
        }
    }
    printf("读取完毕\n");
    fclose(fp);
    return 1;
}

/*
函数名称: print
接受参数: SATList*
函数功能: 输出cnf链表
返回值: void
*/
void print(SATList *CNFList) {
    printf("cnf子句如下：\n");
    SATList* lp;
    SATNode* tp;
    for (lp = CNFList; lp != NULL; lp = lp->next) {
        for (tp = lp->head; tp != NULL; tp = tp->next) {
            printf("%d ", tp->data);
        }
        printf("\n");
    }
}

/*
函数名称: baseDPLL
接受参数: SATList*&,int *&,int *&
函数功能: 求解SAT句式的基础DPLL算法
返回值: bool
*/
bool baseDPLL(SATList *&cnf, int *&value, int *&count) {
    if (cnf == NULL) { //所有的子句都变成了空句，表明存在解
        return true;
    }
FIND:
    SATList *List = cnf;
    while (List && !isUnitClause(List->head)) { //不是单子句
        if (List->head == NULL) { //所有文字都删除了，子句变成了空句，证明每一个文字都是假
            return false;
        }
        List = List->next;
    }
    if (List) { //存在一个子句有单子句
        //找到了单句
        int e = List->head->data; //e是单子句中的文字，可以直接得出其值
        if (e < 0) { //修改最终结果
            value[-e] = 0;
        }
        Delete(cnf, e, count); //删除所有含有e的子句以及删除-e文字
        goto FIND;
    }
    //所有子句都不是单句
    SATList *new_List;
    CopyClause(new_List, cnf); //暂存数据
    int *new_value = copy(value, boolCount);
    int *new_every_bool_num = copy(count, boolCount);
    int e = 0, max = 0;
    for (int i = 1; i <= boolCount; i++) { //找到出现最多的变元
        if (max < count[i]) {
            max = count[i];
            e = i;
        }
    }
    if (e == 0) {
        return true;
    }
    Delete(cnf, e, count);
    int result = baseDPLL(cnf, value, count);
    if (result) { //e的正项分支正确
        destroyClause(new_List); //释放内存
        free(new_value);
        free(new_every_bool_num);
        return true;
    } else { //该分支错误，采用e的负项分支
        destroyClause(cnf); //释放现有内存
        free(value);
        free(count);
        CopyClause(cnf, new_List); //重新赋值暂存数据
        value = copy(new_value, boolCount);
        count = copy(new_every_bool_num, boolCount);
        value[e] = 0;
        Delete(cnf, -e, count);
        result = baseDPLL(cnf, value, count);
        destroyClause(new_List);
        free(new_value);
        free(new_every_bool_num);
        if (result) { //另一分支存在解
            return true;
        } else { //另一分支也不对，继续回溯到上一分支
            return false;
        }
    }
    if (cnf == NULL) //将所有子句都删除了，即得到了最终解
        return true;
    else //无解
        return false;
}

/*
函数名称: destroyClause
接受参数: SATList*&
函数功能: 销毁链表
返回值: void
*/
void destroyClause(SATList *&cnf) {
    SATList* lp1, *lp2;
    SATNode* tp1, *tp2;
    for (lp1 = cnf; lp1 != NULL; lp1 = lp2) {
        lp2 = lp1->next;
        for (tp1 = lp1->head; tp1 != NULL; tp1 = tp2) {
            tp2 = tp1->next;
            free(tp1);
        }
        free(lp1);
    }
    cnf = NULL;
}

/*
函数名称: isUnitClause
接受参数: SATNode*
函数功能: 判断是否为单子句，是返回true，不是返回false
返回值: bool
*/
bool isUnitClause(SATNode *cnf) {
    if (cnf != NULL && cnf->next == NULL)
        return true;
    else
        return false;
}

/*
函数名称: evaluateClause
接受参数: SATList*,int*
函数功能: 评估子句的真假状态，真返回true，假返回false
返回值: bool
*/
bool evaluateClause(SATNode *cnf, int v[]) {
    SATNode* tp = cnf;
    while (tp != NULL) {
        if ((tp->data > 0 && v[tp->data] == 1) ||
                (tp->data < 0 && v[-tp->data] == 0))
            return true;
    }
    return false;
}

/*
函数名称: removeClause
接受参数: SATList*&,SATList*&
函数功能: 在已有的十字链表中删除指定的子句
返回值: void
*/
void removeClause(SATList *&cnf, SATList *&root) {
    SATList* lp = root;
    if (lp == cnf)
        root = root->next;  //删除为头
    else {
        while (lp != NULL && lp->next != cnf)
            lp = lp->next;
        lp->next = lp->next->next;
    }
    free(cnf);
    cnf = NULL;
}

/*
函数名称: removeNote
接受参数: SATNode*&,SATNode*&
函数功能: 在指定的子句中删除指定的文字
返回值: void
*/
void removeNode(SATNode *&cnf, SATNode *&head) {
    SATNode* lp = head;
    if (lp == cnf)
        head = head->next;  //删除为头
    else {
        while (lp != NULL && lp->next != cnf)
            lp = lp->next;
        lp->next = lp->next->next;
    }
    free(cnf);
    cnf = NULL;
}

/*
函数名称: addClause
接受参数: SATList*,SATList*&
函数功能: 在root链表中添加cnf子句，添加成功返回1，失败返回0
返回值: bool
*/
bool addClause(SATList *cnf, SATList *&root) {
    //直接插入在表头
    if (cnf != NULL) {
        cnf->next = root;
        root = cnf;
        return true;
    }
    return false;
}

/*
函数名称: emptyClause
接受参数: SATList*
函数功能: 判断是否含有空子句，是返回1，不是返回0
返回值: bool
*/
bool emptyClause(SATList *cnf) {
    SATList* lp = cnf;
    while (lp != NULL) {
        if (lp->head == NULL) //有空子句
            return true;
        lp = lp->next;
    }
    return false;
}

/*
函数名称: CopyClause
接受参数: SATList*,SATList*
函数功能: 将链表b的值复制到链表a中
返回值: void
*/
void CopyClause(SATList *&a, SATList *b) {
    SATList* lpa, *lpb;
    SATNode* tpa, *tpb;
    a = (SATList *)malloc(sizeof(SATList));
    a->head = (SATNode *)malloc(sizeof(SATNode));
    a->next = NULL;
    a->head->next = NULL;
    for (lpb = b, lpa = a; lpb != NULL; lpb = lpb->next, lpa = lpa->next) {
        for (tpb = lpb->head, tpa = lpa->head; tpb != NULL; tpb = tpb->next, tpa = tpa->next) {
            tpa->data = tpb->data;
            tpa->next = (SATNode *)malloc(sizeof(SATNode));
            tpa->next->next = NULL;
            if (tpb->next == NULL) {
                free(tpa->next);
                tpa->next = NULL;
            }
        }
        lpa->next = (SATList *)malloc(sizeof(SATList));
        lpa->next->head = (SATNode *)malloc(sizeof(SATNode));
        lpa->next->next = NULL;
        lpa->next->head->next = NULL;
        if (lpb->next == NULL) {
            free(lpa->next->head);
            free(lpa->next);
            lpa->next = NULL;
        }
    }
}

/*
函数名称: optimizeDPLL
接受参数: SATList*&,int *
函数功能: 求解SAT问题，给出满足条件时的一个式子,若有解则返回1，无解返回0
返回值: int
*/
int optimizeDPLL(SATList *&cnf, int value[]) {
    SATList* tp = cnf, *lp = cnf, *sp;
    SATNode* dp;
    int* count, i, MaxWord, max, re; //count记录每个文字出现次数,MaxWord记录出现最多次数的文字
    count = (int *)malloc(sizeof(int) * (boolCount * 2 + 1));
FIND:
    while (tp != NULL && isUnitClause(tp->head) == 0)
        tp = tp->next;  //找到表中的单子句
    if (tp != NULL) {
        //单子句规则简化
        if (tp->head->data > 0)
            value[tp->head->data] = 1;
        else
            value[-tp->head->data] = 0;
        re = tp->head->data;
        for (lp = cnf; lp != NULL; lp = sp) {
            sp = lp->next;

            //查找含有核心文字的句子
            for (dp = lp->head; dp != NULL; dp = dp->next) {
                if (dp->data == re) {
                    removeClause(lp, cnf);  //删除子句，简化式子
                    break;
                }
                if (dp->data == -re) {
                    removeNode(dp, lp->head);  //删除文字，简化子句
                    break;
                }
            }
        }
        //极简化规则简化后
        if (cnf == NULL) {
            free(count);
            return 1;
        } else if (emptyClause(cnf)) {
            free(count);
            destroyClause(cnf);
            return 0;
        }
        tp = cnf;
        goto FIND;  //继续简化
    }
    for (i = 0; i <= boolCount * 2; i++)
        count[i] = 0;  //初始化

    for (lp = cnf; lp != NULL; lp = lp->next) {
        for (dp = lp->head; dp != NULL; dp = dp->next) {
            if (dp->data > 0)
                count[dp->data]++;
            else
                count[boolCount - dp->data]++;
        }
    }
    max = 0;

    //找到出现次数最多的正文字
    for (i = 2; i <= boolCount; i++) {
        if (max < count[i]) {
            max = count[i];
            MaxWord = i;
        }
    }

    if (max == 0) {
        //若没有出现正文字,找到出现次数最多的负文字
        for (i = boolCount + 1; i <= boolCount * 2; i++) {
            if (max < count[i]) {
                max = count[i];
                MaxWord = -i;
            }
        }
    }
    free(count);
    lp = (SATList *)malloc(sizeof(SATList));
    lp->head = (SATNode *)malloc(sizeof(SATNode));
    lp->head->data = MaxWord;
    lp->head->next = NULL;
    lp->next = NULL;
    CopyClause(tp, cnf);
    addClause(lp, tp);
    if (optimizeDPLL(tp, value) == 1)
        return 1;  //在第一分支中搜索
    destroyClause(tp);
    lp = (SATList *)malloc(sizeof(SATList));
    lp->head = (SATNode *)malloc(sizeof(SATNode));
    lp->head->data = -MaxWord;
    lp->head->next = NULL;
    lp->next = NULL;
    addClause(lp, cnf);
    re = optimizeDPLL(cnf, value); //回溯到执行分支策略的初态进入另一分支
    destroyClause(cnf);
    return re;
}

/*
* 函数名称: WriteFile
* 接受参数: int,double,int[]
* 函数功能: 将运行结果保存至同名文件，文件拓展名为.res,保存成功返回1，失败返回0
* 返回值: bool
*/
bool WriteFile(int result, double time, int value[]) {
    FILE* fp;
    int i;
    for (i = 0; fileName[i] != '\0'; i++) {
        //修改拓展名
        if (fileName[i] == '.' && fileName[i + 4] == '\0') {
            fileName[i + 1] = 'r';
            fileName[i + 2] = 'e';
            fileName[i + 3] = 's';
            break;
        }
    }
    if (fopen_s(&fp, fileName, "w")) {
        printf("文件打开失败!\n");
        return false;
    }
    fprintf(fp, "s %d\nv ", result);  //求解结果
    if (result == 1) {
        //保存解值
        for (i = 1; i <= boolCount; i++) {
            if (value[i] == 1)
                fprintf(fp, "%d ", i);
            else
                fprintf(fp, "%d ", -i);
        }
    }
    fprintf(fp, "\nt %lfms", time * 1000);  //运行时间/毫秒
    fclose(fp);
    return true;
}

/*
函数名称: SolvePuzzle
接受参数: int[]
函数功能: 求解输入棋盘，有解返回1，无解返回0
返回值: int
*/
int SolvePuzzle(int chess[]) {
    SATList* cnf = NULL, *lp;
    SATNode* dp;
    int* remember, i, j, k, rol;
    boolCount = N * N;

    //添加单子句
    for (i = 1; i <= N *N; i++) {
        if (chess[i] == 0) {
            lp = (SATList *)malloc(sizeof(SATList));
            lp->head = (SATNode *)malloc(sizeof(SATNode));
            lp->head->next = NULL;
            lp->next = NULL;
            lp->head->data = -i;
            addClause(lp, cnf);
        } else if (chess[i] == 1) {
            lp = (SATList *)malloc(sizeof(SATList));
            lp->head = (SATNode *)malloc(sizeof(SATNode));
            lp->head->next = NULL;
            lp->next = NULL;
            lp->head->data = i;
            addClause(lp, cnf);
        }
    }

    //约束1：不出现连续三个重复数字
    for (i = 0; i < N; i++) {
        for (j = 0; j < N - 2; j++) {
            //横向，正文字
            lp = (SATList *)malloc(sizeof(SATList));
            lp->head = (SATNode *)malloc(sizeof(SATNode));
            lp->head->next = (SATNode *)malloc(sizeof(SATNode));
            lp->head->next->next = (SATNode *)malloc(sizeof(SATNode));
            lp->head->next->next->next = NULL;
            lp->next = NULL;
            lp->head->data = i * N + j + 1;
            lp->head->next->data = i * N + j + 2;
            lp->head->next->next->data = i * N + j + 3;
            addClause(lp, cnf);
            //横向，负文字
            lp = (SATList *)malloc(sizeof(SATList));
            lp->head = (SATNode *)malloc(sizeof(SATNode));
            lp->head->next = (SATNode *)malloc(sizeof(SATNode));
            lp->head->next->next = (SATNode *)malloc(sizeof(SATNode));
            lp->head->next->next->next = NULL;
            lp->next = NULL;
            lp->head->data = -(i *N + j + 1);
            lp->head->next->data = -(i *N + j + 2);
            lp->head->next->next->data = -(i *N + j + 3);
            addClause(lp, cnf);
            //纵向，正文字
            lp = (SATList *)malloc(sizeof(SATList));
            lp->head = (SATNode *)malloc(sizeof(SATNode));
            lp->head->next = (SATNode *)malloc(sizeof(SATNode));
            lp->head->next->next = (SATNode *)malloc(sizeof(SATNode));
            lp->head->next->next->next = NULL;
            lp->next = NULL;
            lp->head->data = i + j * N + 1;
            lp->head->next->data = i + (j + 1) * N + 1;
            lp->head->next->next->data = i + (j + 2) * N + 1;
            addClause(lp, cnf);
            //纵向，负文字
            lp = (SATList *)malloc(sizeof(SATList));
            lp->head = (SATNode *)malloc(sizeof(SATNode));
            lp->head->next = (SATNode *)malloc(sizeof(SATNode));
            lp->head->next->next = (SATNode *)malloc(sizeof(SATNode));
            lp->head->next->next->next = NULL;
            lp->next = NULL;
            lp->head->data = -(i + j *N + 1);
            lp->head->next->data = -(i + (j + 1) * N + 1);
            lp->head->next->next->data = -(i + (j + 2) * N + 1);
            addClause(lp, cnf);
        }
    }

    //约束2：在每一行、每一列中1与0的个数相同
    remember = (int *)malloc(sizeof(int) * (N / 2 + 1));
    //每一行
    for (rol = 0; rol < N; rol++) {
        for (i = 0; i < N / 2 + 1; i++)
            remember[i] = i + 1;  //初始化
COMBINATION1:
        for (i = N / 2; remember[i] <= N; remember[i]++) {
            lp = (SATList *)malloc(sizeof(SATList));
            lp->head = (SATNode *)malloc(sizeof(SATNode));
            lp->head->next = NULL;
            lp->next = NULL;
            for (j = 0, dp = lp->head; j < N / 2 + 1; j++, dp = dp->next) {
                dp->data = remember[j] + rol * N;
                if (j == N / 2)
                    break;
                dp->next = (SATNode *)malloc(sizeof(SATNode));
                dp->next->next = NULL;
            }
            addClause(lp, cnf);
            lp = (SATList *)malloc(sizeof(SATList));
            lp->head = (SATNode *)malloc(sizeof(SATNode));
            lp->head->next = NULL;
            lp->next = NULL;
            for (j = 0, dp = lp->head; j < N / 2 + 1; j++, dp = dp->next) {
                dp->data = -(remember[j] + rol *N);
                if (j == N / 2)
                    break;
                dp->next = (SATNode *)malloc(sizeof(SATNode));
                dp->next->next = NULL;
            }
            addClause(lp, cnf);
        }
        for (i = N / 2; i >= 0 && remember[i] >= N / 2 + i; i--);  //找到达到饱和的最高位
        if (i <= 0)
            continue;  //该行组合序列全部排完,进入下一行
        remember[i]++;
        for (j = i + 1; j < N / 2 + 1; j++)
            remember[j] = remember[j - 1] + 1; //序列后移
        goto COMBINATION1;
    }
    //每一列
    for (rol = 1; rol <= N; rol++) {
        for (i = 0; i < N / 2 + 1; i++)
            remember[i] = i;  //初始化
COMBINATION2:
        for (i = N / 2; remember[i] < N; remember[i]++) {
            lp = (SATList *)malloc(sizeof(SATList));
            lp->head = (SATNode *)malloc(sizeof(SATNode));
            lp->head->next = NULL;
            lp->next = NULL;
            for (j = 0, dp = lp->head; j < N / 2 + 1; j++, dp = dp->next) {
                dp->data = remember[j] * N + rol;
                if (j == N / 2)
                    break;
                dp->next = (SATNode *)malloc(sizeof(SATNode));
                dp->next->next = NULL;
            }
            addClause(lp, cnf);
            lp = (SATList *)malloc(sizeof(SATList));
            lp->head = (SATNode *)malloc(sizeof(SATNode));
            lp->head->next = NULL;
            lp->next = NULL;
            for (j = 0, dp = lp->head; j < N / 2 + 1; j++, dp = dp->next) {
                dp->data = -(remember[j] * N + rol);
                if (j == N / 2)
                    break;
                dp->next = (SATNode *)malloc(sizeof(SATNode));
                dp->next->next = NULL;
            }
            addClause(lp, cnf);
        }
        for (i = N / 2; i >= 0 && remember[i] > N / 2 + i; i--);  //找到达到饱和的最高位
        if (i <= 0)
            continue;  //该列组合序列全部排完,进入下一列
        remember[i]++;
        for (j = i + 1; j < N / 2 + 1; j++)
            remember[j] = remember[j - 1] + 1; //序列后移
        goto COMBINATION2;
    }

    //约束3：不存在重复的行与重复的列
    //不重复行：
    for (i = 0; i < N - 1; i++) {
        for (j = i + 1; j < N; j++) {
            rol = boolCount; //记录添加变元之前变元数量
            for (k = 1; k <= N; k++) {
                //引入前一个新变元：
                boolCount++;
                //语句1：
                lp = (SATList *)malloc(sizeof(SATList));
                lp->head = (SATNode *)malloc(sizeof(SATNode));
                lp->head->next = (SATNode *)malloc(sizeof(SATNode));
                lp->head->next->next = NULL;
                lp->next = NULL;
                lp->head->data = i * N + k;
                lp->head->next->data = -boolCount;
                addClause(lp, cnf);
                //语句2：
                lp = (SATList *)malloc(sizeof(SATList));
                lp->head = (SATNode *)malloc(sizeof(SATNode));
                lp->head->next = (SATNode *)malloc(sizeof(SATNode));
                lp->head->next->next = NULL;
                lp->next = NULL;
                lp->head->data = -(j *N + k);
                lp->head->next->data = -boolCount;
                addClause(lp, cnf);
                //语句3：
                lp = (SATList *)malloc(sizeof(SATList));
                lp->head = (SATNode *)malloc(sizeof(SATNode));
                lp->head->next = (SATNode *)malloc(sizeof(SATNode));
                lp->head->next->next = (SATNode *)malloc(sizeof(SATNode));
                lp->head->next->next->next = NULL;
                lp->next = NULL;
                lp->head->data = -(i *N + k);
                lp->head->next->data = j * N + k;
                lp->head->next->next->data = boolCount;
                addClause(lp, cnf);
                //引入后一个新变元：
                boolCount++;
                //语句1：
                lp = (SATList *)malloc(sizeof(SATList));
                lp->head = (SATNode *)malloc(sizeof(SATNode));
                lp->head->next = (SATNode *)malloc(sizeof(SATNode));
                lp->head->next->next = NULL;
                lp->next = NULL;
                lp->head->data = -(i *N + k);
                lp->head->next->data = -boolCount;
                addClause(lp, cnf);
                //语句2：
                lp = (SATList *)malloc(sizeof(SATList));
                lp->head = (SATNode *)malloc(sizeof(SATNode));
                lp->head->next = (SATNode *)malloc(sizeof(SATNode));
                lp->head->next->next = NULL;
                lp->next = NULL;
                lp->head->data = j * N + k;
                lp->head->next->data = -boolCount;
                addClause(lp, cnf);
                //语句3：
                lp = (SATList *)malloc(sizeof(SATList));
                lp->head = (SATNode *)malloc(sizeof(SATNode));
                lp->head->next = (SATNode *)malloc(sizeof(SATNode));
                lp->head->next->next = (SATNode *)malloc(sizeof(SATNode));
                lp->head->next->next->next = NULL;
                lp->next = NULL;
                lp->head->data = i * N + k;
                lp->head->next->data = -(j *N + k);
                lp->head->next->next->data = boolCount;
                addClause(lp, cnf);
            }
            //添加长句：不重复行满足的关系
            lp = (SATList *)malloc(sizeof(SATList));
            lp->head = (SATNode *)malloc(sizeof(SATNode));
            lp->head->next = NULL;
            lp->next = NULL;
            for (k = rol + 1, dp = lp->head; k <= boolCount; k++, dp = dp->next) {
                dp->data = k;
                if (k == boolCount)
                    break;
                dp->next = (SATNode *)malloc(sizeof(SATNode));
                dp->next->next = NULL;
            }
            addClause(lp, cnf);
        }
    }
    //不重复列
    for (i = 1; i <= N - 1; i++) {
        for (j = i + 1; j <= N; j++) {
            rol = boolCount; //记录添加变元之前变元数量
            for (k = 0; k < N; k++) {
                //引入前一个新变元：
                boolCount++;
                //语句1：
                lp = (SATList *)malloc(sizeof(SATList));
                lp->head = (SATNode *)malloc(sizeof(SATNode));
                lp->head->next = (SATNode *)malloc(sizeof(SATNode));
                lp->head->next->next = NULL;
                lp->next = NULL;
                lp->head->data = i + k * N;
                lp->head->next->data = -boolCount;
                addClause(lp, cnf);
                //语句2：
                lp = (SATList *)malloc(sizeof(SATList));
                lp->head = (SATNode *)malloc(sizeof(SATNode));
                lp->head->next = (SATNode *)malloc(sizeof(SATNode));
                lp->head->next->next = NULL;
                lp->next = NULL;
                lp->head->data = -(j + k *N);
                lp->head->next->data = -boolCount;
                addClause(lp, cnf);
                //语句3：
                lp = (SATList *)malloc(sizeof(SATList));
                lp->head = (SATNode *)malloc(sizeof(SATNode));
                lp->head->next = (SATNode *)malloc(sizeof(SATNode));
                lp->head->next->next = (SATNode *)malloc(sizeof(SATNode));
                lp->head->next->next->next = NULL;
                lp->next = NULL;
                lp->head->data = -(i + k *N);
                lp->head->next->data = j + k * N;
                lp->head->next->next->data = boolCount;
                addClause(lp, cnf);
                //引入后一个新变元：
                boolCount++;
                //语句1：
                lp = (SATList *)malloc(sizeof(SATList));
                lp->head = (SATNode *)malloc(sizeof(SATNode));
                lp->head->next = (SATNode *)malloc(sizeof(SATNode));
                lp->head->next->next = NULL;
                lp->next = NULL;
                lp->head->data = -(i + k *N);
                lp->head->next->data = -boolCount;
                addClause(lp, cnf);
                //语句2：
                lp = (SATList *)malloc(sizeof(SATList));
                lp->head = (SATNode *)malloc(sizeof(SATNode));
                lp->head->next = (SATNode *)malloc(sizeof(SATNode));
                lp->head->next->next = NULL;
                lp->next = NULL;
                lp->head->data = j + k * N;
                lp->head->next->data = -boolCount;
                addClause(lp, cnf);
                //语句3：
                lp = (SATList *)malloc(sizeof(SATList));
                lp->head = (SATNode *)malloc(sizeof(SATNode));
                lp->head->next = (SATNode *)malloc(sizeof(SATNode));
                lp->head->next->next = (SATNode *)malloc(sizeof(SATNode));
                lp->head->next->next->next = NULL;
                lp->next = NULL;
                lp->head->data = i + k * N;
                lp->head->next->data = -(j + k *N);
                lp->head->next->next->data = boolCount;
                addClause(lp, cnf);
            }
            //添加长句：不重复行满足的关系
            lp = (SATList *)malloc(sizeof(SATList));
            lp->head = (SATNode *)malloc(sizeof(SATNode));
            lp->head->next = NULL;
            lp->next = NULL;
            for (k = rol + 1, dp = lp->head; k <= boolCount; k++, dp = dp->next) {
                dp->data = k;
                if (k == boolCount)
                    break;
                dp->next = (SATNode *)malloc(sizeof(SATNode));
                dp->next->next = NULL;
            }
            addClause(lp, cnf);
        }
    }
    free(remember);
    remember = (int *)malloc(sizeof(int) * (boolCount + 1));
    for (i = 1; i <= boolCount; i++)
        remember[i] = 1;  //初始化
    if (optimizeDPLL(cnf, remember) == 1) {
        for (i = 1; i <= N *N; i++)
            chess[i] = remember[i];
        free(remember);
        destroyClause(cnf);
        return 1;
    } else {
        free(remember);
        destroyClause(cnf);
        return 0;
    }
}

/*
函数名称: printBoard
接受参数: int[][]
函数功能: 输出棋盘
返回值: void
*/
void printBoard(int board[N][N]) {
    for (int r = 0; r < N; r++) {
        for (int d = 0; d < N; d++) {
            if (board[r][d])
                printf("%d ", board[r][d]);
            else
                printf("- ");
        }
        printf("\n");
    }
}

/*
函数名称: isSafe
接受参数: int[][],int,int,int
函数功能: 判断数独的输入或求解是否满足数独条件
返回值: bool
*/
bool isSafe(int board[N][N], int row, int col, int num) {
    for (int x = 0; x < N; x++) {
        if (board[row][x] == num || board[x][col] == num) {
            return false;
        }
    }
    int startRow = row / 3 * 3, startCol = col / 3 * 3;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i + startRow][j + startCol] == num) {
                return false;
            }
        }
    }
    if (row == col) {
        for (int x = 0; x < N; x++) {
            if (board[x][x] == num)
                return false;
        }
    }
    if (row + col == N - 1) {
        for (int x = 0; x < N; x++) {
            if (board[x][N - 1 - x] == num)
                return false;
        }
    }
    return true;
}

/*
函数名称: solveSudoku
接受参数: int[][]
函数功能: 回溯法求解数独
返回值: bool
*/
bool solveSudoku(int board[N][N]) {
    for (int row = 0; row < N; row++) {
        for (int col = 0; col < N; col++) {
            if (board[row][col] == 0) {
                for (int num = 1; num <= 9; num++) {
                    if (isSafe(board, row, col, num)) {
                        board[row][col] = num;
                        if (solveSudoku(board)) {
                            return true;
                        }
                        board[row][col] = 0;
                    }
                }
                return false;
            }
        }
    }
    return true;
}

/*
函数名称: init
接受参数: int[][]
函数功能: 初始化数独
返回值: void
*/
void init(int board[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            board[i][j] = 0;
        }
    }
}

/*
函数名称: fillBoard
接受参数: int[][]
函数功能: 填充数独数字
返回值: void
*/
void fillBoard(int board[N][N]) {
    for (int num = 1; num <= 9; num++) {
        int row = rand() % N;
        int col = rand() % N;
        while (!isSafe(board, row, col, num)) {
            row = rand() % N;
            col = rand() % N;
        }
        board[row][col] = num;
    }
    solveSudoku(board);
}

/*
函数名称: init
接受参数: int[][],int
函数功能: 挖洞法将数独挖空
返回值: void
*/
void removeNumbers(int board[N][N], int count) {
    while (count != 0) {
        int i = rand() % N;
        int j = rand() % N;
        if (board[i][j] != 0) {
            board[i][j] = 0;
            count--;
        }
    }
}

/*
函数名称: conpute_literal_num
接受参数: SATList *,int*
函数功能: 计算十字链表中每一个变元的数量，保存在count数组中
返回值: void
*/
void compute_literal_num(SATList *cnf, int *count) {
    for (SATList *lp = cnf; lp != NULL; lp = lp->next) {
        for (SATNode *dp = lp->head; dp != NULL; dp = dp->next) {
            count[abs(dp->data)]++;
        }
    }
}

/*
函数名称: Delete
接受参数: SATList *&,SATList *&,SATList *&,int *
函数功能: 删除cnf链表中List指向的子句，pre为List的前继指针
返回值: void
*/
void Delete(SATList *&cnf, SATList *&pre, SATList *&List, int *count) {
    SATNode *p = List->head, *next = p->next;
    while (next != NULL) {
        count[abs(p->data)]--;
        free(p);
        p = next;
        next = p->next;
    }
    count[abs(p->data)]--;
    free(p); //释放每个文字的指针空间
    p = next = NULL;
    if (List == cnf) {
        cnf = List->next;
        pre = cnf;
    } else {
        pre->next = List->next;
    }
    free(List); //释放子句的指针空间
    List = NULL;
}

/*
函数名称: Delete
接受参数: SATLis *&,int,int*
函数功能: 删除cnf链表中含有literal文字的子句以及-literal的文字
返回值: void
*/
void Delete(SATList *&cnf, int literal, int *count) {
    int flag = 0;
    SATList *List = cnf, *pre = cnf;
    while (List) {
        SATNode *Node = List->head, *pre_Node = Node;
        while (Node) {
            if (Node->data == literal) { //在非单子句中找到了变元e，直接删除整个子句
                Delete(cnf, pre, List, count); //删除了整个子句
                Node = NULL;
                List = pre;
                flag = 1;
                break;
            } else if (Node->data == -literal) { //-e为假，只删除该文字，由子句中其他文字判断真假
                if (Node == pre_Node) { //被删除的文字在链头
                    pre_Node = Node->next;
                    count[abs(literal)]--;
                    free(Node);
                    Node = pre_Node;
                    List->head = Node; //改变头结点
                } else { //被删除的文字不在链头
                    pre_Node->next = Node->next;
                    count[abs(literal)]--;
                    free(Node);
                    Node = pre_Node->next;
                }
            } else { //不是e或-e
                pre_Node = Node;
                Node = Node->next;
            }
        }
        pre = List;
        if (flag == 0)
            List = pre->next;
        flag = 0;
    }
}

/*
函数名称: copy
接受参数: int*,int
函数功能: 复制a数组
返回值: int *
*/
int *copy(int *a, int n) {
    int *arr = (int *)malloc(sizeof(int) * (n + 1));
    for (int i = 0; i <= n; i++) {
        arr[i] = a[i];
    }
    return arr;
}

/*
函数名称: init
接受参数: int*&,int
函数功能: 初始化数组
返回值: void
*/
void init(int *&arr, int e) {
    arr = (int *)malloc(sizeof(int) * (boolCount + 1));
    for (int i = 1; i <= boolCount; i++) {
        arr[i] = e;
    }
}

/*
函数名称: copy
接受参数: int [][],int [][]
函数功能: 将数组b复制给数组a
返回值: void
*/
void copy(int a[N][N], int b[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            a[i][j] = b[i][j];
        }
    }
}

/*
函数名称: mySolveSudoku
接受参数: int [][],int [][]
函数功能: 玩家自己求解数独游戏
返回值: void
*/
void mySolveSudoku(int a[N][N], int board[N][N]) {
    int op = 1;
    while (op) {
        printf("\n\n		   功能菜单 \n");
        printf("--------------------------------------------\n");
        printf("1.填入数字\n");
        printf("2.查看棋盘\n");
        printf("0.退出游戏\n");
        printf("--------------------------------------------\n");
        printf("	请选择你的操作[0~2]:");
        scanf("%d", &op);
        if (op == 1) {
            int line, con, num;
            printf("请输入行、列、值：");
            scanf("%d %d %d", &line, &con, &num);
            if (a[line - 1][con - 1] == num) {
                printf("填入成功\n");
                board[line - 1][con - 1] = num;
            } else {
                printf("填入失败，请重新输入\n");
            }
        } else if (op == 2) {
            printBoard(board);
        } else if (op == 0) {
            printf("欢迎下次游戏！");
        } else {
            printf("输入错误，请重新输入\n");
        }
        getchar();
        getchar();
    }
}