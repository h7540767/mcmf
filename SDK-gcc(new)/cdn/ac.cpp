#include "ac.h"
#include "zkw.h"
#include "mcmf.h"
#include "isap.h"

ZKW_MinCost rawzkw;

AC::AC()
{
	sumneed = 0;
	lastbest = INF;
	zkwcnt = 0;
	bestplan.fitness = INF;
	ser = 0;
}

AC::~AC()
{

}

void  AC::loadinfor(char * topo[])
{
	start = clock();

	int topoline = 0;

	sscanf(topo[topoline], "%d %d %d\n", &netdotnum, &edgenum, &consumedotnum);

	for (topoline = topoline + 2; topo[topoline][0] != ENTER; topoline++)
	{
		serverinfo_t tmp;
		int index;
		sscanf(topo[topoline], "%d %d %d\n", &index, &tmp.outpower, &tmp.cost);
		serverinfo.push_back(tmp);
	}
	level = serverinfo.size() - 2;
	//rawzkw.init(netdotnum, edgenum, consumedotnum);
	netdotcost.resize(netdotnum);
	edge.resize(netdotnum + 5);
	area.resize(consumedotnum);
	net2consume.resize(netdotnum, -1);
	//nice.resize(netdotnum, 0);
	//power.resize(netdotnum);
	//magicruler.resize(consumedotnum, INF);

	for (topoline = topoline + 1; topo[topoline][0] != ENTER; topoline++)
	{
		int index;
		int cost;
		sscanf(topo[topoline], "%d %d\n", &index, &cost);
		netdotcost[index] = cost;
	}

	for (topoline = topoline + 1; topo[topoline][0] != ENTER; topoline++)
	{
		int start_, end_, bindwidth_, price_;
		edge_t tmp;

		sscanf(topo[topoline], "%d %d %d %d\n", &start_, &end_, &bindwidth_, &price_);

		tmp.bindwidth = bindwidth_;
		tmp.price = price_;

		tmp.end = end_;
		edge[start_].push_back(tmp);
		tmp.end = start_;
		edge[end_].push_back(tmp);
	}

	/*for (int i = 0; i < netdotnum; i++)
	{
		for (int j = 0; j < edge[i].size(); j++)
		{
			rawzkw.addEdge(i, edge[i][j].end, edge[i][j].bindwidth, edge[i][j].price);
		}
	}*/

	for (topoline = topoline + 1;; topoline++)
	{

		int areaid, netdotid, need_;

		sscanf(topo[topoline], "%d %d %d\n", &areaid, &netdotid, &need_);
		area[areaid].netdot = netdotid;
		net2consume[netdotid] = areaid;
		area[areaid].need = need_;

		//nice[netdotid] += need_;
		//power[netdotid].first = netdotid;
		//power[netdotid].second += need_;

		sumneed += need_;
		//zhilian.insert(netdotid);

		if (areaid == consumedotnum - 1)
		{
			break;
		}
	}

	/*for (int i = 0; i < netdotnum; i++)
	{
		power[i].first = i;
		for (int j = 0; j < edge[i].size(); j++)
		{
			nice[i] += edge[i][j].bindwidth / edge[i][j].price;
			power[i].second += edge[i][j].bindwidth;
		}
	}

	rawpower = power;

	sort(power.begin(), power.end(), compare1);

	for (int i = 0; i < netdotnum; i++)
	{
		if (zhilian.find(power[i].first) != zhilian.end())
		{
			zhilianpower.push_back(power[i]);
		}
	}

	niceruler = nice;

	for (int i = 1; i < netdotnum; i++)
	{
		niceruler[i] += niceruler[i - 1];
	}*/

	return ;
}

void AC::showinfor()
{
	printf("netdotnum = %d, edgenum = %d, consumedotnum = %d\n",
			netdotnum, edgenum, consumedotnum);

	for (auto item : serverinfo)
	{
		printf("%d %d\n", item.outpower, item.cost);
	}
	getchar();
	for (int i = 0; i < netdotnum; i++)
	{
		for (int j = 0; j < edge[i].size(); j++)
		{
			printf("%d %d %d %d\n", i, edge[i][j].end, edge[i][j].bindwidth, edge[i][j].price);
		}
	}

	for (int i = 0; i < consumedotnum; i++)
	{
		printf("%d %d %d\n", i, area[i].netdot, area[i].need);
	}

	return ;
}

int AC::myrand(int start, int end)
{
	if (start > end)
	{
		cout << "myrand usage: start <= end\n";
		cout << "start = " << start << " end = " << end << endl;
		exit(-1);
	}
	return rand() % (end - start + 1) + start;
}

map<vector<int>, int> AC::routeinfor(set <int> &dna)
{
	++zkwcnt;

	ZKW_MinCost zkw;

	zkw.init(netdotnum, edgenum, consumedotnum);

	for (int i = 0; i < netdotnum; i++)
	{
		for (int j = 0; j < edge[i].size(); j++)
		{
			zkw.addEdge(i, edge[i][j].end, edge[i][j].bindwidth, edge[i][j].price);
		}
	}

	for (auto item : dna)
	{
		zkw.addEdge(netdotnum, item, INF, 0);
	}

	map<vector<int>, int> route;

	zkw.minCostMaxFlow(netdotnum, netdotnum + 1, netdotnum + 2, route, 0);

	return route;
}

