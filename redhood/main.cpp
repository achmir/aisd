#include <cstdio>
#include <stdlib.h>
#define MAX 100000000
struct TNode
{
	int id;
	int node;
	int weight;
	TNode * next;
};
struct Heap
{
	int node;
	int pos;
};
struct Graph
{
	int edgeid;
	int d;
	int p;
	bool visited;
	bool disabled;
	Graph() { disabled = false; }
};
void build_list(int m, TNode ** lista)
{
	int x = 0, y = 0, w = 0;
	TNode *p;
	for (int i = 0; i < m; i++)
	{
		scanf("%d %d %d", &x, &y, &w);
		p = new TNode;
		p->next = lista[x]; p->id = i; p->node = y; p->weight = w;  lista[x] = p;
		p = new TNode;
		p->next = lista[y]; p->id = i; p->node = x; p->weight = w;  lista[y] = p;

	}

}
int leftChild(int i)
{
	return 2 * i + 1;
}
int rightChild(int i)
{
	return 2 * i + 2;
}
int parent(int i)
{
	return ((i - 1) / 2);
}
void swap(int *a, int *b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}
void find_min_heap(int heap_size, Graph * graphNode, Heap * minHeap)
{
	int left = 0, right = 0, temp1 = 0, temp2 = 0, parentid = 0;
	while (1)
	{
		right = rightChild(parentid);
		left = leftChild(parentid);
		if (left >= heap_size) break;
		temp1 = graphNode[minHeap[left].node].d;
		temp2 = left;
		if ((right < heap_size) && (temp1 > graphNode[minHeap[right].node].d))
		{
			temp1 = graphNode[minHeap[right].node].d;
			temp2 = right;
		}
		if (graphNode[minHeap[parentid].node].d <= temp1) break;
		swap(&minHeap[parentid].node, &minHeap[temp2].node);
		minHeap[minHeap[parentid].node].pos = parentid; minHeap[minHeap[temp2].node].pos = temp2;
		parentid = temp2;
	}
}
void build_heap(int v, Graph * graphNode, Heap * minHeap)
{
	int child = v;
	int parentid = 0;
	int *heapchild, *heapparent;
	while (1)
	{
		parentid = parent(child);
		heapparent = &minHeap[parentid].node;
		heapchild = &minHeap[child].node;
		if (graphNode[*heapparent].d <= graphNode[*heapchild].d) break;
		swap(heapparent, heapchild);
		minHeap[*heapparent].pos = parentid; minHeap[*heapchild].pos = child;
		child = parentid;
	}

}
void dijkstra(Graph * graphNode, int n, TNode **lista)
{
	int u = 0, parentid = 0, heap_size = n - 1;
	Heap* minHeap = new Heap[n];
	for (int i = 0; i < n; i++)
	{
		graphNode[i].d = MAX;
		graphNode[i].p = -1;
		graphNode[i].visited = false;
		minHeap[i].node = i;
		minHeap[i].pos = i;
	}
	graphNode[0].d = 0;
	for (int i = 0; i < n; i++)
	{
		u = minHeap[0].node;
		minHeap[0].node = minHeap[heap_size].node;
		minHeap[minHeap[0].node].pos = parentid;
		find_min_heap(heap_size, graphNode, minHeap);
		heap_size--;
		graphNode[u].visited = true;
		for (TNode *temp = lista[u]; temp; temp = temp->next)
			if (!graphNode[temp->node].visited && (graphNode[temp->node].d > graphNode[u].d + temp->weight))
			{
				graphNode[temp->node].d = graphNode[u].d + temp->weight;
				graphNode[temp->node].p = u;
				graphNode[temp->node].edgeid = temp->id;
				build_heap(minHeap[temp->node].pos, graphNode, minHeap);
			}
	}
	delete[] minHeap;
}
void clear(int n, TNode ** lista)
{
	for (int i = 0; i < n; i++)
	{
		TNode *listaT = lista[i];
		while (listaT)
		{
			TNode *temp = listaT;
			listaT = listaT->next;
			delete temp;
		}
		if (i == n - 1) delete listaT;
	}
}
bool check(int neighbour, Graph * graphNode, int root, int n)
{
	int * shortestPath = new int[n];
	int counter = 0;
	for (int j = neighbour; j > -1; j = graphNode[j].p) shortestPath[counter++] = j;
	for (int i = 0; i < counter - 1; i++)
	{
		if (shortestPath[i] == root)
		{
			delete[] shortestPath;
			return false;
		}
	}
	delete[] shortestPath;
	return true;
}
void getSecond2(Graph * graphNode, int n, TNode ** lista)
{
	int * shortestPath = new int[n];
	int counter = 0, min = MAX, wartosc = MAX;
	int first = graphNode[n - 1].d;
	struct TNode *p = new TNode;
	for (int j = n - 1; j > -1; j = graphNode[j].p) shortestPath[counter++] = j;
	for (int i = 0; i < counter - 1; i++)
	{
		p = lista[shortestPath[i]];

		while (p)
		{
			if (graphNode[p->node].d && p->weight <= min)
			{
				if (p->id != graphNode[shortestPath[i]].edgeid)
				{
					wartosc = graphNode[p->node].d + p->weight + first - graphNode[shortestPath[i]].d;
					if (wartosc < min && check(p->node, graphNode, shortestPath[i], n)) min = wartosc;
				}
			}
			p = p->next;
		}
	}
	if (first < MAX && min < MAX)
		printf("%d %d", first, min);
	else
		printf("#");
	delete[] shortestPath;
	delete p;
}
int main()
{
	int m = 0, n = 0;
	TNode ** lista;
	Graph * graphNode;
	scanf("%d %d", &n, &m);
	lista = new TNode *[n];
	graphNode = new Graph[n];
	for (int i = 0; i < n; i++) lista[i] = NULL;
	build_list(m, lista);
	dijkstra(graphNode, n, lista);
	getSecond2(graphNode, n, lista);
	clear(n, lista);
	delete[] lista;
	delete[] graphNode;
	return 0;
}