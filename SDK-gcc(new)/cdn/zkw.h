#ifndef ZKW_H
#define ZKW_H

#include <cstdio>
#include <cstring>
#include <iostream>
#include <stack>
#include <map>
#include <unistd.h>
#include <string.h>

//#define INF 0x3f3f3f3f
#define maxpath 10000

using namespace std;

const int MAXN = 1300;
const int MAXM = 20000;

const char *separator = "--------------------------------------";

struct Edge_ZKW
{
    int to,next,cap,flow,cost;
    Edge_ZKW(int _to = 0,int _next = 0,int _cap = 0,int _flow = 0,int _cost = 0):
        to(_to),next(_next),cap(_cap),flow(_flow),cost(_cost){}
};



struct ZKW_MinCost//ת��Ϊ��С������
{
public:
    int nodeNum,edgeNum,consumerNum;
    int head[MAXN],tot;
    int cur[MAXN];
    int dis[MAXN];
    bool vis[MAXN];
    bool iswrite;
    int ss,tt,N;//Դ�㡢���͵���ܸ����������0~N-1��,����Ҫ���⸳ֵ�����û�ֱ�Ӹ�ֵ
    int min_cost, max_flow;
    vector<Edge_ZKW> edges;
    vector<pair<int, int> > common;
    set<int> rightdna;
    int cost = 0;
    int headbk[MAXN];
    int totbk;
    int augcnt = 0;
    int modifycnt = 0;
    int tmpflow = 0;

    stack<int> stk;//���ڴ洢·��
 
    void init(int n,int m,int cn)
    {
        tot = 0;
        iswrite = false;
        memset(head,-1,sizeof(head));
    }

    void addEdge(int u,int v,int cap,int cost)
    {
        edges.push_back(Edge_ZKW(v,head[u],cap,0,cost));
        head[u] = tot++;
        edges.push_back(Edge_ZKW(u,head[v],0,0,-cost));
        head[v] = tot++;
    }

    void save()
    {
        totbk = tot;
        memcpy(headbk, head, sizeof(head));
    }

    void reset()
    {
        tot = totbk;
        memcpy(head, headbk, sizeof(headbk));
    }
    
    int myaug(int u,int flow)
    {
        if(u == tt)
        {
            stk.push(u);
            return flow;
        }

        vis[u] = true;
        for(int i = cur[u];i != -1;i = edges[i].next)
        {
            int v = edges[i].to;
            if(!vis[v] && edges[i].flow > 0)
            {
                int tmp = myaug(v,min(flow,edges[i].flow));//����������ȡ��Сֵ��Ϊ��������

                cur[u] = i;
                if(tmp)
                {
                    stk.push(u);
                    edges[i].flow -= tmp;
                    //edges[i^1].flow += tmp;
                    return tmp;
                }
            }

        }
        return 0;
    }
 
    //����Aug��������ͨ�������BFS������(EK�㷨)���ƣ�������һ����������
    //�����ʽDist[U]=Dist[V]+edge.cost����������
    int aug(int u,int flow)
    {
        //augcnt++;
        if(u == tt)
        {
            return flow;
        }

        vis[u] = true;
        for(int i = cur[u];i != -1;i = edges[i].next)
        {
            int v = edges[i].to;
            if(edges[i].cap > edges[i].flow && !vis[v] && dis[u] == dis[v] + edges[i].cost)
            {
                int tmp = aug(v,min(flow,edges[i].cap-edges[i].flow));//����������ȡ��Сֵ��Ϊ��������
                edges[i].flow += tmp;//�������������
                edges[i^1].flow -= tmp;//����߼�С����

                cur[u] = i;
                if(tmp)
                {
                    //tmpflow += tmp;
                    return tmp;
                }
            }

        }
        return 0;
    }


    //�������粻���������ʱ�����Ǿ��޸ľ��룬�ҳ����л��������ıߣ����ұߵ���㱻���ʣ������յ�δ������
    //��Ȼ��Щ�߶�����Ϊ������Dist[U]=Dist[V]+edge.cost�ű���ϵģ�
    //���Ǽ�¼�������������Dist[V]+edge.cost-Dist[U]��Сֵ
    //�������������ҳ��������·�����������������
    bool modify_label()
    {
        //modifycnt++;
        int d = INF;
        for(int u = 0;u < N;u++){
            if(vis[u]){
                for(int i = head[u];i != -1;i = edges[i].next)
                {
                    int v = edges[i].to;
                    if(edges[i].cap>edges[i].flow && !vis[v]){
                        d = min(d,dis[v]+edges[i].cost-dis[u]);
                    }
                }
            }
        }
        if(d == INF)
        {
            //cout << dis[ss] << endl;
            return false;
        }
        for(int i = 0;i < N;i++)
            if(vis[i])
            {
                vis[i] = false;
                dis[i] += d;
                //cout << dis[ss] << endl;
            }
        return true;
    }