int AC::erase0cost(set <int> &dna, map <int, int> &m)
{
	map<vector<int>, int> route = routeinfor(dna);

	for (auto item : route)
	{
		int cost = 0;
		for (int i = 1; i < item.first.size() - 2; i++)
		{
			for (int j = 0; j < edge[item.first[i]].size(); j++)
			{
				if (edge[item.first[i]][j].end == item.first[i + 1])
				{
					cost += edge[item.first[i]][j].price;
					break;
				}
			}
		}
		m[item.first[item.first.size() - 2]] += cost * item.second;
		//printf("%d -> %d cost %d\n", item.first[1], item.first[item.first.size() - 2], cost * item.second);
	}

	return 0;
}

int AC::calfit(set<int> &dna, bool iswriteret, char * filename)
{
	++zkwcnt;
	ZKW_MinCost zkw;
	const int ab = 0;
	zkw.init(netdotnum, edgenum, consumedotnum);

	for (int i = 0; i < netdotnum; i++)
	{
		for (int j = 0; j < edge[i].size(); j++)
		{
			zkw.addEdge(i, edge[i][j].end, edge[i][j].bindwidth, edge[i][j].price);
		}
	}
	//zkw = rawzkw;

	for (int i = 0; i < consumedotnum; i++)
	{
		zkw.addEdge(area[i].netdot, netdotnum + 1, area[i].need, 0);
		#if ab
		if (dna.find(area[i].netdot) != dna.end())
		{
			zkw.edges[zkw.edges.size() - 1].flow -= area[i].need;
			zkw.edges[zkw.edges.size() - 2].flow += area[i].need;
		}
		#endif
	}

	for (auto item : dna)
	{
		zkw.addEdge(netdotnum, item, serverinfo[level].outpower, 0);
		#if ab
		if (net2consume[item] != -1)
		{
			zkw.edges[zkw.edges.size() - 1].flow -= area[net2consume[item]].need;
			zkw.edges[zkw.edges.size() - 2].flow += area[net2consume[item]].need;
		}
		#endif

	}

	map<vector<int>, int> route;

	int lines;

	pair<int, int> ret;

	if (iswriteret)
	{
		lines = 0;
	}
	else
	{
		lines = -1;
	}

	ret = zkw.minCostMaxFlow(netdotnum, netdotnum + 1, netdotnum + 2, route, lines);

	//cout << ret.first << " " << ret.second << endl;
	int servercost = 0;

	int installcost = 0;

	for (int i = 1; i <= dna.size(); i++)
	{
		int j = serverinfo.size() - 1;
		//cout << zkw.edges[zkw.edges.size() - 2 * i].to << " " << -zkw.edges[zkw.edges.size() - 2 * i + 1].flow << endl;
		for (; j >= 0 && serverinfo[j].outpower >= -zkw.edges[zkw.edges.size() - 2 * i + 1].flow; j--);

		servercost += serverinfo[j + 1].cost;

		/*cout << -zkw.edges[zkw.edges.size() - 2 * i + 1].flow << " " <<
		serverinfo[j + 1].outpower << " " << j + 1 << endl;*/
	}

	for (auto item : dna)
	{
		installcost += netdotcost[item];
	}

	if (iswriteret)
	{
		unordered_map<int, int> serverlevel;

		for (int i = 1; i <= dna.size(); i++)
		{
			int j = serverinfo.size() - 1;
			//cout << zkw.edges[zkw.edges.size() - 2 * i].to << " " << -zkw.edges[zkw.edges.size() - 2 * i + 1].flow << endl;
			//sleep(1);
			for (; j >= 0 && serverinfo[j].outpower >= -zkw.edges[zkw.edges.size() - 2 * i + 1].flow; j--);

			serverlevel[zkw.edges[zkw.edges.size() - 2 * i].to] = j + 1;

		}

		string ret;

		char buf[MAXLINE];

		sprintf(buf, "%d\n\n", route.size());

		ret += buf;

		set<int> s;
		map<int, int> m;
		for (auto i : route)
		{
			//cout << i.first[i.first.size() - 2] << " ";
			m[i.first[1]] += i.second;
			s.insert(i.first[i.first.size() - 2]);
			for (int j = 1; j < i.first.size() - 1; j++)
			{
				sprintf(buf, "%d ", i.first[j]);

				ret += buf;
			}
			sprintf(buf, "%d ", net2consume[i.first[i.first.size() - 2]]);
			ret += buf;
			sprintf(buf, "%d ", i.second);
			ret += buf;
			sprintf(buf, "%d\n", serverlevel[i.first[1]]);
			ret += buf;
		}
		ret[ret.size() - 1] = '\0';
		write_result((char *)ret.data(), filename);
		/*ser += s.size();
		for (auto i : m)
		{
			cout << i.first << " ";

			int j = serverinfo.size() - 1;

			for (; j >= 0 && serverinfo[j].outpower >= i.second; j--);

			cout << j + 1 << endl;
		}*/
		//cout << " " << s.size() << "| ";
		printf("server %d install %d\n", servercost, installcost);
	}

	if (filename == "flow")
	{
		return ret.second;
	}

	if (ret.second != sumneed)
	{
		//cout << "aaa" << endl;
		//printf("lianlu %d servercost %d installcost %d\n", ret.first, servercost, installcost);
		return INF;
	}
	else
	{
		//printf("lianlu %d servercost %d installcost %d\n", ret.first, servercost, installcost);
		return ret.first + servercost + installcost;
	}
}

