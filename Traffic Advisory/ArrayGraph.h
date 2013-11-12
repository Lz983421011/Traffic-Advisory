/* 构造邻接矩阵 */
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
using namespace std;

#ifndef MGRAPH		//if not define的简写,防止头文件的重复包含和编译 如果已经编译则跳过下面代码段

//如果MGRAPH没有被宏定义过，定义MGRAPH
#define MGRAPH

#define MAXNUM 99
#define MAX_MAXNUM 999
#define MIDNUM 50
#define MAX_VERTEX_NUM 20 //最大顶点数
  
const int infinity = INT_MAX;

/*全局变量-文件读取的数据*/
int train_time[MIDNUM][MAXNUM];		//列车时间
int train_pay[MIDNUM][MAXNUM];		//列车价格
string train_num[MIDNUM];		//列车车次名（如K789），应车次多为数字所以命名为_num
string train_line[MIDNUM][MAXNUM];	//列车线路（站点）

struct ArcCell{
	int adj;    //用1，0表示是否相邻，后用来记录权值
    char *info; //该弧的相关信息的指针
};

template <class T>	//模板，T：类型参数
struct _MGraph{
    T vexs[MAX_VERTEX_NUM];		//顶点
	ArcCell arcs[MAX_VERTEX_NUM][MAX_VERTEX_NUM];	//结构体数组，两点
	int vexnum;		//顶点的个数
	int arcnum;		//边的个数
};

/*typedef：为类型创建一个新的名字（类型别名）*/
typedef int PathMatrix_2[MAX_VERTEX_NUM][MAX_VERTEX_NUM][MAX_VERTEX_NUM]; // 3维数组，PathMatrix_2表示3维数组
typedef int DistancMatrix[MAX_VERTEX_NUM][MAX_VERTEX_NUM]; // 2维数组

template <class T>
class MGraph{
public:		
	_MGraph<T> mgraph;
	void CreateGraph(int InputMethod,int plan);            //构造函数
	void DestroyGraph();                 //析构函数销毁图
	int LocateVex (T u);                 //图存在，图中存在顶点u 则返回该顶点在图中的位置
	bool CreateUDN(int InputMethod,int plan);                    //构造无向网
	void DisPlay();                      //输出邻接矩阵
	void ShortestPath_FLOYD(PathMatrix_2 &P,DistancMatrix &D);

	void getdata_from_file(int FileType);
	bool ArraySameItem(string aArray[],int size,string num);
	int GetStationNum(string station);	//获得站点在数组中的位置
	string GetTrain_num(string stationA,string stationB);		//经过两个站点的列车车次号
	string GetTrain_num_array(string station[],int size);		//站点名数组转换为列车车次号
	void ShowTrafficMap(int FileType);		//显示交通图
	void CreateTrafficMap(int FileType);	//创建交通图
};

template <class T>
void MGraph<T>::CreateGraph(int InputMethod,int plan)//ok
{
	CreateUDN(InputMethod,plan);	//本次实例，创建一个无向图
}

template <class T>
void MGraph<T>::DestroyGraph()//ok
{
	for(int i = 0 ;i<mgraph.vexnum;i++)
		for(int j = i+1;j<mgraph.vexnum;j++)
		{
			if(mgraph.arcs[i][j].info)
			{
				delete []mgraph.arcs[i][j].info;	//销毁指针
				mgraph.arcs[i][j].info = mgraph.arcs[j][i].info = false;
			}
		}
	mgraph.vexnum = 0;	//顶点个数
	mgraph.arcnum = 0;	//边的条数
} 

