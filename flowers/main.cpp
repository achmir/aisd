#include <stdlib.h>
#include <cstdio>

const int MAX_N = 8500;

struct TNode
{
	int node;
	int flowers;
	struct TNode * next;
};

struct BTNode
{
	BTNode * left;
	BTNode * right;
	int level;
	int data;
};

TNode *L[MAX_N];
BTNode * root;
int n, depth;
int maxpath = 0;

void build_list()
{
	int x = 0, y = 0, w = 0;
	struct TNode *p;

	for (int i = 0; i < MAX_N; i++) L[i] = NULL;

	scanf("%d %d", &n, &depth);
	for (int i = 0; i < n; i++)
	{
		scanf("%d %d %d", &x, &y, &w);

		p = new TNode;
		p->next = L[x - 1]; p->node = y; p->flowers = w;  L[x - 1] = p;
		p = new TNode;
		p->next = L[y - 1]; p->node = x; p->flowers = w; L[y - 1] = p;
	}

}

void build(TNode *p, BTNode ** vp, int parentknot, int knot)
{
	while (p)
	{
		if (p->node == parentknot + 1) vp[knot]->data = p->flowers;
		else
		{
			if (!vp[knot]->left)
				vp[knot]->left = vp[p->node - 1];
			else
				vp[knot]->right = vp[p->node - 1];
			build(L[p->node - 1], vp, knot, p->node - 1);
		}
		p = p->next;
	}

}

void build_tree()
{
	BTNode ** vp;

	vp = new BTNode *[MAX_N];

	for (int i = 0; i <= n; i++)
	{
		vp[i] = new BTNode;
		vp[i]->left = NULL;
		vp[i]->right = NULL;

		vp[i]->level = 0;
		vp[i]->data = 0;
	}
	struct TNode *p;
	p = L[0];
	int parentknot = 0;
	vp[0]->data = 0;

	vp[0]->left = vp[L[0]->node - 1];
	vp[0]->right = NULL;
	int knot = p->node - 1;
	p = L[knot];

	build(p, vp, parentknot, knot);

	root = vp[0];

	delete[] vp;

}

void DFS(BTNode * v)
{
	if (v)
	{

		int children = 0;

		if (v->left)
		{
			children++;
			v->left->level = v->level + 1;
		}

		if (v->right)
		{
			children++;
			v->right->level = v->level + 1;
		}
		if (v->level > maxpath) maxpath = v->level;
		DFS(v->left);
		DFS(v->right);
	}
}

bool isLeaf(BTNode * v) {
	if (v->left == NULL && v->right == NULL)
	{
		return true;
	}
	return false;
}
int Max(int a, int b)
{
	if (a > b) return a;
	return b;
}
struct tab
{
	int* pathtab;
	int tabSize;
};
void DFSRelease(BTNode * v)
{
	if (v)
	{
		DFSRelease(v->left);
		DFSRelease(v->right);
		delete v;
	}
}
tab* x(BTNode * v)
{
	tab *tablica1;
	tablica1 = new tab;
	tab *tablica2;
	tablica2 = new tab;
	if (v->level == depth)
	{
		if (v->left != NULL)
		{
			DFSRelease(v->left);
			v->left = NULL;
		}
		if (v->right != NULL)
		{
			DFSRelease(v->right);
			v->right = NULL;
		}
	}
	if (isLeaf(v))
	{
		tab *tablica;
		tablica = new tab;
		tablica->pathtab = new int[2];
		tablica->pathtab[0] = 0;
		tablica->pathtab[1] = v->data;
		tablica->tabSize = 2;

		return tablica;
	}
	if (v->left != NULL)
	{
		tablica1 = x(v->left);
	}
	if (v->right != NULL)
	{
		tablica2 = x(v->right);
	}
	tab *wart = new tab;
	wart->pathtab = new int[tablica1->tabSize + tablica2->tabSize];
	wart->tabSize = tablica1->tabSize + tablica2->tabSize;
	for (int j = 0; j < wart->tabSize; j++)
	{
		wart->pathtab[j] = 0;
	}
	wart->pathtab[1] = v->data;

	for (int i = 0; i < tablica1->tabSize; i++)
	{
		for (int j = 0; j < tablica2->tabSize; j++)
		{
			wart->pathtab[i + j + 1] = Max(tablica1->pathtab[i] + tablica2->pathtab[j] + v->data, wart->pathtab[i + j + 1]);
		}
	}
	delete[] tablica1;
	delete[] tablica2;
	return wart;
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

int main()
{
	build_list();
	build_tree();
	DFS(root);
	printf("%d", x(root->left)->pathtab[depth]);
	clear(MAX_N, L);
	DFSRelease(root);

	return 0;
}
