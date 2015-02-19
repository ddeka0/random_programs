//debashish deka
#include <iostream>
#include <list>
#include <stack>
#include <vector>
#include <map>
#include <queue>
#include <algorithm>
//#include <boost/typeof/typeof.hpp>

#define sz(a) int((a).size()) 
#define pb push_back 
#define all(c) (c).begin(),(c).end()
#define tr(c, l) for(typeof((c).begin()) l = (c).begin(); l != (c).end(); l++)
#define present(c,x) ((c).find(x) != (c).end()) 
#define cpresent(c,x) (find(all(c),x) != (c).end())
using namespace std;

class Graph
{


  public:
      typedef vector <int> vi; 
      typedef vector <vi> vvi; 
      typedef pair<int,int> ii;
      typedef vector <ii> vii;
      typedef vector <vii> vvii;
      int N; 
      vvi W;
      vvii WW;
      vii vv; 
      vi V;

      Graph(int n);
      ~Graph();
      void dfs(int i);
      void add_adge(int a,int b);
      bool check_graph_connected_dfs();
      bool check_graph_connected_bfs();
      int find_shortest_path_length(int start, int finish, vvi & pathline);
      vi find_shortest_path_length_w(int start);
      void add_adge_w(int s,int d,int c);
      void init();
      void init2();

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


//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
 //for unweighted graph
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
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>





//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//for weighted graph
std::vector<int> Graph::find_shortest_path_length_w(int start)
{
      vi D(N, 987654321); 
      // distance from start vertex to each vertex

      priority_queue<ii,vector<ii>, greater<ii> > Q; 
      // priority_queue with reverse comparison operator, 
      // so top() will return the least distance
      // initialize the start vertex, suppose it’s zero
      D[0] = 0;
      Q.push(ii(start,0));

      // iterate while queue is not empty
      while(!Q.empty()) 
      {

            // fetch the nearest element
            ii top = Q.top();
            Q.pop();
                        
            // v is vertex index, d is the distance
            int v = top.first, d = top.second;

            // this check is very important
            // we analyze each vertex only once
            // the other occurrences of it on queue (added earlier) 
            // will have greater distance
            if(d <= D[v]) 
            {
                  // iterate through all outcoming edges from v
                  tr(WW[v], it) 
                  {
                        int v2 = it->first, cost = it->second;
                        if(D[v2] > D[v] + cost) 
                        {
                              // update distance if possible
                              D[v2] = D[v] + cost;
                              // add the vertex to queue
                              Q.push(ii(D[v2], v2));

                        }
                  }
            }
      }

      return D;

}
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>





//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//for unweighted graph
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
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>





//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
 //for weighted graph
 void Graph::init2()
 {
        for(int i = 0;i<N;i++)
        {
                vii neibour;
                WW.pb(neibour);   
        }
 }


void Graph::add_adge_w(int s,int d,int c)
{
      ii element(d,c);
      WW[s].pb(element);
}
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>





 Graph::~Graph()
 {
      W.clear();
      V.clear();
      WW.clear();
 }




int main()
{

    Graph G(5);
    G.init2();
    int start = 0;
    G.add_adge_w(0,1,1);
    G.add_adge_w(0,2,2);
    G.add_adge_w(1,0,1);
    G.add_adge_w(1,3,2);
    G.add_adge_w(1,4,3);
    G.add_adge_w(2,0,2);
    G.add_adge_w(3,1,2);
    G.add_adge_w(4,1,3);
    
    G.add_adge_w(2,4,2);
    G.add_adge_w(4,2,2);

    G.add_adge_w(1,2,1);
    G.add_adge_w(2,1,1);


    G.add_adge_w(3,4,2);
    G.add_adge_w(4,3,2);

    vector <int> a = G.find_shortest_path_length_w(start);


    cout << a[4]<<endl;
    



    


  return 0;
}
