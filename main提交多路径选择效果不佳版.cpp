#include<iostream>
#include<vector>
#include<map>
#include<queue>
#include<algorithm>
#include<cstring>
#include<stack>
#include<cmath>
using namespace std;
/***************************�Ͻ�˹�����㷨�������**************************************/
const int maxn = 30100;		//���ǵ��ߵ�˫����
const int inf = 0x3f3f3f3f;
typedef pair<int, int> P;
int idx, h[maxn], e[maxn], ne[maxn];
int w[maxn];                    //�����洢ÿ���ߵ�Ȩֵ
int dist[maxn];
int pre[maxn];
int n, m;
/***************************�Ͻ�˹�����㷨�������**************************************/
/*�������������������֮�������·����Ŀ*/
vector<vector<int>>TwoPointResult;
int v[5010] = { 0 };//������Ŀ
/***************************��ʼ������**************************************/
typedef struct Edge {//�洢�ߵ���Ϣ
	int BeginPoint;
	int EndPoint;
	int Value;
	int id;   //�ߵ�ID��
	bool flag;
}edge;

typedef struct AddEdge {
	int FirstPoint;//��ʼ��
	int SecondPoint;//Ŀ���
	int value;		//���ֻ���ڱ�ʾ·��ʱ����ʹ��
	int id;
	vector<bool>flag;		//��ʾtrue��ʾ����false��ʾ����
	AddEdge();
	AddEdge(int _FirstPoint, int _SecondPoint, int _value, vector<bool> _flag);
}addedge;

typedef struct MdPoint {//@todo �������� �ߵı�ű�ʾ�Ķ��Ǵӿ�ʼ��������û�з������Ĳ���
	int mpoint;
	int value;
	int id;
	MdPoint(int _mpoint, int _value, int _id);//����ͼ�������������໥����ͬһ���ߵ�
}mdpoint;

typedef struct EdgePoint {
	int point;								//��һ��Ŀ���
	int id;
	EdgePoint(int _point, int _id);
	int channel;//������ͨ��
}edgepoint;

EdgePoint::EdgePoint(int _point, int _id)
{
	point = _point;
	id = _id;
	channel = -1;
}

MdPoint::MdPoint(int _mpoint, int _value, int _id)
{
	mpoint = _mpoint;
	value = _value;
	id = _id;
}

AddEdge::AddEdge(int _FirstPoint, int _SecondPoint, int _value, vector<bool> _flag)
{
	FirstPoint = _FirstPoint;
	SecondPoint = _SecondPoint;
	value = _value;
	flag = _flag;
}
AddEdge::AddEdge()
{

}

/***************************��ʼ����������**************************************/
typedef struct MyCmp
{
	bool operator()(AddEdge c1, AddEdge c2)const { return c1.FirstPoint > c2.FirstPoint; }
};

/***************************ȫ�ֱ�����**************************************/
map<int, AddEdge>Id_Edge;									//id��ߺŶ�Ӧ�����ں����������
int const_p;												//ÿ���ߵ�ͨ����
int const_d;												//���˥������
vector<AddEdge>NewAddEdge;									//����ӵ��±ߴ洢
//vector<int>UseTimes(_M, _P);								
map<int, vector<mdpoint>>GraphData;							//�൱���ڽӾ���洢ͼ����Ϣ����ʼ������룬ID��

map<int, vector<int>>map_result;							// ÿ��·���������ıߵ�ID��
map<int, vector<int>>map_Amflier_result;					//�洢ÿ��·���ϵķŴ�������
map<int, int>map_ResultChannel;								//ÿ��·����ʹ�õ�ͨ��

