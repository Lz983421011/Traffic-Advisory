#include "ArrayGraph.h"
#include "SPath_FLOYD.h"
using namespace std;

void opeshow()
{
	cout << "������ȫ����ͨģ����ѯϵͳ������" << endl;
	cout << "***  ��  ��  ***" << endl;   
	cout << " 1. ������ͨͼ" << endl; 
	cout << " 2. ѡ��˳�����" << endl;
	cout << " 3. ��ʾ��ͨͼ" << endl;
	cout << " 4. �˳�" << endl;
	cout << "��ѡ�������";
}

int main()
{		
	MGraph<string> g;
	MGraph<string> plane;
	PathMatrix_2 p; // 3ά����
	DistancMatrix d; // 2ά����
	int i,j,k;
	int plan;

	int transportation;		//��ͨ��ʽ
	string StarStation;		//ʼ��վ
	string EndStation;		//�յ�վ
	string train_line_num[MIDNUM];	//ʼ��վ���յ�վ�ĳ˳�����
	int opekind;	//�˵�����ѡ��
	int InputMethod;	//�������뷽ʽ
	int StarStation_num,EndStation_num;		//��ʼվ���������е�λ��
	string connector = "-";  //�����ַ�

	opeshow();
	cin >> opekind;
	while(opekind>0 && opekind<5)
	{
		if (opekind==1)
		{
			cout << "��ѡ�񴴽���ʽ��1 �ļ���ȡ��2 �ֹ�¼�룩��";
			cin >> InputMethod;
			if (InputMethod==1)
			{
				cout << "\n" << "---ȫ����ͨ·��ͼ---�𳵣�";
				g.ShowTrafficMap(InputMethod);
				cout << "\n" << "---ȫ����ͨ·��ͼ---�ɻ���";
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
				cout << "�������" << endl;
			}
		}
		else if (opekind==2)
		{
			cout << "��������1ʱ�����/2Ǯ���٣�3��/4�ɻ�����";
			cin >> plan >> transportation;
			if (transportation==3)	//��
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
			if (transportation==4)	//�ɻ�
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
			g.ShortestPath_FLOYD(p,d); //��ÿ�Զ��������·����p�����d����Ϊ�������ó�p�����d����

// 			cout<<"d����:"<<endl;
// 			for(i = 0;i<g.mgraph.vexnum;i++)
// 			{
// 				for(j = 0;j<g.mgraph.vexnum;j++)
// 				{
// 					cout<<d[i][j]<<"  ";
// 				}
// 				cout<<endl;
// 			}

			cout << "������ʼ�ص㣺";
			cin >> StarStation >> EndStation;
			StarStation_num = g.GetStationNum(StarStation);
			EndStation_num = g.GetStationNum(EndStation);

			while(StarStation_num == MAXNUM || EndStation_num == MAXNUM)	//��ֹ������
			{
				cout << "�������ʼ�ص㲻�ڽ�ͨͼ�У����������롣" << endl;
				cout << "������ʼ�ص㣺";
				cin >> StarStation >> EndStation;
				StarStation_num = g.GetStationNum(StarStation);
				EndStation_num = g.GetStationNum(EndStation);
			}

			if(StarStation_num != EndStation_num)	//����ͬһ����֮���������
			{
				/*���·���������ֲ�ͬ������*/
				//������
				if(StarStation_num<EndStation_num)
				{
					i=0;
					cout<<"��"<<g.mgraph.vexs[StarStation_num]<<"��"<<g.mgraph.vexs[EndStation_num]<<"��";
					cout << StarStation;
					train_line_num[i] = StarStation;

					if (p[StarStation_num][EndStation_num][StarStation_num] == false)	//û��ֱ��·��
					{
						for(k=0;k<g.mgraph.vexnum;k++)
						{
							if(p[StarStation_num][EndStation_num][k] == 1)		//�������·��
							{	
								i++;	//��ת������
								cout << connector << g.mgraph.vexs[k];		//��·
								train_line_num[i] = g.mgraph.vexs[k];
							}
						}
					}

					cout << connector << EndStation;
					train_line_num[i+1] = EndStation;
					cout << "    " << "������" << g.GetTrain_num_array(train_line_num,i);		//�˳�����
					cout << endl;
				}
				//������
				if(StarStation_num>EndStation_num)
				{
					i=0;
					cout<<"��"<<g.mgraph.vexs[StarStation_num]<<"��"<<g.mgraph.vexs[EndStation_num]<<"��";
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
					cout << "    " << "������" << g.GetTrain_num_array(train_line_num,i);		//�˳�����
					cout << endl;
				}
			}
			else
			{
				cout << "��ʼվΪ��ͬ�ص㣡����ʹ�ý�ͨ���ߣ�" << endl;
			}
		}
		else if(opekind==3)
		{
			cout << "\n" << "---ȫ����ͨ·��ͼ---�𳵣�";
			g.ShowTrafficMap(InputMethod);
			cout << "\n" << "---ȫ����ͨ·��ͼ---�ɻ���";
			plane.ShowTrafficMap(InputMethod+2);
		}
		else
		{
			cout<<"�������н���!"<<endl;
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