vector<plan_t> AC::makelove(vector<plan_t> & tmplans, plan_t b)
{
	vector<plan_t> ret(INITNUM);

	for (int num = 0; num < INITNUM; )
	{
		plan_t fathertmp, mothertmp;

		int father = -1;
		int mother = -1;

		do
		{
			fathertmp = b;//tmplans[myrand(0, INITNUM - 1)];
			mothertmp = tmplans[myrand(0, INITNUM - 1)];

			int fathersize = 1;//myrand(1, fathertmp.dna.size() / 3 + 2);
			int mothersize = 1;//myrand(1, mothertmp.dna.size() / 3 + 2);

			set<int> fatherdna, motherdna;

			for (int i = 0; i < fathersize; i++)
			{

				int cnt = myrand(0, fathertmp.dna.size() - 1);

				int tmp = -1;
				for (set<int>::iterator it = fathertmp.dna.begin(); cnt-- >= 0; it++)
				{
					tmp = *it;
				}
				fathertmp.dna.erase(tmp);
				fatherdna.insert(tmp);
				//cout << tmp << " ";
			}

			for (int i = 0; i < mothersize; i++)
			{
				int cnt = myrand(0, mothertmp.dna.size() - 1);
				int tmp = -1;
				for (set<int>::iterator it = mothertmp.dna.begin(); cnt-- >= 0; it++)
				{
					tmp = *it;
				}
				mothertmp.dna.erase(tmp);
				motherdna.insert(tmp);
				//cout << tmp << endl;
			}

			//showdna(fatherdna);
			//showdna(motherdna);

			if (fatherdna == motherdna)
			{
			    cout << "goto..." << endl;
			    showplan(tmplans);
				//goto xx;
			}

			for (auto item : motherdna)
			{
				fathertmp.dna.insert(item);
			}

			for (auto item : fatherdna)
			{
				mothertmp.dna.insert(item);
			}

			fathertmp.fitness = calfit(fathertmp.dna, false, NULL);
			mothertmp.fitness = calfit(mothertmp.dna, false, NULL);
			//cout << "unormal" << endl;
		}while(fathertmp.fitness == -1 && mothertmp.fitness == -1);


		//cout << father << " " << mother << endl;
		if (fathertmp.fitness != -1 && num < INITNUM)
			ret[num++] = fathertmp;
		if (mothertmp.fitness != -1 && num < INITNUM)
			ret[num++] = mothertmp;
	}

	return ret;
}

bool AC::exchange(vector<plan_t> & tmplans, vector<plan_t> & plans)
{
	int MIN = INF;
	int PLANMAX = 0;
	int PLANMIN = INF;
	int minid, planmaxid, planminid;
	bool isexchange = false;
	for (int i = 0; i < plans.size(); i++)
	{
		if (plans[i].fitness > PLANMAX)
		{
			PLANMAX = plans[i].fitness;
			planmaxid = i;
		}

		if (plans[i].fitness < PLANMIN)
		{
			PLANMIN = plans[i].fitness;
			planminid = i;
		}

		if (tmplans[i].fitness != -1 && tmplans[i].fitness < MIN)
		{
			MIN = tmplans[i].fitness;
			minid = i;
		}
	}
	//printf("the best after makelove %d\n", MIN);
	if (plans[planminid].fitness > tmplans[minid].fitness)
	{
		//printf("exchange %d\n", tmplans[minid].fitness);
		plans[planmaxid] = tmplans[minid];
		isexchange = true;
	}
	/*else if (plans[planmaxid].fitness > tmplans[minid].fitness)
	{
		bool repeate = false;
		for (int i = 0; i < INITNUM; i++)
		{
			if (tmplans[minid].dna == plans[i].dna)
			{
				repeate = true;
				break;
			}
		}
		if (!repeate)
		{
			plans[planmaxid] = tmplans[minid];
			isexchange = true;
		}
	}*/

	return isexchange;
}

bool AC::issame(vector<plan_t> & plans)
{
	bool ret = true;
	for (int i = 1; i < plans.size(); i++)
	{
		if (plans[i].fitness != plans[0].fitness)
		{
			ret = false;
			break;
		}
	}

	return ret;
}

