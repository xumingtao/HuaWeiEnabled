
#include<iostream>
#include<vector>
#include<map>
#include<queue>
#include<algorithm>
#include<cstring>
#include<stack>
using namespace std;
/***************************迪杰斯特拉算法各项参数**************************************/
const int maxn = 30100;		//考虑到边的双向性
const int inf = 0x3f3f3f3f;
typedef pair<int, int> P;
int idx, h[maxn], e[maxn], ne[maxn];
int w[maxn];                    //用来存储每条边的权值
int dist[maxn];
int pre[maxn];
int n, m;
/***************************迪杰斯特拉算法各项参数**************************************/
int weight[maxn];
/*深度优先搜索计算两点之间的所有路径数目*/
vector<vector<int>>TwoPointResult;
int v[5010] = { 0 };//顶点数目
/***************************初始定义区**************************************/
typedef struct Edge {//存储边的信息
	int BeginPoint;
	int EndPoint;
	int Value;
	int id;   //边的ID号
	bool flag;
}edge;

typedef struct AddEdge {
	int FirstPoint;//起始点
	int SecondPoint;//目标点
	int value;		//这个只是在表示路径时可以使用
	vector<bool>flag;		//表示true表示正向，false表示逆向
	AddEdge();
	AddEdge(int _FirstPoint, int _SecondPoint, int _value, vector<bool> _flag);
}addedge;

typedef struct MdPoint {//@todo 存在问题 边的编号表示的都是从开始到结束，没有反过来的操作
	int mpoint;
	int value;
	int id;
	MdPoint(int _mpoint, int _value, int _id);//无向图中两个顶点是相互共享同一条边的
}mdpoint;

typedef struct EdgePoint {
	int point;								//下一个目标点
	int id;
	EdgePoint(int _point, int _id);
	int channel;//经过的通道
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
/***************************初始定义区结束**************************************/
typedef struct MyCmp
{
	bool operator()(AddEdge c1, AddEdge c2)const { return c1.FirstPoint > c2.FirstPoint; }
};

/***************************全局变量区**************************************/
map<int, AddEdge>Id_Edge;									//id与边号对应，便于后续输出处理
int const_p;												//每条边的通道数
int const_d;												//最大衰减距离
vector<AddEdge>NewAddEdge;									//新添加的新边存储
//vector<int>UseTimes(_M, _P);								
map<int, vector<mdpoint>>GraphData;							//相当于邻接矩阵存储图的信息，起始点与距离，ID号
vector<vector<int>>result;									//每条路径所经过的边的ID号
vector<vector<int>>Amflier_result;							//存储每条路径上的放大器数量
vector<int>ResultChannel;//结果通道数						//每条路径所使用的通道
/***************************全局变量区**************************************/

//@ ToDo 调试使用打印路径使用
void printTwoPointResult(vector<int>temp)
{
	for (int i = 0; i < temp.size(); i++)
	{
		cout << temp[i] << " ";
	}
	cout << endl;
}

//@ToDo 此函数用于解决端点v到端点u是否可到达的问题 true表示可以 false 表示不可
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
				if (CmpChannel[t] && Id_Edge[it.id].flag[t]) //用于计算出当前边加入之前路径中，通道是否可以满足
					++countf;
			}
			if (max_count < countf)
			{
				max_count = countf;
				max_id = it.id;								//记录下当前可使用的通道数最大的边 
			}
		}
	}
	if (countf == 0)					//此边中无通道可供路径选择
	{
		//for (int t = 0; t < CmpChannel.size(); t++)
		//{
		//	CmpChannel[t] = CmpChannel[t] & true;  //后续需要安排路径中通道的使用消耗
		//	//cout << CmpChannel[t] << " ";
		//}
		//vector<bool>temp(CmpChannel.size(), false);
		//CmpChannel = temp;
		return false;
	}
	//cout << "通道数目:";
	for (int t = 0; t < Id_Edge[max_id].flag.size(); t++)
	{
		CmpChannel[t] = CmpChannel[t] & Id_Edge[max_id].flag[t];  //后续需要安排路径中通道的使用消耗
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
	//初始化设计时都是可行的
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
		return -1; //不知如何处理 需要进行加边处理了
	P1 = true;
	return dist[n];
}
//@ToDo 不考虑通道约束，计算出两点之前的路径
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

//@ToDo 使用DFS算法来计算两点之间的所有路径，限制路径数目Path_way<=20;
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

