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
int pod_num;
// この値は全ての容器の容量より常に大きくする
int base = 10;


// もしdata==0ならdatasの中身をdataに変換し、それ以外ではdatasにdataの値を変換する
void exchangeData(int* data, int* datas);
// ノードを追加する　もし重複すればNULLを返す
node* addNode(node* head, int newData, node* label);
// ラベルをたどって結果を出力する
void printResult(node* n);


int main() {
	int i, j, k;
	
	// 容器の数を入力する
	printf("number of pods:");
	scanf("%d", &pod_num);
	if (pod_num <= 0) {
		printf("This number is not correct\n");
		return 1;
	}
	
	printf("type as \"XXX:1 2 3\"\n");
	
	// 容器の容量を入力する
	int pod_capa[pod_num];
	printf("capacity of pods:");
	for (i = 0; i < pod_num; i++) {
		scanf("%d", &pod_capa[i]);
		if (pod_capa[i] <= 0) {
			printf("This number is not correct\n");
			return 1;
		}
		else if (pod_capa[i] >= base)
			base = pod_capa[i] + 1;
	}
	
	// 容器の初期状態を入力する
	int pod_init[pod_num];
	printf("initial quantity of pods:");
	for (i = 0; i < pod_num; i++) {
		scanf("%d", &pod_init[i]);
		if (pod_init[i] < 0 || pod_capa[i] < pod_init[i]) {
			printf("This number is not correct\n");
			return 1;
		}
	}
	
	// 求める量を入力する
	int ans_quant;
	printf("quantity you want to get:");
	scanf("%d", &ans_quant);
	if (ans_quant < 0) {
		printf("This number is not correct\n");
		return 1;
	}
	
	// 初期状態の値からノードを生成する
	int begin = 0;
	exchangeData(&begin, pod_init);
	node start = {begin, NULL, NULL};
	node* head = &start;
	
	int pod[pod_num];
	int tmpod[pod_num];
	// nの状態から操作を一回行った結果を、重複しないようにheadの後尾に加える動作を繰り返す
	for (node* n = head; n != NULL; n = n->next) {
		// n->dataをpodに移す
		exchangeData(&n->data, pod);
		
		// pod[i]の中身をpod[j]に移す操作を行い、その結果をheadの後尾に加える
		for (i = 0; i < pod_num; i++) {
			if (pod[i] == 0)
				continue;
			for (j = 0; j < pod_num; j++) {
				if (i == j)
					continue;
				for (k = 0; k < pod_num; k++)
					tmpod[k] = pod[k];
				
				tmpod[j] += tmpod[i];
				tmpod[i] = (tmpod[j] > pod_capa[j])? tmpod[j] - pod_capa[j] : 0;
				tmpod[j] -= tmpod[i];
				
				int newData = 0;
				exchangeData(&newData, tmpod);
				node* m = addNode(head, newData, n);
				// もし求める結果がでたなら出力し、終了
				if (tmpod[i] == ans_quant || tmpod[j] == ans_quant) {
					printResult(m);
					return 0;
				}
			}
		}
	}
	printf("No Answer\n");
	return 0;
}

void exchangeData(int* data, int* datas) {
	int i;
	int tmp = *data;
	if (tmp == 0) {
		for (i = 0; i < pod_num; i++)
			tmp = tmp * base + datas[i];
		*data = tmp;
	}
	else {
		for (i = pod_num - 1; i >= 0; i--) {
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
	int datas[pod_num];
	exchangeData(&data, datas);
	
	printf("%d", datas[0]);
	for (i = 1; i < pod_num; i++)
		printf("_%d", datas[i]);
}

void printResult(node* n) {
	if (n->label == NULL)
		printPod(n->data);
	else {
		printResult(n->label);
		printf("\n->");
		printPod(n->data);
	}
}
