#include "ArrayGraph.h"
#include "SPath_FLOYD.h"
using namespace std;

void opeshow()
{
	cout << "━━━全国交通模拟咨询系统━━━" << endl;
	cout << "***  菜  单  ***" << endl;   
	cout << " 1. 创建交通图" << endl; 
	cout << " 2. 选择乘车方案" << endl;
	cout << " 3. 显示交通图" << endl;
	cout << " 4. 退出" << endl;
	cout << "请选择操作：";
}

int main()
{		
	MGraph<string> g;
	MGraph<string> plane;
	PathMatrix_2 p; // 3维数组
	DistancMatrix d; // 2维数组
	int i,j,k;
	int plan;

	int transportation;		//交通方式
	string StarStation;		//始发站
	string EndStation;		//终点站
	string train_line_num[MIDNUM];	//始发站到终点站的乘车车次
	int opekind;	//菜单操作选项
	int InputMethod;	//数据输入方式
	int StarStation_num,EndStation_num;		//起、始站点在数组中的位置
	string connector = "-";  //连接字符

	opeshow();
	cin >> opekind;
	while(opekind>0 && opekind<5)
	{
		if (opekind==1)
		{
			cout << "请选择创建方式（1 文件读取，2 手工录入）：";
			cin >> InputMethod;
			if (InputMethod==1)
			{
				cout << "\n" << "---全国交通路线图---火车：";
				g.ShowTrafficMap(InputMethod);
				cout << "\n" << "---全国交通路线图---飞机：";
				plane.ShowTrafficMap(InputMethod+2);
			}
			else if (InputMethod==2)
			{
				g.CreateTrafficMap(InputMethod);
				cout << endl;
				plane.CreateTrafficMap(InputMethod+2);
			}
			else
			{
				cout << "输入错误！" << endl;
			}
		}
		else if (opekind==2)
		{
			cout << "输入需求（1时间最短/2钱最少，3火车/4飞机）：";
			cin >> plan >> transportation;
			if (transportation==3)	//火车
			{
				if (plan==1)
				{
					g.CreateGraph(InputMethod,plan);
				} 
				else
				{
					g.CreateGraph(InputMethod,plan);
				}
			}
			if (transportation==4)	//飞机
			{
				if (plan==1)
				{
					g.CreateGraph(InputMethod+2,plan);
				} 
				else
				{
					g.CreateGraph(InputMethod+2,plan);
				}				
			}
/*			g.DisPlay();*/
			g.ShortestPath_FLOYD(p,d); //求每对顶点间的最短路径，p矩阵和d矩阵为参数，得出p矩阵和d矩阵

// 			cout<<"d矩阵:"<<endl;
// 			for(i = 0;i<g.mgraph.vexnum;i++)
// 			{
// 				for(j = 0;j<g.mgraph.vexnum;j++)
// 				{
// 					cout<<d[i][j]<<"  ";
// 				}
// 				cout<<endl;
// 			}

			cout << "输入起、始地点：";
			cin >> StarStation >> EndStation;
			StarStation_num = g.GetStationNum(StarStation);
			EndStation_num = g.GetStationNum(EndStation);

			while(StarStation_num == MAXNUM || EndStation_num == MAXNUM)	//防止误输入
			{
				cout << "输入的起始地点不在交通图中，请重新输入。" << endl;
				cout << "输入起、始地点：";
				cin >> StarStation >> EndStation;
				StarStation_num = g.GetStationNum(StarStation);
				EndStation_num = g.GetStationNum(EndStation);
			}

			if(StarStation_num != EndStation_num)	//避免同一顶点之间距离的输出
			{
				/*输出路径，分两种不同情况输出*/
				//上三角
				if(StarStation_num<EndStation_num)
				{
					i=0;
					cout<<"由"<<g.mgraph.vexs[StarStation_num]<<"到"<<g.mgraph.vexs[EndStation_num]<<"：";
					cout << StarStation;
					train_line_num[i] = StarStation;

					if (p[StarStation_num][EndStation_num][StarStation_num] == false)	//没有直接路径
					{
						for(k=0;k<g.mgraph.vexnum;k++)
						{
							if(p[StarStation_num][EndStation_num][k] == 1)		//存在最短路径
							{	
								i++;	//中转车次数
								cout << connector << g.mgraph.vexs[k];		//线路
								train_line_num[i] = g.mgraph.vexs[k];
							}
						}
					}

					cout << connector << EndStation;
					train_line_num[i+1] = EndStation;
					cout << "    " << "乘坐：" << g.GetTrain_num_array(train_line_num,i);		//乘车方案
					cout << endl;
				}
				//下三角
				if(StarStation_num>EndStation_num)
				{
					i=0;
					cout<<"由"<<g.mgraph.vexs[StarStation_num]<<"到"<<g.mgraph.vexs[EndStation_num]<<"：";
					cout << StarStation;
					train_line_num[i] = StarStation;

					if (p[StarStation_num][EndStation_num][StarStation_num] == false)
					{
						for(k=g.mgraph.vexnum-1;k>=0;k--)
						{
							if(p[StarStation_num][EndStation_num][k] == 1)
							{
								i++;
								cout << connector << g.mgraph.vexs[k];
								train_line_num[i] = g.mgraph.vexs[k];					
							}
						}
					}

					cout << connector << EndStation;
					train_line_num[i+1] = EndStation;
					cout << "    " << "乘坐：" << g.GetTrain_num_array(train_line_num,i);		//乘车方案
					cout << endl;
				}
			}
			else
			{
				cout << "起始站为相同地点！无需使用交通工具！" << endl;
			}
		}
		else if(opekind==3)
		{
			cout << "\n" << "---全国交通路线图---火车：";
			g.ShowTrafficMap(InputMethod);
			cout << "\n" << "---全国交通路线图---飞机：";
			plane.ShowTrafficMap(InputMethod+2);
		}
		else
		{
			cout<<"程序运行结束!"<<endl;
			g.DestroyGraph();
			plane.DestroyGraph();
			break;
		}//if
		cout << endl;
		opeshow();
		cin >> opekind;
	}//while
	return 0;
}//main