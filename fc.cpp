#include <iostream>
#include <stack>
#include <queue>
#include <algorithm>
#include <vector>
#include <sys/time.h>

#define GET_TIME(now) { \
	struct timeval t; \
	gettimeofday(&t, NULL); \
	now = t.tv_sec + t.tv_usec/1000000.0; \
}
#define N 8
#define DEBUG
using namespace std;

int count_all = 0;
int count_test = 0;//用于测试

struct onenode
{
	bool is_const;
	int value;
};

struct value
{
	int min;
	int max;
	bool value[10];
	vector<pair<int, int>> less;
	vector<pair<int, int>> more;
	int count;
};

class node
{
public:
	value data[N][N];

	node()
	{
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				data[i][j].min = 0;
				data[i][j].max = N - 1;
				data[i][j].count = 0;
				for (int k = 0; k < N; k++)
					data[i][j].value[k] = true;
			}
		}
	}

	void assign(onenode data1[N][N], int i1, int j1, int value)	//赋值函数;
	{
		data1[i1][j1].value = value;
		for (int i = 0; i < data[i1][j1].less.size(); i++)
		{
			//data[data[i1][j1].less[i].first][data[i1][j1].less[i].second].max = value - 1;
			data[data[i1][j1].less[i].first][data[i1][j1].less[i].second].max = min(value - 1, data[data[i1][j1].less[i].first][data[i1][j1].less[i].second].max);
		}
		for (int i = 0; i < data[i1][j1].more.size(); i++)
		{
			//data[data[i1][j1].more[i].first][data[i1][j1].more[i].second].min = value + 1;
			data[data[i1][j1].more[i].first][data[i1][j1].more[i].second].min = max(value + 1, data[data[i1][j1].more[i].first][data[i1][j1].more[i].second].min);
		}
		for (int i = 0; i < N; i++)
			data[i][j1].value[value] = false;
		for (int j = 0; j < N; j++)
			data[i1][j].value[value] = false;
	}
};

void readfile(onenode data1[N][N], node& my_csp)
{
	for (int i = 0; i < N; i++)	//读入每一个点的数据;
	{
		for (int j = 0; j < N; j++)
		{
			cin >> data1[i][j].value;
			data1[i][j].value -= 1;
			data1[i][j].is_const = false;
			if (data1[i][j].value != -1)	//删减变量；
			{
				for (int i1 = 0; i1 < N; i1++)
					my_csp.data[i1][j].value[data1[i][j].value] = false;
				for (int j1 = 0; j1 < N; j1++)
					my_csp.data[i][j1].value[data1[i][j].value] = false;
				data1[i][j].is_const = true;
			}
		}
	}
	pair<int, int> p1;
	pair<int, int> p2;
	while (cin >> p1.first)	//建立节点间的大小关系;
	{
		cin >> p1.second >> p2.first >> p2.second;
		my_csp.data[p1.first][p1.second].more.push_back(p2);
		my_csp.data[p2.first][p2.second].less.push_back(p1);
	}
	for (int i = 0; i < N; i++)	//删减值域
		for (int j = 0; j < N; j++)
		{
			if (data1[i][j].is_const)
			{
				for (int i1 = 0; i1 < my_csp.data[i][j].less.size(); i1++)
				{
					my_csp.data[my_csp.data[i][j].less[i1].first][my_csp.data[i][j].less[i1].second].max = data1[i][j].value - 1;
				}
				for (int i1 = 0; i1 < my_csp.data[i][j].more.size(); i1++)
				{
					my_csp.data[my_csp.data[i][j].more[i1].first][my_csp.data[i][j].more[i1].second].min = data1[i][j].value + 1;
				}
			}
		}
}

void show(onenode data[N][N])
{
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
			cout << data[i][j].value + 1 << " ";
		cout << endl;
	}
}


bool findsolution(int length, onenode data1[N][N], node my_csp)	//深度，onenode和node;
{
	count_all += 1;
	int i1 = length / N;	//得到每一个变量的i,j值。
	int j1 = length % N;
	if (length == N * N - 1)
		return true;
	/*if (i1 == 5 && j1 == 3)
		count_test += 1;
	if (i1 == 5 && j1 == 3 && count_test ==2)
	{
		for (int i = 0; i < N; i++)
			cout << my_csp.data[i1][j1].value[i] << " ";
		cout << endl;
		cout << my_csp.data[i1][j1].min << endl;
		cout << my_csp.data[i1][j1].max << endl;
		show(data1);
		//exit(0);
	}*/
	if (!data1[i1][j1].is_const)	//判断是不是常量，不是常量就继续。
	{
		for (int i = my_csp.data[i1][j1].min; i <= my_csp.data[i1][j1].max; i++)	//赋值删减变量;
		{
			if (!my_csp.data[i1][j1].value[i])
				continue;
			node my_csp1 = my_csp;
			my_csp1.assign(data1, i1, j1, i);
			bool to_continue = false;
			for (int i2 = i1; i2 < N; i2++)	//判断删减以后是否会有某个变量的值域为空集;
			{
				for (int j2 = 0; j2 < N; j2++)
				{
					if (data1[i2][j2].is_const)
						continue;
					if (i2 == i1 && j2 <= j1)
						continue;
					int count = 0;
					for (int i3 = my_csp1.data[i2][j2].min; i3 <= my_csp1.data[i2][j2].max; i3++)
						if (my_csp1.data[i2][j2].value[i3])
							count++;
					if (count == 0)
					{
						to_continue = true;
						break;
					}
				}
				if (to_continue)
					break;
			}
			if (to_continue)
			{
				continue;
			}
			bool temp = findsolution(length + 1, data1, my_csp1);
			if (temp)
				return true;
		}
	}
	else
	{
		count_all -= 1;
		return findsolution(length + 1, data1, my_csp);
	}
	return false;
}



#ifdef DEBUG
int main()
{
	onenode data1[N][N];
	node my_csp;
	readfile(data1, my_csp);
	double begin, end;
	GET_TIME(begin);
	findsolution(0, data1, my_csp);
	GET_TIME(end);
	show(data1);
	cout << count_all << endl;
	cout << "运行时间：" << end - begin << "s"<< endl;
}
#endif // DEBUG


#ifndef DEBUG
int main()
{
	return 0;
}
#endif // !DEBUG