/***************************ȫ�ֱ�����**************************************/
//@ ToDo ����ʹ�ô�ӡ·��ʹ��
void printTwoPointResult(vector<int>temp)
{
	for (int i = 0; i < temp.size(); i++)
	{
		cout << temp[i] << " ";
	}
	cout << endl;
}
//@ToDo �����ݼ����ʼ������
void add(int a, int b, int c)
{
	e[idx] = b;
	ne[idx] = h[a];
	w[idx] = c;
	h[a] = idx++;
}
//@ToDo ������ͨ��Լ�������������֮ǰ��·��
int dijkstra(int s,int end)
{
	memset(dist, inf, sizeof dist);
	fill(pre, pre + n + 1, -1);
	dist[s] = 0;
	bool used[maxn] = { false };
	priority_queue<P, vector<P>, greater<P>> heap;
	heap.push(P(0, s));
	while (heap.size())
	{
		P k = heap.top();
		heap.pop();
		int ver = k.second, distance = k.first;
		if (used[ver]) continue;
		used[ver] = true;
		for (int i = h[ver]; i != -1; i = ne[i])
		{
			auto j = e[i];
			if (dist[j] > distance + w[i])
			{
				dist[j] = distance + w[i];
				pre[j] = ver;
				heap.push(P(dist[j], j));
			}
		}
	}
	if (dist[end] == inf) return -1;
	return dist[end];
}
//@ToDo ��������·������Ҫ�����Ľڵ�
vector<int> get_path(int t, int begin)
{
	vector<int> path;
	for (; t != -1; t = pre[t]) path.push_back(t);
	reverse(path.begin(), path.end());
	return path;
}
//@ToDo ��ȡ��Ҫ�ĸ������ݣ������и�ֵ����
void ReadData(vector<edge>&RestorePoint, vector<addedge>&RestoreAddEdge, int M, int T, int P)
{
	int count = M;
	while (count--)
	{
		edge temp, temp2;
		cin >> temp.BeginPoint >> temp.EndPoint >> temp.Value;  //		true˵���ǳ��� false ���
		temp.id = M - count - 1;								//���ڿ��Խ���key-value��ϵ
		vector<bool>flag(P, true);
		temp.flag = true;
		temp2.BeginPoint = temp.EndPoint;
		temp2.EndPoint = temp.BeginPoint;
		temp2.Value = temp.Value;
		temp2.id = temp.id;
		temp2.flag = true;
		Id_Edge.insert(pair<int, AddEdge>(temp.id, AddEdge(temp.BeginPoint, temp.EndPoint, temp.Value, flag)));
		RestorePoint.push_back(temp);
		RestorePoint.push_back(temp2);
		add(temp.BeginPoint, temp.EndPoint, temp.Value);
		add(temp.EndPoint, temp.BeginPoint, temp.Value);
	}
	count = T;
	while (count--)
	{
		addedge temp;
		temp.id = T - count - 1;
		cin >> temp.FirstPoint >> temp.SecondPoint;
		RestoreAddEdge.push_back(temp);
		if (count == 0)
			return;
	}
	fflush(stdout);
}
//@ToDo ��������Ϣ�����ڽӱ�ķ�ʽ�洢��������ͼ������ͼ��������֮����ڶ����ߣ����ձߵ�ID�Ž�������
void GetGraphData(vector<edge>RestorePoint, int P)
{
	//		�������ߵ���Ϣ�洢�����ˣ������ڽӱ�ķ�ʽ
	for (auto it : RestorePoint)
	{
		if (GraphData.find(it.BeginPoint) != GraphData.end())
			GraphData[it.BeginPoint].push_back(MdPoint(it.EndPoint, it.Value, it.id));//����ͨ������λ�����һ���ܴ������
		else
		{
			vector<mdpoint> temp;
			temp.push_back(MdPoint(it.EndPoint, it.Value, it.id));
			GraphData[it.BeginPoint] = temp;
		}
	}
}
//ToDo ����Ŵ�����������λ��
void FuncAmfilerPlace(vector<int>_WayId, vector<int>_path,int _id)
{
	//int count_Am = 0;			//���ڱ�ʾ�Ŵ�������
	vector<int>Temp;
	int Dd = const_d;
	for (int i = 0; i < _WayId.size(); i++)
	{
		int it = _WayId[i];
		if (Dd >= Id_Edge[it].value)
			Dd -= Id_Edge[it].value;
		else {
			Temp.push_back(_path[i]);
			Dd = const_d - Id_Edge[it].value;
		}
	}
	map_Amflier_result[_id] = Temp;
}
//@ToDo �����·���µİ�װ�ķŴ�����Ŀ
int AmpliferNum(vector<int>_WayId, vector<int>_path, vector<int>&Temp)
{
	//int count_Am = 0;			//���ڱ�ʾ�Ŵ�������
	int Dd = const_d;
	for (int i = 0; i < _WayId.size(); i++)
	{
		int it = _WayId[i];
		if (Dd >= Id_Edge[it].value)
			Dd -= Id_Edge[it].value;
		else {
			Temp.push_back(_path[i]);
			Dd = const_d - Id_Edge[it].value;
		}
	}
	return Temp.size();
	//Amflier_result.push_back(Temp);
}
//@ToDo ��������·���������Ľڵ�
vector<int>CalcuId(vector<int>path)
{
	vector<int>WayId;
	for (int j = 0; j < path.size() - 1; j++)
	{
		int min_value = -0XFFFF;
		int min_id;
		for (auto& it : GraphData[path[j]])
		{
			if (it.mpoint == path[j + 1]&&it.value!=inf)
			{
				int temp = count(Id_Edge[it.id].flag.begin(), Id_Edge[it.id].flag.end(), true);
				if (temp > min_value)//�����ڵ�֮������б� ÿ��ѡȡʣ��ͨ�����ı߼���
				{
					min_value = temp;
					min_id = it.id;
				}
			}
		}
		WayId.push_back(min_id);	//�õ��˱���·�����������ıߵļ���
	}
	return WayId;
}
//@ToDo ����·���Ƿ�Ϻ���أ�ʵ�ڲ������ټӱ�
vector<int> DecideWayOk(vector<int>path, bool& flag, int& SelectChannel, int& LackEdgeNum)
{
	if (path.size() == 1)
		flag = false;
	vector<int>WayId=CalcuId(path);

	vector<bool>TFlag(const_p, true);
	for (int i = 0; i < WayId.size(); i++)
	{
		vector<bool>PreTFlag = TFlag;
		for (int j = 0; j < const_p; j++)
			TFlag[j] = TFlag[j] & Id_Edge[WayId[i]].flag[j];
		if (count(TFlag.begin(), TFlag.end(), true) == 0 && i != (WayId.size() - 1)) //������ʱ����ͨ��ʱ����·������
		{
			flag = false;
			LackEdgeNum++;
			TFlag = PreTFlag;
		}
		else if (count(TFlag.begin(), TFlag.end(), true) == 0 && i == (WayId.size() - 1))
		{
			flag = false;
			LackEdgeNum++;
		}
	}
	if (flag)
	{
		for (int i = 0; i < const_p; i++)
		{
			if (TFlag[i] == true)
			{
				SelectChannel = i;
				TFlag[i] = false;
				break;
			}
		}
	}
	return WayId;
}

