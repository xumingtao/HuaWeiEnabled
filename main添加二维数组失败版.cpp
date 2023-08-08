#include<iostream>
#include<vector>
#include<map>
#include<queue>
#include<algorithm>
#include<cstring>
#include<stack>
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

int visit[maxn] = { 0 };

int Weight[maxn / 2][maxn / 2] = {0};

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
	vector<bool>flag;		//��ʾtrue��ʾ����false��ʾ����
	int id;
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
vector<vector<int>>result;									//ÿ��·���������ıߵ�ID��
vector<vector<int>>Amflier_result;							//�洢ÿ��·���ϵķŴ�������
vector<int>ResultChannel;//���ͨ����						//ÿ��·����ʹ�õ�ͨ��


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

//@ToDo �˺������ڽ���˵�v���˵�u�Ƿ�ɵ�������� true��ʾ���� false ��ʾ����

void add(int a, int b, int c)
{
	e[idx] = b;
	ne[idx] = h[a];
	w[idx] = c;
	h[a] = idx++;
}

int dijkstra(int s, int end)
{
	memset(dist, inf, sizeof dist);
	fill(pre, pre + n + 1, -1);
	dist[s] = 0;
	bool used[maxn] = { false };
	//��ʼ�����ʱ���ǿ��е�
	priority_queue<P, vector<P>, greater<P>> heap;
	heap.push(P(0, s));
	bool FlagIsBreak = true;
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
	if (dist[end] == inf)
		return -1; //��֪��δ��� ��Ҫ���мӱߴ�����
	return dist[end];
}
//@ToDo ������ͨ��Լ�������������֮ǰ��·��
int dijkstra(int s)
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
			if (dist[j] > (distance + w[i])*(const_p-Weight[i][j]+1))
			{
				dist[j] = (distance + w[i])*(const_p - Weight[i][j] + 1);
				pre[j] = ver;
				heap.push(P(dist[j], j));
			}
		}
	}
	if (dist[n] == inf) return -1;
	return dist[n];
}

