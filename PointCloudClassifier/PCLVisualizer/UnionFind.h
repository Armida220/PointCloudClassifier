#pragma once



class UnionFind{

private:
	// rank[i]��ʾ��iΪ���ļ�������ʾ�����Ĳ���
	// �ں����Ĵ�����, ���ǲ�����ά��rank������, Ҳ����rank��ֵ��·��ѹ���Ĺ�����, �п��ܲ��������Ĳ���ֵ
	// ��Ҳ�����ǵ�rank����height����depth��ԭ��, ��ֻ����Ϊ�Ƚϵ�һ����׼
	// ����������⣬���Բο��ʴ�����http://coding.imooc.com/learn/questiondetail/7287.html
	int* rank;
	int* parent; // parent[i]��ʾ��i��Ԫ����ָ��ĸ��ڵ�
	int count;   // ���ݸ���
	int* isolated;  //�Ƿ��Ǹ�

public:

	UnionFind(int count);
	~UnionFind();

	// ���ҹ���, ����Ԫ��p����Ӧ�ļ��ϱ��
	int find(int p);

	// �鿴Ԫ��p��Ԫ��q�Ƿ�����һ������
	bool isConnected(int p, int q);

	// �ϲ�Ԫ��p��Ԫ��q�����ļ���
	void unionElements(int p, int q);

	//�ж��Ƿ����ҵ���
	bool isRoot(int p);

};


