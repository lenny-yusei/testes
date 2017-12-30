// 複数の異なる容量の容器があり、容器に入っているものの量は偏りがある。
// このとき、ある容器から別の容器に溢れないように移すという操作を繰り返し、ある量だけを取り出す作業の手順を求める
// 例：容量が３，５，８の３つの入れ物があり、８だけが満杯の状態から量が４になる容器をつくる手順を求める
#include <stdio.h>
#include <stdlib.h>

// 容器の状態をリスト構造で記録する
typedef struct N {
	int data;		// 容器の状態
	struct N* label;// ラベル　前回の状態を記録する
	struct N* next;	// リスト構造の次の要素
} node;

// 容器の数
#define POD_NUM 3
// 容器の容量
int pod_capa[POD_NUM] = {3, 5, 8};
// この値は全ての容器の容量より常に大きくする
int base = 10;

// 容器の初期状態
int pod_begin[POD_NUM] = {0, 0, 8};
// 求める量
int result = 4;

// もしdata==0ならdatasの中身をdataに入れ、それ以外ではdatasにdataの値を移す
void exchangeData(int* data, int datas[POD_NUM]);
// ノードを追加する　もし重複すればNULLを返す
node* addNode(node* head, int newData, node* label);
// ラベルをたどって結果を出力する
void printResult(node* n);


int main() {
	// 初期状態の値からノードを生成する
	int begin = 0;
	exchangeData(&begin, pod_begin);
	node start = {begin, NULL, NULL};
	node* head = &start;
	
	int pod[POD_NUM];
	int tmpod[POD_NUM];
	// nの状態から操作を一回行った結果を、重複しないようにheadの後尾に加える動作を繰り返す
	for (node* n = head; n != NULL; n = n->next) {
		// n->dataをpodに移す
		exchangeData(&n->data, pod);
		
		// pod[i]の中身をpod[j]に移す操作を行い、その結果をheadの後尾に加える
		for (int i = 0; i < POD_NUM; i++) {
			if (pod[i] == 0)
				continue;
			for (int j = 0; j < POD_NUM; j++) {
				if (i == j)
					continue;
				for (int k = 0; k < POD_NUM; k++)
					tmpod[k] = pod[k];
				
				tmpod[j] += tmpod[i];
				tmpod[i] = (tmpod[j] > pod_capa[j])? tmpod[j] - pod_capa[j] : 0;
				tmpod[j] -= tmpod[i];
				
				int newData = 0;
				exchangeData(&newData, tmpod);
				node* m = addNode(head, newData, n);
				// もし求める結果がでたなら出力し、終了
				if (tmpod[i] == result || tmpod[j] == result) {
					printResult(m);
					return 0;
				}
			}
		}
	}
	printf("No Answer\n");
	return 0;
}

void exchangeData(int* data, int datas[POD_NUM]) {
	int i;
	int tmp = *data;
	if (tmp == 0) {
		for (i = 0; i < POD_NUM; i++)
			tmp = tmp * base + datas[i];
		*data = tmp;
	}
	else {
		for (i = POD_NUM - 1; i >= 0; i--) {
			datas[i] = tmp % base;
			tmp /= base;
		}
	}
}

node* addNode(node* head, int newData, node* label) {
	node *n, *m;
	for (n = head; n != NULL; n = n->next) {
		if (n->data == newData)
			return NULL;
		m = n;
	}
	
	node* newNode = (node*)malloc(sizeof(node));
	if (newNode == NULL) { fprintf(stderr, "Over Memory\n"); exit(1); }
	newNode->data = newData;
	newNode->label = label;
	newNode->next = NULL;
	
	m->next = newNode;
	return newNode;
}

void printPod(int data) {
	int i;
	int datas[POD_NUM];
	exchangeData(&data, datas);
	
	printf("%d", datas[0]);
	for (i = 1; i < POD_NUM; i++)
		printf("_%d", datas[i]);
}

void printResult(node* n) {
	if (n->label == NULL)
		printPod(n->data);
	else {
		printResult(n->label);
		printf("->");
		printPod(n->data);
	}
}
