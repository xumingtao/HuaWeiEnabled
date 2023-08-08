//#include<iostream>
//#include<algorithm>
//#include<vector>
//#include<deque>
//using namespace std;
//// 排序实现的接口
//
//// 插入排序
//void InsertSort(int* a, int n);
//
//// 希尔排序
//void ShellSort(vector<int>Data)
//{
//	int gap = 10;
//	while (gap)
//	{
//		for (int i = 0; i < Data.size() - gap; i++)
//		{
//			int x = Data[i + gap];
//			int end = i;
//			while (end >= 0)
//			{
//				if (Data[end] > x)
//				{
//					Data[end + gap] = Data[end];
//					end -= gap;
//				}
//				else
//					break;
//			}
//			Data[end + gap] = x;
//		}
//		gap /= 2;
//	}
//	for (auto& it : Data)
//		cout << it << " ";
//}
//
//// 选择排序
//void SelectSort(vector<int>Data)
//{
//	for (int i = 0; i < Data.size()-1; i++)
//	{
//		int min = Data[i];
//		for (int j = i + 1; j < Data.size(); j++)
//		{
//			if (min > Data[j])
//				swap(min, Data[j]);
//			//min = min > Data[j] ? Data[j] : min;
//		}
//		Data[i] = min;
//	}
//	for (auto& it : Data)
//		cout << it << " ";
//	cout << endl;
//}
//
//// 堆排序
//void AdjustDwon(int* a, int n, int root);
//void HeapSort(int* a, int n);
//
//// 冒泡排序
//void BubbleSort(int* a, int n);
//
//// 快速排序递归实现
//
//// 快速排序hoare版本
//int PartSort1(vector<int>&Data, int left, int right)
//{
//	int mid =left;
//	while (left < right)
//	{
//		while (left < right&&Data[left] <= Data[mid])
//			left++;
//		while (left < right&&Data[right] >= Data[mid])
//			right--;
//		swap(Data[left], Data[right]);
//	}
//	swap(Data[left], Data[mid]);
//	return left;
//}
//// 快速排序挖坑法
//int PartSort2(int* a, int left, int right);
//
//// 快速排序前后指针法
//int PartSort3(int* a, int left, int right);
//void QuickSort(vector<int>&Data, int left, int right)
//{
//	//int left=
//	if (left >= right)
//		return;
//	int key = PartSort1(Data, left, right);
//	QuickSort(Data, left, key-1);
//	QuickSort(Data, key+1, right);
//}
//
//// 快速排序 非递归实现
//void QuickSortNonR(int* a, int left, int right);
//
//// 归并排序递归实现
//void MergeSort(vector<int>&Data,int left,int right)
//{
//	if (left >= right)
//		return;
//	int mid = (left + right) / 2;
//	MergeSort(Data, left, mid);
//	MergeSort(Data, mid + 1, right);
//
//	int begin1 = left,begin2=mid+1;
//	int end1 = mid, end2 = right;
//	vector<int>temp;
//	while (begin1 <= end1 && begin2 <= end2)
//	{
//		if (Data[begin1] < Data[begin2])
//			temp.push_back(Data[begin1++]);
//		else
//			temp.push_back(Data[begin2++]);
//	}
//	while(begin1 <= end1)
//	{
//		temp.push_back(Data[begin1++]);
//	}
//	while (begin2<=end2)
//	{
//		temp.push_back(Data[begin2++]);
//	}
//	for (int j = left; j <= right; j++)
//		Data[j] = temp[j - left];
//}
//
//// 归并排序非递归实现
//void MergeSortNonR(int* a, int n);
//
//// 计数排序
//void CountSort(int* a, int n);
//
//typedef struct data
//{
//	int id;
//	int parent_id;
//	int value;
//	data(int _id, int _parent_id, int _value);
//}Data;
//data::data(int _id, int _parent_id, int _value)
//{
//	id = _id;
//	parent_id = _parent_id;
//	value = _value;
//}
//int max_value = -0XFFFF;
//void blacktracing(vector<bool>&flag, vector<Data>Temp,vector<int>&way,Data NowD,int value)
//{
//	if (NowD.parent_id == -1)
//	{
//		max_value = max(value, max_value);
//		return;
//	}
//	if (flag[NowD.parent_id] == false)
//		{
//			flag[NowD.parent_id] = true;
//			value += Temp[NowD.parent_id].value;
//			way.push_back(NowD.parent_id);
//			blacktracing(flag, Temp, way, Temp[NowD.parent_id], value);
//			flag[NowD.parent_id] = false;
//			value -= Temp[NowD.parent_id].value;
//		}
//}
////int main()
////{
////	//vector<int>Data;
////	//for (int i=0;i<5000;i++)
////	//{
////	//	Data.push_back(rand() % 5000);
////	//}
////	////ShellSort(Data);
////
////	////SelectSort(Data);
////
////	////QuickSort(Data,0,Data.size()-1);
////
////	//MergeSort(Data, 0, Data.size() - 1);
////
////	//for (auto& it : Data)
////	//	cout << it << " ";
////	//cout << endl;
////
////	//cout << Data.size() << endl;
////	/*vector<int>data{ 1,3,45,6,7 };
////	deque<int>data2{ 1,3,45,6,7 };
////	typedef struct flow {
////		int a;
////		int b;
////	}Flow;
////	flow f1, f2, f3;
////	f1.a = 2; f1.b = 3;
////	f2.a = 4; f2.b = 6;
////	f3.a = 7; f3.b = 9;
////	vector<flow>data3;
////	data3.push_back(f1);
////	data3.push_back(f2);
////	data3.push_back(f3);
////	vector<flow>t;
////	t.push_back(f2);
////	for (int i = 0; i < data2.size(); i++)
////		cout << data2[i] << endl;
////	for (int i = 0; i < t.size(); i++)
////	{
////		vector<flow>::iterator it = find(data3.begin(), data3.end(), t[i]);
////		if (it != data3.end())
////			data3.erase(it);
////	}
////	cout << data[0]<<" "<<data[1] <<" "<<data[2]<<endl;*/
////	//计算vector的长度
////	/*vector<double>data;
////	cout << sizeof(data)/sizeof(double) << endl;
////	for (int i = 1; i <= 100; i++)
////		data.push_back(i);
////	cout << sizeof(data[0]) / sizeof(double) << endl;*/
////
////	//vector<int>data{ 1 ,4, 2, 5, 5, 1, 6 };
////	//int temp;
////	////while (cin >> temp)	data.push_back(temp);
////	//int cnt=13;
////	////cin >> cnt ;
////	//int result = -1;
////	//for (int i = 0; i <= ((cnt / data.size()) + 1);i++)
////	//{
////	//	int total=0;
////	//	for (int j = 0; j < data.size(); j++)
////	//	{
////	//		if (data[j] <= i)
////	//			total += data[j];
////	//		else {
////	//			total += i;
////	//		}
////	//	}
////	//	if (total <= cnt)
////	//	{
////	//		result = i;
////	//		//break;
////	//	}
////	//	else {
////	//		//result -= 1;
////	//		break;
////	//	}
////	//}
////	//cout << result;
////
////	/*int N;
////	cin >> N;
////	vector<Data>Temp;
////	int count = N;
////	while (count--)
////	{
////		Data temp(0,0,0);
////		cin >> temp.id >> temp.parent_id >> temp.value;
////		Temp.push_back(temp);
////	}
////	for (auto& it : Temp)
////	{
////		vector<bool>flag(N, false);
////		int value = it.value;
////		vector<int>way;
////		flag[it.id] = true;
////		way.push_back(it.id);
////		max_value = max(value, max_value);
////		if (it.parent_id != -1)
////		{
////			value += Temp[it.parent_id].value;
////			blacktracing(flag, Temp, way, Temp[it.parent_id], value);
////		}
////	}
////	cout << max_value << endl;*/
////	/*for (int i = 0; i < 5; i++)
////	{
////		for (int j = 0; j < 3; j++)
////		{
////			if (j == 1)
////				continue;
////			cout << j << endl;
////		}
////		cout << "error" << endl;
////	}*/
////
////	//for (int i = 0; i < 500; i++)
////	//{
////	//	int a = rand() % 100;
////	//	int b = rand() % 100;
////	//	if(a!=b)
////	//		cout << a << " " << b << " " << (rand() % 6 + 1) << endl;
////	//}
////
////}
//
//#include <vector>
//#include <cmath>
//#include <cstdio>
//#include <queue>
//#include <cstring>
//#include <iostream>
//using namespace std;
//
//#define N 100010
//#define LL long long
//#define inf 0x7f7f7f7f
//#define mem(a,n) memset(a,n,sizeof(a))
//
//int read()
//{
//	int num = 0, flag = 1; char c;
//	while ((c = getchar()) < '0' || c > '9') if (c == '-') flag = -1;
//	while (c >= '0'&&c <= '9') { num = num * 10 + c - 48; c = getchar(); }
//	return num * flag;
//}
//
//struct node {
//	int v, w;
//	node() {}
//	node(int V, int W) { v = V; w = W; }
//	bool operator < (const node &rhs) const {
//		return rhs.w > w;
//	}
//};
//
//int dis[N], n, m, a[N], s, vis[N], t;
//vector<node> G[N];
//
//void dijkstra() {
//	for (int i = 1; i <= n; i++)
//		dis[i] = inf;
//	dis[s] = 0;
//	priority_queue<node> q;
//	q.push(node(s, 0));
//	while (!q.empty()) {
//		node x = q.top(); q.pop();
//		for (int i = 0; i < G[x.v].size(); i++) {
//			node y = G[x.v][i];
//			if (dis[y.v] > x.w + y.w) {
//				dis[y.v] = x.w + y.w;
//				q.push(node(y.v, dis[y.v]));
//			}
//		}
//	}
//}
//int main()
//{
//	/*n = read(); m = read();
//	for (int i = 1, u, v, w; i <= m; i++) {
//		u = read(), v = read(), w = read();
//		G[u].push_back(node(v, w));
//		G[v].push_back(node(u, w));
//	}
//	s = read(); t = read();
//	dijkstra();
//	cout << dis[t];*/
//	pirority_queue<int>temp;
//}
//
//
//
////#include <iostream>
////#include <fstream>
////#include <vector>
////#include <string>
////using namespace std;
////int main()
////{
////	ifstream infile;
////	infile.open("1.txt", ios::in);
////	if (!infile.is_open())
////	{
////		cout << "读取文件失败" << endl;
////		return 1;
////	}
////	string s;
////	vector<string>v1;
////
////	while (!infile.eof())
////	{
////		infile >> s;
////		v1.push_back(s);
////	}
////
////	//while (getline(infile, s))
////	//{
////	//	//infile >> s;
////	//	v1.push_back(s);
////	//}
////	for (int i = 0; i < v1.size(); i++)
////	{
////		cout << v1.at(i);
////		cout << endl;
////	}
////	infile.close();
////	return 0;
////}
//
//#include<cstdio>
//#include<queue>
//#include<cstring>
//using namespace std;
//const int maxn = 1000010;
//const int INF = 2e9;
//int head[maxn], nxt[maxn], to[maxn], val[maxn], tt;
//int n, m, s;
//int dis[maxn];
//bool vis[maxn];
//priority_queue<pair<int, int> >q;//前面存距离，后面存编号
////优先队列维护的规则是以前前面的数字为关键字，然后再按照后面的数字为第二关键字 
//
//void add(int u, int v, int w)
//{
//	printf("tt:%d\n", tt);
//	//std::cout << "tt:" << tt << std::endl;
//	to[++tt] = v;
//	nxt[tt] = head[u];
//	val[tt] = w;
//	head[u] = tt;
//}
//void dijkstra()
//{
//	for (int i = 0; i < n; i++)
//		printf("%d\n", head[i]);
//	for (int i = 0; i <= maxn; i++) dis[i] = INF;
//	dis[s] = 0;
//	q.push(make_pair(0, s));
//	while (!q.empty())
//	{
//		int u = q.top().second;
//		q.pop();
//		if (vis[u]) continue;
//		vis[u] = 1;
//		//printf("%d\n",u);
//		for (int i = head[u]; i; i = nxt[i])
//		{
//			int v = to[i];
//			if (dis[v] > (dis[u] + val[i]))
//			{
//				dis[v] = dis[u] + val[i];
//				q.push(make_pair(-dis[v], v));
//			}
//		}
//	}
//}
//int main()
//{
//	//freopen("123.in","r",stdin);
//	//freopen("123.out","w",stdout); 
//	scanf_s("%d%d%d", &n, &m, &s);
//	for (int i = 1; i <= m; i++)
//	{
//		int x, y, w;
//		scanf_s("%d%d%d", &x, &y, &w);
//		//if(x==y) continue;
//		add(x, y, w);
//	}
//	dijkstra();
//	for (int i = 1; i <= n; i++) printf("%d ", dis[i]);
//	//fclose(stdin);fclose(stdout);
//	return 0;
//}

