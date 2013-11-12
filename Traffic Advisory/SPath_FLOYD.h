#include "ArrayGraph.h"

template <class T>
void MGraph<T>::ShortestPath_FLOYD(PathMatrix_2 &P,DistancMatrix &D)
// 用Floyd算法求有向网G中各对顶点v和w之间的最短路径P[v][w]及其带权长度D[v][w]
// 若P[v][w][u]为TRUE，则u是从v到w当前求得最短路径上的顶点。
{ 
	int u,v,w,i;
	for(v = 0;v<mgraph.vexnum;v++)
		//各对结点之间初始已知路径及距离
	{
		for(w = 0;w<mgraph.vexnum;w++)
		{
			if((mgraph.arcs[v][w].adj)!=infinity)		//两点之间存在直接路径
			{
				D[v][w] = mgraph.arcs[v][w].adj;// 顶点v到顶点w的直接距离
			}
			else//不存在直接路径
			{
				D[v][w] = MAX_MAXNUM;		//将距离设置成一个大值
			}
			
			for(u = 0;u<mgraph.vexnum;u++)
			{
				P[v][w][u] = false;//路径矩阵初值
			}
			if(D[v][w]<infinity)
				//从v到w有直接路径
			{
				P[v][w][v] = P[v][w][w] = true;//由v到w的路径经过v和w两点
			}
		}
	}
	/*递归更新最短路径*/
	for(u = 0;u<mgraph.vexnum;u++)
	{
		for(v = 0;v<mgraph.vexnum;v++)
		{
			for(w = 0;w<mgraph.vexnum;w++)
			{
				if(D[v][u]+D[u][w]<D[v][w])		//从v经u到w的一条路径更短
				{
					D[v][w] = D[v][u]+D[u][w];	// 更新最短距离
					//另外一个矩阵p来记录两点间的最短路径
					for(i = 0;i<mgraph.vexnum;i++)
					{
						P[v][w][i] = P[v][u][i] && P[u][w][i];	//从v到w的路径经过的顶点为从v到u和从u到w的交集
					}
				}
			}
		}
	}	
}