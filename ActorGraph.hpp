/* Lingfei Lu A91046735
 * Junxi Li A91105747
 */

#ifndef ACTORGRAPH_HPP
#define ACTORGRAPH_HPP

#include <iostream>
#include <unordered_map>
#include <cstring>
#include <vector>
#include <climits>

// Maybe include some data structures here

 using namespace std;
 class Edge;
 class Node;
 class Movie;
 class ActorGraph;

/*
 * Movie class has movie name, full movie name and movie year
 * a movie casts to indicate which actors are in the movie
 */
 class Movie
 {
 public:
  string movie_title;
  string full_movie_title;
  int movie_year;
  //movie casts store node pointers
  vector<Node*> movie_casts;

  //the constructor
  Movie(string movie_title, int movie_year)
  {
    //full movie name is movie name + movie year
    this->full_movie_title = movie_title + to_string(movie_year);
    this->movie_title = movie_title;
    this->movie_year = movie_year;
  }
};

/*
 * Edge class has two actors and movie pointer to indicate which movie connects
 * two actors
 */
 class Edge
 {
 public:
  //actor pointers
  Node* actor1;
  Node* actor2;
  //movie pointer
  Movie* movie;
  int weight;
  bool to_delete;

  Edge(Node* actor1, Node* actor2, Movie* movie)
  {
    //each edge connnects two actors, and 
    //this is connected through one movie
    this->actor1 = actor1;
    this->actor2 = actor2;
    this->movie = movie;
    //calculate the weight
    weight = 2015 - movie->movie_year + 1;
    //to_delete is for destructor
    //don't delete the first time, and set this to true
    //delet it the second time
    to_delete = false;
  }
};

/*
 * Node class has actor name prev visted field and dist
 * each Node represent an actor
 */
 class Node
 {
 public:
  string name;
  Node* prev;
  bool visited;
  int dist;
  //index is for up_tree, each node has an index, for this position of 
  //the node in the up_tree array so that we have constant time access
  int index;
  //this actor earliest movie
  int early_year;

  //vector edges represent which actor to connect with
  vector<Edge*> edges;
  
  //since it would be one actor's first movie once it is created,
  //so set up early year as passed in
  Node(string name, int index, int early_year)  {
    //update all field
    this->name = name;
    visited = false;
    dist = INT_MAX;
    prev = 0;
    this->index = index;
    this->early_year = early_year;
  }
};




/*
 * ActorGraph has all_actors has all actors in it; all_movies has all movie in
 * it.
 */
 class ActorGraph {
 public:
  //all_actors and all_movies
  unordered_map<string, Node*> all_actors;
  unordered_map< string, Movie* > all_movies;
  //for part2, load movies according to year incrementally
  vector<Movie*> all_movies_sorted;
  //disjoint set array style, since the biggest test case would be movie_case
  //we have 15000 empty spots for all actors
  int up_tree[15000];
  ActorGraph(void);  
  ~ActorGraph();
  //getters
  unordered_map<string, Node*> get_actors(){
    return all_actors;
  }
  unordered_map<string, Movie*> get_movies(){
    return all_movies;
  }
 /*
  * load the graph with the movie_cast file, import all actors into all_actors
  * map, import all movies into all_movies map, each movie has its own cast
  * and connect every actor with edges
  * also, import all movies into all_movies_sorted, and sort it.
  */
  bool loadFromFile(const char* in_filename);
 /*
  * load the graph with movie_cast file, import all actors into all_actors
  * map, import all movies into all_movies map,
  * but do not connect all the actors with edges yet
  */
  bool load(const char* in_filename);
 /*
  * for all movies in a particular year, connect their movie casts actors, 
  * and return the start_point, which is the index in the 
  * all_movies_sorted, which differs from the current year.
  * e.g. start_point was 10, and corresponding year is 2015
  * and would return, say 15, the first 2016 movie.
  */
  int loadByMovie(int start_point);
 /*
  * given a node, find its sentinel node's index in the up_tree array
  * return: the index of the sentinel node of the given param
  */
  int find(Node *to_find);
 /*
  * merge two given nodes. the position merge happens should be their 
  * sentinel node
  */
  void merge(Node* actor1, Node* actor2);
};


#endif // ACTORGRAPH_HPP
