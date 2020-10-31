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
#define N 9
#define DEBUG
using namespace std;

int count_all = 0;	//��������˶��ٸ��ڵ㡣
int count_test = 0;//���ڲ���

struct onenode	//��ʾһ��������ֵ��������ŵ㶫��������ģ��������ַ�ֵ�Ϳ����ˡ�
{
	int value;
};

struct value	//����Լ���еĻ���������
{
	int min;	//��Χ����Сֵ
	int max;	//��Χ�����ֵ
	bool value[10];	//10�����ܵ�ȡֵ
	vector<pair<int, int>> less;	//��������Щ�ڵ�
	vector<pair<int, int>> more;	//����С����Щ�ڵ�
	int count;
	bool is_const;
};

class node	//������Ǳ�ʾԼ���ˡ�ÿһ���������Լ���Լ����
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

	void assign(onenode data1[N][N], int i1, int j1, int value)	//��ֵ����;
	{
		data1[i1][j1].value = value;
		queue<pair<int, int>> my_queue;
		int counttemp = 0;
		data[i1][j1].is_const = true;
		for (int i = 0; i < data[i1][j1].less.size(); i++)	//�ж��Ƿ�����ھ����������������ô�͸�ֵ����������Ͼ�ȡ����ֵ��
		{
			if (data[data[i1][j1].less[i].first][data[i1][j1].less[i].second].is_const && value <= data1[data[i1][j1].less[i].first][data[i1][j1].less[i].second].value)
			{
				data[i1][j1].is_const = false;
				return;
			}
		}
		for (int i = 0; i < data[i1][j1].more.size(); i++)
		{
			if (data[data[i1][j1].more[i].first][data[i1][j1].more[i].second].is_const && value >= data1[data[i1][j1].more[i].first][data[i1][j1].more[i].second].value)
			{
				data[i1][j1].is_const = false;
				return;
			}
		}
		for (int i = 0; i < data[i1][j1].less.size(); i++)	//����Χδ��ֵ���ھӵ����ֵ����������
		{
			//data[data[i1][j1].less[i].first][data[i1][j1].less[i].second].max = value - 1;
			int temp = data[data[i1][j1].less[i].first][data[i1][j1].less[i].second].max;
			counttemp = 0;
			data[data[i1][j1].less[i].first][data[i1][j1].less[i].second].max = min(value - 1, data[data[i1][j1].less[i].first][data[i1][j1].less[i].second].max);
			if (temp != data[data[i1][j1].less[i].first][data[i1][j1].less[i].second].max)
				my_queue.push(pair<int, int>(data[i1][j1].less[i].first, data[i1][j1].less[i].second));
		}
		for (int i = 0; i < data[i1][j1].more.size(); i++)	//����Χδ��ֵ�ھӵ���Сֵ����������
		{
			//data[data[i1][j1].more[i].first][data[i1][j1].more[i].second].min = value + 1;
			int temp = data[data[i1][j1].more[i].first][data[i1][j1].more[i].second].min;
			data[data[i1][j1].more[i].first][data[i1][j1].more[i].second].min = max(value + 1, data[data[i1][j1].more[i].first][data[i1][j1].more[i].second].min);
			if (temp != data[data[i1][j1].more[i].first][data[i1][j1].more[i].second].min)
				my_queue.push(pair<int, int>(data[i1][j1].more[i].first, data[i1][j1].more[i].second));
		}
		for (int i = 0; i < N; i++)
			data[i][j1].value[value] = false;
		for (int j = 0; j < N; j++)
			data[i1][j].value[value] = false;
		int tempi1 = i1;
		int tempj1 = j1;
		while (my_queue.size() != 0)	//�����һ������Ժ���Щֵ�����仯�ı���ͳͳ���뵽���н������ֵ��Сֵ���������뿪�ú��������ʣ1�����ܵ�ȡֵ��ȡֵ�����û�п��ܵ�ȡֵ��ô��Ҫ��ȡ�µĽڵ��ˡ�
		{
			pair<int, int>temp = my_queue.front();
			i1 = temp.first;
			j1 = temp.second;
			my_queue.pop();
			for (int i = 0; i < data[i1][j1].less.size(); i++)
			{
				//data[data[i1][j1].less[i].first][data[i1][j1].less[i].second].max = value - 1;
				if (data[data[i1][j1].less[i].first][data[i1][j1].less[i].second].is_const)
					continue;
				if (data[i1][j1].less[i].first == tempi1 && data[i1][j1].less[i].second == tempj1)
					continue;
				int temp = data[data[i1][j1].less[i].first][data[i1][j1].less[i].second].max;
				data[data[i1][j1].less[i].first][data[i1][j1].less[i].second].max = min(data[i1][j1].max-1, data[data[i1][j1].less[i].first][data[i1][j1].less[i].second].max);
				if (temp != data[data[i1][j1].less[i].first][data[i1][j1].less[i].second].max)
					my_queue.push(pair<int, int>(data[i1][j1].less[i].first, data[i1][j1].less[i].second));
			}
			for (int i = 0; i < data[i1][j1].more.size(); i++)
			{
				//data[data[i1][j1].more[i].first][data[i1][j1].more[i].second].min = value + 1;
				if (data[data[i1][j1].more[i].first][data[i1][j1].more[i].second].is_const)
					continue;
				if (data[i1][j1].more[i].first == tempi1 && data[i1][j1].more[i].second == tempj1)
					continue;
				int temp = data[data[i1][j1].more[i].first][data[i1][j1].more[i].second].min;
				data[data[i1][j1].more[i].first][data[i1][j1].more[i].second].min = max(data[i1][j1].min + 1, data[data[i1][j1].more[i].first][data[i1][j1].more[i].second].min);
				if (temp != data[data[i1][j1].more[i].first][data[i1][j1].more[i].second].min)
					my_queue.push(pair<int, int>(data[i1][j1].more[i].first, data[i1][j1].more[i].second));
			}
		}
	}
};

