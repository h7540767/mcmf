#ifndef MCMF_H
#define MCMF_H

#include <bits/stdc++.h>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <stack>
#include <map>
#include <unistd.h>
#include <string.h>

using namespace std;

#define MAXDOTNUM 10000

#ifndef INF
#define INF 0x3f3f3f3f
#endif

typedef pair<int,int> pii;

struct edge_s
{
    int to,next,cap,flow,cost;
    edge_s(int _to = 0,int _next = 0,int _cap = 0,int _flow = 0,int _cost = 0):
        to(_to),next(_next),cap(_cap),flow(_flow),cost(_cost){}
};

class MCMF
{
    public:
    int tot;
    vector<edge_s> edges;
    int head[MAXDOTNUM];

    public:
    MCMF();
    ~MCMF();
    void init();
    void addedge(int u,int v,int cap,int cost);
    int dij(int s, int t, int n, int dnasize, int consumedotnum);
    int mcmf(int s, int t, int n, unsigned int dnasize, int consumedotnum);
};

#endif