//@ToDo ʹ��DFS�㷨����������֮�������·��������·����ĿPath_way<=20;
void dfs(int pos, int end, vector<int>wayId)
{
	int i;
	if (TwoPointResult.size() == 20)
		return;
	if (pos == end)
	{
		wayId.push_back(end);
		TwoPointResult.push_back(wayId);
		return;
	}
	v[pos] = 1;
	wayId.push_back(pos);
	int pre_point = -1;
	for (auto it : GraphData[pos])
	{
		if (v[it.mpoint] == 0 && it.mpoint != pre_point)
		{
			dfs(it.mpoint, end, wayId);
			pre_point = it.mpoint;
		}
	}
	v[pos] = 0;
	wayId.pop_back();
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

		Weight[temp.BeginPoint][temp.EndPoint] = const_p;
		Weight[temp.EndPoint][temp.BeginPoint] = const_p;
	}
	count = T;
	while (count--)
	{
		addedge temp;
		cin >> temp.FirstPoint >> temp.SecondPoint;
		temp.id = T - count - 1;
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

//@ToDo ����·���Ƿ�Ϻ���أ�ʵ�ڲ������ټӱ�
vector<int> DecideWayOk(vector<int>path, bool& flag, int& SelectChannel, int& LackEdgeNum)
{
	vector<int>WayId;
	for (int j = 0; j < path.size() - 1; j++)
	{
		int min_value = -0XFFFF;
		int min_id;
		for (auto& it : GraphData[path[j]])
		{
			if (it.mpoint == path[j + 1])
			{
				int temp = count(Id_Edge[it.id].flag.begin(), Id_Edge[it.id].flag.end(), true);
				if (temp > min_value)
				{
					min_value = temp;
					min_id = it.id;
				}
			}
		}
		WayId.push_back(min_id);
	}
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

//@ToDo �����������ı�
vector<int> CacucId(vector<int>path, vector<bool>&cmpId,bool& flag)
{
	vector<int>WayId;
	vector<bool>TFlag=cmpId;
	flag = true;
	bool flag1=true;
	for (int i = 0; i < path.size() - 1; i++)
	{
		vector<bool>T_TFlag;
		int min_value = -0XFFFF;
		int min_id = -0XFFFF;
		for (auto& it : GraphData[path[i]])
		{
			T_TFlag = TFlag;
			if (it.mpoint == path[i + 1]&&visit[it.id]==0)
			{
				for (int j = 0; j < const_p; j++)
					T_TFlag[j] = T_TFlag[j] & Id_Edge[it.id].flag[j];    //ִ��&����
				int temp = count(T_TFlag.begin(), T_TFlag.end(), true);
				if (temp > min_value/*it.id>min_id*/)//�����ڵ�֮������б� ÿ��ѡȡʣ��ͨ�����ı߼���
				{
					min_value = temp;
					min_id = it.id;
				}
			}
		}
		if (min_id != -0XFFFF) 
		{
			for (int j = 0; j < const_p; j++)
				TFlag[j] = TFlag[j] & Id_Edge[min_id].flag[j];    //ִ��&����
			visit[min_id] = 1;
			WayId.push_back(min_id);	//�õ��˱���·�����������ıߵļ���
		}
		else
		{
			flag1 = false;
		}
	}
	int cnt = count(TFlag.begin(), TFlag.end(), true);
	if (cnt != 0 && flag1 == true)
	{
		flag =true;
		cmpId = TFlag;
	}
	else {
		flag = false;
	}
	//flag = (cnt != 0 ? true : false);
	return WayId;
}

bool FindReplace(int begin_b, int end_e, int IdPre,vector<bool>&cmpId,vector<int>&PreId,vector<int>&PrePath)
{
	vector<int>WayId;
	bool flag = false;
	bool flag1 = true;

	vector<int>Temp_preId;
	vector<int>Temp_Prepath;

	vector<int>Pre_Temp_preId= PreId;
	vector<int>Pre_Temp_Prepath= PrePath;


	for (auto& it : GraphData[begin_b])
		{
			if (it.mpoint != end_e&&(dijkstra(it.mpoint, end_e) != -1)&&visit[it.id]==0)
			{
				vector<bool>Temp_cmpId = cmpId;
				vector<int>path = get_path(end_e, it.mpoint);
				vector<int>EdgeId = CacucId(path, Temp_cmpId, flag);
				//path.insert(path.begin(),)
				//EdgeId.insert(EdgeId.begin(), it.id);
				//cout << "hello" << endl;
				if (flag)
				{
					cmpId = Temp_cmpId;
					for (int i = 0; i < PreId.size(); i++)
					{
						if (PreId[i] == IdPre)
						{
							Temp_preId.push_back(it.id);
							for (int j = 0; j < EdgeId.size(); j++)
							{
								if (find(PreId.begin(), PreId.end(), EdgeId[j]) != PreId.end())
									flag1 = false;
								Temp_preId.push_back(EdgeId[j]);
							}
						}
						else
						{
							Temp_preId.push_back(PreId[i]);
						}
					}
					for (int i = 0; i < PrePath.size(); i++)
					{
						if (PrePath[i] == end_e)
						{
							//Temp_Prepath.push_back(PrePath[i]);
							for (int j = 0; j < path.size(); j++)
								Temp_Prepath.push_back(path[j]);
						}
						else {
							Temp_Prepath.push_back(PrePath[i]);
						}
					}
					break;
				}
			}
		}
	if(flag == true && flag1 == true)
	{

		PreId = Temp_preId;
		PrePath = Temp_Prepath;
	}

	return flag==true&&flag1==true;
}

//@ToDo �����·��˳�� �����������װ�� 
bool PathCmp(vector<int>path, vector<bool>&flag,int _id)  //�ӱ߷���true ,���ӱ߷���false;
{
	bool ReturnAddEdgeFlag = false;
	
	vector<int>WayId;
	vector<bool>TFlag(const_p, true);

	for (int j = 0; j < path.size() - 1; j++)
	{
		vector<bool>T_TFlag;
		int min_value = -0XFFFF;
		int min_id;
		for (auto& it : GraphData[path[j]])
		{
			T_TFlag = TFlag;
			if (it.mpoint == path[j + 1])
			{
				for (int t = 0; t < const_p; t++)
					T_TFlag[t] = T_TFlag[t] & Id_Edge[it.id].flag[t];    //ִ��&����
				int temp = count(T_TFlag.begin(), T_TFlag.end(), true);
				if (temp > min_value)//�����ڵ�֮������б� ÿ��ѡȡʣ��ͨ�����ı߼���
				{
					min_value = temp;
					min_id = it.id;
				}
			}
		}
		for (int t = 0; t < const_p; t++)
			TFlag[t] = TFlag[t] & Id_Edge[min_id].flag[t];    //ִ��&����
		WayId.push_back(min_id);	//�õ��˱���·�����������ıߵļ���
		visit[min_id] = 1;
	}
	//cout << "WayId:";
	//printTwoPointResult(WayId);
	//@ToDo ��ģ�����ڽ����ȡ��·�����Ƿ���ͨ������Ҫ��
	int SelectChannel = -1;
	//vector<bool>TFlag(const_p, true);
	//memset(TFlag, true, sizeof TFlag);
	std::fill(TFlag.begin(), TFlag.end(), true);
	vector<int>StoreId;

	vector<int>LackChannel;//ȱ��ͨ���ıߵļ���

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
	//cout << "StoreId:";
	//printTwoPointResult(StoreId);

	for (int i = 0; i < const_p; i++)
	{
		if (TFlag[i] == true)
		{
			SelectChannel = i;  //ͨ��ѡ��Ϊi
			TFlag[i] = false;
			break;
		}
	}
	//�Ƿ�ӱ߱�־λ AddEdgeFlag
	bool AddEdgeFlag = false;

	int MinAddEdge = 0XFFFF;//��С�ļӱߴ���
	vector<int>Add_MinAmpliferNumResult;			//���ڴ����С�ķŴ���·��
	vector<int>Add_MinAmpliferNum;					//������ٵķŴ����ڷŵ�λ��
	int Add_MinSelectChannel;						//�����С�ķŴ���������������ͨ��
	//int TempMin = 0XFFFF;
	vector<int>Add_Minpathway;

	if (!WayId.empty() && SelectChannel != -1 && StoreId.empty())
	{
		for (int i = 0; i < WayId.size(); i++)
		{
			Id_Edge[WayId[i]].flag[SelectChannel] = false;

			int x = Id_Edge[WayId[i]].FirstPoint;
			int y = Id_Edge[WayId[i]].SecondPoint;
			Weight[x][y]--;
			Weight[y][x]--;

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
		int begin = path.front();
		int end = path.back();

		vector<int>MinAmpliferNumResult;			//���ڴ����С�ķŴ���·��
		vector<int>MinAmpliferNum;					//������ٵķŴ����ڷŵ�λ��
		int MinSelectChannel;						//�����С�ķŴ���������������ͨ��
		int TempMin = 0XFFFF;
		vector<int>Minpathway;

		//LackChannel[StoreId.size()] = WayId;  //����map��


		for (int i = 0; i < WayId.size(); i++)
		{
			//�ȱ����ֳ� Ȼ��ָ�
			int id = WayId[i];
			int IdValue = w[id * 2];
			w[id * 2] = 0XFFFF;
			w[id * 2 + 1] = 0XFFFF;
			dijkstra(begin);
			vector<int>pathway = get_path(end, begin);
			bool _flag = true;
			int _SelectChannel = -1;

			int LackEdgeNum = 0;//��·����ȱ�ٱߵ�����

			vector<int>ResultRe = DecideWayOk(pathway, _flag, _SelectChannel, LackEdgeNum);
			if (_flag)
			{
				vector<int>TempAlip;//��ʱ�ķŴ�������λ��
				int Amnum = AmpliferNum(ResultRe, pathway, TempAlip);		//������Ŵ���������
				if ((Amnum * 100 + ResultRe.size()) < TempMin)
				{
					Minpathway = pathway;
					MinAmpliferNumResult = ResultRe;
					MinAmpliferNum = TempAlip;
					TempMin = (Amnum * 100 + ResultRe.size());
					MinSelectChannel = _SelectChannel;
				}
				AddEdgeFlag = true;
			}
			else {
				if (MinAddEdge >= LackEdgeNum)
				{
					MinAddEdge = LackEdgeNum;
					Add_Minpathway = pathway;
					LackChannel = ResultRe;
				}
			}
		}
		if (AddEdgeFlag)
		{
			FuncAmfilerPlace(MinAmpliferNumResult, Minpathway,_id);
			for (int i = 0; i < MinAmpliferNumResult.size(); i++)
			{
				Id_Edge[MinAmpliferNumResult[i]].flag[MinSelectChannel] = false;
				int x = Id_Edge[MinAmpliferNumResult[i]].FirstPoint;
				int y = Id_Edge[MinAmpliferNumResult[i]].SecondPoint;
				Weight[x][y]--;
				Weight[y][x]--;
			}

			map_result[_id] = MinAmpliferNumResult;
			map_ResultChannel[_id] = MinSelectChannel;
		}
	}

	if (!StoreId.empty() && AddEdgeFlag == false)
	{
		//cout <<"��Ҫ��ӵı�" <<StoreId.size() << endl;
		//printTwoPointResult(StoreId);
		vector<bool>CmpChannel(const_p, true);
		vector<int>LessEdgeWay = WayId;
		vector<int>LessPath = path;
		if (MinAddEdge != 0XFFFF)
		{
			LessEdgeWay = LackChannel;
			LessPath = Add_Minpathway;
		}
		vector<int>Pre_LessEdgeWay = LessEdgeWay;
		vector<int>Pre_Add_Minpathway = LessPath;
		//�洢�ӱ����ݣ����ӱ�λ�ý��б�������������滻
		vector<int>StoreAddEdge;
		bool StoreEdge = false;
		bool FindFlag1 = false;
		for (int i = 0; i < LessEdgeWay.size(); i++)
		{
			vector<bool>BeforeCmpChannel = CmpChannel;
			for (int t = 0; t < Id_Edge[LessEdgeWay[i]].flag.size(); t++)
			{
				CmpChannel[t] = CmpChannel[t] & Id_Edge[LessEdgeWay[i]].flag[t];  //������Ҫ����·����ͨ����ʹ������
				//cout << CmpChannel[t] << " ";
			}
			if (count(CmpChannel.begin(), CmpChannel.end(), true) == 0)   //������ʱ��Ҫ��ӱ���
			{
				//���������һ��������������ȥѰ��һ������������ߵ�·��
				int begin_b = LessPath[i];
				int end_e = LessPath[i + 1];
				vector<bool>Temp_BeforeCmpChannel= BeforeCmpChannel;
				bool FindFlag = false;//FindReplace(begin_b, end_e, LessEdgeWay[i], Temp_BeforeCmpChannel, Pre_LessEdgeWay, Pre_Add_Minpathway);
				if (FindFlag)
				{
					CmpChannel = Temp_BeforeCmpChannel;
					//LessEdgeWay = Pre_LessEdgeWay;
					//Add_Minpathway = Pre_Add_Minpathway;
					FindFlag1 = true;
					//cout << "ok" << endl;
					continue;
					//cout << "���߳ɹ���" << endl;
				}
				else
				{
					//@ToDo Ѱ������߽��д���
					int min_id = LessEdgeWay[i];
					//cout << min_id << " ";
					AddEdge it = Id_Edge[min_id];
					vector<bool>AddFlag(const_p, true);
					int Edge_Size = Id_Edge.size();

					StoreAddEdge.push_back(min_id);
					StoreAddEdge.push_back(Edge_Size);
					StoreEdge = true;

					Id_Edge.insert(pair<int, AddEdge>(Edge_Size, AddEdge(it.FirstPoint, it.SecondPoint, it.value, AddFlag)));
					GraphData[it.FirstPoint].push_back(MdPoint(it.SecondPoint, it.value, Edge_Size));
					GraphData[it.SecondPoint].push_back(MdPoint(it.FirstPoint, it.value, Edge_Size));
					NewAddEdge.push_back(AddEdge(it.FirstPoint, it.SecondPoint, it.value, AddFlag));//��ӱ�
					LessEdgeWay[i] = Edge_Size;
					add(it.FirstPoint, it.SecondPoint, it.value);
					add(it.SecondPoint, it.FirstPoint, it.value);

					Weight[it.FirstPoint][it.SecondPoint] = const_p;
					Weight[it.SecondPoint][it.FirstPoint] = const_p;

					CmpChannel = BeforeCmpChannel;
					ReturnAddEdgeFlag = true;
				}
			}
		}
		//cout << endl;
		for (int i = 0; i < CmpChannel.size(); i++)
		{
			if (CmpChannel[i] == true)
			{
				SelectChannel = i;
				break;
			}
		}

		vector<int>T_Pre_LessEdgeWay;
		vector<int>T_Pre_Add_Minpathway;

		if (StoreEdge == true&& FindFlag1==true)
		{
			for (int i = 0; i < StoreAddEdge.size(); i+=2)
			{
				vector<int>::iterator itor = find(Pre_LessEdgeWay.begin(), Pre_LessEdgeWay.end(), StoreAddEdge[i]);
				Pre_LessEdgeWay.insert(itor, StoreAddEdge[i + 1]);
				itor = find(Pre_LessEdgeWay.begin(), Pre_LessEdgeWay.end(), StoreAddEdge[i]);
				Pre_LessEdgeWay.erase(itor);
			}
			T_Pre_LessEdgeWay = Pre_LessEdgeWay;
			T_Pre_Add_Minpathway = Pre_Add_Minpathway;
		}
		else if (StoreEdge == true&& FindFlag1 == false)
		{
			T_Pre_LessEdgeWay = LessEdgeWay;
			T_Pre_Add_Minpathway = Add_Minpathway;
		}
		else if (StoreEdge == false&&FindFlag1 == true)
		{
			T_Pre_LessEdgeWay = Pre_LessEdgeWay;
			T_Pre_Add_Minpathway = Pre_Add_Minpathway;
		}


		FuncAmfilerPlace(T_Pre_LessEdgeWay, T_Pre_Add_Minpathway,_id);
		
		for (int i = 0; i < T_Pre_LessEdgeWay.size(); i++)
		{
			Id_Edge[T_Pre_LessEdgeWay[i]].flag[SelectChannel] = false;
			int x = Id_Edge[T_Pre_LessEdgeWay[i]].FirstPoint;
			int y = Id_Edge[T_Pre_LessEdgeWay[i]].SecondPoint;
			Weight[x][y]--;
			Weight[y][x]--;
		}
		map_result[_id] = T_Pre_LessEdgeWay;
		map_ResultChannel[_id] = SelectChannel;

	}
	return ReturnAddEdgeFlag;
}

//ToDo ��������֮���·��
void FindWayTwoPoint(vector<addedge>RestoreAddEdge, int M, int N)
{
	int pre_s = -1;//���ڱ���ǰһ���ڵ㣬��ǰ�������׽ڵ�һ�£����Բ����ٵ���·��Ѱ���㷨
	memset(visit, 0, sizeof visit);
	vector<bool>CmpChannel(const_p, true);
	bool flag = false;
	for (int i = 0; i < RestoreAddEdge.size(); i++)
	{
		int s, end,id;
		s = RestoreAddEdge[i].FirstPoint;			//·����ʼ�ڵ�
		end = RestoreAddEdge[i].SecondPoint;		//·�������ڵ�
		id = RestoreAddEdge[i].id;
		
		vector<bool>temp(const_p, true);
		if (pre_s == s&&flag==false)  //����ʼ��һ��ʱ��ʹ����һ�μ��������õ��ĵ�
		{
			vector<int>path = get_path(end, s);
			flag=PathCmp(path, CmpChannel,id);
		}
		else
		{
			CmpChannel = temp;//
			bool P = false;
			int ftemp = dijkstra(s,end);
			vector<int>path = get_path(end, s);
			flag=PathCmp(path, CmpChannel,id);
		}
		pre_s = s;
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
	//sort(RestorePoint.begin(), RestorePoint.end(), [](edge c1, edge c2) {return c1.BeginPoint < c2.BeginPoint; });
	//sort(RestoreAddEdge.begin(), RestoreAddEdge.end(), [](addedge c1, addedge c2) {return c1.SecondPoint>c2.SecondPoint; });
	//��������֮��·����ϵ
	//cout << "����OK" << endl;
	GetGraphData(RestorePoint, P);					//��ȡͼ�������Ϣ
	FindWayTwoPoint(RestoreAddEdge, M, Nn);
	//cout << result.size() << endl;
	//cout << result.size() << endl;
	PrintResult(D);
	return 0;
}