void AC::variation(vector<plan_t> & plans)
{
	/*for (int i = 1; i < INITNUM; i++)
	{
		int f = -1;
		do
		{
			int cnt = myrand(0, plans[i].dna.size() - 1);

		}while(f == -1);
	}*/

	return ;
}

bool AC::violence(vector<plan_t> & plans)
{
	int MIN = INF;
    int MAX = 0;
    int minid;
    int maxid;
    bool foundnew = false;

    /*for (int i = 0; i < plans.size(); i++)
    {
    	if (plans[i].fitness < MIN)
        {
        	MIN = plans[i].fitness;
            minid = i;
        }

        if (plans[i].fitness > MAX)
        {
        	MAX = plans[i].fitness;
            maxid = i;
        }
    }*/
	//for (int k = 0; k < INITNUM; k++)
    for (auto item : plans[0].dna)
	{
		for (int i = 0; i < netdotnum; i++)
		{
			if ((clock() - start) / CLOCKS_PER_SEC > 85)
			{
				return false;
			}
			set<int> dna = plans[0].dna;
			dna.erase(item);
			dna.insert(i);
			int fitness = calfit(dna, false, NULL);
			if (fitness != -1 && fitness < plans[0].fitness)
			{
				foundnew = true;

				if (zhilian.find(item) != zhilian.end())
				{
					cout << item << " zhilian" << endl;
				}

				if (zhilian.find(i) != zhilian.end())
				{
					cout << i << " zhilian" << endl;
				}

				if (dna.size() == plans[0].dna.size())
				{
					printf("erase %d insert %d\n", item, i);
				}
				else
				{
					printf("erase %d\n", item);
				}
				plans[0].dna = dna;
				plans[0].fitness = fitness;
				cout << "fitness " << fitness << endl;
				return foundnew;
			}
		}
	}

    for (int i = 0; i < netdotnum; i++)
    {
    	if ((clock() - start) / CLOCKS_PER_SEC > 85)
		{
			return false;
		}
    	set<int> dna = plans[0].dna;

        dna.insert(i);

        int fitness = calfit(dna, false, NULL);

        if (fitness != -1 && fitness < plans[0].fitness)
        {
        	foundnew = true;
            plans[0].dna = dna;
            plans[0].fitness = fitness;
            if (zhilian.find(i) != zhilian.end())
			{
				cout << i << " zhilian" << endl;
			}
            cout << "insert " << i << endl;
            cout << "fitness " << fitness << endl;
            return foundnew;
        }
    }

	//showplan(plans);
	return foundnew;
}

void AC::writeret(set<int> &dna, char * filename)
{
	cout << calfit(dna, true, filename) << endl;
	return ;
}

void AC::initpop(vector<plan_t> &plans)
{
	//cout << plans[0].fitness << endl;
	//return ;
	if (netdotnum > 700)
	{
		int loopcnt = consumedotnum - 1;
		int deta = 0;
		for (int i = 0; i < INITNUM; i++)
		{
			int timeout = 0;

			for (;;)
			{
				if (timeout == 20)
				{
					cout << "deta " << deta << endl;
					deta += 2;
					timeout = 0;
				}
				set<int> tmpdna;
				int tmpfitness;

				for (int j = 0; j < loopcnt + deta; j++)
				{
					if (myrand(0, 1 + deta) > 0)
					{
						tmpdna.insert(area[myrand(0, consumedotnum- 1)].netdot);
						//tmpdna.insert(myrand(0, netdotnum - 1));
						/*int randnum = myrand(0, niceruler.back());

						for (int k = 0; k < niceruler.size(); k++)
						{
							if (randnum <= niceruler[k])
							{
								tmpdna.insert(k);
								break;
							}
						}*/
					}
				}

				if ((tmpfitness = calfit(tmpdna, false, NULL)) != -1)
				{
					plans[i].dna = tmpdna;
					plans[i].fitness = tmpfitness;
					//cout << tmpfitness << endl;
					break;
				}
				timeout++;
				//cout << "unormal" << endl;
			}
			cout << i << endl;
			//cout << plans[i].fitness << " ";
		}
	}
	else
	{
		for (int i = 0; i < INITNUM; i++)
		{
			for(;;)
			{
				plan_t tmp;
				set<int> s;

				int consumepart = myrand(1, consumedotnum / 3);

				while(tmp.dna.size() != consumepart)
				{
					int id = area[myrand(0, consumedotnum - 1)].netdot;
					tmp.dna.insert(id);
					s.insert(id);
				}

				int sumpart = myrand(consumedotnum / 2, consumedotnum - 1);

				while(tmp.dna.size() != sumpart)
				{
					tmp.dna.insert(myrand(0, netdotnum - 1));
				}

				if ((tmp.fitness = calfit(tmp.dna, false, NULL)) != -1)
				{
					plans[i] = tmp;
					break;
				}
			}
			cout << plans[i].fitness << endl;
			//cout << "consume servers " << plansconsume[i].size() << endl;
		}
	}

	sort(plans.begin(), plans.end(), compare);

	return ;
}

