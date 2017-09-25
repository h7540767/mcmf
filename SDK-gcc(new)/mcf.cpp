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

typedef pair<int, int> P;//first保存最短距离，second保存顶点的编号

struct Edge
{
	int to, cap, cost, rev;//终点，容量（指残量网络中的），费用，反向边编号
	Edge(int t, int c, int cc, int r) :to(t), cap(c), cost(cc), rev(r){}
};

int dotnum = 4;//顶点数
vector<Edge> G[N];//图的邻接表
int h[N];//顶点的势
int dist[N];//最短距离
int prevdot[N];//最短路中的父结点
int prevedge[N];//最短路中的父边
stack<int> route;

void addedge(int from, int to, int cap, int cost)
{
	G[from].push_back(Edge( to, cap, cost, G[to].size()));
	G[to].push_back(Edge( from, cap, cost, G[from].size() - 1 ));
}

int min_cost_flow(int s, int t, int flow)//返回最小费用
{
	int res = 0;
	int maxflow = 0;
	fill(h, h + dotnum, 0);
	while (flow > 0)//flow>0时还需要继续增广
	{
		vector<bool> visited(dotnum);
		fill(visited.begin(), visited.end(), false);
		priority_queue<P, vector<P>, greater<P> > q;
		fill(dist, dist + dotnum, INF);//距离初始化为INF
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
			//p.first是v入队列时候的值，dist[dotid]是目前的值，如果目前的更优，扔掉旧值
			for (int i = 0; i < G[dotid].size(); i++)
			{
				
				Edge&e = G[dotid][i];
				if (e.cap > 0 && dist[e.to] > dist[dotid] + e.cost + h[dotid] - h[e.to])
					//松弛操作
				{
					dist[e.to] = dist[dotid] + e.cost + h[dotid] - h[e.to];
					prevdot[e.to] = dotid;//更新父结点
					prevedge[e.to] = i;//更新父边编号
					q.push(P(dist[e.to], e.to));
					//cout << "push " << e.to << endl;
				}
			}
		}
		
		if (dist[t] == INF)//如果dist[t]还是初始时候的INF，那么说明s-t不连通，不能再增广了
		{
			return -1;
		}
		//cout << "aaaaaaaa" << endl;
		for (int j = 0; j < dotnum; j++)//更新h
			h[j] += dist[j];
		int d = flow;
		
		for (int x = t; x != s; x = prevdot[x])
		{
			route.push(x);
			d = min(d, G[prevdot[x]][prevedge[x]].cap);//从t出发沿着最短路返回s找可改进量
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
		//h[t]表示最短距离的同时，也代表了这条最短路上的费用之和，乘以流量d即可得到本次增广
		//所需的费用
		for (int x = t; x != s; x = prevdot[x])
		{
			Edge&e = G[prevdot[x]][prevedge[x]];
			e.cap -= d;//修改残量值
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

