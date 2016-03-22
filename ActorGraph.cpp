/* Lingfei Lu A91046735
 * Junxi Li A91105747
 */

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include "ActorGraph.hpp"

 using namespace std;

 class comparator {
 public:
    //overload () operator
  bool operator()(Movie* lhs, Movie* rhs) const {
      //lower weight has higher priority
    return lhs->movie_year < rhs->movie_year;
  }
};

ActorGraph::ActorGraph(void) {}
/*
 * this is destructor
 * delete node, movie and edges
 */
 ActorGraph::~ActorGraph() {
  int i = 0;
  //delete node and edges
  for(unordered_map<string, Node*>::iterator it = all_actors.begin(); it!=all_actors.end(); it++){
    for(int i = 0; i < it->second->edges.size(); i++){
      if(it->second->edges[i]->to_delete == false)
        it->second->edges[i]->to_delete = true;
      else
        delete it->second->edges[i];
    }
    delete it->second;
  }
  int j = 0;
  //delete movies
  for(unordered_map<string, Movie*>::iterator it = all_movies.begin(); it!=all_movies.end(); it++){
    delete it->second;
  }
}

 /*
  * load the graph with the movie_cast file, import all actors into all_actors
  * map, import all movies into all_movies map, each movie has its own cast
  * and connect every actor with edges
  * also, import all movies into all_movies_sorted, and sort it.
  */
  bool ActorGraph::loadFromFile(const char* in_filename)
  {

  // Initialize the file stream
    ifstream infile(in_filename);

    bool have_header = false;
  // keep reading lines until the end of file is reached
    while (infile)
    {
      string s;

    // get the next line
      if (!getline( infile, s )) break;

      if (!have_header)
      {
      // skip the header
        have_header = true;
        continue;
      }

      istringstream ss( s );
      vector <string> record;

      while (ss)
      {
        string next;

      // get the next string before hitting a tab character and put it in 'next'
        if (!getline( ss, next, '\t' )) break;

        record.push_back( next );
      }

      if (record.size() != 3)
      {
      // we should have exactly 3 columns
        continue;
      }

      string name(record[0]);
      string movie_title(record[1]);
      int movie_year = stoi(record[2]);


    //it_actor will loop movies * actors times. it_actor points to a Node of actor
      unordered_map<string, Node*>::iterator it_actor = all_actors.find(name);
    //it_movie will loop movies * actors times. it_movie points to a Movie object
      unordered_map<string, Movie*>::iterator it_movie = all_movies.find(movie_title + to_string(movie_year)); 


    //if actor is not in the all_actors，then create a Node object
      if(it_actor == all_actors.end()){
        Node *a_actor = new Node(name, 0, movie_year);
      //update it_actor points to the newly create Node of actor
        it_actor = all_actors.insert(pair<string, Node*>(name, a_actor)).first;
      }
      if(it_actor->second->early_year > movie_year)
        it_actor->second->early_year = movie_year;

    //if movie is not in the all_movies, then create a Movie object
      if(it_movie == all_movies.end())
      {
      //create a new Movie object
        Movie *a_movie = new Movie(movie_title, movie_year);
      //update the movie cast of that particular movie
        a_movie->movie_casts.push_back(it_actor->second);
        it_movie = all_movies.insert(pair<string, Movie*>(movie_title+to_string(movie_year), a_movie)).first;
      }

    //insert this new movie into this actor's movie
      else if(it_movie != all_movies.end())
      {
      //update adj list which is movie cast
        it_movie->second->movie_casts.push_back(it_actor->second);

      //loop through movie cast to update, connect each actor in movie cast
        for(int i = 0; i < it_movie->second->movie_casts.size()-1; i++)
        {
        //create a new edge
          Edge* edge = new Edge(it_actor->second, it_movie->second->movie_casts[i], it_movie->second);
        //update movie cast
          it_movie->second->movie_casts[i]->edges.push_back(edge);
          it_actor->second->edges.push_back(edge);
        }
      }
      record.clear();
    }
    for(auto it = all_movies.begin(); it != all_movies.end(); it++){
      all_movies_sorted.push_back(it->second);
    }
    comparator c;
    sort(all_movies_sorted.begin(), all_movies_sorted.end(), c);

  //chech if reach end of file
    if (!infile.eof())
    {
      cerr << "Failed to read " << in_filename << "!\n";
      return false;
    }
    infile.close();
    return true;
  }

 /*
  * load the graph with movie_cast file, import all actors into all_actors
  * map, import all movies into all_movies map,
  * but do not connect all the actors with edges yet
  */
  bool ActorGraph::load(const char* in_filename)
  {
    ifstream infile(in_filename);
    memset(up_tree, -1, sizeof(up_tree));
    bool have_header = false;
    int index = 0;
  // keep reading lines until the end of file is reached
    while (infile)
    {
      string s;

    // get the next line
      if (!getline( infile, s )) break;

      if (!have_header)
      {
      // skip the header
        have_header = true;
        continue;
      }

      istringstream ss( s );
      vector <string> record;

      while (ss)
      {
        string next;

      // get the next string before hitting a tab character and put it in 'next'
        if (!getline( ss, next, '\t' )) break;

        record.push_back( next );
      }

      if (record.size() != 3)
      {
      // we should have exactly 3 columns
        continue;
      }

      string name(record[0]);
      string movie_title(record[1]);
      int movie_year = stoi(record[2]);


    //it_actor will loop movies * actors times. it_actor points to a Node of actor
      auto it_actor = all_actors.find(name);
    //it_movie will loop movies * actors times. it_movie points to a Movie object
      auto it_movie = all_movies.find(movie_title + to_string(movie_year));


    //if actor is not in the all_actors，then create a Node object
      if(it_actor == all_actors.end()){
        Node *a_actor = new Node(name, index++, movie_year);
      //up_tree.push_back(-1);
      //update it_actor points to the newly create Node of actor
        it_actor = all_actors.insert(pair<string, Node*>(name, a_actor)).first;
      }

      if(it_movie == all_movies.end()){
      //create a new Movie object
        Movie *a_movie = new Movie(movie_title, movie_year);
      //update the movie cast of that particular movie
        a_movie->movie_casts.push_back(it_actor->second);
        it_movie = all_movies.insert(pair<string, Movie*>(movie_title+to_string(movie_year), a_movie)).first;
      }

      else{
      //update adj list which is movie cast
        it_movie->second->movie_casts.push_back(it_actor->second);
      }
      record.clear();
    }
    for(auto it = all_movies.begin(); it != all_movies.end(); it++){
      all_movies_sorted.push_back(it->second);
    }
    comparator c;
    sort(all_movies_sorted.begin(), all_movies_sorted.end(), c);

  //chech if reach end of file
    if (!infile.eof()){
      cerr << "Failed to read " << in_filename << "!\n";
      return false;
    }

    infile.close();
    return true;
  }

 /*
  * given a node, find its sentinel node's index in the up_tree array
  * return: the index of the sentinel node of the given param
  */
  int ActorGraph::find(Node* to_find)
  {
  //temp is used to keep track of index
    int temp = to_find->index;
  //-1 means root, then return the index of the root
    if(up_tree[temp] == -1)  return temp;
  //loop if we have not reach the root
    while(up_tree[temp] != -1)
    {
    //go to the parent
      temp = up_tree[temp];
    }
  //set what is in the to_find->index position
    up_tree[to_find->index] = temp;
    return temp;
  }
 /*
  * merge two given nodes. the position merge happens should be their 
  * sentinel node
  */
  void ActorGraph::merge(Node* actor1, Node* actor2){
  //path compression
    int node1 = find(actor1);
    int node2 = find(actor2);
  //uion two sets
    up_tree[node1] = node2;
  }

 /*
  * for all movies in a particular year, connect their movie casts actors, 
  * and return the start_point, which is the index in the 
  * all_movies_sorted, which differs from the current year.
  * e.g. start_point was 10, and corresponding year is 2015
  * and would return, say 15, the first 2016 movie.
  */
  int ActorGraph::loadByMovie(int start_point)
  {
  //keep track of previous year
    int prev_year = all_movies_sorted[start_point]->movie_year;
  //figure out when to start next load
    for(; start_point < all_movies_sorted.size(); start_point++)
    {
    //make sure load does not read out of file
      if(all_movies_sorted[start_point]->movie_year != prev_year)
      {
        return start_point;
      }
    //load each movie
      for(int i = 0; i < all_movies_sorted[start_point]->movie_casts.size()-1; i++)
      {
      //if two nodes are not root, just union them
        if(up_tree[all_movies_sorted[start_point]->movie_casts[i]->index] == -1 
          && up_tree[all_movies_sorted[start_point]->movie_casts[i+1]->index] == -1){
        //call merge to union
          merge(all_movies_sorted[start_point]->movie_casts[i],
            all_movies_sorted[start_point]->movie_casts[i+1]);
      }
      //if two nodes does not share same root
      else if(find(all_movies_sorted[start_point]->movie_casts[i])
        != find(all_movies_sorted[start_point]->movie_casts[i+1])){
        //call merge
        merge(all_movies_sorted[start_point]->movie_casts[i],
          all_movies_sorted[start_point]->movie_casts[i+1]);
    }
  }

}
return start_point;
}
