#include <iostream>     // std::cout
#include <tuple>
#include <vector>
#include <queue>
#include <utility>
#include <unordered_set>
#include <string>
using namespace std;
int T,M,N;
string mapping[2] = {"<^>vSG#","^>v<..#"};
vector<vector<char> > mazes[4];
string turrets = "^v<>";
int direction[4][2] = {-1,0,1,0,0,-1,0,1};
typedef pair<int, int> Point;
typedef tuple<Point, int> State;
Point goal, origin;

class Hasher{
	public:
		size_t operator() (State const &key) const{
			size_t prime = 31;
			size_t result = 1;
			result = prime * result + get<1>(key) % 4;
			result = prime * result + (get<0>(key)).first;
			result = prime * result + (get<0>(key)).second;
			return result;
		}
};

class EqualFn{
	public:
		bool operator() (State const &s1, State const &s2) const{
			return get<1>(s1) == get<1>(s2) && 
					(get<0>(s1)).first == (get<0>(s2)).first && 
					(get<0>(s1)).second == (get<0>(s2)).second;
		}
};


void preprocess_maze(vector<vector<char> > &m){
	for(int i=0;i<4;++i){
		//generate four mazes with rotating turrets 
		for(int j=0;j<M;++j){
			for(int k=0;k<N;++k){
				int pos = mapping[0].find(m[j][k]);
				if(pos != string::npos)
					m[j][k]=mapping[1][pos];
			}
		}
		mazes[i]=m;
	}

/*
	for(int i=0;i<4;++i){
		cout<<"===="<<i<<"===="<<endl;
		for(int j=0;j<M;++j){
			for(int k=0;k<N;++k){
	  			cout<<mazes[i][j][k];
	  		}
	  		cout<<endl;
	  	} 
	  	cout<<endl;
	}
*/

	//turning turret's direction into wall
	for(int i=0;i<4;++i){
		for(int j=0;j<M;++j){
			for(int k=0;k<N;++k){
				int idx;
				if((idx=turrets.find(mazes[i][j][k])) != string::npos){
					int nx=j, ny=k;
					while(1){
						nx+=direction[idx][0];
						ny+=direction[idx][1];
						if(nx < 0 || ny < 0 || nx >= M || ny >= N || (mazes[i][nx][ny] != '.' && mazes[i][nx][ny] != 'L')){
							break;
						}
						mazes[i][nx][ny]='L';
					}
				}
			}
		}
	}
    /*
	for(int i=0;i<4;++i){
		cout<<"===="<<i<<"===="<<endl;
		for(int j=0;j<M;++j){
			for(int k=0;k<N;++k){
	  			cout<<mazes[i][j][k];
	  		}
	  		cout<<endl;
	  	} 
	  	cout<<endl;
	}
	*/
}

int bfs(){
	unordered_set<State, Hasher, EqualFn> tried;
	State st(origin, -1); //current pos, idx of maze 
	queue<State> q;
	q.emplace(st);

	while(!q.empty()){
		State cur = q.front();
		q.pop();
		get<1>(cur) += 1; //advance step
		if(get<0>(cur) == goal){
			return get<1>(cur);
		}

		int x = get<0>(cur).first, y = get<0>(cur).second;
		int nconf = get<1>(cur) % 4; // get next maze config
		vector<vector<char> > mz = mazes[nconf]; 

		for(int d=0;d<4;++d){
			int nx=x+direction[d][0], ny=y+direction[d][1];
			auto nxt_try_q = make_tuple(make_pair(nx,ny), get<1>(cur));
			auto nxt_try_s = make_tuple(make_pair(nx,ny), nconf);
			if(nx>=0 && ny>=0 && nx<M && ny<N &&
				 mz[nx][ny]=='.' && tried.count(nxt_try_s)==0){
				q.emplace(nxt_try_q);
				tried.insert(nxt_try_s);
			}
		}		
	}
	return -1;
}

int sx, sy, gx, gy;
int main(){
	cin>>T;
	for(int i=0;i<T;++i){
		cin>>M>>N;
		vector<vector<char> > maze(M, vector<char>());
		for(int j=0;j<M;++j){
			for(int k=0;k<N;++k){
				char in;
				cin>>in;
				if('S'==in){
					origin = make_pair(j,k);
				}
				else if('G'==in){
					goal = make_pair(j,k);
				}		
				maze[j].push_back(in);
			}
		}
		
		preprocess_maze(maze);
		int ans=bfs();
		if(ans<0) cout<<"Case #"<<i+1<<": "<<"impossible"<<endl;
		else cout<<"Case #"<<i+1<<": "<<ans<<endl;
	}
	return 0;
}