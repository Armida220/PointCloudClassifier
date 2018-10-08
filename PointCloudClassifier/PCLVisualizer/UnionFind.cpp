#include "UnionFind.h"

#include <cassert>

// ���캯��
UnionFind::UnionFind(int count){
	parent = new int[count];
	rank = new int[count];
	isolated = new int[count];
	this->count = count;
	for (int i = 0; i < count; i++){
		parent[i] = i;
		rank[i] = 1;
		isolated[i] = 1;  //1 �����ǹ����� �� 0 �����Ѿ��ҵ���֯
	}
}

// ��������
UnionFind::~UnionFind(){
	delete[] parent;
	delete[] rank;
	delete[] isolated;
}

// ���ҹ���, ����Ԫ��p����Ӧ�ļ��ϱ��
// O(h)���Ӷ�, hΪ���ĸ߶�
int UnionFind::find(int p){
	assert(p >= 0 && p < count);
	while (p != parent[p]){
		parent[p] = parent[parent[p]];
		p = parent[p];
	}
	return p;
}

// �鿴Ԫ��p��Ԫ��q�Ƿ�����һ������
// O(h)���Ӷ�, hΪ���ĸ߶�
bool UnionFind::isConnected(int p, int q){
	return find(p) == find(q);
}

// �ϲ�Ԫ��p��Ԫ��q�����ļ���
// O(h)���Ӷ�, hΪ���ĸ߶�
void UnionFind::unionElements(int p, int q){

	int pRoot = find(p);
	int qRoot = find(q);

	if (pRoot == qRoot)
		return;

	// ��������Ԫ����������Ԫ�ظ�����ͬ�жϺϲ�����
	// ��Ԫ�ظ����ٵļ��Ϻϲ���Ԫ�ظ�����ļ�����
	if (rank[pRoot] < rank[qRoot]){
		parent[pRoot] = qRoot;
		isolated[pRoot] = 0;
	}
	else if (rank[qRoot] < rank[pRoot]){
		parent[qRoot] = pRoot;
		isolated[qRoot] = 0;
	}
	else{ // rank[pRoot] == rank[qRoot]
		parent[qRoot] = pRoot;  ///������Ҷ���Ϊ��������
		isolated[qRoot] = 0;
		rank[qRoot] += 1;   // ��ʱ, ��ά��rank��ֵ
	}

}

bool UnionFind::isRoot(int p){
	if (isolated[p] == 1) //���������ѭ����Ļ�
		return true;
	return false;
}

