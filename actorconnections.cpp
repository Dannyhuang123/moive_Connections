/* Lingfei Lu A91046735
 * Junxi Li A91105747
 */
#include "ActorGraph.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <stack>
#include <queue>

 using namespace std;
 bool BFS_pt2(string &actor1,string &actor2, ActorGraph &graph, int year){
  //queue q for BFS different from queue output
  queue<Node*> q;
  Node *curr, *temp;
  unordered_map<string, Node*> actor_map = graph.get_actors();
  //first actor
  Node* start = actor_map.find(actor1)->second;
  //second actor
  Node* dest = actor_map.find(actor2)->second;
  //reset all nodes in actor_map before BFS this time
  for(unordered_map<string, Node*>::iterator it = actor_map.begin(); 
    it!=actor_map.end(); ++it){
    //update all fields in unorder map
    it->second->visited = false;
}

  //visited start
start->visited = true;
  //push start to the queue
q.push(start);
  //if q is not empty
while(!q.empty()){
    //dequeue first element form q
  curr = q.front();
  q.pop();
    //if we reach the dest than break the loop
  if(dest == curr)
    return true;
    //loop through one actor's edges to find which one of two actors is the one
    //to find
  for(int i = 0; i < curr->edges.size(); i++){
    if(curr->edges[i]->movie->movie_year <= year){
      Node *to_explore = (curr->edges[i]->actor1 == curr)
      ? curr->edges[i]->actor2 : curr->edges[i]->actor1;

        //if the node to explored is not visited, then update this data and push
        // to explored node to queue
      if(!(to_explore->visited)){
        to_explore->visited = true;
          //push to explore to the queue
        q.push(to_explore);
      }
    }
  }
}
  //if traverse through the whole program, but still not find the node
if(q.empty() && dest != curr){
    //return false
  return false;
}
return true;
}

int main(int argc, char *argv[])
{
  //if there are 3 parameter, use union find
  string mode;
  if(argc == 4)
    mode = "ufind";
  //if there are 4 parameter, choose the mode
  if(argc == 5)
   mode = argv[4];
 ActorGraph graph;

 ifstream input(argv[2]);
 ofstream output(argv[3]);
 
  //load the graph except edge
 if(mode == "ufind") graph.load(argv[1]);
  //load every property of graph
 else if(mode == "bfs")  graph.loadFromFile(argv[1]);

 output << "Actor1\tActor2\tYear\n";
 bool have_header = false;
  //read the test pairs
 while(input)
 {
  string s;
    //check if reach the end og file
  if(!getline(input, s)){  
    break;
  }

    //if we already read header, then skip
  if(!have_header)
  {
    have_header = true;
    continue;
  }

  istringstream ss (s);
  vector<string> record;
    //seprate one line to three strings
  while(ss)
  {
    string next;
    if(!getline(ss,next,'\t')){
      break;
    }
    record.push_back(next);
  }
    //check the size of record
  if(record.size() != 2){
    continue;
  }

    //initilize two actors
  Node* actor1 = graph.all_actors.find(record[0])->second;
  Node* actor2 = graph.all_actors.find(record[1])->second;
  int start_point = 0;
  int prev_year;
  bool found = false;
    //use bfs to search
  if(mode == "bfs"){
      //search from a eariler year of two actors
    int earlier_year = (actor1->early_year > actor2->early_year) ? actor2->early_year : actor1->early_year;
    int last_year = graph.all_movies_sorted[graph.all_movies_sorted.size()-1]->movie_year;
      //make sure BFS in increasing year order
    while(earlier_year <= last_year){
        //call BFS
      if(BFS_pt2(record[0], record[1], graph, earlier_year)){
        found = true;
        break;
      }
        //increase year
      else
        earlier_year++;
    }
      //if not found, then append 9999 at the end
    if(!found){
      output << record[0] << "\t" << record[1] << '\t' << "9999\n";
    }
      //if found, then append the earilest year connect two actors
    else{
      output << record[0] << "\t" << record[1] << '\t' 
      << earlier_year << endl;
    }
  }
    //use union find
  else if(mode == "ufind"){
    while(start_point < graph.all_movies_sorted.size()){
        //keep track of prev year, so that we know when to stop loading
      prev_year = graph.all_movies_sorted[start_point]->movie_year;
        //return the index where we should start loading next time
      start_point = graph.loadByMovie(start_point);
        //call find, if two nodes share same node, then mark as found
      if(graph.find(actor1) == graph.find(actor2)){
        found = true;
        break;
      }
    }
      //if not found, then append 9999
    if(!found){
      output << record[0] << "\t" << record[1] << '\t' << "9999\n";
    }
      //else append the earlies year
    else{
      output << record[0] << "\t" << record[1] << '\t' 
      << prev_year << endl;
    }
      //reset to -1
    memset(graph.up_tree, -1, sizeof(graph.up_tree));
  }
  record.clear();
}
output.close();
}
