#include<bits/stdc++.h>
using namespace std;

vector<vector<int> >frequent_sets;
void FP_algo(vector<pair<set<int>,int> > datum, vector<int>symb,int mins);
struct FP_Node
{
	FP_Node* parent;
	int val;
	int cnt;
	bool visited;
	map<int,FP_Node*>children;
};

FP_Node* TreeNode(FP_Node* p)
{
	FP_Node* newN = new FP_Node;
	newN->val =-1;newN->cnt=0;
	newN->parent = p;newN->visited = false;
	return newN;
}
// FP_Node* updateNode(FP_Node * t,int cur_trans,int freq)
// {
// 	cout<<"update\n";
// // 	t->children[cur_trans]->cnt += freq;
// 	cout<<"why\n";
// 	return t->children[cur_trans];
// }
void generate_FP_Tree(FP_Node *root,set<int>trans,int freq,vector<pair<int,int> >one_items, vector<set<FP_Node*> >&tb,map<int,int>items_order )
{
	FP_Node* crawler = root;
	for(int i=0;i<one_items.size();i++)
	{
		if(trans.find(one_items[i].second) != trans.end())
            {

                if(crawler->children.find(one_items[i].second) == crawler->children.end())
                {
                    FP_Node *temp = TreeNode(crawler);
                    temp->val = one_items[i].second;
                    crawler->children[one_items[i].second] = temp;
                    tb[items_order[(one_items[i].second)]].insert(temp);
                }

                crawler->children[one_items[i].second]->cnt += freq;
                crawler = crawler->children[one_items[i].second];
            }
	}

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
FP_Node* helper(vector<int>&path,vector<int>&fr,vector<FP_Node*>&address,FP_Node* n)
{
	int val = n->parent->val;
	path.push_back(val);
	int cnt = n->parent->cnt;
	fr.push_back(cnt);
	FP_Node* p;
	address.push_back(p);
	return n->parent;
}
bool marker(vector<FP_Node*>address,int i)
{
	return address[i]->visited;
}
void traverse_table(set<FP_Node*>&trans,vector<set<FP_Node*> >tb,map<int,int>items_order,int indx,vector<vector<pair<set<int> , int> > >&dataset)
{
	for(auto it=trans.begin();it!=trans.end();it++)
	{
		FP_Node* n=*it;
		if((n->visited)==true)continue;
		vector<int>path,fr;
		vector<FP_Node*>address;
		while(n->parent->val!=-1)
			n = helper(path,fr,address,n);
		set<int>set_trans;
		if(path.size()>0)set_trans.insert(path.back());
		if(path.size()>0)address.back()->visited=true;
		for(int i=path.size()-2;i>=0;i--)
		{
			int pos= items_order[i];
			bool is_address= marker(address,i);
			if(is_address)
			{
				dataset[pos].push_back({set_trans,fr[i]});
				address[i]->visited = true;
			}
			set_trans.insert(path[i]);
		}
		if(set_trans.size()>0)
		{
			if(n->visited == false)
			{
				dataset[indx].push_back({set_trans,n->cnt});
			}
		}

		n->visited=true;
	}
	return;
}

bool traverse_tree(FP_Node* root,vector<set<FP_Node*> >&tb,map<int,int>items_order,vector<int>symb,vector<pair<int,int> >one_items,int mins)
{
	FP_Node* temp = root;
	vector<vector<pair<set<int> , int> > > dataset(tb.size());	
	bool single_branch =true;
	while(temp!=nullptr)
	{
		if(temp->children.size()==0)break;
		else if(temp->children.size() >1)
		{
			single_branch = false;
			for(int i=tb.size()-1;i>=0;i--)
			{
				traverse_table(tb[i],tb, items_order,i,dataset);
				vector<int>syma = symb;
				syma.push_back(one_items[i].second);
				frequent_sets.push_back(syma);
				FP_algo(dataset[i],syma,mins);
			}
			break;
		}
		else
		{
			map<int, FP_Node*>::iterator itr; 

            itr = temp->children.begin();
            temp = itr->second;   
		}
	}
	return single_branch;
}
void FP_algo(vector<pair<set<int>,int> > datum, vector<int>symb,int mins)
	{
		vector<pair<int,int> >one_items;
		map<int,int>freq;
		for(int i=0;i<datum.size();i++)
		{

			int ini_cnt= datum[i].second;
			set<int>::iterator sit;
			// cout<<"transaction "<<i<<" sz "<<datum[i].first.size()<<endl;
			for(sit=datum[i].first.begin();sit!=datum[i].first.end();sit++)
			{
				// cout<<"here\n it"<<*it<<" freq sz "<<freq.size()<<endl ;
				if(freq.find(*sit)!=freq.end())
					freq[*sit]=freq[*sit]+ini_cnt;
				else
				{
					freq[*sit]=0;
					freq[*sit]+=ini_cnt;
				}
			}
		}
		// cout<<"freq "<<freq.size()<<endl; 
		map<int,int>::iterator it ;
		for(it=freq.begin();it!=freq.end();it++)
		{
			int sup_cnt = it->second;
			if(sup_cnt>=mins)
			{
				// cout<<"adding into one_items\n";
				one_items.push_back({sup_cnt,it->first});
			}
		}
		// cout<<" one_items "<<one_items.size()<<endl;
		if(one_items.size()==0)
		{
			return ;
		}

		sort(one_items.rbegin(),one_items.rend());
		for(int i=0;i<one_items.size();i++)
            cout<<one_items[i].first<<" "<<one_items[i].second<<endl;
		map<int,int>items_order;
		for(int i=0;i<one_items.size();i++)
			items_order[one_items[i].second]=i;

		
		FP_Node* root = TreeNode(nullptr);
	    vector<set<FP_Node*> >tb(one_items.size());  
		for(int i=0;i<datum.size();i++)
		{
			// cout<<"i "<<i<<endl;
			generate_FP_Tree(root,datum[i].first,datum[i].second,one_items,tb,items_order);
		}
	    if(root->children.size()==0)return;
	    bool single_branch= traverse_tree(root,tb,items_order,symb,one_items,mins);
	    if(single_branch ==true)
	    {

			vector<int> s_path;
			for (auto & it : one_items) {
			    s_path.push_back(it.second);
				}
			vector<int>itemsets;
			for(int i=1;i<(int)pow(2,s_path.size());i++)
			{
				string subsets = "";
				int n =i;
				itemsets=symb;
			    while(n > 0)
			    {
			        int t = n%2;
			        if(t == 0)
			            subsets += "0";
			        else
			            subsets += "1";
			        
			        n = n>>2;
			    }
			    for(int i=0;i<subsets.size();i++)
			    {
			    	if(subsets[i]=='1')
			    		itemsets.push_back(s_path[i]);
			    }
			    frequent_sets.push_back(itemsets);
			    itemsets.clear();
			}
			return;
	    }

	}
int main()
{
	int mins;string inp,tupl; 
	// cout<<"Enter the Minimum Support Value :  ";
	cin>>mins;
	// cout<<"Enter the name of Transactional Dataset File : ";
	cin>>inp;
	ifstream file_inp(inp);
	if(!file_inp)
	{
		// cout<<"File not found \n";
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
		item = "";
		datum.push_back({record,1});
		record.clear();
	}
	file_inp.close();
	vector<int>symb;

	FP_algo(datum, symb,mins);
	cout<<"here"<<endl;
	vector<pair<int, vector<int> > >beautify_sets;
	// cout<<" FP algprithm is executed successfully\n";


	// int Size,cs;
 //    vector<int> counts;
	// for(int i = 0; i < frequent_sets.size(); i++)
 //    {
 //        beautify_sets.push_back({frequent_sets[i].size(), frequent_sets[i]});
 //    }

 //    sort(beautify_sets.begin(), beautify_sets.end());
 //    for(int i = 0; i < beautify_sets.size();)
 //    {
 //        Size=beautify_sets[i].first;
 //        cs=0;
 //        while(i<beautify_sets.size() && beautify_sets[i].first==Size)
 //        {
 //            cs++;
 //            i++;
 //        }
 //        counts.push_back(cs);
 //    }
 //    reverse(counts.begin(),counts.end());
    // for(int i = 0; i < beautify_sets.size();)
    // {
    //     Size=beautify_sets[i].first;
    //     // cout<<"Frequent Itemsets for k = "<<Size<<endl;
    //     // cout<<"Number of Itemsets formed are : "<<counts.back()<<endl<<endl<<"[";
    //     counts.pop_back();
    //     while(i<beautify_sets.size() && beautify_sets[i].first==Size)
    //     {
    //         // cout<<"[";
    //         for(int j = 0; j < beautify_sets[i].second.size(); j++)
    //         {
    //             // cout << beautify_sets[i].second[j] ;
    //             // if(j!=beautify_sets[i].second.size()-1)
    //                 // cout<<",";
    //         }
    //         // cout << "]";
    //         i++;
    //         // if(i<beautify_sets.size() && beautify_sets[i].first==Size)
    //         // cout<<",";
    //     }
    //     // cout<<"]"<<endl<<endl;
    // }
}
