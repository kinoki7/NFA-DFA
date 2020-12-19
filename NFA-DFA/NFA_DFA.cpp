#include "NFA_DFA.h"

map<int, map<int, map<char, int> > > nfa;//nfa[0][1][a]==1表示0状态可经过a弧到达1状态
set<char> Arcs;//弧,这里不包括‘～’，‘～’在求eclosure时单独处理
vector<DFA> dfa;//st状态经过w弧可以到达ed状态
map<set<int>, int> flag;//标记子集
int line_num;//行数,也是状态数
vector<set<int>> sub;//存储子集
void read_keyboard()
{
	string input,line_string;//line_string为1行的字符串
	char ch;
	vector<string> all;
	while (cin >> input)
	{
		cin.get(ch);//获取空字符	
		line_string += input;
		if (ch != '\n')
		{
			line_string += " ";
		}
		else
		{
			all.push_back(line_string);
			line_string = "";
		}
	}
	line_num = all.size();
	int cnt;
	int state1,state2,condition;
	for (int i = 0; i < line_num; i++)
	{
		line_string = all[i];
		cnt = line_string.length()-1;
		for (int i = 0; i < cnt/7; i++)
		{
			state1 = line_string[2 + 7 * i];
			condition = line_string[4 + 7 * i];
			state2 = line_string[7 + 7 * i];
			if (state1 == 88)
				state1 = 0;
			else if (state1 == 89)
				state1 = line_num - 1;
			else
				state1 = state1 - 47;
			if (state2 == 88)
				state2 = 0;
			else if (state2 == 89)
				state2 = line_num - 1;
			else
				state2 = state2 - 47;
			nfa[state1][state2][condition] = 1;
			//添加弧
			if (condition == '~')
				continue;
			else
				Arcs.insert(condition);
		}
	}
}

//求e-闭包
set<int> eclosure(set<int> gather)
{
	set<int>::iterator iter;//指向
	set<int> result;
	stack<int> s;
	int s_top;
	for (iter = gather.begin(); iter != gather.end(); iter++)
	{
		result.insert(*iter);
		s.push(*iter);
	}
	while (!s.empty())
	{
		s_top = s.top();
		s.pop();
		for (int i = 0; i < line_num; i++)
		{
			if (nfa[s_top][i]['~'] == 1)
			{
				result.insert(i);
				s.push(i);
			}
		}
	}
	return result;
}

//求将旧集合通过符号a到达的新字符
set<int> move(set<int> gather, char a)
{
	set<int> result;
	set<int>::iterator iter;
	for (iter = gather.begin(); iter != gather.end(); iter++)
	{
		for (int i = 0; i < line_num; i++)
		{
			if (nfa[*iter][i][a] == 1)
			{
				result.insert(i);
			}
		}
	}
	return result;
}

void subConstruct()
{
	set<int> init,t;
	init.insert(0);
	init = eclosure(init);//初态
	sub.push_back(init);
	while (1)
	{
		int st = -1;
		for (int i = 0; i < sub.size(); i++)
		{
			if (flag[sub[i]] == 0)
			{
				st = i;
				t = sub[i];
				break;
			}
		}
		if (st == -1)//全部都标记好就直接退出
		{
			break;
		}
		flag[t] = 1;
		set<int> sub2;
		for (set<char>::iterator iter = Arcs.begin(); iter != Arcs.end(); iter++)
		{
			sub2 = eclosure(move(t,*iter));
			int exits = 0;
			for (int i = 0; i < sub.size(); i++)
			{
				if (sub2 == sub[i])
				{
					exits = 1;
					break;
				}
			}
			if (exits != 1 && sub2.size() != 0)
			{
				sub.push_back(sub2);
			}
			for (int i = 0; i < sub.size(); i++)
			{
				if (sub2 == sub[i])
				{
					dfa.push_back(DFA{st,*iter,i});
					break;
				}
			}
		}
	}
}

//重新编号
void reName()
{
	map<int, vector<string> > lines;
	vector<string> toPrint;
	string single_ = "";
	//string all_="";
	//将dfa中的状态转移关系提取出来做成X X-a->1这种形式
	for (auto x : dfa)
	{
		if (x.st == 0)
			single_ = single_ + "X-" + x.w + "->";
		else if (x.st != sub.size() - 1)
			single_ = single_ + char(x.st - 1 + 48) + "-" + x.w + "->";
		else
			single_ = single_ + "Y-" + x.w + "->";
		if (x.ed == 0)
			single_ = single_ + "X";
		else if (x.ed != sub.size() - 1)
			single_ = single_ + char(x.ed - 1 + 48);
		else
			single_ = single_ + "Y";
		lines[x.st].push_back(single_);//填入到对应状态的vector<string>中
		single_ = "";//置空，以进入下一次循环
	}
	//以下是控制输出格式
	if (sub.size() > 0)
	{
		cout << "X";
		for (auto l : lines[0])
			cout << " " << l;
		cout << endl;
	}
	if (sub.size() > 1)
	{
		cout << "Y";
		for (auto l : lines[sub.size() - 1])
			cout << " " << l;
		cout << endl;
	}
	for (int i = 1; i < sub.size() - 1; i++)
	{
		toPrint = lines[i];
	    if (i < sub.size() - 1)
		{
			cout << i - 1;
			for (auto l : toPrint)
				cout << " " << l;
			cout << endl;
		}
	}
}