void CalcNwNc(vector<int>Result_Kways, int& nw, int& nc)
{
	for (int i = 0; i < Result_Kways.size(); i++)
	{
		nc += Id_Edge[Result_Kways[i]].value;
		auto it = Id_Edge[Result_Kways[i]].flag;
		for (int j = 0; j < it.size(); j++)
		{
			if (it[j])
				nw++;
		}
	}
}


//@ToDo ���������֮���k����·��
vector<int>  K_ways(int b, int e,vector<int>&_path)
{
	map<vector<int>, vector<int>>Result_Kways;
	
//	vector<vector<int>>Result_Kways;
	dijkstra(b, e);
	vector<int>path = get_path(e,b);
	vector<int>WayId = CalcuId(path);
	_path = path;
	//Result_Kways.push_back(WayId);
	//Result_Kways[WayId] = path;
	int k = 0;
	vector<int>T_wayId;//��ŵ�ǰ���·��

	vector<int>ReturnResult= WayId;
	_path = path;

	while (k < 1)
	{
		int min = 0XFFFF;
		map<int, int>data;
		for (int i = 0; i < WayId.size()&&Result_Kways.size()<3; i++)
		{
			int id = WayId[i];
			int value = Id_Edge[id].value;
			data[id] = value;
			w[2 * id] = inf;
			w[2 * id + 1] = inf;
			Id_Edge[id].value = inf;
			if (dijkstra(b, e) != -1)
			{
				path = get_path(e, b);
				WayId = CalcuId(path);
				int temp_min = 0;
				//if(Result_Kways.find(WayId)== Result_Kways.end())
				Result_Kways[WayId] = path;
			}
		}
		for (auto it = data.begin(); it != data.end(); it++)
		{
			int id = it->first;
			int value = it->second;
			w[id * 2] = value;
			w[id * 2 + 1] = value;
			Id_Edge[id].value = value;
		}
		k++;
	}
	//cout <<"���е�·��֮��:"<< "begin: "<<b<<"  "<<"end: "<<e<<" "<<Result_Kways.size() << endl;
	//map<double, vector<int>>IdData;
	
	int Total = const_p * Id_Edge.size();
	int LeftChannel=0;
	for (auto it = map_result.begin(); it != map_result.end(); it++)
		LeftChannel += it->second.size();
	LeftChannel = Total - LeftChannel;
	double rw = LeftChannel / Total;
	double a = 9.8*rw + 0.2;

	double max_weight = -1;


	for (auto it=Result_Kways.begin(); it!= Result_Kways.end(); it++)
	{
		int nw=0;//��ʾ·����ʣ���ͨ�����ܺ�
		int nc=0;//��ʾ·���Ͼ���������
		CalcNwNc(it->first, nw, nc);
		double weight = 1.0;
		if(nc!=0)
			weight = nw / pow(nc, a);
		if (max_weight < weight)
		{
			_path = it->second;
			ReturnResult = it->first;
			max_weight = weight;
		}

		//IdData[-weight] = it->first;
		//cout << "weight:" << weight << endl;
		//int left=Total
		//printTwoPointResult(Result_Kways[i]);
	}
	//auto it = IdData.begin();
	//cout << "max_weight: " << max_weight <<"  "<<it->first<< endl;
	//cout << "it:" << it->second.size() << endl;
	return ReturnResult;
}

