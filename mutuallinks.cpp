#include<bits/stdc++.h>
#define node_number 1483277 
#define edge_number 52973671 

using namespace std;

//ページファイルの読み込み
void readpages(vector<string> &v){

	ifstream ifs("pages.txt");

	int n;
	string str;

	while(ifs >> n >> str){
		v.push_back(str);
	}

}

//リンクファイルの読み込み 一列の配列でもって各nodeのstart,endを記録しておく
void readlinks(pair<int,int> *v,long int *start,long int *end){

	ifstream ifs("links.txt");

	int n;
	int m;
	long int i=0;

	while(ifs>>n>>m){
		v[i].first=n;
		v[i].second=m;
		//firstが変わるときstart,endを記録
		if(i!=0 && v[i].first-v[i-1].first!=0){
			start[v[i].first]=i;
			end[v[i-1].first]=i-1;
		}
		i++;
	}
	sort(v,v+edge_number-1);
}

//pairのsecond（エッジの先）でi-jの範囲でkを二分探索
int binarysearch(pair<int,int> *v,int i,int j,int k){

	int low=i; int high=j; int ret=0;

	while(low<=high&&low>=i&&high<=j){
		int middle=(high+low)/2;
		if(k==v[middle].second){
			ret=1;
			break;
		}
		else if(k>v[middle].second)low=middle+1;
		else high=middle-1;
	}
	return ret;
}

//相互リンクの数え上げ
long int mutuallink(pair<int,int> *edge,long int *start,long int *end,int *indegree,long int *count,int n,vector<string> node,pair<int,int>*terminal){
	//出力ファイル
	ofstream ofs1("mutuallink.txt");
	ofstream ofs2("mutualrate.txt");

	//相互リンクの合計カウント
	long int totalcount=0;

	for(int i=0;i<start[n-1];i++){
		int h=edge[i].first; //エッジのもと
		int j=edge[i].second; //エッジの先
		
		indegree[j]++;

		if(h<j){
			//エッジの先の範囲：k〜l
			long int k=start[j];
			long int l=end[j];
			if(k==0&&l==0)count+=0;
			else {
				//注目しているエッジの反対向きがあるか探索
				int m=binarysearch(edge,k,l,h);

				//あったとき
				if(m!=0){
					ofs1<<h<<" "<<j<<" "<<node[h]<<" "<<node[j]<<endl;
					count[h]++; count[j]++;
				}

				totalcount+=binarysearch(edge,k,l,h);
			}
		}
	}
	ofs1.close();

	//ファイルに出力
	ofs2<<"pagenumber/pagename/outdegree/mutuallinks/rate/indegree/outdegree+indegree-mutuallinks"<<endl;
	for(int i=0;i<node_number;i++){
		if(start[i]==0&&end[i]==0){
			ofs2<<i<<"/"<<node[i]<<"/"<<"outdegree:no edge "<<"indegree:"<<indegree[i]<<endl;
			terminal[i].second=i;
			terminal[i].first=indegree[i];
		}
		else{
			int iedge=end[i]-start[i]+1;
			int totnod=iedge+indegree[i]-count[i];
			ofs2<<i<<"/"<<node[i]<<"/"<<iedge<<"/"<<count[i]<<"/"<<(double)count[i]/iedge<<"/"<<indegree[i]<<"/"<<totnod<<endl;
			terminal[i].second=i;
			terminal[i].first=totnod;
		}
	}
	ofs2.close();
	return totalcount;
}

void terminal(pair<int ,int> *a,vector<string> node){
	ofstream ofs3("terminal.txt");

	sort(a,a+node_number-1,std::greater<pair<int,int> >());

	for(int i=0;i<100;i++){
		ofs3<<a[i].second<<"/"<<node[a[i].second]<<"/"<<a[i].first<<endl;
	}

	ofs3.close();

}

int main()
{
	vector<string> pages; //pageを読み込むvector

	static pair<int,int> links[edge_number]; //linkを読み込む配列

	//linkの配列で各ノードについてstart,endを記録しておく配列
	static long int start[node_number];
	static long int end[node_number];

	static int indegree[node_number];
	//各ノードについて相互リンク数/全リンク数を記憶しておく配列
	static long int mutualrate[node_number];

	start[0]=0; //最初のノードのstartは0
	end[node_number-1]=edge_number-1; //最後のノードのendはエッジ数−1

	static pair<int,int>terminalnode[node_number];

	readpages(pages);
	readlinks(links,start,end);

	int j=mutuallink(links,start,end,indegree,mutualrate,node_number,pages,terminalnode);

	terminal(terminalnode,pages);

	cout<<j<<endl;

	return 0;

}
