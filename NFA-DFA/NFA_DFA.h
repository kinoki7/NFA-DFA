#pragma once
#ifndef NFA_DFA_H
#define NFA_DFA_H

using namespace std;
//要引入的库
#include<iostream>
#include<string>
#include<vector>
#include<stack>
#include<map>
#include<set>

#define MAX_VERTEX_NUM 100
//数据结构
struct DFA
{
    int st;
    char w;
    int ed;
};

set<int> eclosure(set<int> gather);
void read_keyboard();
set<int> move(set<int> gather, char a);
void reName();
void subConstruct();
#endif NFA_DFA_H
