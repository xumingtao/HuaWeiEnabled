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
//@ToDo 将数据加入初始数组中
void add(int a, int b, int c)
{
	e[idx] = b;
	ne[idx] = h[a];
	w[idx] = c;
	h[a] = idx++;
}
//@ToDo 不考虑通道约束，计算出两点之前的路径
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
//@ToDo 单独计算路径所经过的节点
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
				if (temp > min_value)//遍历节点之间的所有边 每次选取剩余通道最多的边加入
				{
					min_value = temp;
					min_id = it.id;
				}
			}
		}
		WayId.push_back(min_id);	//得到了本条路径下所经过的边的集合
	}
	return WayId;
}
//@ToDo 计算路径是否合乎规矩，实在不存在再加边
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
//@ToDo 计算出两点之间的k多条路径
//vector<vector<int>> K_ways(int b, int e)
//{
//	vector<vector<int>>Result_Kways;
//	dijkstra(b, e);
//	vector<int>path = get_path(b,e);
//	vector<int>WayId = CalcuId(path);
//	Result_Kways.push_back(WayId);
//	int k = 0;
//	while (k < 2)
//	{
//		for (int i = 0; i < WayId.size(); i++)
//		{
//
//		}
//	}
//
//	PathCmp(path);
//}




//@ToDo 计算出路径顺序 并将结果数据装填
void PathCmp(vector<int>path)
{
	vector<int>WayId=CalcuId(path);
	//@ToDo 此模块用于解决求取的路径中是否有通道满足要求
	int SelectChannel = -1;
	vector<bool>TFlag(const_p, true);
	vector<int>StoreId;						//用于存放当前有几条边的通道不能使用

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
	vector<int>Add_Minpathway;
	vector<int>LackChannel;							//缺少通道的边的集合

	if (!WayId.empty() && SelectChannel != -1 && StoreId.empty())  //此为最顺利的情况
	{
		for (int i = 0; i < WayId.size(); i++)
		{
			Id_Edge[WayId[i]].flag[SelectChannel] = false;
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
		vector<int>MinAmpliferNumResult;			//用于存放最小的放大器路径  不需要加边的状态
		vector<int>MinAmpliferNum;					//存放最少的放大器摆放的位置 不需要加边的状态
		int MinSelectChannel;						//存放最小的放大器数量所经过的通道 不需要加边的状态
		int TempMin = 0x7fffffff;
		int AddTempMin = 0XFFFF;
		vector<int>Minpathway;

		MinAddEdge = StoreId.size();					//保留路径中需要加边次数最少的边数
		Add_Minpathway = path;						//初始化设计 保留最短路径下需要加边的操作
		LackChannel = WayId;
		int k = 0;
		int LackEdgeNum = 0;//此路径中缺少边的数量
		while (k < 3 && !AddEdgeFlag)
			{
			map<int, int>Temp_value;
			for (int i = 0; i < WayId.size(); i++)
				{
					//先保存现场 然后恢复
					int id = WayId[i];
					int value = Id_Edge[id].value;
					Temp_value[id] = value;
					w[id * 2] = inf;
					w[id * 2 + 1] = inf;
					Id_Edge[id].value = inf;
					//此举用于解决两点之间存在多条边时最短路径的选取易受干扰
					if (dijkstra(begin,end) != -1)  //表明此时存在路径
					{
						vector<int>pathway = get_path(end, begin);
						
						bool _flag = true;
						int _SelectChannel = -1;
						LackEdgeNum = 0;
						vector<int>ResultRe = DecideWayOk(pathway, _flag, _SelectChannel, LackEdgeNum);
						//cout << "k:" << k << "lackEdgeNum:" << LackEdgeNum << "MinAddEage" << MinAddEdge << endl;
						
						if (_flag)  //此种情况表明不需要加边
						{
							
							//cout << "id:";
							//printTwoPointResult(pathway);
							
							vector<int>TempAlip;//临时的放大器放置位置
							int Amnum = AmpliferNum(ResultRe, pathway, TempAlip);		//计算出放大器的数量
							//此举为了选择出当前所有可行路径下需要放大器和距离最小的点
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
							vector<int>TempAlip;//临时的放大器放置位置
							int Amnum = AmpliferNum(ResultRe, pathway, TempAlip);		//计算出放大器的数量
							if (MinAddEdge >= LackEdgeNum /*&& (Amnum * 100 + ResultRe.size() <= AddTempMin)*/)				//减少加边数量为第一头等大事
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
			for (auto it = Temp_value.begin(); it != Temp_value.end(); it++)
			{
				int id = it->first;
				int value = it->second;
				w[id * 2] = value;
				w[id * 2 + 1] = value;
				Id_Edge[id].value = value;
			}
				if (AddEdgeFlag)
				{
					FuncAmfilerPlace(MinAmpliferNumResult, Minpathway);
					result.push_back(MinAmpliferNumResult);
					for (int i = 0; i < MinAmpliferNumResult.size(); i++)
						Id_Edge[MinAmpliferNumResult[i]].flag[MinSelectChannel] = false;
					ResultChannel.push_back(MinSelectChannel);
				}
				WayId = LackChannel;
				k++;
			}
	}
	
	if (!StoreId.empty() && AddEdgeFlag == false)
	{
		vector<bool>CmpChannel(const_p, true);
		vector<int>LessEdgeWay = LackChannel;
		for (int i = 0; i < LessEdgeWay.size(); i++)
		{
			vector<bool>BeforeCmpChannel = CmpChannel;
			for (int t = 0; t < Id_Edge[LessEdgeWay[i]].flag.size(); t++)
			{
				CmpChannel[t] = CmpChannel[t] & Id_Edge[LessEdgeWay[i]].flag[t];  //后续需要安排路径中通道的使用消耗
			}
			if (count(CmpChannel.begin(), CmpChannel.end(), true) == 0)   //表明此时需要添加边了
			{
				int min_id = LessEdgeWay[i];
				AddEdge it = Id_Edge[min_id];
				vector<bool>AddFlag(const_p, true);
				int Edge_Size = Id_Edge.size();
				Id_Edge.insert(pair<int, AddEdge>(Edge_Size, AddEdge(it.FirstPoint, it.SecondPoint, it.value, AddFlag)));
				GraphData[it.FirstPoint].push_back(MdPoint(it.SecondPoint, it.value, Edge_Size));
				GraphData[it.SecondPoint].push_back(MdPoint(it.FirstPoint, it.value, Edge_Size));

				//it.value = it.value>1?1:it.value;

				NewAddEdge.push_back(AddEdge(it.FirstPoint, it.SecondPoint, it.value, AddFlag));//添加边
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
		FuncAmfilerPlace(LessEdgeWay, Add_Minpathway);
		result.push_back(LessEdgeWay);
		for (int i = 0; i < LessEdgeWay.size(); i++)
			Id_Edge[LessEdgeWay[i]].flag[SelectChannel] = false;
		ResultChannel.push_back(SelectChannel);
	}
}
//ToDo 查找两点之间的路径
void FindWayTwoPoint(vector<addedge>RestoreAddEdge, int M, int N)
{
	int pre_s = -1;//用于保存前一个节点，若前后两次首节点一致，可以不用再调用路径寻找算法
	for (int i = 0; i < RestoreAddEdge.size(); i++)
	{
		int s, end;
		s = RestoreAddEdge[i].FirstPoint;			//路径开始节点
		end = RestoreAddEdge[i].SecondPoint;		//路径结束节点
		if (pre_s == s)  //当起始点一样时，使用上一次计算搜索得到的点
		{
			vector<int>path = get_path(end, s);
			PathCmp(path);
		}
		else
		{
			dijkstra(s,end);
			vector<int>path = get_path(end, s);
			//cout << "路径关系:";
			//printTwoPointResult(path);
			PathCmp(path);
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
	sort(RestorePoint.begin(), RestorePoint.end(), [](edge c1, edge c2) {return c1.BeginPoint < c2.BeginPoint; });
	sort(RestoreAddEdge.begin(), RestoreAddEdge.end(), [](addedge c1, addedge c2) {return c1.FirstPoint > c2.FirstPoint; });
	//查找两点之间路径关系
	//cout << "排序OK" << endl;
	GetGraphData(RestorePoint, P);					//获取图的相关信息
	FindWayTwoPoint(RestoreAddEdge, M, Nn);
	//cout << result.size() << endl;
	//cout << result.size() << endl;
	PrintResult(D);
	return 0;
}