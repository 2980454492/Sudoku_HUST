#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 9 //�����ı߳�
#define MIN_NUM 17 //����Ҫ�뱣֤��Ψһ�⣬���ٵĳ�ʼ���ָ�������17�����Ǿ�������ʵ��۲�õ���
int boolCount;  //������Ԫ����
int clauseCount;  //�Ӿ�����
char fileName[100]; //�ļ���

//ʮ������ṹ��
typedef struct SATNode {
    int data;  //������
    SATNode *next;
} SATNode;

typedef struct SATList {
    SATNode *head;  //��ͷ
    SATList *next;
} SATList;

//��������
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

//��������
/*
��������: main
���ܲ���: void
��������: ������
����ֵ: int
 */
int main(void) {
    srand(time_t(NULL));
    SATList* CNFList = NULL;
    int board[N][N];
    clock_t start, finish;  //����ʱ�����
    double time;
    int op = 1, i, result;
    int* value, *count; //��������ÿ����Ԫ���ֵ�����
    while (op) {
        system("cls");
        printf("\n\n		   ���ܲ˵� \n");
        printf("--------------------------------------------\n");
        printf("1.��ȡcnf�ļ�	         2.�������ÿ���Ӿ�\n");
        printf("3.����DPLL�������       4.�Ż�DPLL�������\n");
        printf("5.������Ϸ\n");
        printf("0.�˳�\n");
        printf("--------------------------------------------\n");
        printf("	��ѡ����Ĳ���[0~6]:");
        scanf("%d", &op);
        switch (op) {
            case 1:
                printf("����Ҫ��ȡ��cnf�ļ�:");
                scanf("%s", fileName);
                ReadFile(CNFList);
                getchar();
                getchar();
                break;
            case 2:
                if (CNFList == NULL)
                    printf("δ�����ļ�\n");
                else {
                    print(CNFList);
                }
                getchar();
                getchar();
                break;
            case 3:
                if (CNFList == NULL)
                    printf("δ�����ļ�\n");
                else {
                    init(value, 1);
                    init(count);
                    compute_literal_num(CNFList, count);
                    start = clock();  //��ʱ��ʼ;
                    result = baseDPLL(CNFList, value, count);
                    finish = clock();    //����
                    printf("�������%d\n", result);
                    if (result == 1) {
                        for (i = 1; i <= boolCount; i++) {
                            if (value[i] == 1)
                                printf("%d ", i);
                            else
                                printf("%d ", -i);
                        }
                        printf("\n");
                    }
                    time = (double)(finish - start) / CLOCKS_PER_SEC * 1000; //��������ʱ��
                    printf("start=%ld,finish=%ld,����ʱ��=%lf ms\n", start, finish, 1.0 * time); //�������ʱ��
                    WriteFile(result, time, value);
                    destroyClause(CNFList);
                    printf("�ļ�����ɹ�\n");
                }
                getchar();
                getchar();
                break;
            case 4:
                if (CNFList == NULL)
                    printf("δ�����ļ�\n");
                else {
                    value = (int *)malloc(sizeof(int) * (boolCount + 1));
                    for (i = 1; i <= boolCount; i++)
                        value[i] = 1;  //��ʼ��������Ϊ1
                    start = clock();  //��ʱ��ʼ;
                    result = optimizeDPLL(CNFList, value);
                    finish = clock();    //����
                    printf("�������%d\n", result);
                    if (result == 1) {
                        for (i = 1; i <= boolCount; i++) {
                            if (value[i] == 1)
                                printf("%d ", i);
                            else
                                printf("%d ", -i);
                        }
                        printf("\n");
                    }
                    time = (double)(finish - start) / CLOCKS_PER_SEC;//��������ʱ��
                    printf("����ʱ��=%lfms\n", time * 1000);//�������ʱ��
                    if (WriteFile(result, time, value) == 1)
                        printf("����ѱ�����ͬ���ļ�.res\n");
                    else
                        printf("�������ʧ��\n");
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
                printf("��ʼ���̣�\n");
                printBoard(board);
                printf("\n\n		   ���ܲ˵� \n");
                printf("--------------------------------------------\n");
                printf("1.�����Զ����\n");
                printf("2.����ֶ����\n");
                printf("--------------------------------------------\n");
                printf("	��ѡ����Ĳ���[1~2]:");
                scanf("%d", &i);
                if (i == 1) {
                    if (solveSudoku(board)) {
                        printf("�������̣�\n");
                        printBoard(board);
                    } else {
                        printf("No solution exists\n");
                    }
                } else if (i == 2) {
                    mySolveSudoku(a, board);
                } else {
                    printf("�������");
                }
                getchar();
                getchar();
                break;
        }
    }
    printf("��ӭ�´�ʹ�ã�");
    return 0;
}

/*
��������: ReadFile
���ܲ���: SATList*&
��������: ���ļ�ָ��fp���û�ָ�����ļ�������ȡ�ļ����ݱ��浽���������У���ȡ�ɹ�����1��ʧ�ܷ���0
����ֵ: int
*/
int ReadFile(SATList *&cnf) {
    FILE* fp = fopen(fileName, "r"); //���ļ�
    char ch;
    int number;
    SATList* lp;
    SATNode* tp;
    if (!fp) {
        printf("�ļ���ʧ��!\n");
        return 0;
    }
    while ((ch = getc(fp)) == 'c') {
        while ((ch = getc(fp)) != '\n')
            continue;           //��ȥһ����
    }   //���е��ˣ��Ѿ���ȡ���ַ�p
    getc(fp);
    getc(fp);
    getc(fp);
    getc(fp); //��ȥcnf������ĸ
    fscanf(fp, "%d", &boolCount);    //p��ĵ�1����ֵ�ǲ�����Ԫ����
    fscanf(fp, "%d", &clauseCount);  //p��ĵ�2����ֵ���Ӿ�����
    cnf = (SATList *)malloc(sizeof(SATList));
    cnf->next = NULL;
    cnf->head = (SATNode *)malloc(sizeof(SATNode));
    cnf->head->next = NULL;
    lp = cnf;
    tp = cnf->head;

    //��������ΪclauseCount���Ӿ�, iΪ������
    for (int i = 0; i < clauseCount; i++, lp = lp->next, tp = lp->head) {
        fscanf(fp, "%d", &number);
        for (; number != 0; tp = tp->next) {
            tp->data = number;  //������ֵ
            tp->next = (SATNode *)malloc(sizeof(SATNode)); //�����½��
            fscanf(fp, "%d", &number);
            if (number == 0)
                tp->next = NULL;
        }
        lp->next = (SATList *)malloc(sizeof(SATList)); //�����±�
        lp->next->head = (SATNode *)malloc(sizeof(SATNode));
        if (i == clauseCount - 1) {
            lp->next = NULL;
            break;
        }
    }
    printf("��ȡ���\n");
    fclose(fp);
    return 1;
}

/*
��������: print
���ܲ���: SATList*
��������: ���cnf����
����ֵ: void
*/
void print(SATList *CNFList) {
    printf("cnf�Ӿ����£�\n");
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
��������: baseDPLL
���ܲ���: SATList*&,int *&,int *&
��������: ���SAT��ʽ�Ļ���DPLL�㷨
����ֵ: bool
*/
bool baseDPLL(SATList *&cnf, int *&value, int *&count) {
    if (cnf == NULL) { //���е��Ӿ䶼����˿վ䣬�������ڽ�
        return true;
    }
FIND:
    SATList *List = cnf;
    while (List && !isUnitClause(List->head)) { //���ǵ��Ӿ�
        if (List->head == NULL) { //�������ֶ�ɾ���ˣ��Ӿ����˿վ䣬֤��ÿһ�����ֶ��Ǽ�
            return false;
        }
        List = List->next;
    }
    if (List) { //����һ���Ӿ��е��Ӿ�
        //�ҵ��˵���
        int e = List->head->data; //e�ǵ��Ӿ��е����֣�����ֱ�ӵó���ֵ
        if (e < 0) { //�޸����ս��
            value[-e] = 0;
        }
        Delete(cnf, e, count); //ɾ�����к���e���Ӿ��Լ�ɾ��-e����
        goto FIND;
    }
    //�����Ӿ䶼���ǵ���
    SATList *new_List;
    CopyClause(new_List, cnf); //�ݴ�����
    int *new_value = copy(value, boolCount);
    int *new_every_bool_num = copy(count, boolCount);
    int e = 0, max = 0;
    for (int i = 1; i <= boolCount; i++) { //�ҵ��������ı�Ԫ
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
    if (result) { //e�������֧��ȷ
        destroyClause(new_List); //�ͷ��ڴ�
        free(new_value);
        free(new_every_bool_num);
        return true;
    } else { //�÷�֧���󣬲���e�ĸ����֧
        destroyClause(cnf); //�ͷ������ڴ�
        free(value);
        free(count);
        CopyClause(cnf, new_List); //���¸�ֵ�ݴ�����
        value = copy(new_value, boolCount);
        count = copy(new_every_bool_num, boolCount);
        value[e] = 0;
        Delete(cnf, -e, count);
        result = baseDPLL(cnf, value, count);
        destroyClause(new_List);
        free(new_value);
        free(new_every_bool_num);
        if (result) { //��һ��֧���ڽ�
            return true;
        } else { //��һ��֧Ҳ���ԣ��������ݵ���һ��֧
            return false;
        }
    }
    if (cnf == NULL) //�������Ӿ䶼ɾ���ˣ����õ������ս�
        return true;
    else //�޽�
        return false;
}

/*
��������: destroyClause
���ܲ���: SATList*&
��������: ��������
����ֵ: void
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
��������: isUnitClause
���ܲ���: SATNode*
��������: �ж��Ƿ�Ϊ���Ӿ䣬�Ƿ���true�����Ƿ���false
����ֵ: bool
*/
bool isUnitClause(SATNode *cnf) {
    if (cnf != NULL && cnf->next == NULL)
        return true;
    else
        return false;
}

/*
��������: evaluateClause
���ܲ���: SATList*,int*
��������: �����Ӿ�����״̬���淵��true���ٷ���false
����ֵ: bool
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
��������: removeClause
���ܲ���: SATList*&,SATList*&
��������: �����е�ʮ��������ɾ��ָ�����Ӿ�
����ֵ: void
*/
void removeClause(SATList *&cnf, SATList *&root) {
    SATList* lp = root;
    if (lp == cnf)
        root = root->next;  //ɾ��Ϊͷ
    else {
        while (lp != NULL && lp->next != cnf)
            lp = lp->next;
        lp->next = lp->next->next;
    }
    free(cnf);
    cnf = NULL;
}

/*
��������: removeNote
���ܲ���: SATNode*&,SATNode*&
��������: ��ָ�����Ӿ���ɾ��ָ��������
����ֵ: void
*/
void removeNode(SATNode *&cnf, SATNode *&head) {
    SATNode* lp = head;
    if (lp == cnf)
        head = head->next;  //ɾ��Ϊͷ
    else {
        while (lp != NULL && lp->next != cnf)
            lp = lp->next;
        lp->next = lp->next->next;
    }
    free(cnf);
    cnf = NULL;
}

/*
��������: addClause
���ܲ���: SATList*,SATList*&
��������: ��root���������cnf�Ӿ䣬��ӳɹ�����1��ʧ�ܷ���0
����ֵ: bool
*/
bool addClause(SATList *cnf, SATList *&root) {
    //ֱ�Ӳ����ڱ�ͷ
    if (cnf != NULL) {
        cnf->next = root;
        root = cnf;
        return true;
    }
    return false;
}

/*
��������: emptyClause
���ܲ���: SATList*
��������: �ж��Ƿ��п��Ӿ䣬�Ƿ���1�����Ƿ���0
����ֵ: bool
*/
bool emptyClause(SATList *cnf) {
    SATList* lp = cnf;
    while (lp != NULL) {
        if (lp->head == NULL) //�п��Ӿ�
            return true;
        lp = lp->next;
    }
    return false;
}

/*
��������: CopyClause
���ܲ���: SATList*,SATList*
��������: ������b��ֵ���Ƶ�����a��
����ֵ: void
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
��������: optimizeDPLL
���ܲ���: SATList*&,int *
��������: ���SAT���⣬������������ʱ��һ��ʽ��,���н��򷵻�1���޽ⷵ��0
����ֵ: int
*/
int optimizeDPLL(SATList *&cnf, int value[]) {
    SATList* tp = cnf, *lp = cnf, *sp;
    SATNode* dp;
    int* count, i, MaxWord, max, re; //count��¼ÿ�����ֳ��ִ���,MaxWord��¼����������������
    count = (int *)malloc(sizeof(int) * (boolCount * 2 + 1));
FIND:
    while (tp != NULL && isUnitClause(tp->head) == 0)
        tp = tp->next;  //�ҵ����еĵ��Ӿ�
    if (tp != NULL) {
        //���Ӿ�����
        if (tp->head->data > 0)
            value[tp->head->data] = 1;
        else
            value[-tp->head->data] = 0;
        re = tp->head->data;
        for (lp = cnf; lp != NULL; lp = sp) {
            sp = lp->next;

            //���Һ��к������ֵľ���
            for (dp = lp->head; dp != NULL; dp = dp->next) {
                if (dp->data == re) {
                    removeClause(lp, cnf);  //ɾ���Ӿ䣬��ʽ��
                    break;
                }
                if (dp->data == -re) {
                    removeNode(dp, lp->head);  //ɾ�����֣����Ӿ�
                    break;
                }
            }
        }
        //���򻯹���򻯺�
        if (cnf == NULL) {
            free(count);
            return 1;
        } else if (emptyClause(cnf)) {
            free(count);
            destroyClause(cnf);
            return 0;
        }
        tp = cnf;
        goto FIND;  //������
    }
    for (i = 0; i <= boolCount * 2; i++)
        count[i] = 0;  //��ʼ��

    for (lp = cnf; lp != NULL; lp = lp->next) {
        for (dp = lp->head; dp != NULL; dp = dp->next) {
            if (dp->data > 0)
                count[dp->data]++;
            else
                count[boolCount - dp->data]++;
        }
    }
    max = 0;

    //�ҵ����ִ�������������
    for (i = 2; i <= boolCount; i++) {
        if (max < count[i]) {
            max = count[i];
            MaxWord = i;
        }
    }

    if (max == 0) {
        //��û�г���������,�ҵ����ִ������ĸ�����
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
        return 1;  //�ڵ�һ��֧������
    destroyClause(tp);
    lp = (SATList *)malloc(sizeof(SATList));
    lp->head = (SATNode *)malloc(sizeof(SATNode));
    lp->head->data = -MaxWord;
    lp->head->next = NULL;
    lp->next = NULL;
    addClause(lp, cnf);
    re = optimizeDPLL(cnf, value); //���ݵ�ִ�з�֧���Եĳ�̬������һ��֧
    destroyClause(cnf);
    return re;
}

/*
* ��������: WriteFile
* ���ܲ���: int,double,int[]
* ��������: �����н��������ͬ���ļ����ļ���չ��Ϊ.res,����ɹ�����1��ʧ�ܷ���0
* ����ֵ: bool
*/
bool WriteFile(int result, double time, int value[]) {
    FILE* fp;
    int i;
    for (i = 0; fileName[i] != '\0'; i++) {
        //�޸���չ��
        if (fileName[i] == '.' && fileName[i + 4] == '\0') {
            fileName[i + 1] = 'r';
            fileName[i + 2] = 'e';
            fileName[i + 3] = 's';
            break;
        }
    }
    if (fopen_s(&fp, fileName, "w")) {
        printf("�ļ���ʧ��!\n");
        return false;
    }
    fprintf(fp, "s %d\nv ", result);  //�����
    if (result == 1) {
        //�����ֵ
        for (i = 1; i <= boolCount; i++) {
            if (value[i] == 1)
                fprintf(fp, "%d ", i);
            else
                fprintf(fp, "%d ", -i);
        }
    }
    fprintf(fp, "\nt %lfms", time * 1000);  //����ʱ��/����
    fclose(fp);
    return true;
}

/*
��������: SolvePuzzle
���ܲ���: int[]
��������: ����������̣��нⷵ��1���޽ⷵ��0
����ֵ: int
*/
int SolvePuzzle(int chess[]) {
    SATList* cnf = NULL, *lp;
    SATNode* dp;
    int* remember, i, j, k, rol;
    boolCount = N * N;

    //��ӵ��Ӿ�
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

    //Լ��1�����������������ظ�����
    for (i = 0; i < N; i++) {
        for (j = 0; j < N - 2; j++) {
            //����������
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
            //���򣬸�����
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
            //����������
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
            //���򣬸�����
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

    //Լ��2����ÿһ�С�ÿһ����1��0�ĸ�����ͬ
    remember = (int *)malloc(sizeof(int) * (N / 2 + 1));
    //ÿһ��
    for (rol = 0; rol < N; rol++) {
        for (i = 0; i < N / 2 + 1; i++)
            remember[i] = i + 1;  //��ʼ��
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
        for (i = N / 2; i >= 0 && remember[i] >= N / 2 + i; i--);  //�ҵ��ﵽ���͵����λ
        if (i <= 0)
            continue;  //�����������ȫ������,������һ��
        remember[i]++;
        for (j = i + 1; j < N / 2 + 1; j++)
            remember[j] = remember[j - 1] + 1; //���к���
        goto COMBINATION1;
    }
    //ÿһ��
    for (rol = 1; rol <= N; rol++) {
        for (i = 0; i < N / 2 + 1; i++)
            remember[i] = i;  //��ʼ��
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
        for (i = N / 2; i >= 0 && remember[i] > N / 2 + i; i--);  //�ҵ��ﵽ���͵����λ
        if (i <= 0)
            continue;  //�����������ȫ������,������һ��
        remember[i]++;
        for (j = i + 1; j < N / 2 + 1; j++)
            remember[j] = remember[j - 1] + 1; //���к���
        goto COMBINATION2;
    }

    //Լ��3���������ظ��������ظ�����
    //���ظ��У�
    for (i = 0; i < N - 1; i++) {
        for (j = i + 1; j < N; j++) {
            rol = boolCount; //��¼��ӱ�Ԫ֮ǰ��Ԫ����
            for (k = 1; k <= N; k++) {
                //����ǰһ���±�Ԫ��
                boolCount++;
                //���1��
                lp = (SATList *)malloc(sizeof(SATList));
                lp->head = (SATNode *)malloc(sizeof(SATNode));
                lp->head->next = (SATNode *)malloc(sizeof(SATNode));
                lp->head->next->next = NULL;
                lp->next = NULL;
                lp->head->data = i * N + k;
                lp->head->next->data = -boolCount;
                addClause(lp, cnf);
                //���2��
                lp = (SATList *)malloc(sizeof(SATList));
                lp->head = (SATNode *)malloc(sizeof(SATNode));
                lp->head->next = (SATNode *)malloc(sizeof(SATNode));
                lp->head->next->next = NULL;
                lp->next = NULL;
                lp->head->data = -(j *N + k);
                lp->head->next->data = -boolCount;
                addClause(lp, cnf);
                //���3��
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
                //�����һ���±�Ԫ��
                boolCount++;
                //���1��
                lp = (SATList *)malloc(sizeof(SATList));
                lp->head = (SATNode *)malloc(sizeof(SATNode));
                lp->head->next = (SATNode *)malloc(sizeof(SATNode));
                lp->head->next->next = NULL;
                lp->next = NULL;
                lp->head->data = -(i *N + k);
                lp->head->next->data = -boolCount;
                addClause(lp, cnf);
                //���2��
                lp = (SATList *)malloc(sizeof(SATList));
                lp->head = (SATNode *)malloc(sizeof(SATNode));
                lp->head->next = (SATNode *)malloc(sizeof(SATNode));
                lp->head->next->next = NULL;
                lp->next = NULL;
                lp->head->data = j * N + k;
                lp->head->next->data = -boolCount;
                addClause(lp, cnf);
                //���3��
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
            //��ӳ��䣺���ظ�������Ĺ�ϵ
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
    //���ظ���
    for (i = 1; i <= N - 1; i++) {
        for (j = i + 1; j <= N; j++) {
            rol = boolCount; //��¼��ӱ�Ԫ֮ǰ��Ԫ����
            for (k = 0; k < N; k++) {
                //����ǰһ���±�Ԫ��
                boolCount++;
                //���1��
                lp = (SATList *)malloc(sizeof(SATList));
                lp->head = (SATNode *)malloc(sizeof(SATNode));
                lp->head->next = (SATNode *)malloc(sizeof(SATNode));
                lp->head->next->next = NULL;
                lp->next = NULL;
                lp->head->data = i + k * N;
                lp->head->next->data = -boolCount;
                addClause(lp, cnf);
                //���2��
                lp = (SATList *)malloc(sizeof(SATList));
                lp->head = (SATNode *)malloc(sizeof(SATNode));
                lp->head->next = (SATNode *)malloc(sizeof(SATNode));
                lp->head->next->next = NULL;
                lp->next = NULL;
                lp->head->data = -(j + k *N);
                lp->head->next->data = -boolCount;
                addClause(lp, cnf);
                //���3��
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
                //�����һ���±�Ԫ��
                boolCount++;
                //���1��
                lp = (SATList *)malloc(sizeof(SATList));
                lp->head = (SATNode *)malloc(sizeof(SATNode));
                lp->head->next = (SATNode *)malloc(sizeof(SATNode));
                lp->head->next->next = NULL;
                lp->next = NULL;
                lp->head->data = -(i + k *N);
                lp->head->next->data = -boolCount;
                addClause(lp, cnf);
                //���2��
                lp = (SATList *)malloc(sizeof(SATList));
                lp->head = (SATNode *)malloc(sizeof(SATNode));
                lp->head->next = (SATNode *)malloc(sizeof(SATNode));
                lp->head->next->next = NULL;
                lp->next = NULL;
                lp->head->data = j + k * N;
                lp->head->next->data = -boolCount;
                addClause(lp, cnf);
                //���3��
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
            //��ӳ��䣺���ظ�������Ĺ�ϵ
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
        remember[i] = 1;  //��ʼ��
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
��������: printBoard
���ܲ���: int[][]
��������: �������
����ֵ: void
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
��������: isSafe
���ܲ���: int[][],int,int,int
��������: �ж����������������Ƿ�������������
����ֵ: bool
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
��������: solveSudoku
���ܲ���: int[][]
��������: ���ݷ��������
����ֵ: bool
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
��������: init
���ܲ���: int[][]
��������: ��ʼ������
����ֵ: void
*/
void init(int board[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            board[i][j] = 0;
        }
    }
}

/*
��������: fillBoard
���ܲ���: int[][]
��������: �����������
����ֵ: void
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
��������: init
���ܲ���: int[][],int
��������: �ڶ����������ڿ�
����ֵ: void
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
��������: conpute_literal_num
���ܲ���: SATList *,int*
��������: ����ʮ��������ÿһ����Ԫ��������������count������
����ֵ: void
*/
void compute_literal_num(SATList *cnf, int *count) {
    for (SATList *lp = cnf; lp != NULL; lp = lp->next) {
        for (SATNode *dp = lp->head; dp != NULL; dp = dp->next) {
            count[abs(dp->data)]++;
        }
    }
}

/*
��������: Delete
���ܲ���: SATList *&,SATList *&,SATList *&,int *
��������: ɾ��cnf������Listָ����Ӿ䣬preΪList��ǰ��ָ��
����ֵ: void
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
    free(p); //�ͷ�ÿ�����ֵ�ָ��ռ�
    p = next = NULL;
    if (List == cnf) {
        cnf = List->next;
        pre = cnf;
    } else {
        pre->next = List->next;
    }
    free(List); //�ͷ��Ӿ��ָ��ռ�
    List = NULL;
}

/*
��������: Delete
���ܲ���: SATLis *&,int,int*
��������: ɾ��cnf�����к���literal���ֵ��Ӿ��Լ�-literal������
����ֵ: void
*/
void Delete(SATList *&cnf, int literal, int *count) {
    int flag = 0;
    SATList *List = cnf, *pre = cnf;
    while (List) {
        SATNode *Node = List->head, *pre_Node = Node;
        while (Node) {
            if (Node->data == literal) { //�ڷǵ��Ӿ����ҵ��˱�Ԫe��ֱ��ɾ�������Ӿ�
                Delete(cnf, pre, List, count); //ɾ���������Ӿ�
                Node = NULL;
                List = pre;
                flag = 1;
                break;
            } else if (Node->data == -literal) { //-eΪ�٣�ֻɾ�������֣����Ӿ������������ж����
                if (Node == pre_Node) { //��ɾ������������ͷ
                    pre_Node = Node->next;
                    count[abs(literal)]--;
                    free(Node);
                    Node = pre_Node;
                    List->head = Node; //�ı�ͷ���
                } else { //��ɾ�������ֲ�����ͷ
                    pre_Node->next = Node->next;
                    count[abs(literal)]--;
                    free(Node);
                    Node = pre_Node->next;
                }
            } else { //����e��-e
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
��������: copy
���ܲ���: int*,int
��������: ����a����
����ֵ: int *
*/
int *copy(int *a, int n) {
    int *arr = (int *)malloc(sizeof(int) * (n + 1));
    for (int i = 0; i <= n; i++) {
        arr[i] = a[i];
    }
    return arr;
}

/*
��������: init
���ܲ���: int*&,int
��������: ��ʼ������
����ֵ: void
*/
void init(int *&arr, int e) {
    arr = (int *)malloc(sizeof(int) * (boolCount + 1));
    for (int i = 1; i <= boolCount; i++) {
        arr[i] = e;
    }
}

/*
��������: copy
���ܲ���: int [][],int [][]
��������: ������b���Ƹ�����a
����ֵ: void
*/
void copy(int a[N][N], int b[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            a[i][j] = b[i][j];
        }
    }
}

/*
��������: mySolveSudoku
���ܲ���: int [][],int [][]
��������: ����Լ����������Ϸ
����ֵ: void
*/
void mySolveSudoku(int a[N][N], int board[N][N]) {
    int op = 1;
    while (op) {
        printf("\n\n		   ���ܲ˵� \n");
        printf("--------------------------------------------\n");
        printf("1.��������\n");
        printf("2.�鿴����\n");
        printf("0.�˳���Ϸ\n");
        printf("--------------------------------------------\n");
        printf("	��ѡ����Ĳ���[0~2]:");
        scanf("%d", &op);
        if (op == 1) {
            int line, con, num;
            printf("�������С��С�ֵ��");
            scanf("%d %d %d", &line, &con, &num);
            if (a[line - 1][con - 1] == num) {
                printf("����ɹ�\n");
                board[line - 1][con - 1] = num;
            } else {
                printf("����ʧ�ܣ�����������\n");
            }
        } else if (op == 2) {
            printBoard(board);
        } else if (op == 0) {
            printf("��ӭ�´���Ϸ��");
        } else {
            printf("�����������������\n");
        }
        getchar();
        getchar();
    }
}