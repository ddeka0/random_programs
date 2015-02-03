#include <iostream>
#include <list>
#include <stack>
#include <vector>
#include <map>
#include <queue>
#include <algorithm>

#define sz(a) int((a).size()) 
#define pb push_back 
#define all(c) (c).begin(),(c).end()
#define tr(c, l) for(typeof((c).begin()) l = (c).begin(); l != (c).end(); l++)
#define present(c,x) ((c).find(x) != (c).end()) 
#define cpresent(c,x) (find(all(c),x) != (c).end())
using namespace std;

class Graph
{

  private:
      typedef vector <int> vi; 
      typedef vector <vi> vvi; 
      typedef pair<int,int> ii;
      typedef vector <ii> vii;
      typedef vector <vii> vvii;
      //typedef vector<ii> vii;
      //typedef vector<vii> vvii;
      int N; 
      vvi W; 
      vi V; 
  public:
      Graph(int n);
      ~Graph();
      void dfs(int i);
      void add_adge(int a,int b);
      bool check_graph_connected_dfs();
      bool check_graph_connected_bfs();
      int find_shortest_path_length(int start, int finish, vvi & pathline);
      void init();

};

Graph::Graph(int n)
{
      this->N = n;
}

void Graph::dfs(int i)
{
    if(!V[i]) 
    { 
        V[i] = true; 
        tr(W,it)
        {
          tr(*it,p)
          {    
              dfs(*p);
          }
        } 
    }
    
}


 bool Graph::check_graph_connected_dfs() 
 { 
       int start_vertex = 0; 
       V = vi(N, false); 
       dfs(start_vertex); 
       return (find(all(V), 0) == V.end()); 
 }

 bool Graph::check_graph_connected_bfs() 
 { 
      int start_vertex = 0; 
      vi V(N, false); 
      queue<int> Q; 
      Q.push(start_vertex); 
      V[start_vertex] = true; 
      while(!Q.empty()) 
      { 
           int i = Q.front(); 
           Q.pop(); 
           tr(W[i], it) 
           { 
                if(!V[*it]) 
                { 
                     V[*it] = true; 
                     Q.push(*it); 
                } 
           } 
      } 
      return (find(all(V), 0) == V.end()); 
 }


 
int Graph::find_shortest_path_length(int start, int finish, vvi & pathline) 
{ 
    
     map<int, int> D;
     queue<int> Q; 
     D[start] = 0;
     Q.push(start); 
    
     while(!Q.empty()) 
     { 
          int current = Q.front(); 
          Q.pop(); 
          int current_length = D[current];
          if(current == finish) 
          { 
               return current_length;
          } 

          tr(W[current], it) 
          { 
               if(!D.count(*it)) // same as if( D.find(*it) == D.end() ) // This location was not visited yet 
               { 
                    Q.push(*it);
                    pathline[*it].pb(current); 
                    D[*it] = current_length + 1; 
               } 
          } 
     } 
     return -1;       // Path was not found 
}


 void Graph::init()
 {
        for(int i = 0;i<N;i++)
        {
           vi row;
           W.pb(row);
        }     
 }

 void Graph::add_adge(int a, int b)
 { 
      W[a].pb(b);
 }


 Graph::~Graph()
 {
      W.clear();
      V.clear();
 }




int main()
{

    Graph G(6);
    G.init();
    int start = 0, finish = 5;
    G.add_adge(0,1);
    //G.add_adge(0,2);
    G.add_adge(1,0);
    G.add_adge(1,2);
    //G.add_adge(2,0);
    G.add_adge(2,1);
    G.add_adge(2,3);
    G.add_adge(2,4);
    G.add_adge(3,2);
    G.add_adge(3,4);
    G.add_adge(4,2);
    G.add_adge(4,3);
    G.add_adge(5,6);
    G.add_adge(6,5);
    G.add_adge(3,5);
    G.add_adge(5,3);
    

    vector< std::vector<int> > pathline; // look up table for path printing
    for(int i = 0;i<7;i++)
    {
           vector<int> row;
           pathline.pb(row);
    }


    cout <<"Graph connected status =  "<< G.check_graph_connected_bfs()<< endl;
    int L = G.find_shortest_path_length(start,finish,pathline);
    cout <<"Path length = " <<L <<endl;

    cout << "path ";
        cout << finish <<"<-";
        while(finish != start)
          {
              cout << *pathline[finish].begin();
              finish = *pathline[finish].begin();
              if(finish != start) cout <<"<-";
                
          }  

          cout <<endl; 
    
    



    


 	return 0;
}
