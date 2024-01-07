#include <stdio.h>  
#include <stdlib.h>   
#include <time.h>  
#include <graphics.h>
#include <conio.h> 
#define PICTURE_LENTH 50
#define PICTURE_WIDTH  50
#define LEI_LENGTH 30
#define LEI_WIDTH 30

IMAGE number_piture[10];
IMAGE Empty_piture;
IMAGE Empty_Lei_piture;

void random_gezi(int hang, int lie, int random_cishu, int** array) {
    int random_hang, random_lie;
    int i, j;
    for (i = 0; i < hang; i++) {
        for (j = 0; j < lie; j++) {
            array[i][j] = 0;
        }
    }
    for (i = 0; i < random_cishu; i++) {
        do {
            random_hang = rand() % hang;
            random_lie = rand() % lie;
        } while (array[random_hang][random_lie] != 0);
        array[random_hang][random_lie] = -1;
    }
}

void draw_gezi(int** array,int hang,int lie,IMAGE * number_piture) {
    initgraph(hang* PICTURE_LENTH, lie* PICTURE_WIDTH);
    setbkcolor(WHITE);
    int i, j;
    for (i = 0; i < hang; i++) {
        for (j = 0; j < lie; j++){
            putimage(i * PICTURE_LENTH, j * PICTURE_WIDTH, &Empty_piture);
        }
    }
}

void init_source(IMAGE * number_piture) {
    loadimage(&number_piture[0], "./number_image\\0.png", PICTURE_LENTH, PICTURE_WIDTH);
    loadimage(&number_piture[1], "./number_image\\1.png", PICTURE_LENTH, PICTURE_WIDTH);
    loadimage(&number_piture[2], "./number_image\\2.png", PICTURE_LENTH, PICTURE_WIDTH);
    loadimage(&number_piture[3], "./number_image\\3.png", PICTURE_LENTH, PICTURE_WIDTH);
    loadimage(&number_piture[4], "./number_image\\4.png", PICTURE_LENTH, PICTURE_WIDTH);
    loadimage(&number_piture[5], "./number_image\\5.png", PICTURE_LENTH, PICTURE_WIDTH);
    loadimage(&number_piture[6], "./number_image\\6.png", PICTURE_LENTH, PICTURE_WIDTH);
    loadimage(&number_piture[7], "./number_image\\7.png", PICTURE_LENTH, PICTURE_WIDTH);
    loadimage(&number_piture[8], "./number_image\\8.png", PICTURE_LENTH, PICTURE_WIDTH);
    loadimage(&number_piture[9], "./number_image\\9.png", PICTURE_LENTH, PICTURE_WIDTH);
    loadimage(&Empty_piture, "./number_image\\00.png", PICTURE_LENTH, PICTURE_WIDTH);
    loadimage(&Empty_Lei_piture, "./number_image\\000.png", LEI_LENGTH,LEI_WIDTH);
}

