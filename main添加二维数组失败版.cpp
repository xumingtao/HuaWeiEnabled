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

/*深度优先搜索计算两点之间的所有路径数目*/
vector<vector<int>>TwoPointResult;
int v[5010] = { 0 };//顶点数目

int visit[maxn] = { 0 };

int Weight[maxn / 2][maxn / 2] = {0};

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
	int id;
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


map<int, vector<int>>map_result;							// 每条路径所经过的边的ID号
map<int, vector<int>>map_Amflier_result;					//存储每条路径上的放大器数量
map<int, int>map_ResultChannel;								//每条路径所使用的通道


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

			if (dist[j] > distance + w[i])
			{
				dist[j] = distance + w[i];
				pre[j] = ver;
				heap.push(P(dist[j], j));
			}
		}
	}
	if (dist[end] == inf)
		return -1; //不知如何处理 需要进行加边处理了
	return dist[end];
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
void FuncAmfilerPlace(vector<int>_WayId, vector<int>_path,int _id)
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
	map_Amflier_result[_id] = Temp;
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

//@ToDo 计算所经过的边
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
					T_TFlag[j] = T_TFlag[j] & Id_Edge[it.id].flag[j];    //执行&操作
				int temp = count(T_TFlag.begin(), T_TFlag.end(), true);
				if (temp > min_value/*it.id>min_id*/)//遍历节点之间的所有边 每次选取剩余通道最多的边加入
				{
					min_value = temp;
					min_id = it.id;
				}
			}
		}
		if (min_id != -0XFFFF) 
		{
			for (int j = 0; j < const_p; j++)
				TFlag[j] = TFlag[j] & Id_Edge[min_id].flag[j];    //执行&操作
			visit[min_id] = 1;
			WayId.push_back(min_id);	//得到了本条路径下所经过的边的集合
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

//@ToDo 计算出路径顺序 并将结果数据装填 
bool PathCmp(vector<int>path, vector<bool>&flag,int _id)  //加边返回true ,不加边返回false;
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
		visit[min_id] = 1;
	}
	//cout << "WayId:";
	//printTwoPointResult(WayId);
	//@ToDo 此模块用于解决求取的路径中是否有通道满足要求
	int SelectChannel = -1;
	//vector<bool>TFlag(const_p, true);
	//memset(TFlag, true, sizeof TFlag);
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

			int x = Id_Edge[WayId[i]].FirstPoint;
			int y = Id_Edge[WayId[i]].SecondPoint;
			Weight[x][y]--;
			Weight[y][x]--;

		}
		//@pram 
		//计算放大器的摆放位置
		FuncAmfilerPlace(WayId, path,_id);

		map_result[_id] = WayId;
		map_ResultChannel[_id] = SelectChannel;

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
		//cout <<"需要添加的边" <<StoreId.size() << endl;
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
		//存储加边数据，将加边位置进行保留，后面进行替换
		vector<int>StoreAddEdge;
		bool StoreEdge = false;
		bool FindFlag1 = false;
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
				//在这里添加一个函数，让其再去寻找一条可替代此条边的路径
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
					//cout << "补边成功！" << endl;
				}
				else
				{
					//@ToDo 寻找替代边进行处理
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
					NewAddEdge.push_back(AddEdge(it.FirstPoint, it.SecondPoint, it.value, AddFlag));//添加边
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

//ToDo 查找两点之间的路径
void FindWayTwoPoint(vector<addedge>RestoreAddEdge, int M, int N)
{
	int pre_s = -1;//用于保存前一个节点，若前后两次首节点一致，可以不用再调用路径寻找算法
	memset(visit, 0, sizeof visit);
	vector<bool>CmpChannel(const_p, true);
	bool flag = false;
	for (int i = 0; i < RestoreAddEdge.size(); i++)
	{
		int s, end,id;
		s = RestoreAddEdge[i].FirstPoint;			//路径开始节点
		end = RestoreAddEdge[i].SecondPoint;		//路径结束节点
		id = RestoreAddEdge[i].id;
		
		vector<bool>temp(const_p, true);
		if (pre_s == s&&flag==false)  //当起始点一样时，使用上一次计算搜索得到的点
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
//@ToDo 功能代码
void PrintResult(const int D)//打印结果代码
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
	//sort(RestoreAddEdge.begin(), RestoreAddEdge.end(), [](addedge c1, addedge c2) {return c1.SecondPoint>c2.SecondPoint; });
	//查找两点之间路径关系
	//cout << "排序OK" << endl;
	GetGraphData(RestorePoint, P);					//获取图的相关信息
	FindWayTwoPoint(RestoreAddEdge, M, Nn);
	//cout << result.size() << endl;
	//cout << result.size() << endl;
	PrintResult(D);
	return 0;
}