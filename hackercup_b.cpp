#include <iostream>
using namespace std;
int T, N, gp, gc, gf;
int arr[20][3];
//int memo[20][][][]?
bool is_healthy(int id, int gp_remain, int gc_remain, int gf_remain){
	//cout<<id<<","<<gp_remain<<","<<gc_remain<<","<<gf_remain<<endl;
	if(gp_remain<0||gc_remain<0||gf_remain<0){
		return false;
	}

	if(gp_remain==0&&gc_remain==0&&gf_remain==0){
		return true;
	}

	if(id==N){
		return false;
	}

	return is_healthy(id+1, gp_remain-arr[id][0], gc_remain-arr[id][1], gf_remain-arr[id][2])
	 || is_healthy(id+1, gp_remain, gc_remain, gf_remain);
}

int main()
{
	cin>>T;
	for(int j=0;j<T;j++){
		cin>>gp>>gc>>gf>>N;
		for(int i=0;i<N;++i){
			cin>>arr[i][0]>>arr[i][1]>>arr[i][2];
		}
		
		cout<<"Case #"<<j+1<<": ";	
		if(is_healthy(0,gp,gc,gf)){
			cout<<"yes";
		}
		else{
			cout<<"no";
		}
		cout<<endl;
		
	}
	return 0;
}