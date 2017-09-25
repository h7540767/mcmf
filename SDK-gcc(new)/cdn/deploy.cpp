#include "deploy.h"
#include <stdio.h>
#include "ac.h"

void deploy_server(char * topo[MAX_EDGE_NUM], int line_num,char * filename)
{
	
	AC ac;	
	
	ac.loadinfor(topo);

	//ac.showinfor();

	for (int i = 0; i < 1; i++)
	{
		//ac.test(filename);
		//ac.testzkw();
		//ac.ga(filename);
		//ac.randgreedy();
		//ac.newway(filename);
		//ac.ga(filename);
		ac.ga(filename);
		//ac.level = 4;
		//ac.testspfa(filename);
	}

	//ac.writeret(ac.bestdna, filename);

	return ;

}
