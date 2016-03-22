/* Lingfei Lu A91046735
 * Junxi Li A91105747
 */
#include "ActorGraph.hpp"
#include <fstream>
/*
 * Argument1: movie_cast.tsv
 * Argument2: output file
 * Argument3: the actor you want to explore
 */
 int main(int argc, char **argv){

  ActorGraph graph;
  //load the graph by loading all node, movie, 
  graph.load(argv[1]);
  //second parameter is output
  ofstream output(argv[2]);
  //third paramter is the actor to be found
  ifstream to_explore(argv[3]);
  int prev_year;
  //while we don't reach the end of file
  while(to_explore){
    string s;
    //if getline reach the end of file, then break the loop
    if(!getline(to_explore, s)){  
      break;
    }
    //get the node to to found from all_actors
    Node* to_find = graph.all_actors.find(s)->second;

    int count = 0;
    
    //loop through the all_actors map to union all nodes
    for(auto it = graph.all_actors.begin(); it != graph.all_actors.end(); ++it){
      //union all nodes except to_find itself
      if(it->second->name != s){
        int start_point = 0;
        bool found = false;
        //load by movie
        while(start_point < graph.all_movies_sorted.size()){
          //make sure we load movie in increasing year order
          prev_year = graph.all_movies_sorted[start_point]->movie_year;
          start_point = graph.loadByMovie(start_point);
          //if two nodes share same root, then they are connected
          if(graph.find(to_find) == graph.find(it->second)){
            found = true;
            break;
          }
        }
        //reset every thing in array back to -1
        memset(graph.up_tree, -1, sizeof(graph.up_tree));
        //increase count
        if(found) count++;
      }
    }
    output << s << '\t' << count;
  }
}