    pair<int,int> minCostMaxFlow(int start,int end,int n,
                                 map<vector<int>, int> &minCostPath,int m){
        if (m == 0)
        {
            iswrite = true;
        }
        ss = start, tt = end, N = n;
        min_cost = max_flow = 0;
        for(int i = 0;i < n;i++)dis[i] = 0;
        int cnt = 0;
        while(1)
        {
            //cnt++;
            for(int i = 0;i < n;i++)cur[i] = head[i];
            //cout << "bbb" << endl;
            while(1)
            {
                //cout << "aaaa" << endl;
                for(int i = 0;i < n;i++)vis[i] = false;

                int tmp = aug(ss,INF);

                if(tmp == 0)break;

                max_flow += tmp;
                min_cost += tmp*dis[ss];
                //cout << (tmpflow += tmp) << endl; 
                //cout << tmp << " * " << dis[ss] << endl;
            }
            if(!modify_label())break;
        }
        //cout << cnt << endl;
        //int cur[MAXN];
        //int dis[MAXN];
        /*for (int i = 0; i < n; i++)
        {
            cout << cur[i] << " ";
        }
        cout << endl;

        for (int i = 0; i < n; i++)
        {
            cout << dis[i] << " ";
        }
        cout << endl;*/
        if (iswrite)
        {
            for(int i = 0;i < n;i++)cur[i] = head[i];
    		while(1)
    		{
    			for(int i = 0;i < n;i++)vis[i] = false;
    			int tmp = myaug(ss,INF);
    			if(tmp){
    			    vector<int> v;
    				while(!stk.empty()){
    					int node=stk.top();
    					stk.pop();
    					v.push_back(node);
    				}

    				minCostPath[v] += tmp;
    			}
    			if(tmp == 0)break;

            }
        }
        
        /*for (auto item : edges)
        {
            cout << item.flow << endl;
        }*/
        //printf("modifycnt = %d, augcnt = %d\n", modifycnt, augcnt);
        return make_pair(min_cost,max_flow);
    }
    
    bool emodify_label()
    {
        //cout << "emodify_label" << endl;
        int d = INF;
        for(int u = 0;u < N;u++){
            if(vis[u]){
                for(int i = head[u];i != -1;i = edges[i].next)
                {
                    int v = edges[i].to;
                    if(edges[i].cap>edges[i].flow && !vis[v]){
                        d = min(d,dis[v]+edges[i].cost-dis[u]);
                    }
                }
            }
        }
        if(d == INF)
        {
            //cout << dis[ss] << endl;
            return false;
        }
        for(int i = 0;i < N;i++)
            if(vis[i])
            {
                vis[i] = false;
                dis[i] += d;
                //cout << dis[ss] << endl;
            }
        return true;
    }

    int eaug(int u,int flow)
    {
        //cout << "eaug" << endl;
        if(u == tt)
        {
            return flow;
        }

        vis[u] = true;
        for(int i = cur[u];i != -1;i = edges[i].next)
        {
            int v = edges[i].to;
            if(edges[i].cap > edges[i].flow && !vis[v] && dis[u] == dis[v] + edges[i].cost)
            {
                int tmp = eaug(v,min(flow,edges[i].cap-edges[i].flow));//����������ȡ��Сֵ��Ϊ��������
                edges[i].flow += tmp;//�������������
                edges[i^1].flow -= tmp;//����߼�С����

                cur[u] = i;
                if(tmp)
                {
                    return tmp;
                }
            }

        }
        return 0;
    }

    pair<int,int> eminCostMaxFlow(int start,int end,int n,
                                 map<vector<int>, int> &minCostPath,int m, int flow){
        if (m == 0)
        {
            iswrite = true;
        }
        ss = start, tt = end, N = n;
        min_cost = max_flow = 0;
        //for(int i = 0;i < n;i++)dis[i] = 0;
        int cnt = 0;
        while(1)
        {
            cnt++;
            for(int i = 0;i < n;i++)cur[i] = head[i];
            //cout << "bbb" << endl;
            while(1)
            {
                //cout << "aaaa" << endl;
                for(int i = 0;i < n;i++)vis[i] = false;

                int tmp = eaug(ss,INF);

                if(tmp == 0)break;

                max_flow += tmp;
                min_cost += tmp*dis[ss];
                //if (max_flow == flow)
                //return make_pair(min_cost,max_flow);
                //cout << tmp << " * " << dis[ss] << endl;
            }
            
            if(!emodify_label())break;
        }
        cout << cnt << endl;
        return make_pair(min_cost,max_flow);
    }