//#include<iostream>
//using namespace std;
//int main()
//{
//	int count = 0;
//	for (int i = 0; count<1000; i++)
//	{
//		int a = rand() % 5000;
//		int b = rand() % 5000;
//		if (a!=b)
//		{
//			cout << a << " " << b << endl;
//			count++;
//		}
//		//int b = i;
//		//cout << b << endl;
//	}
//	return 0;
//}



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

			if (dist[j] > distance + w[i])
			{
				dist[j] = distance + w[i];
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
			if (dist[j] > distance + w[i])
			{
				dist[j] = distance + w[i];
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
	for (int j = 0; j < path.size() - 1; j++)
	{
		int min_value = -0XFFFF;
		int min_id;
		for (auto& it : GraphData[path[j]])
		{
			if (it.mpoint == path[j + 1])
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
	//cout << "WayId:";
	//printTwoPointResult(WayId);
	//@ToDo 此模块用于解决求取的路径中是否有通道满足要求
	int SelectChannel = -1;
	vector<bool>TFlag(const_p, true);
	vector<int>StoreId;

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
	vector<int>Add_Minpathway;
	vector<int>LackChannel;//缺少通道的边的集合

	if (!WayId.empty() && SelectChannel != -1 && StoreId.empty())
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

		vector<int>MinAmpliferNumResult;			//用于存放最小的放大器路径
		vector<int>MinAmpliferNum;					//存放最少的放大器摆放的位置
		int MinSelectChannel;						//存放最小的放大器数量所经过的通道
		int TempMin = 0XFFFF;
		int AddTempMin = 0XFFFF;
		vector<int>Minpathway;

		//LackChannel[StoreId.size()] = WayId;  //进入map中
		MinAddEdge = StoreId.size();
		Add_Minpathway = path;
		LackChannel = WayId;
		int k = 0;
		//while(k<3&& !AddEdgeFlag)
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
				vector<int>TempAlip;//临时的放大器放置位置
				int Amnum = AmpliferNum(ResultRe, pathway, TempAlip);		//计算出放大器的数量
				if (MinAddEdge >= LackEdgeNum&&((Amnum * 100 + ResultRe.size()) < AddTempMin))
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
				Id_Edge[MinAmpliferNumResult[i]].flag[MinSelectChannel] = false;
			ResultChannel.push_back(MinSelectChannel);
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
			if (it.mpoint == path[j + 1])
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
//@ ToDo 计算出两个节点之间的最短K条路径 若是没有就求出所有当前路径然后退出
void YenAlgorithm(int begin,int end)
{
	vector<vector<int>>WayStoge;
	dijkstra(begin);
	vector<int>path = get_path(end, begin); //计算出来了路径经过的各个节点
	vector<int>WayId = CalcuId(path);
	WayStoge.push_back(WayId);
	for (int i = 0; i < WayId.size(); i++)
	{
		int id = WayId[i];
		int IdValue = w[id * 2];
		w[id * 2] = 0XFFFF;
		w[id * 2 + 1] = 0XFFFF;
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
		pre_s = s;
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