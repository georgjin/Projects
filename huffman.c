#include "huffman.h"
int compress(const char *filename, const char *outputfile)
{
    char temparray[256];
    unsigned char c;
    long i, j, m, n, f;
    long lowest, pt, filelength;
    FILE *inputfilep, *outputfilep;
    int k, q;
    inputfilep = fopen(filename, "rb");
    if (inputfilep == NULL)
    {
        printf("打开文件失败\n");
        return 0;
    }
    outputfilep = fopen(outputfile, "wb");
    if (outputfilep == NULL)
    {
        printf("打开文件失败\n");
        return 0;
    }
    filelength = 0;
    while (!feof(inputfilep))
    {
        fread(&c, 1, 1, inputfilep);
        TreeNode[c].count++;
        filelength++;
    }
    filelength--;
    TreeNode[c].count--;
    for (i = 0; i < 512; i++)
    {
        if (TreeNode[i].count != 0)
            TreeNode[i].letter = i;
        else
            TreeNode[i].letter = -1;
        TreeNode[i].parent = -1;
        TreeNode[i].lchild = TreeNode[i].rchild = -1;
    }
    for (i = 0; i < 256; i++)
    {
        k = i;
        for (q = i + 1; q < 256; q++)
            if (TreeNode[k].count < TreeNode[q].count)
                k = q;
        temp = TreeNode[i];
        TreeNode[i] = TreeNode[k];
        TreeNode[k] = temp;
    }
    for (i = 0; i < 256; i++)
    {
        if (TreeNode[i].count == 0)
            break;
    }
    n = i;
    m = 2 * n - 1;
    for (i = n; i < m; i++)
    {
        lowest = 999999999;
        for (j = 0; j < i; j++)
        {
            if (TreeNode[j].parent != -1)
                continue;
            if (lowest > TreeNode[j].count)
            {
                lowest = TreeNode[j].count;
                pt = j;
            }
        }
        TreeNode[i].count = TreeNode[pt].count;
        TreeNode[pt].parent = i;
        TreeNode[i].lchild = pt;
        lowest = 999999999;
        for (j = 0; j < i; j++)
        {
            if (TreeNode[j].parent != -1)
                continue;
            if (lowest > TreeNode[j].count)
            {
                lowest = TreeNode[j].count;
                pt = j;
            }
        }
        TreeNode[i].count += TreeNode[pt].count;
        TreeNode[i].rchild = pt;
        TreeNode[pt].parent = i;
    }
    for (i = 0; i < n; i++)
    {
        f = i;
        TreeNode[i].bits[0] = 0;
        while (TreeNode[f].parent != -1)
        {
            j = f;
            f = TreeNode[f].parent;
            if (TreeNode[f].lchild == j)
            {
                j = strlen(TreeNode[i].bits);
                memmove(TreeNode[i].bits + 1, TreeNode[i].bits, j + 1);
                TreeNode[i].bits[0] = '0';
            }
            else
            {
                j = strlen(TreeNode[i].bits);
                memmove(TreeNode[i].bits + 1, TreeNode[i].bits, j + 1);
                TreeNode[i].bits[0] = '1';
            }
        }
    }
    fseek(inputfilep, 0, SEEK_SET);
    fseek(outputfilep, 8, SEEK_SET);
    temparray[0] = 0;
    pt = 8;
    printf("读取将要压缩的文件:%s\n", filename);
    printf("当前文件:%d字符\n", filelength);
    //压缩文件
    while (!feof(inputfilep))
    {
        c = fgetc(inputfilep);
        for (i = 0; i < n; i++)
        {
            if (c == TreeNode[i].letter)
                break;
        }
        strcat(temparray, TreeNode[i].bits);
        j = strlen(temparray);
        c = 0;
        if (j >= 8)
        {
            for (i = 0; i < 8; i++)
            {
                if (temparray[i] == '1')
                {
                    c = c << 1;
                    c = c + 1;
                }
                else
                    c = c << 1;
            }
            fwrite(&c, sizeof(char), 1, outputfilep);
            // printf("%s", temparray); // temparray数组存储的是char保存的二进制内容
            pt++;
            strcpy(temparray, temparray + 8);
        }
    }
    if (j > 0)
    {
        strcat(temparray, "00000000");
        for (i = 0; i < 8; i++)
        {
            if (temparray[i] == '1')
            {
                c = c << 1;
                c += 1;
            }
            else
                c = c << 1;
        }
        fwrite(&c, 1, 1, outputfilep);
        pt++;
    }
    fseek(outputfilep, 0, SEEK_SET);
    fwrite(&filelength, 4, 1, outputfilep);
    fwrite(&pt, 4, 1, outputfilep);
    fseek(outputfilep, pt, SEEK_SET);
    fwrite(&n, 4, 1, outputfilep);
    for (i = 0; i < n; i++)
    {
        tmp = TreeNode[i];
        fwrite(&(TreeNode[i].letter), 1, 1, outputfilep);
        pt++;
        c = strlen(TreeNode[i].bits);
        fwrite(&c, 1, 1, outputfilep);
        pt++;
        j = strlen(TreeNode[i].bits);
        if (j % 8 != 0)
        {
            for (f = j % 8; f < 8; f++)
                strcat(TreeNode[i].bits, "0");
        }
        while (TreeNode[i].bits[0] != 0)
        {
            c = 0;
            for (j = 0; j < 8; j++)
            {
                if (TreeNode[i].bits[j] == '1')
                {
                    c = c << 1;
                    c += 1;
                }
                else
                    c = c << 1;
            }
            strcpy(TreeNode[i].bits, TreeNode[i].bits + 8);
            fwrite(&c, 1, 1, outputfilep);
            pt++;
        }

        TreeNode[i] = tmp;
    }
    fclose(inputfilep);
    fclose(outputfilep);
    printf("\n压缩后文件为:%s\n", outputfile);
    printf("压缩后文件有:%d字符\n", pt + 4);
    return pt + 4;
}

