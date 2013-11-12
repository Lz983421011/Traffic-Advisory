/* �����ڽӾ��� */
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
using namespace std;

#ifndef MGRAPH		//if not define�ļ�д,��ֹͷ�ļ����ظ������ͱ��� ����Ѿ�������������������

//���MGRAPHû�б��궨���������MGRAPH
#define MGRAPH

#define MAXNUM 99
#define MAX_MAXNUM 999
#define MIDNUM 50
#define MAX_VERTEX_NUM 20 //��󶥵���
  
const int infinity = INT_MAX;

/*ȫ�ֱ���-�ļ���ȡ������*/
int train_time[MIDNUM][MAXNUM];		//�г�ʱ��
int train_pay[MIDNUM][MAXNUM];		//�г��۸�
string train_num[MIDNUM];		//�г�����������K789����Ӧ���ζ�Ϊ������������Ϊ_num
string train_line[MIDNUM][MAXNUM];	//�г���·��վ�㣩

struct ArcCell{
	int adj;    //��1��0��ʾ�Ƿ����ڣ���������¼Ȩֵ
    char *info; //�û��������Ϣ��ָ��
};

template <class T>	//ģ�壬T�����Ͳ���
struct _MGraph{
    T vexs[MAX_VERTEX_NUM];		//����
	ArcCell arcs[MAX_VERTEX_NUM][MAX_VERTEX_NUM];	//�ṹ�����飬����
	int vexnum;		//����ĸ���
	int arcnum;		//�ߵĸ���
};

/*typedef��Ϊ���ʹ���һ���µ����֣����ͱ�����*/
typedef int PathMatrix_2[MAX_VERTEX_NUM][MAX_VERTEX_NUM][MAX_VERTEX_NUM]; // 3ά���飬PathMatrix_2��ʾ3ά����
typedef int DistancMatrix[MAX_VERTEX_NUM][MAX_VERTEX_NUM]; // 2ά����

template <class T>
class MGraph{
public:		
	_MGraph<T> mgraph;
	void CreateGraph(int InputMethod,int plan);            //���캯��
	void DestroyGraph();                 //������������ͼ
	int LocateVex (T u);                 //ͼ���ڣ�ͼ�д��ڶ���u �򷵻ظö�����ͼ�е�λ��
	bool CreateUDN(int InputMethod,int plan);                    //����������
	void DisPlay();                      //����ڽӾ���
	void ShortestPath_FLOYD(PathMatrix_2 &P,DistancMatrix &D);

	void getdata_from_file(int FileType);
	bool ArraySameItem(string aArray[],int size,string num);
	int GetStationNum(string station);	//���վ���������е�λ��
	string GetTrain_num(string stationA,string stationB);		//��������վ����г����κ�
	string GetTrain_num_array(string station[],int size);		//վ��������ת��Ϊ�г����κ�
	void ShowTrafficMap(int FileType);		//��ʾ��ͨͼ
	void CreateTrafficMap(int FileType);	//������ͨͼ
};

template <class T>
void MGraph<T>::CreateGraph(int InputMethod,int plan)//ok
{
	CreateUDN(InputMethod,plan);	//����ʵ��������һ������ͼ
}

template <class T>
void MGraph<T>::DestroyGraph()//ok
{
	for(int i = 0 ;i<mgraph.vexnum;i++)
		for(int j = i+1;j<mgraph.vexnum;j++)
		{
			if(mgraph.arcs[i][j].info)
			{
				delete []mgraph.arcs[i][j].info;	//����ָ��
				mgraph.arcs[i][j].info = mgraph.arcs[j][i].info = false;
			}
		}
	mgraph.vexnum = 0;	//�������
	mgraph.arcnum = 0;	//�ߵ�����
} 