    pair<int, int> eraseone(set<int> dna, int erasenum, int edgenum, int netdotnum)
    {
        pair<int, int> ret;

    	int flow = 0;
    		
    	for (int i = edges.size() - 1; ; i--)
    	{
    		if (edges[i].to == erasenum)
    		{
    			flow = edges[i].flow;
    			break;
    		}
    	}
    	//cout << "flow " << flow << endl;
    	edges.resize(4 * edgenum);

    	reset();

    	dna.erase(erasenum);

    	for (auto item : dna)
    	{
    		addEdge(netdotnum, item, INF, 0);
    	}

    	addEdge(erasenum, netdotnum + 1, flow, 0);

    	map<vector<int>, int> route;
    	
    	ret = eminCostMaxFlow(netdotnum, netdotnum + 1, netdotnum + 2, route, 0, flow);

    	return ret;
    }

    bool imodify_label()
    {
        int d = INF;
        for(int u = 0;u < N;u++){
            if(vis[u]){
                for(int i = head[u];i != -1;i = edges[i].next)
                {
                    int v = edges[i].to;
                    if(edges[i].flow < 0 && !vis[v]){
                        d = min(d,dis[v]+edges[i].cost-dis[u]);
                    }
                }
            }
        }
        cout << "d " << d << endl;
        if(d == INF)
        {
            //cout << dis[ss] << endl;
            return false;
        }
        for(int i = 0;i < N;i++)
            if(vis[i])
            {
                vis[i] = false;
                dis[i] += d;
                //cout << dis[ss] << endl;
            }
        return true;
    }

    int iaug(int u,int flow)
    {
        
        if(u == tt)
        {
            return flow;
        }

        vis[u] = true;

        for(int i = cur[u];i != -1;i = edges[i].next)
        {
            int v = edges[i].to;
            
            //cout << u << " " << v << endl;
            //cout << edges[i].cap << " " << edges[i].flow << endl;
            if(edges[i].flow < 0 && !vis[v] && dis[u] == dis[v] + edges[i].cost)
            {
                //cout << edges[i].cap << " " << edges[i].flow << endl;
                //cout << min(flow,edges[i].cap-edges[i].flow) << endl;
                int tmp = iaug(v,min(flow,edges[i].cap-edges[i].flow));//����������ȡ��Сֵ��Ϊ��������
                //cout << tmp << endl;
                edges[i].flow += tmp;//�������������
                edges[i^1].flow -= tmp;//����߼�С����
                //cout << tmp << endl;
                cur[u] = i;
                if(tmp)
                {
                    return tmp;
                }

                /*if (edges[i].next == -1)
                {
                    cout << min(flow,edges[i].cap-edges[i].flow) << endl;
                    return 0;
                }*/
            }

            if (edges[i].next == -1)
            {
                cout << min(flow,edges[i].cap-edges[i].flow) << endl;
                //return 0;
            }
            
        }
        return 0;
    }

    pair<int,int> iminCostMaxFlow(int start,int end,int n,
                                 map<vector<int>, int> &minCostPath,int m){
        if (m == 0)
        {
            iswrite = true;
        }
        ss = start, tt = end, N = n;
        min_cost = max_flow = 0;
        while(1)
        {
            for(int i = 0;i < n;i++)cur[i] = head[i];
            //cout << "bbb" << endl;
            while(1)
            {
                //cout << "aaaa" << endl;
                for(int i = 0;i < n;i++)vis[i] = false;

                int tmp = iaug(ss,INF);

                if(tmp == 0)break;

                max_flow += tmp;
                min_cost += tmp*dis[ss];
                cout << tmp << " * " << dis[ss] << endl;
            }
            if(!imodify_label())break;
        }

        return make_pair(min_cost,max_flow);
    }
    
    pair<int, int> insertone(set<int> dna, int insertnum, int edgenum, int netdotnum)
    {
        edges.resize(4 * edgenum);

        reset();
        
        for (auto item : dna)
        {
            addEdge(item, netdotnum + 1, INF, 0);
            //addEdge(netdotnum + 1, item, INF, 0);
        }

        //addEdge(netdotnum, insertnum, INF, 0);
        //addEdge(insertnum, netdotnum, INF, 0);

        map<vector<int>, int> route;
        
        pair<int, int> ret = iminCostMaxFlow(insertnum, netdotnum + 1, netdotnum + 2, route, -1);

        return ret;
    }
};

#endif // ZKW_H
