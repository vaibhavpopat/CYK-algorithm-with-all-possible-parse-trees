#include<bits/stdc++.h>
using namespace std;
struct Node						//recursive structure to store all the trees
{
	char c,c1;
	Node *left,*right;
};
map<int,vector<vector<Node*>>> tree;					//map used to store pointers while making and then printing a tree 
vector<Node*> temp1;									//global variable added in map "tree"
ofstream fout;
string combi(string s1,string s2,map<char,set<string>> m,vector<Node*> v1,vector<Node*> v2)
{
	int i,j,l1=s1.length(),l2=s2.length(),h=0;			//the string s1 contains only 1 character.
	string str,ans="";
	if(s2[0]=='0')										//this if condition works only while filling the bottom-most level
	{	
		for(auto it=m.begin();it!=m.end();it++)			//the for loop is traversed to check all the grammar rules where this character is contained.  
		{
			string temp(1,s1[i]);
			if((it->second).find(temp)!=(it->second).end())
			{
				ans+=it->first;	
			}	
		}
	}
	else
	{
		for(i=0;i<l1;i++)								//the for loops take all combinations from two strings and finds it in grammar. If possible, adds it to ans. 
		{
			for(j=0;j<l2;j++)
			{
				str="";
				str+=s1[i];
				str+=+s2[j];
				for(auto it=m.begin();it!=m.end();it++)
				{
					if((it->second).find(str)!=(it->second).end())			//this makes the temp1(a vector) which by creating nodes to left and right pointers 
					{
						ans+=it->first;
						Node *n1=(Node*)malloc(sizeof(Node));
						n1->c=it->first;
						n1->left=v1[i];
						n1->right=v2[j];
						temp1.push_back(n1);								//temp1 added in tree in below function(its global variable)
					}	
				}                      
			}
		}	
	}
	return ans;
}
void traverse(Node* n1)										//this function uses BFS to traverse the complete tree (left and right pointers recursively)  
{
	queue<Node*> q;
	q.push(n1);
	Node* n2;
	int c,f=1;
	c=q.size();
	while(c!=0)
	{
		while(c--)
		{
			n2=q.front();
			fout<<n2->c<<"->";
			if(n2->left==NULL && n2->right==NULL)
			fout<<n2->c1<<" ";
			else
			{
				fout<<n2->left->c<<n2->right->c<<" ";
				q.push(n2->left);
				q.push(n2->right);
			}
			q.pop();
		}
		c=q.size();
		fout<<endl;
	}
}
void tcom()
{
	int n,i,j,l,start,x,end,t,level=0,k,z,l2;
	string str,temp,s;
	map<char,set<string>> m;								//maps grammar rules with one non-terminal symbol as left side in map
	map<int,vector<string>> cyk;							//map where int represents each level and vector of strings represent possibilities where this map stores cyk algo. levels  
	ifstream fin;
	fin.open("input.txt");									//input file
	fout.open("output.txt");								//writing all possible trees in output file
	fin>>n;													//taking number of grammar rules as input
	set<string> st;									
	vector<string> v;										
	vector<Node*> v1;								
	vector<vector<Node*>> v2;
	for(i=0;i<n;i++)										//traversing grammar rules 
	{
		fin>>str;
		j=3;
		l=str.length();
		st.clear();
		m.insert({str[0],st});								//takes the input start character on the left side of the map.
		while(j<l)											//string parsing to get the various subsequents from each character.
		{
			start=j;
			while(j<l && str[j]!='|')
			{
				j++;
			}
			end=j-1;
			temp=str.substr(start,end-start+1);				//creating individual substring to move on from starting charecter. 
			m[str[0]].insert(temp);							//inserting it in map.
			j++;
		}
	}
	fin>>t;													//inputting the number of test cases, i.e. the number of strings for which CYK algo. need to be implemented.
	while(t--)
	{
		fin>>s;
		cout<<"String - ";
		cout<<s;
		level=0;
		l=s.length();
		v.clear();
		v2.clear();
		cyk.clear();
		tree.clear();
		cyk.insert({level,v});								//inserting dummy vector in cyk tree
		tree.insert({level,v2});							//inserting dummy vector in 'tree' tree
		string s4,s3="0";
		for(i=0;i<l;i++)									//this for loop fills the 0th level, i.e. the bottom-most level  
		{
			s4="";
			s4+=s[i];
			temp=combi(s4,s3,m,v1,v1);						//temp string contains all the non-terminal symbols from which s[i] can be produced. 
			l2=temp.length();								
			v1.clear();
			for(z=0;z<l2;z++)								//this for loop is used to store vector of nodes in tree "tree" which is used for printing all the trees.
			{
				Node* n1=(Node*)malloc(sizeof(Node));
				n1->c=temp[z];
				n1->left=n1->right=NULL;					//left and right pointers NULL(bottom-most level)
				n1->c1=s[i];
				v1.push_back(n1);
			}
			cyk[level].push_back(temp);						//this string is inserted in cyk map.
			tree[level].push_back(v1);						//inserting v1 in tree "tree"
		}
		level++;
		while(level<l)										//Creating levels starting from 1  
		{
		
			v.clear();
			cyk.insert({level,v});							//inserting dummy vector in cyk tree
			v2.clear();
			tree.insert({level,v2});						//inserting dummy vector in 'tree' tree
			for(i=0;i<l-level;i++)							//creating all the combination of strings of below levels for filling the current level. 
			{
				k=0;
				j=i+1;
				x=level;
				temp="";
				temp1.clear();
				while(x--)									//traversing number of combinations for a level
				{
					temp+=combi(cyk[k][i],cyk[level-k-1][j],m,tree[k][i],tree[level-k-1][j]);			//temp contains the complete set of symbols from which the combinations of 2 strings can be obtained. 
					j++;
					k++;
				}
				cyk[level].push_back(temp);					//inserting the obtained string in cyk tree
				tree[level].push_back(temp1);				//modified temp1 variable(in combi function) pushed in tree "tree" 
			}
			level++;
		}
		cout<<endl;
		for(auto it=cyk.begin();it!=cyk.end();it++)			//printing the complete cyk tree
		{
			cout<<it->first<<" ";
			for(i=0;i<(it->second).size();i++)
			{
				if((it->second)[i]!="")
				cout<<(it->second)[i]<<" ";
				else
				cout<<"-"<<" ";
			}
			cout<<endl;
		}
		level--;
		int qwe=0;
		fout<<"String - "<<s<<endl;							//Writing all trees in output file
		for(i=0;i<tree[level].size();i++)
		{
			for(j=0;j<tree[level][i].size();j++)
			{
				if(tree[level][i][j]->c=='S')				//seeing all the trees starting from 'S'
				{
					qwe++;
					traverse(tree[level][i][j]);			
					fout<<endl;
				}
			}
		}
		cout<<"Total unique trees: "<<qwe<<endl;
		if(qwe==0){cout<<"Hence, string is not accepted."<<endl;}
		cout<<endl;
		fout<<"Total unique trees: "<<qwe<<endl;
		if(qwe==0){fout<<"Hence, string is not accepted."<<endl;}
		fout<<endl<<endl;
	}
	fout.close();
}
int main()
{
	tcom();
	return 0;	
}