int uncompress(const char *filename, const char *outputfile)
{
    char temparray[256], bx[256];
    unsigned char c;
    long i, j, m, n, k, f, p, l;
    long filelength;
    int len = 0;
    FILE *inputfilep, *outputfilep;
    char cname[512] = {0};
    inputfilep = fopen(filename, "rb");
    if (inputfilep == NULL)
    {
        printf("文件打开失败！");
        return 0;
    }

    outputfilep = fopen(outputfile, "wb");
    if (outputfilep == NULL)
    {

        return 0;
    }
    fseek(inputfilep, 0, SEEK_END);
    len = ftell(inputfilep);

    fseek(inputfilep, 0, SEEK_SET);
    printf("将要读取解压的文件:%s\n", filename);
    printf("当前文件:%d字符\n", len);
    fread(&filelength, sizeof(long), 1, inputfilep);
    fread(&f, sizeof(long), 1, inputfilep);
    fseek(inputfilep, f, SEEK_SET);
    fread(&n, sizeof(long), 1, inputfilep);
    for (i = 0; i < n; i++)
    {
        fread(&TreeNode[i].letter, 1, 1, inputfilep);
        fread(&c, 1, 1, inputfilep);
        p = (long)c;
        TreeNode[i].count = p;
        TreeNode[i].bits[0] = 0;
        if (p % 8 > 0)
            m = p / 8 + 1;
        else
            m = p / 8;
        for (j = 0; j < m; j++)
        {
            fread(&c, 1, 1, inputfilep);
            f = c;
            _itoa(f, temparray, 2);
            f = strlen(temparray);
            for (l = 8; l > f; l--)
            {
                strcat(TreeNode[i].bits, "0");
            }
            strcat(TreeNode[i].bits, temparray);
        }
        TreeNode[i].bits[p] = 0;
    }

    for (i = 0; i < n; i++)
    {
        k = i;
        for (j = i + 1; j < n; j++)
            if (strlen(TreeNode[k].bits) > strlen(TreeNode[j].bits))
                k = j;
        tmp = TreeNode[i];
        TreeNode[i] = TreeNode[k];
        TreeNode[k] = tmp;
    }

    p = strlen(TreeNode[n - 1].bits);
    fseek(inputfilep, 8, SEEK_SET);
    m = 0;
    bx[0] = 0;
    while (1)
    {
        while (strlen(bx) < (unsigned int)p)
        {
            fread(&c, 1, 1, inputfilep);
            f = c;
            _itoa(f, temparray, 2);
            f = strlen(temparray);
            for (l = 8; l > f; l--)
            {
                strcat(bx, "0");
            }
            strcat(bx, temparray);
        }
        for (i = 0; i < n; i++)
        {
            if (memcmp(TreeNode[i].bits, bx, TreeNode[i].count) == 0)
                break;
        }
        strcpy(bx, bx + TreeNode[i].count);
        c = TreeNode[i].letter;
        fwrite(&c, 1, 1, outputfilep);
        m++;
        if (m == filelength)
            break;
    }
    fclose(inputfilep);
    fclose(outputfilep);
    printf("解压后文件为:%s\n", outputfile);
    printf("解压后文件有:%d字符\n", filelength);
    return filelength;
}