void jisuan_array(int** array, int hang, int lie);
void xun_huan_jian_ce(int **array,IMAGE *number_piture,int hang,int lie,int random_cishu) {
    ExMessage m;

    //�����ж��Ƿ��ǵ�һ�ε��������ǣ�������һ��������
    int cishu=0;


    //�ж��Ƿ����
    int lei = 0;

    //��¼ÿ�����ӵ�״̬,0Ϊԭʼ״̬��-1Ϊ���Ϊ�׵�״̬��1Ϊ������ͬʱ��������ͼƬ��״̬
    int** array_1 = (int**)malloc(hang * sizeof(int*));
    int i,j;
    for (i = 0; i < hang; i++) {
        array_1[i] = (int*)malloc(lie * sizeof(int));
    }
    for (i = 0; i < hang; i++) {
        for (j = 0; j < lie; j++) {
                array_1[i][j] = 0;
        }
    }


    while (1) {
        m = getmessage(EX_MOUSE | EX_KEY);
        if (_kbhit()) {
            char ch = _getch(); 
            //ESC�˳�
            if (ch == 27) {
                closegraph();
                break; 
            }
        }

        //������
        if (m.lbutton == 1) {
            int array_hang = m.x / PICTURE_LENTH;
            int array_lie = m.y / PICTURE_WIDTH;
            //��һ��һ��������
            while (array[array_hang][array_lie] == -1 && cishu==0) {
                random_gezi(hang, lie, random_cishu, array);
                jisuan_array(array, hang, lie);
            }
            //�㵽�ף�������ͼƬ���
            if (array[array_hang][array_lie] == -1 && array_1[array_hang][array_lie] == 0) {
                for (i=0; i < hang; i++) {
                    for (j=0; j < lie; j++) {
                        //�㵽��
                        if (array_1[i][j] == 0 || array_1[i][j]==-1) {
                            //0״̬������
                            if (array[i][j] != -1 && array_1[i][j]== 0) {
                                putimage(i * PICTURE_LENTH, j * PICTURE_WIDTH, &number_piture[array[i][j]]);
                            }
                            //0״̬����
                            else if (array[i][j] == -1 && array_1[i][j]==0) {
                                putimage(i * PICTURE_LENTH, j * PICTURE_WIDTH, &number_piture[9]);
                            }
                            //-1״̬����
                            else if (array[i][j] == -1 && array_1[i][j] == -1) {
                                putimage(i * PICTURE_LENTH, j * PICTURE_WIDTH, &number_piture[9]);
                                putimage(i * PICTURE_LENTH+(PICTURE_LENTH-LEI_LENGTH)/2, j * PICTURE_WIDTH + (PICTURE_WIDTH-LEI_WIDTH)/2, &Empty_Lei_piture);
                            }
                            else if (array[i][j] != -1 && array_1[i][j] == -1) {
                                putimage(i * PICTURE_LENTH, j * PICTURE_WIDTH, &number_piture[array[i][j]]);
                                putimage(i * PICTURE_LENTH + (PICTURE_LENTH - LEI_LENGTH) / 2, j * PICTURE_WIDTH + (PICTURE_WIDTH - LEI_WIDTH) / 2, &Empty_Lei_piture);
                            }
                        }
                    }
                }
                Sleep(3000);
                for (i = 0; i < hang; i++) {
                    free(array_1[i]);
                }
                free(array_1);
                break;
            }
            //0״̬�ĸ��ӣ����ǲ����׵ĸ���
            else if(array[array_hang][array_lie] !=-1 && array_1[array_hang][array_lie]==0) {
                putimage(array_hang * PICTURE_LENTH, array_lie * PICTURE_WIDTH, &number_piture[array[array_hang][array_lie]]);
                cishu=1;
                array_1[array_hang][array_lie] = 1;
            }
            
        }

        //�Ҽ����
        if (m.rbutton == 1) {
            int array_hang = m.x / PICTURE_LENTH;
            int array_lie = m.y / PICTURE_WIDTH;
            //���û�в�������ֱ�ӷű��ͼƬ
            if (array_1[array_hang][array_lie] == 0) {
                array_1[array_hang][array_lie] = -1;
                lei++;
                putimage(array_hang * PICTURE_LENTH + (PICTURE_LENTH - LEI_LENGTH) / 2, array_lie * PICTURE_WIDTH + (PICTURE_WIDTH - LEI_WIDTH) / 2, &Empty_Lei_piture);
            }
            //�������ǹ��ף���ȡ����ǣ�����ԭʼͼƬ
            else if (array_1[array_hang][array_lie] == -1) {
                array_1[array_hang][array_lie] = 0;
                lei--;
                putimage(array_hang * PICTURE_LENTH, array_lie * PICTURE_WIDTH, &Empty_piture);
            }
            //������ù����֣���û�в���
            else if (array_1[array_hang][array_lie] == 1) {
                continue;
            }
        }

        //�ж��Ƿ����
        int jieshu=0;
            for (i = 0; i < hang; i++) {
                for (j = 0; j < lie; j++) {
                    if(array_1[i][j]==0){
                        jieshu++;
                    }
                }
            }
        if (jieshu == 0 && lei==random_cishu) {
            for (i=0; i < hang; i++) {
                for (j = 0; j < lie; j++) {
                    if (array_1[i][j] == -1) {
                        putimage(i* PICTURE_LENTH, j* PICTURE_WIDTH, &number_piture[9]);
                        putimage(i* PICTURE_LENTH + (PICTURE_LENTH - LEI_LENGTH) / 2, j* PICTURE_WIDTH + (PICTURE_WIDTH - LEI_WIDTH) / 2, &Empty_Lei_piture);
                    }
                }
            }
            Sleep(2000);
            for (i = 0; i < hang; i++) {
                free(array_1[i]);
            }
            free(array_1);
            break;
        }
        else {
            continue;
        }
    }
}

