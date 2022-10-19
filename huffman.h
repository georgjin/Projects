#pragma once                    //防止重复引用造成二义
#define _CRT_SECURE_NO_WARNINGS //忽略安全检查
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <windows.h>

struct HuffmanNode
{
    int letter;
    long count;
    long parent, lchild, rchild;
    char bits[256];
};

struct HuffmanNode TreeNode[512], tmp, temp;

int compress(const char *filename, const char *outputfile);

int uncompress(const char *filename, const char *outputfile);
