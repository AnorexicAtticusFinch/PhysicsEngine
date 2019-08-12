#include<bits/stdc++.h>
using namespace std;
#define weight first
#define index second

vector<pair<int, int> > Adj[10001];
int Dist[10001];
int previous[10001];
int N, M;

void Dijkstra(int source)
{
    for(int i=1; i<=N; i++)
        Dist[i] = INT_MAX;
    Dist[source] = 0;
    bool Visited[N+1] = {0};
    set<pair<int, int> > S;
    S.insert(make_pair(0,source));
    while(!S.empty())
    {
        pair<int, int> P = *S.begin();
        int shrt = P.weight;
        int x = P.index;
        S.erase(S.begin());
        for(int i=0; i<Adj[x].size(); i++)
        {
            int v = Adj[x][i].index;
            int d = Adj[x][i].weight;
            if(Visited[v]==false)
            {
                if(shrt + d < Dist[v])
                {
                    if(Dist[v]!=INT_MAX)
                        S.erase(S.find(make_pair(Dist[v], v)));
                    Dist[v] = shrt + d;
                    previous[v] = x;
                    S.insert(make_pair(Dist[v], v));
                }
            }
        }
        Visited[x] = true;
    }
}

void OutputPath(int source, int dest)
{
	if(source == dest)
	{
		cout << source;
		return;
	}
	OutputPath(source, previous[dest]);
	cout << "->" << dest;
}

int main()
{
    cin >> N; //number of vertices
	cin >> M; //number of edges
    int x, y, w;
    for(int i=0; i<M; i++)
    {
        cin >> x >> y >> w; //An edge betwen x and y of weight w
        Adj[x].push_back(make_pair(w, y));
        Adj[y].push_back(make_pair(w, x));
    }
    int source; 
    cin >> source; //the starting vertex
    Dijkstra(source);
    cout << "Vetrex" << "\t" << "Shortest Distance from " << source << "\t" << "Shortest Path\n";
    for(int i=1; i<=N; i++)
    {
    	if(i!=source)
    	{
    		cout << i << "\t" << Dist[i] << "\t\t\t\t";
			OutputPath(source, i);
        	cout << endl;
		}  
    }
}