void AC::refreshbestplan(vector < plan_t > & plans)
{
	int best = INF;
	int bestid = -1;
	for (int i = 0; i < plans.size(); i++)
	{
		if (best > plans[i].fitness)
		{
			best = plans[i].fitness;
			bestid = i;
		}
	}

	if (best < bestplan.fitness)
	{
		cout << "find better" << endl;
		bestplan = plans[bestid];
	}
}

void AC::testmcmf(char * filename)
{
	int num[] = {7,14,17,26,32,35,43,50,59,74,82,89,96,101,111,120,124,126,138,147,
	161,166,167,175,178,186,187,194,203,205,218,227,234,238,242,252,254,263,267,270,
	271,275,278,281,288,308,321,326,328,331,334,336,338,346,349,363,370,375,383,387,
	397,402,409,415,416,423,426,438,463,464,474,475,482,491,500,503,505,507,520,525,
	529,542,548,552,557,570,576,584,592,594,617,632,634,641,643,646,660,663,671,677,
	679,686,711,718,719,724,731,739,742,745,751,756,757,760,767,770,774,778,784,791,795};

	set<int> server(num, num+sizeof(num) / sizeof(int));

	/*for (int i = 0; i < netdotnum; i++)
	{
		for (int j = 0; j < edge[i].size(); j++)
		{
			addedge(i, edge[i][j].end, edge[i][j].price, edge[i][j].bindwidth);
		}
	}

	for (int i = 0; i < consumedotnum; i++)
	{
		addedge(area[i].netdot, netdotnum + 1, 0, area[i].need);
	}

	for (auto item : server)
	{
		addedge(netdotnum, item, 0, INF);
	}*/


	cout << "cost " << calfit(server, false, NULL) << endl;

	return ;
}

void AC::testzkw()
{
	/*7,14,17,26,32,35,43,50,59,74,82,89,96,101,111,120,124,126,138,147,
	161,166,167,175,178,186,187,194,203,205,218,227,234,238,242,252,254,263,267,270,
	271,275,278,281,288,308,321,326,328,331,334,336,338,346,349,363,370,375,383,387,
	397,402,409,415,416,423,426,438,463,464,474,475,482,491,500,503,505,507,520,525,
	529,542,548,552,557,570,576,584,592,594,617,632,634,641,643,646,660,663,671,677,
	679,686,711,718,719,724,731,739,742,745,751,756,757,760,767,770,774,778,784,791,795*/
	int num[] = {7,14,17,26,32,35,43,50,59,74,82,89,96,101,111,120,124,126,138,147,
	161,166,167,175,178,186,187,194,203,205,218,227,234,238,242,252,254,263,267,270,
	271,275,278,281,288,308,321,326,328,331,334,336,338,346,349,363,370,375,383,387,
	397,402,409,415,416,423,426,438,463,464,474,475,482,491,500,503,505,507,520,525,
	529,542,548,552,557,570,576,584,592,594,617,632,634,641,643,646,660,663,671,677,
	679,686,711,718,719,724,731,739,742,745,751,756,757,760,767,770,774,778,784,791,795};

	set<int> server(num, num+sizeof(num)/sizeof(int));

	int erasenum = 17;
	int insertnum = 799;

	set<int> s = server;

	s.erase(erasenum);
	//s.insert(insertnum);

	cout << calfit(s, false, NULL) << endl;

	ZKW_MinCost zkw;

	zkw.init(netdotnum, edgenum, consumedotnum);

	for (int i = 0; i < netdotnum; i++)
	{
		for (int j = 0; j < edge[i].size(); j++)
		{
			zkw.addEdge(i, edge[i][j].end, edge[i][j].bindwidth, edge[i][j].price);
		}
	}

	zkw.save();

	for (int i = 0; i < consumedotnum; i++)
	{
		zkw.addEdge(area[i].netdot, netdotnum + 1, area[i].need, 0);
	}

	for (auto item : server)
	{
		zkw.addEdge(netdotnum, item, INF, 0);
	}

	map<vector<int>, int> route;

	pair<int, int> p = zkw.minCostMaxFlow(netdotnum, netdotnum + 1, netdotnum + 2, route, -1);

	cout << p.first << " " << p.second << endl;

	start = clock();

	for (int cnt = 0; cnt < 1; cnt++)
	{
		//ZKW_MinCost zkwtmp = zkw;
		//calfit(server, false, NULL);
		//p = zkw.minCostMaxFlow(netdotnum, netdotnum + 1, netdotnum + 2, route, -1);
		//cout << server.size() << endl;

		for (int i = 1; i > 0; i--)
		{
			//ZKW_MinCost zkwtmp = zkw;
			//p = zkw.eraseone(server, erasenum, edgenum, netdotnum);
			//calfit(server, false, NULL);
			p = zkw.eraseone(server, erasenum, edgenum, netdotnum);
			//server.erase(*server.begin());
			//cout << calfit(server, false, NULL) << endl;
		}


		cout << p.first << " " << p.second << endl;

		//cout << (server.size() - 1) * servercost + p.first << endl;
	}



	cout << "time " << (double)(clock() - start) / CLOCKS_PER_SEC << endl;
}

