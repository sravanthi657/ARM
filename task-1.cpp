#include<bits/stdc++.h>
using namespace std;
struct FP_Node
{
	FP_Node* parent;
	int val;
	int cnt;
	bool visited;
	map<int,FP_Node*>children;
	// FP_Node(int val ,FP_Node* p): val(val),parent(p),cnt(0),visited(false) {}
};
FP_Node* TreeNode(FP_Node* p)
{
	FP_Node* newN = new FP_Node;
	newN->val =-1;newN->cnt=0;
	newN->parent = p;newN->visited = false;
	return newN;
}
void generate_FP_Tree(FP_Node *root,set<int>trans,int freq,vector<pair<int,int> >one_items)
{
	FP_Node* temp = root;
	for(int i=0;i<one_items.size();i++)
	{
		cur_trans= one_items[i].second;
		if(trans.find(cur_trans)!=trans.end())
		{
			
		}
	}

}
map<int,int> fun_one_item(vector<pair<set<int>,int> >datum,int mins,vector<pair<int,int> >&one_items)
{
	map<int,int>freq;
	for(int i=0;i<datum.size();i++)
	{

		int ini_cnt= datum[i].second;
		// cout<<"transaction "<<i<<" sz "<<datum[i].first.size()<<endl;
		for(auto it=datum[i].first.begin();it!=datum[i].first.end();it++)
		{
			// cout<<"here\n it"<<*it<<" freq sz "<<freq.size()<<endl ;
			if(freq.find(*it)!=freq.end())
				freq[*it]=freq[*it]+ini_cnt;
			else
				freq[*it]=1;
		}
	}
	// cout<<"freq "<<freq.size()<<endl;
	for(auto it=freq.begin();it!=freq.end();it++)
	{
		int sup_cnt = it->second;
		if(sup_cnt>=mins)
		{
			// cout<<"adding into one_items\n";
			one_items.push_back({sup_cnt,it->first});
		}
	}
	// cout<<" one_items "<<one_items.size()<<endl;
	if(one_items.size())
	{
		sort(one_items.rbegin(),one_items.rend());
	}
	map<int,int>store_item_order;
	for(int i=0;i<one_items.size();i++)
		store_item_order[one_items[i].second]=i;
	return store_item_order;
}

string preprocess_record(string tupl,string item,int &max_1_item,set<int>&record,int &i)
{
	char c = tupl[i];
	switch(c)
	{
		case ' ':
		{
			stringstream each_item(item);
			int item_cnt=0; each_item>>item_cnt;
			max_1_item = max(max_1_item,item_cnt);
			item = "";
			record.insert(item_cnt);
			break;
		}
		case '-':
			i=i+2;
			break;
		default:
			item +=c;
			break;
	}
	i++;
	return item;

}

int main()
{
	int mins;string inp,tupl; 
	cout<<"Enter the Minimum Support Value :  ";
	cin>>mins;
	cout<<"Enter the name of Transactional Dataset File : ";
	cin>>inp;
	ifstream file_inp(inp);
	if(!file_inp)
	{
		cout<<"File not found \n";
		return -1;
	}
	// Preprocessing the Dataset
	set<int>record;
	string item="";
	int max_1_item = INT_MIN;
	vector<pair<set<int>,int> > datum;
	while(getline(file_inp,tupl))
	{
		//processing each item of the tuple
		for(int i=0;i<tupl.size();)
		{
			item= preprocess_record(tupl,item,max_1_item,record,i);
		}
		datum.push_back({record,1});
		record.clear();
	}
	file_inp.close();

	//starter2
	vector<pair<int,int> >one_items;
	map<int,int>store_item_order=fun_one_item(datum,mins,one_items);
	if(one_items.empty())
	{
		cout<<"No items to mine\n";
		return 0;
	}
	FP_Node* root = TreeNode(nullptr);
	for(int i=0;i<datum.size();i++)
	{
		generate_FP_Tree(root,datum[i].first,datum[i].second,one_items);
	}

}