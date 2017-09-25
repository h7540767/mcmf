#include "mcmf.h"

MCMF::MCMF()
{

}

MCMF::~MCMF()
{

}

void MCMF::init()
{
	tot = 0;
	memset(head,-1,sizeof(head));
	return ;
}

void MCMF::addedge(int u,int v,int cap,int cost)
{
	edges.push_back(edge_s(v,head[u], cap, 0, cost));
    head[u] = tot++;
    edges.push_back(edge_s(u,head[v], 0, 0, -cost));
    head[v] = tot++;
	return ;
}

int MCMF::dij(int s, int t, int n, int dnasize, int consumedotnum)
{
	priority_queue<pii, vector<pii>, greater<pii> > q;
	bool visited[MAXDOTNUM];
	int dis[MAXDOTNUM];
	int cur[MAXDOTNUM];
	pii prev[MAXDOTNUM];

	for (int i = 0; i < n; i++)
	{
		dis[i] = (i == s ? 0 : INF);
		cur[i] = head[i];
		prev[i] = make_pair(-1, -1);
	}

	memset(visited, false, sizeof(visited));

	q.push(make_pair(dis[s], s));

	while (!q.empty())
	{
		pii u = q.top();
		q.pop();
		int x = u.second;
		if (visited[x])
		{
			continue;
		}

		visited[x] = true;

		for (int i = cur[x]; i != -1; i = edges[i].next)
		{
			int y = edges[i].to;
			if (edges[i].cap - edges[i].flow > 0 && dis[y] > dis[x] + edges[i].cost)
			{
				dis[y] = dis[x] + edges[i].cost;
				prev[y] = make_pair(x, i);
				q.push(make_pair(dis[y], y));
			}
		}
	}

	int approfit = 0;

	for (int i = edges.size() - 1 - dnasize * 2; consumedotnum--; i -= 2)
	{
		//cout << edges[i].to << " ";
		//cout << dis[edges[i].to] << " * " << edges[i ^ 1].cap << endl;
		approfit += dis[edges[i].to] * edges[i ^ 1].cap;
	}

	//cout << "jinshi " << approfit << endl;
	return approfit;
	/*if (dis[t] == INF)
	{
		return false;
	}

	int dot = t;
	flow = INF;

	while (prev[dot].first != -1)
	{
		flow = min(flow, edges[prev[dot].second].cap - edges[prev[dot].second].flow);
		dot = prev[dot].first;
	}
	dot = t;
	while (prev[dot].first != -1)
	{
		edges[prev[dot].second].flow += flow;
		edges[prev[dot].second ^ 1].flow -= flow;
		dot = prev[dot].first;
	}

	cost = dis[t];
	return true;*/
}

int MCMF::mcmf(int s, int t, int n, unsigned int dnasize, int consumedotnum)
{
	return dij(s, t, n, dnasize, consumedotnum);
}