void AC::testspfa(char * filename)
{
	//3,7,14,36,69,103,125,129,155
	//14,	15,	19,	22,	32,	54,	62,	68,	94,	99,	182,	184,	192,	215,	223,	226,	245,	257,	265,	282,	301,	318,	320,	335,	339,	351,	370,	381,	383,	397,	413,	425,	445,	481,	511,	541,	543,	579,	610,	620,	626,	639,	641,	673,	679,	691,	702,	745,	768,	796,


	int num[] = {0,	3,	21,	29,	47,	49,	50,	70,	71,	88,	135,	141,	178,	179,	187,	205,	214,	223,	224,	236,	254,	255,	269,	271,	282,	290,	301,	321,	335,	344,	345,	351,	352,	365,	384,	387,	392,	393,	395,	401,	413,	423,	431,	476,	478,	511,	523,	539,	545,	546,	578,	582,	590};
	set<int> xx(num, num+sizeof(num)/sizeof(int));
	//for (auto i : xx)
	//{
		//int num[] = {i};
		//set<int> dna(num, num+sizeof(num)/sizeof(int));
		//cout << dna.size() << endl;
		//set<int> dna = randna(100);
		/*for (int i = 0; i < netdotnum; i++)
		{
			sort(edge[i].begin(), edge[i].end(), cmp);
		}*/
		//clock_t start = clock();
		//cout << "server " << i << endl;
	for (auto item : xx)
	{
		if (net2consume[item] != -1)
		ser++;
	}
	level = 5;
		cout << calfit(xx, true, filename) << endl;
		//cout << i << " ";
		//cout << (double)(clock() - start) / CLOCKS_PER_SEC << endl;
	//}
	cout << ser << endl;
	return ;
}

int AC::newapprofit(set<int> & dna)
{
	ISAP isap;

	for (int i = 0; i < netdotnum; i++)
	{
		for (int j = 0; j < edge[i].size(); j++)
		{
			isap.AddEdge(i, edge[i][j].end, edge[i][j].bindwidth, edge[i][j].price);
		}
	}

	for (int i = 0; i < consumedotnum; i++)
	{
		isap.AddEdge(area[i].netdot, netdotnum + 1, area[i].need, 0);
	}

	for (auto item : dna)
	{
		isap.AddEdge(netdotnum, item, serverinfo[level].outpower, 0);
	}

	CostFlow ret = isap.MaxFlow(netdotnum, netdotnum + 1, netdotnum);

	int servercost = 0;

	for (int i = 1; i <= dna.size(); i++)
	{
		int j = serverinfo.size() - 1;

		for (; j >= 0 && serverinfo[j].outpower >= -isap.Edges[isap.Edges.size() - 2 * i + 1].flow; j--);

		servercost += serverinfo[j + 1].cost;
	}

	int installcost = 0;

	for (auto item : dna)
	{
		installcost += netdotcost[item];
	}

	ret.cost += installcost + servercost;

	if (ret.flow != sumneed)
	{
		ret.cost = INF;
	}
	//printf("servercost %d installcost %d\n", servercost, installcost);
	return ret.cost;
}

int AC::approfit(set<int> & dna)
{
	MCMF mcmf;

	mcmf.init();

	for (int i = 0; i < netdotnum; i++)
	{
		for (int j = 0; j < edge[i].size(); j++)
		{
			mcmf.addedge(i, edge[i][j].end, edge[i][j].bindwidth, edge[i][j].price);
		}
	}

	for (int i = 0; i < consumedotnum; i++)
	{
		mcmf.addedge(area[i].netdot, netdotnum + 1, area[i].need, 0);
	}

	for (auto item : dna)
	{
		mcmf.addedge(netdotnum, item, INF, 0);
	}

	int installcost = 0;

	for (auto item : dna)
	{
		installcost += netdotcost[item];
	}

	return installcost + mcmf.mcmf(netdotnum, netdotnum + 1, netdotnum + 2, dna.size(), consumedotnum);
}

void AC::test(char * filename)
{
	int cnt = 100;
	while(cnt--)
	{
		//int length = myrand(1, consumedotnum - 1);
		//set<int> server = randna(length);
		//cout << "servers: " << endl;
		//showdna(server);
		//0,4,9,14,15,17,18,23
		//7,8,11,12,16,21,22,23,26
		//1,2,3,14,21
		//11,19,22,24,27
		//3,8,14,16,19,21,22,24,25
		//2,3,6,17,21,22,24
		//int num[] = {1,2,3,14,21};

					/*4,11,17,20,22,25,30,36,37,46,49,54,56,57,60,61,66,69,72,81,
		83,85,89,97,103,106,108,114,123,126,128,131,135,137,139,147,155,159*/
		//set<int> server(num, num+sizeof(num)/sizeof(int));

		/*for (int i = 0; i < sizeof(num) / sizeof(int); i++)
		{
			if (zhilian.find(num[i]) == zhilian.end())
			{
				cout << num[i] << " netdot\n";
			}
			//cout << num[i] << ":" << rawpower[num[i]].second << endl;
		}
		cout << endl;*/
		//while(!server.empty())
		//{
			set<int> server = randna(204);
			cout << calfit(server, false, NULL) << endl;
			//server.erase(*server.begin());
		//}

		//exit(0);
	}
}

