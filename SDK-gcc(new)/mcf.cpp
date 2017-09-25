#include<iostream>
#include<algorithm>
#include<string>
#include<sstream>
#include<set>
#include<vector>
#include<stack>
#include<map>
#include<queue>
#include<deque>
#include<cstdlib>
#include<cstdio>
#include<cstring>
#include<cmath>
#include<ctime>
#include<functional>
using namespace std;

#define N 1000
#define INF 100000000

typedef pair<int, int> P;//first������̾��룬second���涥��ı��

struct Edge
{
	int to, cap, cost, rev;//�յ㣬������ָ���������еģ������ã�����߱��
	Edge(int t, int c, int cc, int r) :to(t), cap(c), cost(cc), rev(r){}
};

int dotnum = 4;//������
vector<Edge> G[N];//ͼ���ڽӱ�
int h[N];//�������
int dist[N];//��̾���
int prevdot[N];//���·�еĸ����
int prevedge[N];//���·�еĸ���
stack<int> route;

void addedge(int from, int to, int cap, int cost)
{
	G[from].push_back(Edge( to, cap, cost, G[to].size()));
	G[to].push_back(Edge( from, cap, cost, G[from].size() - 1 ));
}

int min_cost_flow(int s, int t, int flow)//������С����
{
	int res = 0;
	int maxflow = 0;
	fill(h, h + dotnum, 0);
	while (flow > 0)//flow>0ʱ����Ҫ��������
	{
		vector<bool> visited(dotnum);
		fill(visited.begin(), visited.end(), false);
		priority_queue<P, vector<P>, greater<P> > q;
		fill(dist, dist + dotnum, INF);//�����ʼ��ΪINF
		dist[s] = 0;
		q.push(P(0, s));
		while (!q.empty())
		{
			P p = q.top(); 
			q.pop();
			int dotid = p.second;
			//cout << "pop " << dotid << endl;
			if (dist[dotid] < p.first || visited[dotid] == true)
				continue;
			//visited[dotid] = true;
			//cout << "ddddddddddd" << endl;
			//cout << dotid << endl;
			//p.first��v�����ʱ���ֵ��dist[dotid]��Ŀǰ��ֵ�����Ŀǰ�ĸ��ţ��ӵ���ֵ
			for (int i = 0; i < G[dotid].size(); i++)
			{
				
				Edge&e = G[dotid][i];
				if (e.cap > 0 && dist[e.to] > dist[dotid] + e.cost + h[dotid] - h[e.to])
					//�ɳڲ���
				{
					dist[e.to] = dist[dotid] + e.cost + h[dotid] - h[e.to];
					prevdot[e.to] = dotid;//���¸����
					prevedge[e.to] = i;//���¸��߱��
					q.push(P(dist[e.to], e.to));
					//cout << "push " << e.to << endl;
				}
			}
		}
		
		if (dist[t] == INF)//���dist[t]���ǳ�ʼʱ���INF����ô˵��s-t����ͨ��������������
		{
			return -1;
		}
		//cout << "aaaaaaaa" << endl;
		for (int j = 0; j < dotnum; j++)//����h
			h[j] += dist[j];
		int d = flow;
		
		for (int x = t; x != s; x = prevdot[x])
		{
			route.push(x);
			d = min(d, G[prevdot[x]][prevedge[x]].cap);//��t�����������·����s�ҿɸĽ���
		}
		route.push(s);
		while (!route.empty())
		{
			int top = route.top();
			route.pop();
			printf("%d ", top);
		}
		maxflow += d;
		printf("%d\n", d);
		flow -= d;
		res += d * h[t];
		//h[t]��ʾ��̾����ͬʱ��Ҳ�������������·�ϵķ���֮�ͣ���������d���ɵõ���������
		//����ķ���
		for (int x = t; x != s; x = prevdot[x])
		{
			Edge&e = G[prevdot[x]][prevedge[x]];
			e.cap -= d;//�޸Ĳ���ֵ
			//G[x][e.rev].cap += d;
		}
	}
	return res;
}

int main()
{
	FILE *fp = fopen("data.txt", "r");
	char buf[20];
	while (fgets(buf, sizeof(buf), fp) != NULL)
	{
		int from, to, cap, cost;
		sscanf(buf, "%d %d %d %d\n", &from, &to, &cap, &cost);
		addedge(from, to, cap, cost);
	}
	cout << min_cost_flow(0, 3, 28) << endl;
	return 0;
}