//@ToDo 计算出最短路径所需要经过的节点
vector<int> get_path(int t, int begin)
{
	vector<int> path;
	for (; t != -1; t = pre[t]) path.push_back(t);
	reverse(path.begin(), path.end());
	return path;
}
//ToDo 将选择过的路径的通道进行处理掉  貌似没有用到
//void SelectChannel(vector<int>path, bool flag)
//{
//	vector<bool>Cmpchannel(const_p, true);
//	vector<int>WayId;
//	for (int j = 0; j < path.size() - 1; j++)
//	{
//		int max_value = -0XFFFF;
//		int max_id = GraphData[path[j]][0].id;  //保证必须有一个，初始化设定
//		for (auto& it : GraphData[path[j]])
//		{
//			if (it.mpoint == path[j + 1])		//表示
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

//@ToDo 读取需要的各项数据，并进行赋值运算
void ReadData(vector<edge>&RestorePoint, vector<addedge>&RestoreAddEdge, int M, int T, int P)
{
	int count = M;
	while (count--)
	{
		edge temp, temp2;
		cin >> temp.BeginPoint >> temp.EndPoint >> temp.Value;  //		true说明是出点 false 入点
		temp.id = M - count - 1;								//后期可以建立key-value关系
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

//@ToDo 将各边信息按照邻接表的方式存储起来，此图是无向图，两个点之间存在多条边，按照边的ID号进行区分
void GetGraphData(vector<edge>RestorePoint, int P)
{
	//		将各个边的信息存储起来了，按照邻接表的方式
	for (auto it : RestorePoint)
	{
		if (GraphData.find(it.BeginPoint) != GraphData.end())
			GraphData[it.BeginPoint].push_back(MdPoint(it.EndPoint, it.Value, it.id));//两边通道数如何划分是一个很大的问题
		else
		{
			vector<mdpoint> temp;
			temp.push_back(MdPoint(it.EndPoint, it.Value, it.id));
			GraphData[it.BeginPoint] = temp;
		}
	}
}

//ToDo 计算放大器的数量与位置
void FuncAmfilerPlace(vector<int>_WayId, vector<int>_path)
{
	//int count_Am = 0;			//用于表示放大器个数
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

//@ToDo 计算路径是否合乎规矩，实在不存在再加边
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
		if (count(TFlag.begin(), TFlag.end(), true) == 0 && i != (WayId.size() - 1)) //表明此时经过通道时，道路被堵死
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
//@ToDo 计算此路径下的安装的放大器数目
int AmpliferNum(vector<int>_WayId, vector<int>_path, vector<int>&Temp)
{
	//int count_Am = 0;			//用于表示放大器个数
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
//@ToDo 计算出路径顺序 并将结果数据装填
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
					T_TFlag[t] = T_TFlag[t] & Id_Edge[it.id].flag[t];    //执行&操作
				int temp = count(T_TFlag.begin(), T_TFlag.end(), true);
				if (temp > min_value)//遍历节点之间的所有边 每次选取剩余通道最多的边加入
				{
					min_value = temp;
					min_id = it.id;
				}
			}
		}
		for (int t = 0; t < const_p; t++)
			TFlag[t] = TFlag[t] & Id_Edge[min_id].flag[t];    //执行&操作
		WayId.push_back(min_id);	//得到了本条路径下所经过的边的集合
	}
	//cout << "WayId:";
	//printTwoPointResult(WayId);
	//@ToDo 此模块用于解决求取的路径中是否有通道满足要求
	int SelectChannel = -1;
	std::fill(TFlag.begin(), TFlag.end(), true);
	vector<int>StoreId;

	vector<int>LackChannel;//缺少通道的边的集合

	for (int i = 0; i < WayId.size(); i++)
	{
		vector<bool>T_TFlag = TFlag;
		for (int j = 0; j < const_p; j++)
			TFlag[j] = TFlag[j] & Id_Edge[WayId[i]].flag[j];    //执行&操作
		if (count(TFlag.begin(), TFlag.end(), true) == 0 && (i != WayId.size() - 1)) //表明此时经过通道时，道路被堵死
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
			SelectChannel = i;  //通道选择为i
			TFlag[i] = false;
			break;
		}
	}
	//是否加边标志位 AddEdgeFlag
	bool AddEdgeFlag = false;



	int MinAddEdge = 0XFFFF;//最小的加边次数
	vector<int>Add_MinAmpliferNumResult;			//用于存放最小的放大器路径
	vector<int>Add_MinAmpliferNum;					//存放最少的放大器摆放的位置
	int Add_MinSelectChannel;						//存放最小的放大器数量所经过的通道
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
		//计算放大器的摆放位置
		FuncAmfilerPlace(WayId, path);
		result.push_back(WayId);
		ResultChannel.push_back(SelectChannel);
	}
	else if (!StoreId.empty())						//表示此时求出的最短路径中无合适的通道以供使用
	{
		//@ToDo 此时需要计算K条最短路径算法
		//将path中的权值全置为无穷大
		int begin = path.front();
		int end = path.back();

		vector<int>MinAmpliferNumResult;			//用于存放最小的放大器路径
		vector<int>MinAmpliferNum;					//存放最少的放大器摆放的位置
		int MinSelectChannel;						//存放最小的放大器数量所经过的通道
		int TempMin = 0XFFFF;
		vector<int>Minpathway;

		//LackChannel[StoreId.size()] = WayId;  //进入map中


		for (int i = 0; i < WayId.size(); i++)
		{
			//先保存现场 然后恢复
			int id = WayId[i];
			int IdValue = w[id * 2];
			w[id * 2] = 0XFFFF;
			w[id * 2 + 1] = 0XFFFF;
			dijkstra(begin);
			vector<int>pathway = get_path(end, begin);
			bool _flag = true;
			int _SelectChannel = -1;

			int LackEdgeNum = 0;//此路径中缺少边的数量

			vector<int>ResultRe = DecideWayOk(pathway, _flag, _SelectChannel, LackEdgeNum);
			if (_flag)
			{
				vector<int>TempAlip;//临时的放大器放置位置
				int Amnum = AmpliferNum(ResultRe, pathway, TempAlip);		//计算出放大器的数量
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
		//cout <<"需要添加的边" <<StoreId.size() << endl;
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
				CmpChannel[t] = CmpChannel[t] & Id_Edge[LessEdgeWay[i]].flag[t];  //后续需要安排路径中通道的使用消耗
				//cout << CmpChannel[t] << " ";
			}
			if (count(CmpChannel.begin(), CmpChannel.end(), true) == 0)   //表明此时需要添加边了
			{
				int min_id = LessEdgeWay[i];
				//cout << min_id << " ";
				AddEdge it = Id_Edge[min_id];
				vector<bool>AddFlag(const_p, true);
				int Edge_Size = Id_Edge.size();
				Id_Edge.insert(pair<int, AddEdge>(Edge_Size, AddEdge(it.FirstPoint, it.SecondPoint, it.value, AddFlag)));
				GraphData[it.FirstPoint].push_back(MdPoint(it.SecondPoint, it.value, Edge_Size));
				GraphData[it.SecondPoint].push_back(MdPoint(it.FirstPoint, it.value, Edge_Size));
				NewAddEdge.push_back(AddEdge(it.FirstPoint, it.SecondPoint, it.value, AddFlag));//添加边
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

//ToDo 查找两点之间的路径
void FindWayTwoPoint(vector<addedge>RestoreAddEdge, int M, int N)
{
	int pre_s = -1;//用于保存前一个节点，若前后两次首节点一致，可以不用再调用路径寻找算法
	vector<bool>CmpChannel(const_p, true);
	for (int i = 0; i < RestoreAddEdge.size(); i++)
	{
		int s, end;
		s = RestoreAddEdge[i].FirstPoint;			//路径开始节点
		end = RestoreAddEdge[i].SecondPoint;		//路径结束节点
		vector<bool>temp(const_p, true);
		if (pre_s == s)  //当起始点一样时，使用上一次计算搜索得到的点
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
//@ToDo 功能代码
void PrintResult(const int D)//打印结果代码
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
	//控制台数据获取
	int Nn;					//节点数量N  2-5000
	int M;					//连边数量M	2-5000
	int T;					//业务数量T	2-10000
	int P;					//单边通道数量P 2-80
	int D;					//最大衰减距离  2-1000
	vector<edge>RestorePoint;
	vector<addedge>RestoreAddEdge;
	cin >> Nn >> M >> T >> P >> D;
	n = Nn - 1;
	m = M;
	const_p = P;
	const_d = D;
	vector<int>UseTimes(M, P);
	ReadData(RestorePoint, RestoreAddEdge, M, T, P);				//获取输入信息
	//进行排序
	//sort(RestorePoint.begin(), RestorePoint.end(), [](edge c1, edge c2) {return c1.BeginPoint < c2.BeginPoint; });
	//sort(RestoreAddEdge.begin(), RestoreAddEdge.end(), [](addedge c1, addedge c2) {return c1.FirstPoint < c2.FirstPoint; });
	//查找两点之间路径关系
	//cout << "排序OK" << endl;
	GetGraphData(RestorePoint, P);					//获取图的相关信息
	FindWayTwoPoint(RestoreAddEdge, M, Nn);
	//cout << result.size() << endl;
	//cout << result.size() << endl;
	PrintResult(D);
	return 0;
}