vector<plan_t> AC::merg(vector<plan_t> plans, vector<plan_t> tmplans)
{
	vector<plan_t> newplans(INITNUM);

	int plansid = 0;
	int tmplansid = 0;

	for (int i = 0; i < INITNUM; i++)
	{
		if (plans[plansid].fitness <= tmplans[tmplansid].fitness)
		{
			newplans[i] = plans[plansid];
			plansid++;
		}
		else
		{
			newplans[i] = tmplans[tmplansid];
			tmplansid++;
		}

	}

	return newplans;
}

void AC::ga(char * filename)
{
	srand((unsigned int)time(NULL));

	for (int i = 0; i < consumedotnum; i++)
	{
		splan.dna.insert(area[i].netdot);
	}

	splan.fitness = newapprofit(splan.dna);

	vector<plan_t> plans(INITNUM);

	vector<plan_t> secondplans;

	plans[0] = splan;
	//plans[0].dna.insert(myrand(0, netdotnum - 1));
	//plans[0].fitness = INF;
	int old = 0;

	plan_t b = plans[0];

	map<int, plan_t> plan_m;

	plan_m[plans[0].fitness] = plans[0];

	int cnt = 0;
	int deta = 15;
	//level = 4;

	while(1)
	{
		if ((clock() - start) / CLOCKS_PER_SEC > 87)
		{
			if (plan_m.begin()->first < bestplan.fitness)
			{
				bestplan = plan_m.begin()->second;
			}

			break;
		}

		if (bestplan.fitness <= plan_m.begin()->first)
		{
			old++;
			//cout << old << endl;
		}
		else
		{
			bestplan = plan_m.begin()->second;
			//cout << "better " << bestplan.fitness << endl;
			old = 0;
		}

		/*if (plan_m.size() == INITNUM + 1)
		{
			plan_m.erase(--plan_m.end());
		}*/
		//netdotnum / 3 + 200 - bestplan.dna.size()
		//-netdotnum / 2 + 1200 - bestplan.dna.size()
		if (old >= 1400 - netdotnum)//1400 - netdotnum
		{
			plan_m.erase(plan_m.begin());
			deta += 5;
			old = 0;
			//cout << "erase " << plan_m.begin()->first << endl;
		}

		set<int> dna = plan_m.begin()->second.dna;

		for (int i = 0; i < 1; i++)
		dna.insert(myrand(0, netdotnum - 1));

		for (int i = 0; i < netdotnum / 70 + deta; i++)
		{
			dna.erase(myrand(0, netdotnum - 1));
		}

		int fit;
		if (netdotnum < 700)
			fit = calfit(dna, false, NULL);
		else
			fit = newapprofit(dna);

		plan_t p;
		p.dna = dna;
		p.fitness = fit;
		plan_m[p.fitness] = p;
		//cout << plan_m.begin()->second.dna.size() << endl;
		//cout << bestplan.fitness << endl;
		//cout << sumneed / 250 << endl;
	}
	//cout << "sumneed " << sumneed << endl;
	//cout << "best " << calfit(bestplan.dna, false, NULL) << endl;
	#if 0
	bestplan.fitness = calfit(bestplan.dna, false, NULL);

	for (map<int, plan_t>::iterator it = --plan_m.end(); it != plan_m.begin(); it--)
	//for (auto it : plan_m)
	{
		int f = calfit(it->second.dna, false, NULL);
		if (f != -1 && f < bestplan.fitness)
		{
			bestplan = it->second;

			bestplan.fitness = f;

			cout << bestplan.fitness << endl;

			break;
		}

	}
	#endif
	//showdna(bestplan.dna);
	//while(level--)
	cout << "best " << calfit(bestplan.dna, true, filename) << endl;
	cout << "servernum " << bestplan.dna.size() << endl;
	return ;
}

void AC::showplan(vector<plan_t> &plans)
{
	for (int i = 0; i < plans.size(); i++)
	{
		cout << plans[i].fitness << "-";
		for (auto item : plans[i].dna)
		{
			cout << item << " ";
		}
		cout << endl;
	}

	return ;
}

void AC::checkplan(vector<plan_t> plans)
{
	for (int i = 0; i < plans.size(); i++)
	{
		if (plans[i].fitness != calfit(plans[i].dna, false, NULL))
		{
			cout << "error" << endl;
			cout << plans[i].fitness << " != " << calfit(plans[i].dna, false, NULL) << endl;
			exit(-1);
		}
	}
	return ;
}