template <class T>
bool MGraph<T>::CreateUDN(int InputMethod,int plan)//ok
//构造无向网
{
	int i,j,w;
	int train_station_num = 0;		//站点的个数（图的顶点个数）
	string train_station[MIDNUM];	//站点数组
	int train_side_num = 0;		//边的条数
	T v1, v2;

	getdata_from_file(InputMethod);	//从文件中读取数据
	
	for (i=1;i<=train_time[0][0];i++)	//train_time[0][0],列车的次数
	{
		for (j=1;j<=train_time[i][0];j++)	//train_time[i][0],i次列车的站点数
		{
			if (train_station_num==0)		//初始化（第一次）
			{
				//train_station站点数组下标1开始存储
				train_station[train_station_num+1] = train_line[i][j];
				train_station_num++;					
			}
			//如果站点数组中已存在站点则不存入
			if(!ArraySameItem(train_station,train_station_num,train_line[i][j]))
			{
				train_station[train_station_num+1] = train_line[i][j];
				train_station_num++;		//顶点个数
			}
		}
		train_side_num += train_time[i][0]-1;	//此次车次边的个数等于顶点的个数-1
	}
	mgraph.arcnum = train_side_num;	//边（弧）的条数
	mgraph.vexnum = train_station_num;	//顶点的个数

	for(i = 0;i<mgraph.vexnum;i++)
	//构造顶点向量
	{
		mgraph.vexs[i]=train_station[i+1];	//i+1！
	}
	for(i = 0;i<mgraph.vexnum;i++)	
	//初始化邻接矩阵
	{
		for(j = 0;j<mgraph.vexnum;j++)
		{
			if(i==j)
				mgraph.arcs[i][j].adj = 0;	//如果是相同的点，表示为不通
			else
				mgraph.arcs[i][j].adj = infinity;	//初始化两点距离为一个极大值
				mgraph.arcs[i][j].info = false;
		}
	}
	for (j=1;j<=train_time[0][0];j++)
	{
		for (int k=1;k<train_time[j][0];k++)
		{
			/*两个顶点及顶点间的权值*/
			v1 = train_line[j][k];
			v2 = train_line[j][k+1];

			if (plan==1)	//时间最优方案
			{
				w = train_time[j][k+1]-train_time[j][k];	//站点间数据的差值为权值
			}
			if (plan==2)	//价格最优方案
			{
				w = train_pay[j][k+1]-train_pay[j][k];	//站点间数据的差值为权值
			}
			
			int m = LocateVex(v1);	//确定顶点在顶点数组中的位置
			int n = LocateVex(v2);
			mgraph.arcs[m][n].adj = w;// <v1, v2>
			mgraph.arcs[n][m].adj = w;// <v2, v1>
		}
	}
	return true;
}

template <class T>
int MGraph<T>::LocateVex(T u)//ok
{
	for(int i = 0 ;i<MAX_VERTEX_NUM;i++)
	{
		if(u == mgraph.vexs[i])
		{
			return i;
		}
	}
	return -1;
}  

template <class T>
void MGraph<T>::DisPlay()		//输出邻接矩阵
{
	int i,j;
	char graphkind[7];
	char arckind[3];

	strcpy(graphkind,"无向网");
	strcpy(arckind,"边");

	cout<<mgraph.vexnum<<"个顶点"<<mgraph.arcnum<<"条"<<arckind<<"的"<<graphkind<<endl;
	//输出顶点
	cout<<"顶点为";
	for(i = 0;i<mgraph.vexnum;i++)
		cout<<mgraph.vexs[i]<<" ";
	//输出权值的邻接矩阵
	cout<<"权值的邻接矩阵为"<<endl;
	for(i = 0;i<mgraph.vexnum;i++)
	{
		for(j = 0;j<mgraph.vexnum;j++)
			if(mgraph.arcs[i][j].adj==infinity)
				cout<<"∞"<<'\t';
			else
				cout<<mgraph.arcs[i][j].adj<<'\t';
		cout<<endl;
	}//for
}

template <class T>
void MGraph<T>::getdata_from_file(int FileType)		//文件读取函数
{
	ifstream in_stream;

	string cut_off;
	int i,j;
	if (FileType==1)
	{
		in_stream.open("D:\\yz_train.txt");
	}
	else if(FileType==2)
	{
		in_stream.open("D:\\yz_train_input.txt");
	}
	else if(FileType==3)
	{
		in_stream.open("D:\\yz_plane.txt");
	}
	else
	{
		in_stream.open("D:\\yz_plane_input.txt");
	}
	
	//判断打开文件是否成功
	if (in_stream.fail())
	{
		cout << "文件打开失败！" << endl;
		exit(1);
	}
	else
	{
		cout << "";
	}

	for(i=0;!in_stream.eof();)
	{
		in_stream >> cut_off;
		//预读取一次，如果读取的是分隔符 - 则本趟列车线路已读完
		if (cut_off=="-")
		{
			/*存储数据统一从下标1开始，train_time数组的0号位置用作它用*/
			i++;
			train_time[i][0] = 0;		//初始化
			j=1;
			in_stream >> train_num[i];	//列车的车次
		}
		else{
			train_line[i][j] = cut_off;		
			in_stream >> train_time[i][j] >> train_pay[i][j];
			train_time[i][0]++;		//第i趟车的站点数
			j++;
		}
	}
	train_time[0][0] = i;	//统计有多少车次
	in_stream.close();
	return;
}

template <class T>
bool MGraph<T>::ArraySameItem(string aArray[],int size,string num)		//判断数组中是否存在这个值
{
	//遍历
	for (int i=1;i<=size;i++)
	{
		if (aArray[i]==num)
		{
			return true;
		}
	}
	return false;
}

template <class T>
int MGraph<T>::GetStationNum(string station)	//站点在数组中的位置
{
	for(int i=0;i<mgraph.vexnum;i++)		//遍历图的顶点
	{
		if (mgraph.vexs[i]==station)
		{
			return i;
		}	
	}
	return MAXNUM;		//未找到站点返回一个大数
}

