
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
int weight[maxn];
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
vector<vector<int>>result;									//ÿ��·���������ıߵ�ID��
vector<vector<int>>Amflier_result;							//�洢ÿ��·���ϵķŴ�������
vector<int>ResultChannel;//���ͨ����						//ÿ��·����ʹ�õ�ͨ��
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
bool IsEmptyChannel(int u, int v, vector<bool>&CmpChannel)
{
	int max_count = -0XFFFF;
	int max_id;
	int countf = 0;
	for (auto it : GraphData[u])
	{
		if (it.mpoint == v)
		{
			countf = 0;
			for (int t = 0; t < Id_Edge[it.id].flag.size(); t++)
			{
				if (CmpChannel[t] && Id_Edge[it.id].flag[t]) //���ڼ������ǰ�߼���֮ǰ·���У�ͨ���Ƿ��������
					++countf;
			}
			if (max_count < countf)
			{
				max_count = countf;
				max_id = it.id;								//��¼�µ�ǰ��ʹ�õ�ͨ�������ı� 
			}
		}
	}
	if (countf == 0)					//�˱�����ͨ���ɹ�·��ѡ��
	{
		//for (int t = 0; t < CmpChannel.size(); t++)
		//{
		//	CmpChannel[t] = CmpChannel[t] & true;  //������Ҫ����·����ͨ����ʹ������
		//	//cout << CmpChannel[t] << " ";
		//}
		//vector<bool>temp(CmpChannel.size(), false);
		//CmpChannel = temp;
		return false;
	}
	//cout << "ͨ����Ŀ:";
	for (int t = 0; t < Id_Edge[max_id].flag.size(); t++)
	{
		CmpChannel[t] = CmpChannel[t] & Id_Edge[max_id].flag[t];  //������Ҫ����·����ͨ����ʹ������
		//cout << CmpChannel[t] << " ";
	}
	//cout << endl;
	return true;
}

void add(int a, int b, int c)
{
	e[idx] = b;
	ne[idx] = h[a];
	w[idx] = c;
	h[a] = idx++;
}