void jisuan_array(int** array, int hang, int lie) {
    int i, j;
    for (i = 0; i < hang; i++) {
        for (j = 0; j < lie; j++) {
            if (array[i][j] == -1) {
                continue;
            }
            if (i - 1 >= 0) {
                //����2
                if (j - 1 >= 0 && j + 1 >= lie) {
                    if (array[i - 1][j - 1] == -1) {
                        array[i][j]++;
                    }
                    if (array[i - 1][j] == -1) {
                        array[i][j]++;
                    }
                }
                //����2
                else if (j - 1 < 0 && j + 1 < lie) {
                    if (array[i - 1][j + 1] == -1) {
                        array[i][j]++;
                    }
                    if (array[i - 1][j] == -1) {
                        array[i][j]++;
                    }
                }
                //����
                else if (j - 1 >= 0 && j + 1 < lie) {
                    if (array[i - 1][j + 1] == -1) {
                        array[i][j]++;
                    }
                    if (array[i - 1][j] == -1) {
                        array[i][j]++;
                    }
                    if (array[i - 1][j - 1] == -1) {
                        array[i][j]++;
                    }
                }
            }
            if (i + 1 < hang) {
                //����2
                if (j - 1 >= 0 && j + 1 >= lie) {
                    if (array[i + 1][j - 1] == -1) {
                        array[i][j]++;
                    }
                    if (array[i + 1][j] == -1) {
                        array[i][j]++;
                    }
                }
                //����2
                else if (j - 1 < 0 && j + 1 < lie) {
                    if (array[i + 1][j + 1] == -1) {
                        array[i][j]++;
                    }
                    if (array[i + 1][j] == -1) {
                        array[i][j]++;
                    }
                }
                //����
                else if (j - 1 >= 0 && j + 1 < lie) {
                    if (array[i + 1][j + 1] == -1) {
                        array[i][j]++;
                    }
                    if (array[i + 1][j] == -1) {
                        array[i][j]++;
                    }
                    if (array[i + 1][j - 1] == -1) {
                        array[i][j]++;
                    }
                }
            }
            //������
            if (j - 1 >= 0 && j + 1 < lie) {
                if (array[i][j - 1] == -1)
                    array[i][j]++;
                if (array[i][j + 1] == -1)
                    array[i][j]++;
            }
            //����
            if (j + 1 < lie && j - 1 < 0) {
                if (array[i][j + 1] == -1)
                    array[i][j]++;
            }
            //����
            if (j + 1 >= lie && j - 1 >= 0) {
                if (array[i][j - 1] == -1)
                    array[i][j]++;
            }
        }
    }
}




int main(void) {

    int hang;
    int lie;
    int random_cishu;
    int i, j;

    printf("����һ�еĳ��ȣ�һ�еĳ���,������");
    scanf_s("%d%d%d", &hang, &lie, &random_cishu);
    if (random_cishu > hang * lie || hang <= 0 || lie <= 0 || random_cishu <= 0) {
        printf("error");
        Sleep(5000);
        return 0;
    }

    int** array = (int**)malloc(hang * sizeof(int*));

    for (i = 0; i < hang; i++) {
        array[i] = (int*)malloc(lie * sizeof(int));
    }

    srand(time(0));

    random_gezi(hang, lie, random_cishu, array);

    init_source(number_piture);

    jisuan_array(array, hang, lie);

    draw_gezi(array, hang, lie, number_piture);

    xun_huan_jian_ce(array,number_piture,hang,lie,random_cishu);
    
    closegraph();

    for (i = 0; i < hang; i++) {
        free(array[i]);
    }

    free(array);

    return 0;
}