template <class T>
string MGraph<T>::GetTrain_num(string stationA,string stationB)		//经过两个站点的列车车次号
{
	for (int i=1;i<=train_time[0][0];i++)
	{
		for (int j=1;j<train_time[i][0];j++)
		{
			//A-B或者B-A是线路中的两个站点则说明可以乘坐此线路
			if (train_line[i][j]==stationA&&train_line[i][j+1]==stationB)
			{
				return train_num[i];
			}
			if (train_line[i][j]==stationB&&train_line[i][j+1]==stationA)
			{
				return train_num[i];
			}
		}
	}
}

template <class T>
string MGraph<T>::GetTrain_num_array(string station[],int size)		//站点名数组转换为列车车次号（乘车方案）
{
	int i,j,k;
	string connector;
	string train_num = "";
	string train_num_array[MIDNUM];
	size++;
	for (i=0,j=0;i<size;i++)
	{
		if(i==0)
		{
			train_num_array[j] = GetTrain_num(station[i],station[i+1]);
			j++;
		}
		else
		{
			//如果目前两个站点的线路和上一条线路一样则说明是在一条线路上
			if (train_num_array[j-1] != GetTrain_num(station[i],station[i+1]))
			{
				train_num_array[j] = GetTrain_num(station[i],station[i+1]);
				j++;
			}
		}		
	}
	for (k=0;k<j;k++)
	{
		//连接字符
		if (k==j-1)
		{
			connector = "";
		}
		else
		{
			connector = "转";
		}
		train_num += train_num_array[k];
		train_num += connector;
	}
	return train_num;
}

template <class T>
void MGraph<T>::ShowTrafficMap(int FileType)
{
	int i,j;
	getdata_from_file(FileType);
	cout << "\n" << "车次" << "		   途经站点/时间/价格" << endl;
	for (i=1;i<=train_time[0][0];i++)
	{
		cout << train_num[i] << "	";
		for (j=1;j<=train_time[i][0];j++)
		{
			cout << "   " << train_line[i][j] << "/" << train_time[i][j] << ":00/" << train_pay[i][j];
		}
		cout << endl;
	}
}

template <class T>
void MGraph<T>::CreateTrafficMap(int FileType)
{
	int i;
	string str;		//列车信息
	string ch1;
	string ch2;
	ofstream fout;

	if (FileType==2)
	{
		cout << "添加火车车次信息" << endl;
		fout.open("D:\\yz_train_input.txt", ios::app);	//打开到文件尾
		if (fout.fail())
		{
			cout << "文件打开失败！" << endl;
			exit(1);
		}
		do 
		{
			cout << "输入一辆火车的车次号：";
			cin >> str;
			fout << "-\n" << str << "\n";
			do 
			{
				cout << "添加线路信息（站点 时间 价格）：";
				for (i=0;i<3;i++)
				{
					cin >> str;
					fout << str;
					if (i!=2)	//非最后添加一个空格
					{
						fout << " ";	
					}
				}
				cout << "继续添加线路信息？（y/n）：";
				cin >> ch2;
				if (ch2=="y"||ch2=="Y")
				{
					fout << "\n";
				}
			} while (ch2=="y"||ch2=="Y");
			cout << "继续添加车次？（y/n）：";
			cin >> ch1;
			if (ch1=="y"||ch1=="Y")
			{
				fout << "\n";
			}
		} while (ch1=="y"||ch1=="Y");
		fout.close();
	}
	if (FileType==4)
	{
		cout << "添加飞机航班信息" << endl;
		fout.open("D:\\yz_plane_input.txt", ios::app);	//打开到文件尾
		if (fout.fail())
		{
			cout << "文件打开失败！" << endl;
			exit(1);
		}
		do 
		{
			cout << "输入一辆飞机的航班号：";
			cin >> str;
			fout << "-\n" << str << "\n";
			do 
			{
				cout << "添加航班线路信息（站点 时间 价格）：";
				for (i=0;i<3;i++)
				{
					cin >> str;
					fout << str;
					if (i!=2)	//非最后添加一个空格
					{
						fout << " ";	
					}
				}
				cout << "继续添加线路信息？（y/n）：";
				cin >> ch2;
				if (ch2=="y"||ch2=="Y")
				{
					fout << "\n";
				}
			} while (ch2=="y"||ch2=="Y");
			cout << "继续添加航班号？（y/n）：";
			cin >> ch1;
			if (ch1=="y"||ch1=="Y")
			{
				fout << "\n";
			}
		} while (ch1=="y"||ch1=="Y");
		fout.close();
	}
}

#endif