template <class T>
bool MGraph<T>::CreateUDN(int InputMethod,int plan)//ok
//����������
{
	int i,j,w;
	int train_station_num = 0;		//վ��ĸ�����ͼ�Ķ��������
	string train_station[MIDNUM];	//վ������
	int train_side_num = 0;		//�ߵ�����
	T v1, v2;

	getdata_from_file(InputMethod);	//���ļ��ж�ȡ����
	
	for (i=1;i<=train_time[0][0];i++)	//train_time[0][0],�г��Ĵ���
	{
		for (j=1;j<=train_time[i][0];j++)	//train_time[i][0],i���г���վ����
		{
			if (train_station_num==0)		//��ʼ������һ�Σ�
			{
				//train_stationվ�������±�1��ʼ�洢
				train_station[train_station_num+1] = train_line[i][j];
				train_station_num++;					
			}
			//���վ���������Ѵ���վ���򲻴���
			if(!ArraySameItem(train_station,train_station_num,train_line[i][j]))
			{
				train_station[train_station_num+1] = train_line[i][j];
				train_station_num++;		//�������
			}
		}
		train_side_num += train_time[i][0]-1;	//�˴γ��αߵĸ������ڶ���ĸ���-1
	}
	mgraph.arcnum = train_side_num;	//�ߣ�����������
	mgraph.vexnum = train_station_num;	//����ĸ���

	for(i = 0;i<mgraph.vexnum;i++)
	//���춥������
	{
		mgraph.vexs[i]=train_station[i+1];	//i+1��
	}
	for(i = 0;i<mgraph.vexnum;i++)	
	//��ʼ���ڽӾ���
	{
		for(j = 0;j<mgraph.vexnum;j++)
		{
			if(i==j)
				mgraph.arcs[i][j].adj = 0;	//�������ͬ�ĵ㣬��ʾΪ��ͨ
			else
				mgraph.arcs[i][j].adj = infinity;	//��ʼ���������Ϊһ������ֵ
				mgraph.arcs[i][j].info = false;
		}
	}
	for (j=1;j<=train_time[0][0];j++)
	{
		for (int k=1;k<train_time[j][0];k++)
		{
			/*�������㼰������Ȩֵ*/
			v1 = train_line[j][k];
			v2 = train_line[j][k+1];

			if (plan==1)	//ʱ�����ŷ���
			{
				w = train_time[j][k+1]-train_time[j][k];	//վ������ݵĲ�ֵΪȨֵ
			}
			if (plan==2)	//�۸����ŷ���
			{
				w = train_pay[j][k+1]-train_pay[j][k];	//վ������ݵĲ�ֵΪȨֵ
			}
			
			int m = LocateVex(v1);	//ȷ�������ڶ��������е�λ��
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
void MGraph<T>::DisPlay()		//����ڽӾ���
{
	int i,j;
	char graphkind[7];
	char arckind[3];

	strcpy(graphkind,"������");
	strcpy(arckind,"��");

	cout<<mgraph.vexnum<<"������"<<mgraph.arcnum<<"��"<<arckind<<"��"<<graphkind<<endl;
	//�������
	cout<<"����Ϊ";
	for(i = 0;i<mgraph.vexnum;i++)
		cout<<mgraph.vexs[i]<<" ";
	//���Ȩֵ���ڽӾ���
	cout<<"Ȩֵ���ڽӾ���Ϊ"<<endl;
	for(i = 0;i<mgraph.vexnum;i++)
	{
		for(j = 0;j<mgraph.vexnum;j++)
			if(mgraph.arcs[i][j].adj==infinity)
				cout<<"��"<<'\t';
			else
				cout<<mgraph.arcs[i][j].adj<<'\t';
		cout<<endl;
	}//for
}

template <class T>
void MGraph<T>::getdata_from_file(int FileType)		//�ļ���ȡ����
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
	
	//�жϴ��ļ��Ƿ�ɹ�
	if (in_stream.fail())
	{
		cout << "�ļ���ʧ�ܣ�" << endl;
		exit(1);
	}
	else
	{
		cout << "";
	}

	for(i=0;!in_stream.eof();)
	{
		in_stream >> cut_off;
		//Ԥ��ȡһ�Σ������ȡ���Ƿָ��� - �����г���·�Ѷ���
		if (cut_off=="-")
		{
			/*�洢����ͳһ���±�1��ʼ��train_time�����0��λ����������*/
			i++;
			train_time[i][0] = 0;		//��ʼ��
			j=1;
			in_stream >> train_num[i];	//�г��ĳ���
		}
		else{
			train_line[i][j] = cut_off;		
			in_stream >> train_time[i][j] >> train_pay[i][j];
			train_time[i][0]++;		//��i�˳���վ����
			j++;
		}
	}
	train_time[0][0] = i;	//ͳ���ж��ٳ���
	in_stream.close();
	return;
}

template <class T>
bool MGraph<T>::ArraySameItem(string aArray[],int size,string num)		//�ж��������Ƿ�������ֵ
{
	//����
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
int MGraph<T>::GetStationNum(string station)	//վ���������е�λ��
{
	for(int i=0;i<mgraph.vexnum;i++)		//����ͼ�Ķ���
	{
		if (mgraph.vexs[i]==station)
		{
			return i;
		}	
	}
	return MAXNUM;		//δ�ҵ�վ�㷵��һ������
}

template <class T>
string MGraph<T>::GetTrain_num(string stationA,string stationB)		//��������վ����г����κ�
{
	for (int i=1;i<=train_time[0][0];i++)
	{
		for (int j=1;j<train_time[i][0];j++)
		{
			//A-B����B-A����·�е�����վ����˵�����Գ�������·
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
string MGraph<T>::GetTrain_num_array(string station[],int size)		//վ��������ת��Ϊ�г����κţ��˳�������
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
			//���Ŀǰ����վ�����·����һ����·һ����˵������һ����·��
			if (train_num_array[j-1] != GetTrain_num(station[i],station[i+1]))
			{
				train_num_array[j] = GetTrain_num(station[i],station[i+1]);
				j++;
			}
		}		
	}
	for (k=0;k<j;k++)
	{
		//�����ַ�
		if (k==j-1)
		{
			connector = "";
		}
		else
		{
			connector = "ת";
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
	cout << "\n" << "����" << "		   ;��վ��/ʱ��/�۸�" << endl;
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
	string str;		//�г���Ϣ
	string ch1;
	string ch2;
	ofstream fout;

	if (FileType==2)
	{
		cout << "��ӻ𳵳�����Ϣ" << endl;
		fout.open("D:\\yz_train_input.txt", ios::app);	//�򿪵��ļ�β
		if (fout.fail())
		{
			cout << "�ļ���ʧ�ܣ�" << endl;
			exit(1);
		}
		do 
		{
			cout << "����һ���𳵵ĳ��κţ�";
			cin >> str;
			fout << "-\n" << str << "\n";
			do 
			{
				cout << "�����·��Ϣ��վ�� ʱ�� �۸񣩣�";
				for (i=0;i<3;i++)
				{
					cin >> str;
					fout << str;
					if (i!=2)	//��������һ���ո�
					{
						fout << " ";	
					}
				}
				cout << "���������·��Ϣ����y/n����";
				cin >> ch2;
				if (ch2=="y"||ch2=="Y")
				{
					fout << "\n";
				}
			} while (ch2=="y"||ch2=="Y");
			cout << "������ӳ��Σ���y/n����";
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
		cout << "��ӷɻ�������Ϣ" << endl;
		fout.open("D:\\yz_plane_input.txt", ios::app);	//�򿪵��ļ�β
		if (fout.fail())
		{
			cout << "�ļ���ʧ�ܣ�" << endl;
			exit(1);
		}
		do 
		{
			cout << "����һ���ɻ��ĺ���ţ�";
			cin >> str;
			fout << "-\n" << str << "\n";
			do 
			{
				cout << "��Ӻ�����·��Ϣ��վ�� ʱ�� �۸񣩣�";
				for (i=0;i<3;i++)
				{
					cin >> str;
					fout << str;
					if (i!=2)	//��������һ���ո�
					{
						fout << " ";	
					}
				}
				cout << "���������·��Ϣ����y/n����";
				cin >> ch2;
				if (ch2=="y"||ch2=="Y")
				{
					fout << "\n";
				}
			} while (ch2=="y"||ch2=="Y");
			cout << "������Ӻ���ţ���y/n����";
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