void readfile(onenode data1[N][N], node& my_csp)
{
	for (int i = 0; i < N; i++)	//����ÿһ���������;
	{
		for (int j = 0; j < N; j++)
		{
			cin >> data1[i][j].value;
			data1[i][j].value -= 1;
			my_csp.data[i][j].is_const = false;
			if (data1[i][j].value != -1)	//ɾ��������
			{
				for (int i1 = 0; i1 < N; i1++)
					my_csp.data[i1][j].value[data1[i][j].value] = false;
				for (int j1 = 0; j1 < N; j1++)
					my_csp.data[i][j1].value[data1[i][j].value] = false;
				my_csp.data[i][j].is_const = true;
			}
		}
	}
	pair<int, int> p1;
	pair<int, int> p2;
	while (cin >> p1.first)	//�����ڵ��Ĵ�С��ϵ;
	{
		cin >> p1.second >> p2.first >> p2.second;
		my_csp.data[p1.first][p1.second].more.push_back(p2);
		my_csp.data[p2.first][p2.second].less.push_back(p1);
	}
	for (int i = 0; i < N; i++)	//ɾ��ֵ��
		for (int j = 0; j < N; j++)
		{
			if (my_csp.data[i][j].is_const)
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

void show(onenode data[N][N])	//չʾ�����
{
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
			cout << data[i][j].value + 1 << " ";
		cout << endl;
	}
}


bool findsolution(int length, onenode data1[N][N], node my_csp)	//��ȣ�onenode��node;
{
	count_all += 1;
	int i1 = length / N;	//�õ�ÿһ��������i,jֵ��
	int j1 = length % N;
	if (length == N * N - 1)
		return true;
	/*if (i1 == 2 && j1 == 1)
		count_test += 1;
	if (i1 == 2 && j1 == 1 && count_test == 1)
	{
		for (int i = 0; i < N; i++)
			cout << my_csp.data[i1][j1].value[i] << " ";
		cout << endl;
		//cout << my_csp.data[i1][j1].min << endl;
		//cout << my_csp.data[i1][j1].max << endl;
		//cout << my_csp.data[1][2].min << endl;
		//cout << my_csp.data[1][2].max << endl;
		show(data1);
		cout << endl;
		//exit(0);
	}*/
	if (!my_csp.data[i1][j1].is_const)	//�ж��ǲ��ǳ��������ǳ����ͼ�����
	{
		for (int i = my_csp.data[i1][j1].min; i <= my_csp.data[i1][j1].max; i++)	//��ֵɾ������;
		{
			if (!my_csp.data[i1][j1].value[i])
				continue;
			node my_csp1 = my_csp;
			my_csp1.assign(data1, i1, j1, i);
			if (!my_csp1.data[i1][j1].is_const)
				continue;
			bool to_continue = false;
			for (int i2 = i1; i2 < N; i2++)	//�ж�ɾ���Ժ��Ƿ����ĳ��������ֵ��Ϊ�ռ�;
			{
				for (int j2 = 0; j2 < N; j2++)
				{
					if (my_csp.data[i2][j2].is_const)
						continue;
					if (i2 == i1 && j2 <= j1)
						continue;
					int count = 0;
					int tempvalue;
					for (int i3 = my_csp1.data[i2][j2].min; i3 <= my_csp1.data[i2][j2].max; i3++)
						if (my_csp1.data[i2][j2].value[i3])
						{
							tempvalue = i3;
							count++;
						}
					if (count == 0)	//�Ҳ���ֵ��˵�����ܸ�ֵ;
					{
						to_continue = true;
						break;
					}
					else if (count == 1)	//����պ��ҵ�һ���͸�����ֵ����������ж�;
					{
						for (int i = 0; i < my_csp1.data[i2][j2].less.size(); i++)	//����Ҫ�ж��ھ��Ƿ������������Ȼ�������
						{
							if (my_csp1.data[my_csp1.data[i2][j2].less[i].first][my_csp1.data[i2][j2].less[i].second].is_const && tempvalue <= data1[my_csp1.data[i2][j2].less[i].first][my_csp1.data[i2][j2].less[i].second].value)
								to_continue = true;
						}
						for (int i = 0; i < my_csp1.data[i2][j2].more.size(); i++)
						{
							if (my_csp1.data[my_csp1.data[i2][j2].more[i].first][my_csp1.data[i2][j2].more[i].second].is_const && tempvalue >= data1[my_csp1.data[i2][j2].more[i].first][my_csp1.data[i2][j2].more[i].second].value)
								to_continue = true;
						}
						if (to_continue)
							break;
						data1[i2][j2].value = tempvalue;
						my_csp1.data[i2][j2].is_const = true;
						my_csp1.data[i2][j2].min = tempvalue;
						my_csp1.data[i2][j2].max = tempvalue;
						for (int i = 0; i < N; i++)
							my_csp1.data[i][j2].value[tempvalue] = false;
						for (int j = 0; j < N; j++)
							my_csp1.data[i2][j].value[tempvalue] = false;
							//if(!(i2 == 2&& j2==2))
								//exit(0);
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
	data1[i1][j1].value = -1;
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
	cout << "����ʱ�䣺" << end - begin << "s" << endl;
}
#endif // DEBUG


#ifndef DEBUG
int main()
{
	return 0;
}
#endif // !DEBUG