void TestPathCmp(vector<int>path, int _id, int b, int e, vector<int>_pathway_)
{
	vector<int>WayId = path;
	//@ToDo ��ģ�����ڽ����ȡ��·�����Ƿ���ͨ������Ҫ��
	int SelectChannel = -1;
	vector<bool>TFlag(const_p, true);
	vector<int>StoreId;						//���ڴ�ŵ�ǰ�м����ߵ�ͨ������ʹ��

	for (int i = 0; i < WayId.size(); i++)
	{
		vector<bool>T_TFlag = TFlag;
		for (int j = 0; j < const_p; j++)
			TFlag[j] = TFlag[j] & Id_Edge[WayId[i]].flag[j];    //ִ��&����
		if (count(TFlag.begin(), TFlag.end(), true) == 0 && (i != WayId.size() - 1)) //������ʱ����ͨ��ʱ����·������
		{
			StoreId.push_back(WayId[i]);
			TFlag = T_TFlag;
		}
		else if (count(TFlag.begin(), TFlag.end(), true) == 0 && (i == WayId.size() - 1))
		{
			StoreId.push_back(WayId[i]);
		}
	}

	for (int i = 0; i < const_p; i++)
	{
		if (TFlag[i] == true)
		{
			SelectChannel = i;  //ͨ��ѡ��Ϊi
			TFlag[i] = false;
			break;
		}
	}

	bool AddEdgeFlag = false;
	int MinAddEdge = 0XFFFF;//��С�ļӱߴ���
	vector<int>Add_MinAmpliferNumResult;			//���ڴ����С�ķŴ���·��
	vector<int>Add_MinAmpliferNum;					//������ٵķŴ����ڷŵ�λ��
	int Add_MinSelectChannel;						//�����С�ķŴ���������������ͨ��
	vector<int>Add_Minpathway=_pathway_;
	vector<int>LackChannel;							//ȱ��ͨ���ıߵļ���

	if (!WayId.empty() && SelectChannel != -1 && StoreId.empty())  //��Ϊ��˳�������
	{
		for (int i = 0; i < WayId.size(); i++)
		{
			Id_Edge[WayId[i]].flag[SelectChannel] = false;
		}
		//@pram 
		//����Ŵ����İڷ�λ��
		FuncAmfilerPlace(WayId, path, _id);
		map_result[_id] = WayId;
		map_ResultChannel[_id] = SelectChannel;
	}
	else if (!StoreId.empty())
	{
		vector<bool>CmpChannel(const_p, true);
		vector<int>LessEdgeWay = WayId;
		for (int i = 0; i < LessEdgeWay.size(); i++)
		{
			vector<bool>BeforeCmpChannel = CmpChannel;
			for (int t = 0; t < Id_Edge[LessEdgeWay[i]].flag.size(); t++)
			{
				CmpChannel[t] = CmpChannel[t] & Id_Edge[LessEdgeWay[i]].flag[t];  //������Ҫ����·����ͨ����ʹ������
			}
			if (count(CmpChannel.begin(), CmpChannel.end(), true) == 0)   //������ʱ��Ҫ��ӱ���
			{
				int min_id = LessEdgeWay[i];
				AddEdge it = Id_Edge[min_id];
				vector<bool>AddFlag(const_p, true);
				int Edge_Size = Id_Edge.size();
				Id_Edge.insert(pair<int, AddEdge>(Edge_Size, AddEdge(it.FirstPoint, it.SecondPoint, it.value, AddFlag)));
				GraphData[it.FirstPoint].push_back(MdPoint(it.SecondPoint, it.value, Edge_Size));
				GraphData[it.SecondPoint].push_back(MdPoint(it.FirstPoint, it.value, Edge_Size));

				//it.value = it.value>1?1:it.value;

				NewAddEdge.push_back(AddEdge(it.FirstPoint, it.SecondPoint, it.value, AddFlag));//��ӱ�
				LessEdgeWay[i] = Edge_Size;
				add(it.FirstPoint, it.SecondPoint, it.value);
				add(it.SecondPoint, it.FirstPoint, it.value);
				CmpChannel = BeforeCmpChannel;
			}
		}
		for (int i = 0; i < CmpChannel.size(); i++)
		{
			if (CmpChannel[i] == true)
			{
				SelectChannel = i;
				break;
			}
		}
		FuncAmfilerPlace(LessEdgeWay, Add_Minpathway, _id);
		for (int i = 0; i < LessEdgeWay.size(); i++)
			Id_Edge[LessEdgeWay[i]].flag[SelectChannel] = false;
		map_result[_id] = LessEdgeWay;
		map_ResultChannel[_id] = SelectChannel;
	}
}