int dijkstra(int s, vector<bool>& Cmpchannel, bool& P1)
{
	P1 = false;
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

			int max_id = -0XFFFF;
			int max_p = 0;
			for (auto& it : GraphData[i])
			{
				if (j == it.mpoint)
				{
					if (max_id < weight[it.id])
					{
						max_id = weight[it.id];

					}
				}
			}
			if (max_id == -0XFFFF)
				max_id = const_p;
			if (dist[j] > (distance/**0.382*/ + /*0.618**/w[i]* (const_p - max_id + 1)))
			{
				dist[j] = (distance/**0.382*/ + /*0.618**/w[i] /** (const_p - max_id + 1)*/);
				pre[j] = ver;
				heap.push(P(dist[j], j));
			}
		}
	}
	if (dist[n] == inf)
		return -1; //��֪��δ��� ��Ҫ���мӱߴ�����
	P1 = true;
	return dist[n];
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

			int max_id = -0XFFFF;
			int max_p = 0;
			for (auto& it : GraphData[i])
			{
				if (j == it.mpoint)
				{
					if (max_id < weight[it.id])
					{
						max_id = weight[it.id];

					}
				}
			}
			if (max_id == -0XFFFF)
				max_id = const_p;

			if (dist[j] > ((distance + w[i]*(const_p - max_id + 1))))
			{
				dist[j] = (distance + w[i]);
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
//ToDo ��ѡ�����·����ͨ�����д����  ò��û���õ�
//void SelectChannel(vector<int>path, bool flag)
//{
//	vector<bool>Cmpchannel(const_p, true);
//	vector<int>WayId;
//	for (int j = 0; j < path.size() - 1; j++)
//	{
//		int max_value = -0XFFFF;
//		int max_id = GraphData[path[j]][0].id;  //��֤������һ������ʼ���趨
//		for (auto& it : GraphData[path[j]])
//		{
//			if (it.mpoint == path[j + 1])		//��ʾ
//			{
//				int countf = 0;
//				for (int t = 0; t < Id_Edge[it.id].flag.size(); t++)
//				{
//					if (Id_Edge[it.id].flag[t] == true)
//						countf++;
//				}
//				if (max_value < countf)
//				{
//					countf = max_value;
//					max_id = it.id;
//				}
//			}
//		}
//		auto it = Id_Edge[max_id].flag;
//		for (int t = 0; t < it.size(); t++)
//			if (Cmpchannel[t] && it[t])
//				it[t] = true;
//			else
//				it[t] = false;
//		WayId.push_back(max_id);
//		Id_Edge[max_id].flag = it;
//	}
//}

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
		weight[temp.id] = const_p;
		//weight[temp.BeginPoint][temp.EndPoint] = const_p;
		//weight[temp.EndPoint][temp.BeginPoint] = const_p;

	}
	count = T;
	while (count--)
	{
		addedge temp;
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
void FuncAmfilerPlace(vector<int>_WayId, vector<int>_path)
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
	Amflier_result.push_back(Temp);
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
//@ToDo �����·��˳�� �����������װ��
void PathCmp(vector<int>path, vector<bool>&flag)
{
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
	}
	//cout << "WayId:";
	//printTwoPointResult(WayId);
	//@ToDo ��ģ�����ڽ����ȡ��·�����Ƿ���ͨ������Ҫ��
	int SelectChannel = -1;
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
			weight[WayId[i]]--;
			//int x = Id_Edge[WayId[i]].FirstPoint;
			//int y = Id_Edge[WayId[i]].SecondPoint;
			//weight[x][y]--;
			//weight[y][x]--;

		}
		//@pram 
		//����Ŵ����İڷ�λ��
		FuncAmfilerPlace(WayId, path);
		result.push_back(WayId);
		ResultChannel.push_back(SelectChannel);
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
				//w[id * 2] = IdValue;
				//w[id * 2 + 1] = IdValue;
				//cout << "OK_place" << endl;
				AddEdgeFlag = true;
				//break;
			}
			else {
				//w[id * 2] = IdValue;
				//w[id * 2 + 1] = IdValue;
				if (MinAddEdge > LackEdgeNum)
				{
					MinAddEdge = LackEdgeNum;
					Add_Minpathway = pathway;
					LackChannel = ResultRe;
				}
			}
		}
		if (AddEdgeFlag)
		{
			FuncAmfilerPlace(MinAmpliferNumResult, Minpathway);
			result.push_back(MinAmpliferNumResult);
			for (int i = 0; i < MinAmpliferNumResult.size(); i++)
			{
				Id_Edge[MinAmpliferNumResult[i]].flag[MinSelectChannel] = false;
				//int x = Id_Edge[MinAmpliferNumResult[i]].FirstPoint;
				//int y = Id_Edge[MinAmpliferNumResult[i]].SecondPoint;
				//weight[x][y]--;
				//weight[x][y]--;
				weight[MinAmpliferNumResult[i]]--;
			}
			ResultChannel.push_back(MinSelectChannel);
		}
	}

	if (!StoreId.empty() && AddEdgeFlag == false)
	{
		//cout <<"��Ҫ��ӵı�" <<StoreId.size() << endl;
		//printTwoPointResult(StoreId);
		vector<bool>CmpChannel(const_p, true);

		vector<int>LessEdgeWay = WayId;

		if (MinAddEdge != 0XFFFF)
		{
			LessEdgeWay = LackChannel;
		}
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
				int min_id = LessEdgeWay[i];
				//cout << min_id << " ";
				AddEdge it = Id_Edge[min_id];
				vector<bool>AddFlag(const_p, true);
				int Edge_Size = Id_Edge.size();
				Id_Edge.insert(pair<int, AddEdge>(Edge_Size, AddEdge(it.FirstPoint, it.SecondPoint, it.value, AddFlag)));
				GraphData[it.FirstPoint].push_back(MdPoint(it.SecondPoint, it.value, Edge_Size));
				GraphData[it.SecondPoint].push_back(MdPoint(it.FirstPoint, it.value, Edge_Size));
				NewAddEdge.push_back(AddEdge(it.FirstPoint, it.SecondPoint, it.value, AddFlag));//��ӱ�
				LessEdgeWay[i] = Edge_Size;
				add(it.FirstPoint, it.SecondPoint, it.value);
				add(it.SecondPoint, it.FirstPoint, it.value);

				//weight[it.FirstPoint][it.SecondPoint] = const_p;
				//weight[it.SecondPoint][it.FirstPoint] = const_p;
				weight[Edge_Size]=const_p;
				CmpChannel = BeforeCmpChannel;
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
		FuncAmfilerPlace(LessEdgeWay, Add_Minpathway);
		result.push_back(LessEdgeWay);
		for (int i = 0; i < LessEdgeWay.size(); i++)
		{
			Id_Edge[LessEdgeWay[i]].flag[SelectChannel] = false;
			//int x = Id_Edge[LessEdgeWay[i]].FirstPoint;
			//int y = Id_Edge[LessEdgeWay[i]].SecondPoint;
			//weight[x][y]--;
			//weight[y][x]--;
			weight[LessEdgeWay[i]]--;
		}
		ResultChannel.push_back(SelectChannel);
	}
}

//ToDo ��������֮���·��
void FindWayTwoPoint(vector<addedge>RestoreAddEdge, int M, int N)
{
	int pre_s = -1;//���ڱ���ǰһ���ڵ㣬��ǰ�������׽ڵ�һ�£����Բ����ٵ���·��Ѱ���㷨
	vector<bool>CmpChannel(const_p, true);
	for (int i = 0; i < RestoreAddEdge.size(); i++)
	{
		int s, end;
		s = RestoreAddEdge[i].FirstPoint;			//·����ʼ�ڵ�
		end = RestoreAddEdge[i].SecondPoint;		//·�������ڵ�
		vector<bool>temp(const_p, true);
		if (pre_s == s)  //����ʼ��һ��ʱ��ʹ����һ�μ��������õ��ĵ�
		{
			vector<int>path = get_path(end, s);
			PathCmp(path, CmpChannel);
		}
		else
		{
			CmpChannel = temp;//
			bool P = false;
			int ftemp = dijkstra(s, CmpChannel, P);
			vector<int>path = get_path(end, s);
			PathCmp(path, CmpChannel);
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
	for (int i = 0; i < result.size(); i++)
	{
		cout << ResultChannel[i] << " " << result[i].size() << " " << Amflier_result[i].size() << " ";
		for (int j = 0; j < result[i].size(); j++)
			cout << result[i][j] << " ";
		for (int j = 0; j < Amflier_result[i].size(); j++)
			cout << Amflier_result[i][j] << " ";
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
	//sort(RestoreAddEdge.begin(), RestoreAddEdge.end(), [](addedge c1, addedge c2) {return c1.FirstPoint < c2.FirstPoint; });
	//��������֮��·����ϵ
	//cout << "����OK" << endl;
	GetGraphData(RestorePoint, P);					//��ȡͼ�������Ϣ
	FindWayTwoPoint(RestoreAddEdge, M, Nn);
	//cout << result.size() << endl;
	//cout << result.size() << endl;
	PrintResult(D);
	return 0;
}