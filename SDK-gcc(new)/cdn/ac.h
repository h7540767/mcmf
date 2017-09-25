#ifndef AC_H
#define AC_H

#include <bits/stdc++.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "deploy.h"
#include <time.h>
#include <random>
#include <unordered_map>
//#include "zkw.h"

using namespace std;

#define INF 0x3f3f3f3f
#define MAXLINE 5000
#define INITNUM 120000 / netdotnum
#define CNT 1000

#ifdef _WIN32
#define ENTER '\n'
#endif

#ifdef __linux__
#define ENTER '\r'
#endif

typedef struct
{
	int end;
	int bindwidth;
	int price;
}edge_t;

typedef struct
{
	int netdot;
	int need;
}area_t;

typedef struct
{
    set<int> dna;
    vector<int> level;
    int fitness;
}plan_t;

typedef struct
{
    int outpower;
    int cost;
}serverinfo_t;

typedef pair<int, int> pair_t;

struct CostFlow
{
    int cost=0;
    int flow=0;
};

class AC
{
    public:
        AC();
        ~AC();
        void loadinfor(char * topo[]);
        void showinfor();
		void ga(char * filename);
        void showret();
        void result(char * filename);
        set<int> bestdna;
        int lastbest;
        vector<int> magicruler;
        plan_t bestplan;

        void writeret(set<int> &dna, char * filename);
        void randgreedy();
        void newway(char * filename);
        void testzkw();
        void test(char * filename);
        void testmcmf(char * filename);
        void testspfa(char * filename);
        int zkwcnt;
        int badserver;
        int level;
        
    private:
        vector<vector<edge_t>> edge;
	    vector<area_t> area;
        int netdotnum, edgenum, consumedotnum;
	    vector<serverinfo_t> serverinfo;
	    vector<int> netdotcost;
        int bits;
        int sumneed;
        vector<int> net2consume;
        vector<int> nice;
        vector<pair<int, int> > power;
        vector<pair<int, int> > rawpower;
        vector<pair<int, int> > zhilianpower;
        vector<int> niceruler;
		clock_t start;
		unordered_map<string, bool> myhash;
		plan_t splan;
		plan_t nplan;
		plan_t initplan;
		set<int> zhilian;
		int ser;
		

        int myrand(int start, int end);
        int calfit(set<int> &dna, bool iswriteret, char * filename);
        vector<plan_t> makelove(vector<plan_t> & tmplans, plan_t b);
        bool exchange(vector<plan_t> & tmplans, vector<plan_t> & plans);
        bool issame(vector<plan_t> & plans);
        bool violence(vector<plan_t> & plans);
        void variation(vector<plan_t> & plans);
        void showplan(vector<plan_t> & plans);
        void checkplan(vector<plan_t> plans);
        void showdna(set<int> dna);
        static bool compare(plan_t a, plan_t b);
        static bool compare1(pair<int, int> a, pair<int, int> b);
        static bool cmp(edge_t a, edge_t b);
        set<int> randna(int length);
        string set2string(set<int> dna);
        void mark(set<int> dna);
        bool refresh(int length);
        int min_cost_flow(set<int> & server, bool iswriteret, char * filename);
        int checkroute(map<vector<int>, int>route);
        void showroute(map<vector<int>, int> route);
        void initpop(vector<plan_t> &plans);
        void refreshbestplan(vector<plan_t> &plans);
        
        vector<plan_t> merg(vector<plan_t> plans, vector<plan_t> tmplans);
        int erase0cost(set<int> &dna, map<int, int> &m);
        int fenxiroute(set<int> &dna);
        map<vector<int>, int> routeinfor(set<int> &dna);
        void crossdot(set<int> &dna);
        int approfit(set<int> & dna);
        int newapprofit(set<int> & dna);
};


#endif