//@ToDo �����·��˳�� �����������װ��
void PathCmp(vector<int>path,int _id,int b,int e,vector<int>_pathway_)
{
	int countt = 0;
	vector<int>WayId=path;
	//@ToDo ��ģ�����ڽ����ȡ��·�����Ƿ���ͨ������Ҫ��
	int SelectChannel = -1;
	vector<bool>TFlag(const_p, true);
	vector<int>StoreId;						//���ڴ�ŵ�ǰ�м����ߵ�ͨ������ʹ��

	for (int i = 0; i < WayId.size(); i++)
	{
		vector<bool>T_TFlag = TFlag;
		for (int j = 0; j < const_p; j++)
			TFlag[j] = TFlag[j] & Id_Edge[WayId[i]].flag[j];    //ִ��&����
		if (count(TFlag.begin(), TFlag.end(), true) == 0 && (i != WayId.size() - 1)) //������ʱ����ͨ��ʱ����·������
		{
			StoreId.push_back(WayId[i]);
			TFlag = T_TFlag;
		}
		else if (count(TFlag.begin(), TFlag.end(), true) == 0 && (i == WayId.size() - 1))
		{
			StoreId.push_back(WayId[i]);
		}
	}
	//cout << "OK" << countt++ << endl;
	for (int i = 0; i < const_p; i++)
	{
		if (TFlag[i] == true)
		{
			SelectChannel = i;  //ͨ��ѡ��Ϊi
			TFlag[i] = false;
			break;
		}
	}
	//cout << "OK" << countt++ << endl;

	//�Ƿ�ӱ߱�־λ AddEdgeFlag
	bool AddEdgeFlag = false;
	int MinAddEdge = 0XFFFF;//��С�ļӱߴ���
	vector<int>Add_MinAmpliferNumResult;			//���ڴ����С�ķŴ���·��
	vector<int>Add_MinAmpliferNum;					//������ٵķŴ����ڷŵ�λ��
	int Add_MinSelectChannel;						//�����С�ķŴ���������������ͨ��
	vector<int>Add_Minpathway;
	vector<int>LackChannel;							//ȱ��ͨ���ıߵļ���

	if (!WayId.empty() && SelectChannel != -1 && StoreId.empty())  //��Ϊ��˳�������
	{
		for (int i = 0; i < WayId.size(); i++)
		{
			Id_Edge[WayId[i]].flag[SelectChannel] = false;
		}
		//@pram 
		//����Ŵ����İڷ�λ��
		FuncAmfilerPlace(WayId, path,_id);
		map_result[_id] = WayId;
		map_ResultChannel[_id] = SelectChannel;
	}
	else if (!StoreId.empty())						//��ʾ��ʱ��������·�����޺��ʵ�ͨ���Թ�ʹ��
	{
		//@ToDo ��ʱ��Ҫ����K�����·���㷨
		//��path�е�Ȩֵȫ��Ϊ�����
		int begin = b;
		int end = e;
		vector<int>MinAmpliferNumResult;			//���ڴ����С�ķŴ���·��  ����Ҫ�ӱߵ�״̬
		vector<int>MinAmpliferNum;					//������ٵķŴ����ڷŵ�λ�� ����Ҫ�ӱߵ�״̬
		int MinSelectChannel;						//�����С�ķŴ���������������ͨ�� ����Ҫ�ӱߵ�״̬
		int TempMin = 0x7fffffff;
		int AddTempMin = 0XFFFF;
		vector<int>Minpathway;

		MinAddEdge = StoreId.size();					//����·������Ҫ�ӱߴ������ٵı���
		Add_Minpathway= _pathway_;						//��ʼ����� �������·������Ҫ�ӱߵĲ���
		LackChannel = WayId;
		int k = 0;
		int LackEdgeNum = 0;//��·����ȱ�ٱߵ�����
//		cout << "OK" << countt++ << endl;

		while (k < 1 && !AddEdgeFlag)
			{
			map<int, int>Temp_value;
			for (int i = 0; i < WayId.size(); i++)
				{
					//�ȱ����ֳ� Ȼ��ָ�
					int id = WayId[i];
					int value = Id_Edge[id].value;
					Temp_value[id] = value;
					w[id * 2] = 0XFFFF;
					w[id * 2 + 1] = 0XFFFF;
					//Id_Edge[id].value = 0XFFFF;
					//�˾����ڽ������֮����ڶ�����ʱ���·����ѡȡ���ܸ���
					if (dijkstra(begin,end) != -1)  //������ʱ����·��
					{
						vector<int>pathway = get_path(end, begin);
						
						bool _flag = true;
						int _SelectChannel = -1;
						LackEdgeNum = 0;
						vector<int>ResultRe = DecideWayOk(pathway, _flag, _SelectChannel, LackEdgeNum);
						//cout << "k:" << k << "lackEdgeNum:" << LackEdgeNum << "MinAddEage" << MinAddEdge << endl;
						
						if (_flag)  //���������������Ҫ�ӱ�
						{
							
							//cout << "id:";
							//printTwoPointResult(pathway);
							
							vector<int>TempAlip;//��ʱ�ķŴ�������λ��
							int Amnum = AmpliferNum(ResultRe, pathway, TempAlip);		//������Ŵ���������
							//�˾�Ϊ��ѡ�����ǰ���п���·������Ҫ�Ŵ����;�����С�ĵ�
							if ((Amnum * 100 + ResultRe.size()) <= TempMin)
							{
								Minpathway = pathway;
								MinAmpliferNumResult = ResultRe;
								MinAmpliferNum = TempAlip;
								TempMin = (Amnum * 100 + ResultRe.size());
								MinSelectChannel = _SelectChannel;
							}
							AddEdgeFlag = true;
							/*w[id * 2] = value;
							w[id * 2 + 1] = value;*/
						}
						else
						{
							vector<int>TempAlip;//��ʱ�ķŴ�������λ��
							int Amnum = AmpliferNum(ResultRe, pathway, TempAlip);		//������Ŵ���������
							if (MinAddEdge >= LackEdgeNum /*&& (Amnum * 100 + ResultRe.size() <= AddTempMin)*/)				//���ټӱ�����Ϊ��һͷ�ȴ���
							{
								//cout << "error" << endl;
								MinAddEdge = LackEdgeNum;
								Add_Minpathway = pathway;
								LackChannel = ResultRe;
								AddTempMin = Amnum * 100 + ResultRe.size();
							}
							//w[id * 2] = value;
							//w[id * 2 + 1] = value;
						}
					}
					//w[id * 2] = value;
					//w[id * 2 + 1] = value;
					//Id_Edge[id].value = value;
					}
			/*for (auto it = Temp_value.begin(); it != Temp_value.end(); it++)
			{
				int id = it->first;
				int value = it->second;
				w[id * 2] = value;
				w[id * 2 + 1] = value;
				Id_Edge[id].value = value;
			}*/
			//cout << "OK" << countt++ << endl;

				if (AddEdgeFlag)
				{
					FuncAmfilerPlace(MinAmpliferNumResult, Minpathway,_id);
					for (int i = 0; i < MinAmpliferNumResult.size(); i++)
						Id_Edge[MinAmpliferNumResult[i]].flag[MinSelectChannel] = false;
					map_result[_id] = MinAmpliferNumResult;
					map_ResultChannel[_id] = MinSelectChannel;

				}
				WayId = LackChannel;
				k++;
			}
	}
	//cout << "OK" << countt++ << endl;

	if (!StoreId.empty() && AddEdgeFlag == false)
	{
		vector<bool>CmpChannel(const_p, true);
		vector<int>LessEdgeWay = LackChannel;
		for (int i = 0; i < LessEdgeWay.size(); i++)
		{
			vector<bool>BeforeCmpChannel = CmpChannel;
			for (int t = 0; t < Id_Edge[LessEdgeWay[i]].flag.size(); t++)
			{
				CmpChannel[t] = CmpChannel[t] & Id_Edge[LessEdgeWay[i]].flag[t];  //������Ҫ����·����ͨ����ʹ������
			}
			if (count(CmpChannel.begin(), CmpChannel.end(), true) == 0)   //������ʱ��Ҫ��ӱ���
			{
				int min_id = LessEdgeWay[i];
				AddEdge it = Id_Edge[min_id];
				vector<bool>AddFlag(const_p, true);
				int Edge_Size = Id_Edge.size();
				Id_Edge.insert(pair<int, AddEdge>(Edge_Size, AddEdge(it.FirstPoint, it.SecondPoint, it.value, AddFlag)));
				GraphData[it.FirstPoint].push_back(MdPoint(it.SecondPoint, it.value, Edge_Size));
				GraphData[it.SecondPoint].push_back(MdPoint(it.FirstPoint, it.value, Edge_Size));

				//it.value = it.value>1?1:it.value;

				NewAddEdge.push_back(AddEdge(it.FirstPoint, it.SecondPoint, it.value, AddFlag));//��ӱ�
				LessEdgeWay[i] = Edge_Size;
				add(it.FirstPoint, it.SecondPoint, it.value);
				add(it.SecondPoint, it.FirstPoint, it.value);
				CmpChannel = BeforeCmpChannel;
			}
		}
		for (int i = 0; i < CmpChannel.size(); i++)
		{
			if (CmpChannel[i] == true)
			{
				SelectChannel = i;
				break;
			}
		}
		FuncAmfilerPlace(LessEdgeWay, Add_Minpathway,_id);
		for (int i = 0; i < LessEdgeWay.size(); i++)
			Id_Edge[LessEdgeWay[i]].flag[SelectChannel] = false;
		map_result[_id] = LessEdgeWay;
		map_ResultChannel[_id] = SelectChannel;
	}
}
//ToDo ��������֮���·��
void FindWayTwoPoint(vector<addedge>RestoreAddEdge, int M, int N)
{
	int pre_s = -1;//���ڱ���ǰһ���ڵ㣬��ǰ�������׽ڵ�һ�£����Բ����ٵ���·��Ѱ���㷨
	for (int i = 0; i < RestoreAddEdge.size(); i++)
	{
		int s, end,id;
		s = RestoreAddEdge[i].FirstPoint;			//·����ʼ�ڵ�
		end = RestoreAddEdge[i].SecondPoint;		//·�������ڵ�
		id = RestoreAddEdge[i].id;
		if (pre_s == s)  //����ʼ��һ��ʱ��ʹ����һ�μ��������õ��ĵ�
		{
			//vector<int>path = get_path(end, s);
			//PathCmp(path,id);
		}
		else
		{
			vector<int>pathway;
			vector<int>IdResult=K_ways(s, end,pathway);
			//dijkstra(s,end);
			//vector<int>path = get_path(end, s);
			//cout << "·����ϵ:";
			//printTwoPointResult(path);
			//cout << "pathway:"; printTwoPointResult(pathway);
			//cout << "IdResult:"; printTwoPointResult(IdResult);
			PathCmp(IdResult,id,s,end,pathway);
		}
		//pre_s = s;
	}
}
//@ToDo ���ܴ���
void PrintResult(const int D)//��ӡ�������
{
	cout << NewAddEdge.size() << endl;
	for (auto& it : NewAddEdge)
		cout << it.FirstPoint << " " << it.SecondPoint << endl;
	for (int i = 0; i < map_result.size(); i++)
	{
		cout << map_ResultChannel[i] << " " << map_result[i].size() << " " << map_Amflier_result[i].size() << " ";
		for (int j = 0; j < map_result[i].size(); j++)
			cout << map_result[i][j] << " ";
		for (int j = 0; j < map_Amflier_result[i].size(); j++)
			cout << map_Amflier_result[i][j] << " ";
		cout << endl;
	}
}
int main()
{
	memset(h, -1, sizeof(h));
	//����̨���ݻ�ȡ
	int Nn;					//�ڵ�����N  2-5000
	int M;					//��������M	2-5000
	int T;					//ҵ������T	2-10000
	int P;					//����ͨ������P 2-80
	int D;					//���˥������  2-1000
	vector<edge>RestorePoint;
	vector<addedge>RestoreAddEdge;
	cin >> Nn >> M >> T >> P >> D;
	n = Nn - 1;
	m = M;
	const_p = P;
	const_d = D;
	vector<int>UseTimes(M, P);
	ReadData(RestorePoint, RestoreAddEdge, M, T, P);				//��ȡ������Ϣ
	//��������
	sort(RestorePoint.begin(), RestorePoint.end(), [](edge c1, edge c2) {return c1.BeginPoint < c2.BeginPoint; });
	sort(RestoreAddEdge.begin(), RestoreAddEdge.end(), [](addedge c1, addedge c2) {return c1.FirstPoint > c2.FirstPoint; });
	//��������֮��·����ϵ
	//cout << "����OK" << endl;
	GetGraphData(RestorePoint, P);					//��ȡͼ�������Ϣ
	FindWayTwoPoint(RestoreAddEdge, M, Nn);
	//cout << result.size() << endl;
	//cout << result.size() << endl;
	PrintResult(D);
	return 0;
}