bool AC::compare(plan_t a, plan_t b)
{
	return a.fitness < b.fitness;
}

bool AC::compare1(pair<int, int> a, pair<int, int> b)
{
	return a.second > b.second;
}

bool AC::cmp(edge_t a, edge_t b)
{
	return a.price < b.price;
}

void AC::showdna(set<int> dna)
{
	for (auto item : dna)
	{
		cout << item << ",";
	}
	cout << endl;
	return ;
}

string AC::set2string(set<int> dna)
{
	string ret;
	for (auto item : dna)
	{
		char buf[10];
		sprintf(buf, "%d", item);
		ret += buf;
	}

	return ret;
}

set<int> AC::randna(int length)
{
	set<int> dna;
	//cout << length << endl;
	while (dna.size() != length)
	{
		int randnum = myrand(0, netdotnum - 1);

		dna.insert(randnum);
	}

	return dna;
}

void AC::mark(set<int> dna)
{

	/*if (dna.empty())
	{
		return ;
	}

	if (myhash[set2string(dna)])
	{
		return ;
	}*/

	//cout << set2string(dna) << endl;
	myhash[set2string(dna)] = true;

	/*for (auto item : dna)
	{
		set<int> tmp = dna;
		tmp.erase(item);
		mark(tmp);
	}*/

	return ;
}

bool AC::refresh(int length)
{
	if (length <= 0)
	{
		cout << "refresh usage: length > 0" << endl;
		exit(0);
	}

	int foundbetter = false;

	set<int> dna = randna(length);
	//showdna(dna);
	//cout << "------------------------------------" << endl;
	if (!myhash[set2string(dna)])
	{
		int fitness = calfit(dna, false, NULL);

		if (fitness == -1)
		{
			mark(dna);
		}
		else
		{
			magicruler[length] = min(fitness, magicruler[length]);

			if (fitness < lastbest)
			{
				lastbest = fitness;
				bestdna = dna;
				foundbetter = true;
			}
		}
	}
	else
	{
		//cout << ++zkwcnt << endl;
	}

	return foundbetter;
}

void AC::randgreedy()
{
	srand(time(NULL));
	vector<plan_t> plans(INITNUM);
	vector<set<int>> plansconsume(INITNUM);

	for (int i = 0; i < INITNUM; i++)
	{
		for(;;)
		{
			plan_t tmp;
			set<int> s;

			int consumepart = myrand(1, consumedotnum / 4);

			while(tmp.dna.size() != consumepart)
			{
				int id = area[myrand(0, consumedotnum - 1)].netdot;
				tmp.dna.insert(id);
				s.insert(id);
			}

			while(tmp.dna.size() != consumedotnum - 1)
			{
				tmp.dna.insert(myrand(0, netdotnum - 1));
			}

			if ((tmp.fitness = calfit(tmp.dna, false, NULL)) != -1)
			{
				plans[i] = tmp;
				plansconsume[i] = s;
				break;
			}
		}
		cout << plans[i].fitness << endl;
		//cout << "consume servers " << plansconsume[i].size() << endl;
	}

	for (int i = 0; i < INITNUM; i++)
	{
		int timeout = 0;
		cout << plans[i].fitness << endl;
		for (;;)
		{
			if (timeout == 10)
			{
				break;
			}

			plan_t tmp = plans[i];

			while(tmp.dna.size() == plans[i].dna.size())
			{
				int eraseid = myrand(0, netdotnum - 1);
				tmp.dna.erase(eraseid);
			}

			tmp.fitness = calfit(tmp.dna, false, NULL);

			if (tmp.fitness != -1 && tmp.fitness < plans[i].fitness)
			{
				cout << "new " << tmp.fitness << endl;
				plans[i] = tmp;
				timeout = 0;
			}
			else
			{
				//cout << timeout << " " << tmp.fitness << endl;
				timeout++;
			}
		}

		cout << plans[i].fitness << endl;
	}

	/*plan_t plan;

	for (int i = 0; i < netdotnum; i++)
	{
		plan.dna.insert(i);
	}

	for (int i = 0; i < consumedotnum; i++)
	{
		plan.dna.erase(area[i].netdot);
	}

	cout << calfit(plan.dna, false, NULL) << endl;*/
}

int AC::checkroute(map<vector<int>, int>route)
{
	int mincost = 0;
	for (auto i : route)
	{
		int price = 0;
		for (int j = 0; j < i.first.size() - 1; j++)
		{
			int start = i.first[j];
			int end = i.first[j + 1];
			for (int k = 0; k < edge[start].size(); k++)
			{
				if (edge[start][k].end == end)
				{
					price += edge[start][k].price;
				}
			}
		}
		mincost += price * i.second;
		//cout << separator << endl;
		//cout << i.second << " * " << price << endl;
	}
	return mincost;
}

void AC::showroute(map<vector<int>, int> route)
{
	for (auto item : route)
	{
		for (int i = 0; i < item.first.size(); i++)
		{
			cout << item.first[i] << " ";
		}
		cout << item.second << endl;
	}
}
