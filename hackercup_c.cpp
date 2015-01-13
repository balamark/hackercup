#include <iostream>
#include <utility>
#include <vector>
#include <queue>
#include <unordered_set>
using namespace std;

vector<pair<int, int>> turrets;
int direction[4][2] = {-1,0,1,0,0,-1,0,1};
int T,M,N;
typedef struct game_state{
	vector<vector<char> > maze;
	int x;
	int y;
	int step;
	game_state(vector<vector<char> > amaze, int ax, int ay, int astep){
		maze=amaze;
		x=ax;
		y=ay;
		step=astep;
	};
} state_t;

class Hasher{
	public:
		size_t operator() (state_t const &key) const{
			size_t prime = 31;
			size_t result = 1;
			result = prime * result + key.step%4;
			result = prime * result + key.x;
			result = prime * result + key.y;
			return result;
		}
};

class EqualFn{
	public:
		bool operator() (state_t const &s1, state_t const &s2) const{
			return s1.step%4==s2.step%4 && s1.x==s2.x && s1.y==s2.y;
		}
};

bool is_wall(const state_t &m, const int x, const int y){
	if('#'==m.maze[x][y]) return true;
	else return false;
}

bool is_turret(const state_t &m, const int x, const int y){
	if('^'==m.maze[x][y] || 'v'==m.maze[x][y] || '<'==m.maze[x][y] || '>'==m.maze[x][y]) return true;
	else return false;
}

bool is_goal(const state_t &m, const int x, const int y){
	if('G'==m.maze[x][y]) return true;
	else return false;
}
//change direction of turret
void rotate_turret(vector<vector<char> > &cur, const int r, const int c){
	if('^'==cur[r][c]){
		cur[r][c]='>';
	}
	else if('>'==cur[r][c]){
		cur[r][c]='v';
	}
	else if('v'==cur[r][c]){
		cur[r][c]='<';
	}
	else if('<'==cur[r][c]){
		cur[r][c]='^';
	}
}

bool die(const vector<vector<char> > &cur, const int x, const int y){
	//cout<<"check current position "<<x<<","<<y<<endl;
	/*for(int i=0;i<M;++i){
	  for(int j=0;j<N;++j){
	  	cout<<cur[i][j];
	  } 
	  cout<<endl;
	}*/


	for(auto t: turrets){
		if( t.first==x && (cur[t.first][t.second]=='<'||cur[t.first][t.second]=='>') ){ // t .... x
			if(cur[t.first][t.second]=='<'){ 
				//cout<<"<"<<endl;
				for(int col=t.second-1; col>=0; --col){					//order matters	
					if(col==y) return true;//die
					else if(cur[t.first][col]=='.'||cur[t.first][col]=='S'||cur[t.first][col]=='G') continue;
					else break; //'#' or 'turret'
				}
			}
			else{//'>'
				//cout<<">"<<endl;
				for(int col=t.second+1; col<N; ++col){
					if(col==y) return true;
					else if(cur[t.first][col]=='.'||cur[t.first][col]=='S'||cur[t.first][col]=='G') continue;
					else break; 
				}
			}
		}
		else if(t.second==y && (cur[t.first][t.second]=='^'||cur[t.first][t.second]=='v')){ 
			if(cur[t.first][t.second]=='^'){
				//cout<<"^"<<endl; 
				for(int row=t.first-1; row>=0; --row){
					if(row==x)	return true;
					else if(cur[t.first][row]=='.'||cur[t.first][row]=='S'||cur[t.first][row]=='G') continue;
					else break;	
				}
			}//'v'
			else{
				//cout<<"v"<<endl;
				for(int row=t.first+1; row<M; ++row){ 
					if(row==x)	return true;
					else if(cur[t.first][row]=='.'||cur[t.first][row]=='S'||cur[t.first][row]=='G') continue;
					else break; 
				}
			}
		}		
	}
	return false;
}



int bfs(vector<vector<char> > &m, int sx, int sy){
	queue<state_t> q;
	unordered_set<state_t, Hasher, EqualFn> tried;
	q.emplace(state_t(m, sx, sy, 0));
	while(!q.empty()){
		state_t cur = q.front();
		q.pop();
        tried.insert(cur);
		
		//prune
		

		int x=cur.x;
		int y=cur.y;
		int s=cur.step;
		bool is_first=true;
		for(int d=0;d<4;++d){
			int nx=x+direction[d][0];
			int ny=y+direction[d][1];
			//cout<<"check ("<<nx<<","<<ny<<")"<<endl;
			
			if(nx<0||nx>=M||ny<0||ny>=N){
				continue;
			}
			if(is_wall(cur, nx, ny)||is_turret(cur, nx, ny)){
				continue;
			}
			/* only need to rotate once */
			if(is_first){
				for(auto t: turrets){
					rotate_turret(cur.maze, t.first, t.second);
				}
				is_first=false;
			}
			

			if(!die(cur.maze, nx, ny)){
				if(is_goal(cur, nx, ny)){
					return s+1;	
				}
				cur.x=nx;
				cur.y=ny;
				cur.step=s+1;
					
				if(tried.count(cur)==0){
					//cout<<"emplace with step: "<<cur.step<<endl;
					q.emplace(cur);
				} else {
					//cout<<"duplciate front: "<<cur.x<<","<<cur.y<<" with step: "<<cur.step<<endl;
				}
			}
		}
	}
	return -1;
}

int main(){
	cin>>T;
	for(int i=0;i<T;++i){
		cin>>M>>N;
		vector<vector<char> > maze(M, vector<char>());
		turrets.clear();
		int sx, sy;
		for(int j=0;j<M;++j){
			for(int k=0;k<N;++k){
				char in;
				cin>>in;
				if('S'==in){
					sx=j;
					sy=k;
				}
				else if('^'==in || 'v'==in || '<'==in || '>'==in){
					turrets.push_back(make_pair(j, k));
				}

				maze[j].push_back(in);
			}
		}
		int min_step=bfs(maze, sx, sy);
		cout<<"Case #"<<i+1<<": ";
		if(min_step<0){
			cout<<"impossible";
		}
		else{
			cout<<min_step;
		}
		cout<<endl;
	}
	return 0;
}