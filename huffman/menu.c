#include "huffman.h"
void menu()
{
    printf("\n1.压缩文件\n");
    printf("2.解压缩文件\n");
    printf("3.读取解压缩文件\n");
    printf("4.查看霍夫曼编码\n");
    printf("输入任意字母退出程序\n");
    printf("请选择操作：\n");
}

int main(void)
{
    double cost;
    clock_t begin, end;
    FILE *fp;
    char ch, temparray[30];
    int ret1, ret2;
    int choice, count = 0, n;
    long f;
    memset(&TreeNode, 0, sizeof(TreeNode));
    memset(&temp, 0, sizeof(tmp));

    menu();

    while (scanf("%d", &choice) == 1)
    {
        system("cls");
        switch (choice)
        {
        case 1:
            begin = clock();
            ret1 = compress("data.txt", "data.txt.cj");
            end = clock();
            cost = (double)(end - begin) / CLOCKS_PER_SEC;
            printf("压缩耗时: %lf secs\n", cost);
            menu();
            break;

        case 2:
            begin = clock();
            ret2 = uncompress("data.txt.cj", "data.txt.cj.txt");
            end = clock();
            cost = (double)(end - begin) / CLOCKS_PER_SEC;
            printf("解压缩耗时: %lf secs\n", cost);
            printf("压缩率为%.3lf%%", ((double)ret1 / ret2) * 100);
            menu();
            break;

        case 3:
            printf("读取解压后的文件：\n");
            if ((fp = fopen("data.txt.cj.txt", "rt")) == NULL)
            {
                printf("读取失败！\n");
                exit(1);
            }
            ch = fgetc(fp);
            while (ch != EOF)
            {
                putchar(ch);
                ch = fgetc(fp);
            }
            fclose(fp);
            menu();
            break;
        case 4:
            if ((fp = fopen("data.txt.cj", "rb")) == NULL)
            {
                printf("读取失败！\n");
                exit(1);
            }
            fseek(fp, 4, SEEK_SET);
            fread(&f, sizeof(long), 1, fp);
            fseek(fp, f, SEEK_SET);
            fread(&n, sizeof(long), 1, fp); //读取字符种类总数
            for (int i = 0; i < n; i++)
            {
                printf("字符:%-5c频数:%-6ld哈夫曼编�?:%-20s", TreeNode[i].letter, TreeNode[i].count, TreeNode[i].bits);
                count++;
                if (count % 4 == 0)
                    printf("\n");
            }
            menu();
            break;
        case 6:
            printf("欢迎再次使用！\n");
        default:
            break;
        }
    }
}