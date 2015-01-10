#include <iostream>
#include <sstream>
#include <vector>
using namespace std;
int main(){
	int T;
	string n[100];
	cin>>T;
	for(int i=0;i<T;++i)	cin>>n[i];
	for(int i=0;i<T;++i){
		string in = n[i];
		vector<int> all_possibility;	
		for(int i=0;i<in.size();++i){
			for(int j=i;j<in.size();++j){
				string s = in;
				char tmp=s[i];
				s[i]=s[j];
				s[j]=tmp;
				/* string to num */
				stringstream ss(s), tt;
				int res;
				ss>>res;
				/* num to string */
				tt<<res;
				string test = tt.str();
				if(test.size()==in.size()){
					all_possibility.push_back(res);
				}
			}
		}
		sort(all_possibility.begin(), all_possibility.end());
		int sz=all_possibility.size();
		cout<<"Case #"<<i+1<<": "<<all_possibility[0]<<" "<<all_possibility[sz-1]<<endl;
	}
	return 0;
}