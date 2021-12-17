#include<bits/stdc++.h>
  
using namespace std;



vector<vector<int> >frequent_sets;

struct FP_Node {    

    int count;
    FP_Node* parent;
    int val;
    bool visited;
    map<int,FP_Node*>children;

};


FP_Node* TreeNode(FP_Node* p)
{
    FP_Node* newN = new FP_Node;
    newN->val =-1;newN->count=0;
    newN->parent = p;newN->visited = false;
    return newN;
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

void generate_FP_Tree(FP_Node *root,set<int>trans,int freq,vector<pair<int,int> >order, vector<set<FP_Node*> >&tb,map<int,int>items_order )
{
    FP_Node* crawler = root;
    for(int i=0;i<order.size();i++)
    {
        if(trans.find(order[i].second) != trans.end())
            {

                if(crawler->children.find(order[i].second) == crawler->children.end())
                {
                    FP_Node *temp = TreeNode(crawler);
                    temp->val = order[i].second;
                    crawler->children[order[i].second] = temp;
                    tb[items_order[(order[i].second)]].insert(temp);
                }

                crawler->children[order[i].second]->count += freq;
                crawler = crawler->children[order[i].second];
            }
    }

}
bool traverse_tree(FP_Node* root)
{
    FP_Node* t =  root;
    while(t != nullptr)
    {
        
        if(t->children.size() == 0) return true;
        else if(t->children.size() > 1) return false;
        map<int, FP_Node*>::iterator itr; itr = t->children.begin();
        t = itr->second;             
    }
    return true;

}
void starter2(vector<pair<set<int>, int> >datum, int mins, vector<int>alpha)
{   

    map<int,int>freq; //to store the frequency of items (singletons)

    set<int>::iterator transaction_itr;

    for(int i = 0; i < datum.size(); i++)
    {
        set<int>transaction = datum[i].first;
        int ini_cnt= datum[i].second;
        for(transaction_itr = transaction.begin(); transaction_itr != transaction.end(); transaction_itr++)
        {
            if(freq.find(*transaction_itr) != freq.end())
                freq[*transaction_itr] += ini_cnt;
            else
            {
                freq[*transaction_itr] =0;
                freq[*transaction_itr]+=  ini_cnt;
            }
        }
    }

    map<int,int>::iterator freq_itr; //

    vector<pair<int,int> >order; 

    for(freq_itr = freq.begin(); freq_itr != freq.end(); freq_itr++)
    {
        int cntt = freq_itr->second ; 
        if(cntt>= mins)
            order.push_back(make_pair(cntt, freq_itr->first));
    }


    if(order.size() == 0)
        return;

   
    sort(order.rbegin(), order.rend()); // singletons (descending order)

    // for(int i=0;i<order.size();i++)
    //         cout<<order[i].first<<" "<<order[i].second<<endl;

    map<int,int>items_order; //descending order

    for(int i = 0; i < order.size(); i++)
    {
       items_order[order[i].second] = i;
    }

    vector<set<FP_Node*> >tb(order.size());

    FP_Node* root = TreeNode(nullptr); 

    FP_Node* crawler =  root;

    set<int>::iterator itr;
    for(int t_id = 0; t_id < datum.size(); t_id++)
    {
        generate_FP_Tree(root,datum[t_id].first,datum[t_id].second,order,tb,items_order);
        
    }


    if(root->children.size() == 0)
        return;

    bool is_single_branch = traverse_tree(root);


    if(is_single_branch) //is singe branch
    {
        vector<int> s_path;
        vector<int>itemset;
            for (auto & it : order) {
                s_path.push_back(it.second);
                }

        int num = pow(2,s_path.size());
        for(int i = 1; i < num; i++)
        {
            string subsets = "";
            int n=i;
            while(n > 0)
            {
                if(!(n%2))
                    subsets += "0";
                else
                    subsets += "1";
                
                n = n>>2;
            }

            itemset=alpha;int k=0;
            for(auto j:subsets)
            {
                if(j == '1')
                    itemset.push_back(s_path[k]);
                i++;
            }
            frequent_sets.push_back(itemset);
            itemset.clear();
        }
        return;
    }
    else
    {
        vector<vector<pair<set<int> , int> > >  dataset_arr(tb.size());
        for(int i = tb.size()-1; i >= 0; i--)
        {
            set<FP_Node*>::iterator it;
            for(it=tb[i].begin();it!=tb[i].end();it++)
            {
                if((*it)->visited==true)
                {
                    continue;
                }
                
                vector<int>path,fr;
                vector< FP_Node* > address;
                FP_Node* crawler = *it;

                while(crawler->parent->val != -1)
                {
                    path.push_back(crawler->parent->val);fr.push_back(crawler->parent->count);
                    address.push_back(crawler->parent);
                    crawler = crawler->parent;
                }
                set<int>transaction;
                if(path.size()>0) 
                {
                    transaction.insert(path.back());
                    
                }
                if(path.size()>0)
                {
                    address.back()->visited=true;
                }
                for(int k=path.size()-2;k>=0;k--)
                {
                    bool marked = address[k]->visited;
                    if(marked)transaction.insert(path[k]);
                    if(!marked)
                    {
                        dataset_arr[items_order[path[k]]].push_back({transaction,fr[k]});
                        transaction.insert(path[k]);
                        address[k]->visited=true;
                    }
                    
                }
                FP_Node* cur_trans=*it;
                if(transaction.size() > 0)
                {
                    if((cur_trans)->visited==false)
                        dataset_arr[i].push_back({transaction, cur_trans->count});
                }
                (*it)->visited=true;
            }
            vector<int>beta=alpha;
            beta.push_back(order[i].second);
            frequent_sets.push_back(beta);
            starter2(dataset_arr[i],mins,beta); //RECURSION
        }
    }
}

int main(void)
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

    vector<int>alpha,counts;
    vector<pair<int, vector<int> > >beautify_sets;int Size,cs;

    starter2(datum, mins, alpha);
    for(int i = 0; i < frequent_sets.size(); i++)
    {
        beautify_sets.push_back(make_pair(frequent_sets[i].size(), frequent_sets[i]));
    }

    sort(beautify_sets.begin(), beautify_sets.end());
    for(int i = 0; i < beautify_sets.size();)
    {
        Size=beautify_sets[i].first;
        cs=0;
        while(i<beautify_sets.size() && beautify_sets[i].first==Size)
        {
            cs++;
            i++;
        }
        counts.push_back(cs);
    }
    reverse(counts.begin(),counts.end());
    for(int i = 0; i < beautify_sets.size();)
    {
        Size=beautify_sets[i].first;
        cout<<"Frequent Itemsets for k = "<<Size<<endl;
        cout<<"Number of Itemsets formed are : "<<counts.back()<<endl<<endl<<"[";
        counts.pop_back();
        while(i<beautify_sets.size() && beautify_sets[i].first==Size)
        {
            cout<<"[";
            for(int j = 0; j < beautify_sets[i].second.size(); j++)
            {
                cout << beautify_sets[i].second[j] ;
                if(j!=beautify_sets[i].second.size()-1)
                    cout<<",";
            }
            cout << "]";
            i++;
            if(i<beautify_sets.size() && beautify_sets[i].first==Size)
            cout<<",";
        }
        cout<<"]"<<endl<<endl;
    }
        cout << "Total number of Itemsets : " << beautify_sets.size() << endl << endl << endl;

    return 0;
}

