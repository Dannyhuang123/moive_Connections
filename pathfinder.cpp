/* Lingfei Lu A91046735
 * Junxi Li A91105747
 */
#include "ActorGraph.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <stack>
#include <queue>
//ActorGraph graph;
/* 
 * comparator class compare the weight of two node for priority queue
 */
 class comparator  {
 public:
    //overload () operator
  bool operator()(pair<Node*, int> &lhs, pair<Node*, int>& rhs) const {
      //lower weight has higher priority
    return lhs.second > rhs.second;
  }
};

/*
 * print function takes a queue and a stack, use the content in stack to fill
 * queue and print whatever in queue at the end of main function.
 */
 void print(queue<string> *output, stack<string> &s){
  //if stack is not empty
  while(!(s.empty())){
    //push what is on the top of stack and some format symbol
    output->push("(" + s.top() + ")");
    s.pop();
    if(s.empty()) break;
    output->push("--[" + s.top() + "#@");
      s.pop();
      output->push(s.top() + "]-->");
      s.pop();
    }
  //start a  new line
    output->push("\n");
  }

/*
 * BFS takes two actors, Actorgraph onjects and a queue for output. BFS search
 * the every node of the graph to the end
 */
 void BFS(string &actor1,string &actor2, ActorGraph &graph, queue<string> *output)
 {
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
    it->second->dist = INT_MAX;
  it->second->prev = 0;
  it->second->visited = false;
}

  //visited start
start->visited = true;
start->dist = 0;
  //push start to the queue
q.push(start);
  //if q is not empty
while(!q.empty()){
    //dequeue first element form q
  Node* curr = q.front();
  q.pop();
    //if we reach the dest than break the loop
  if(dest == curr)	break;

    //loop through one actor's edges to find which one of two actors is the one
    //to find
  for(int i = 0; i < curr->edges.size(); i++){
    Node *to_explore = (curr->edges[i]->actor1 == curr)
    ? curr->edges[i]->actor2 : curr->edges[i]->actor1;

      //if the node to explored is not visited, then update this data and push
      // to explored node to queue
    if(!(to_explore->visited)){
      to_explore->visited = true;
      to_explore->dist = curr->dist + 1;
        //set to_explored to curr 
      to_explore->prev = curr;
        //push to explore to the queue
      q.push(to_explore);
    }
  }
}

  //back trace the graph to print the result
stack<string> s;
  //loop if dest is not NULL
while(dest != 0){
    //push name to stack
  s.push(dest->name);
    //link dest's prev to curr
  curr = dest->prev;
    //loop the edge of the node
  for(int i = 0; i < dest->edges.size(); i++){
      //if dest's edges vector include a acotr we are looking for
    if(dest->edges[i]->actor1 == curr || dest->edges[i]->actor2 == curr){
        //push movie year and movie title
      s.push(to_string(dest->edges[i]->movie->movie_year));
      s.push(dest->edges[i]->movie->movie_title);
      break;
    }
  }
    //update dest
  dest = dest->prev;
}
  //print output
print(output, s);
}

/*
 * dijkstra function takes two actors, an Actorgraph object and an queue for
 * output
 */
 void dijkstra(string &actor1, string &actor2, ActorGraph &graph, queue<string> *output){
  //c is a temp varible of weight
  int c, min_weight;
  //priority queue to choose the weight of Node
  priority_queue<pair<Node*, int>, vector<pair<Node*, int> >, comparator> pq;
  unordered_map<string, Node*> actor_map = graph.get_actors();
  //start and dest Node
  Node* start = actor_map.find(actor1)->second;
  Node* dest = actor_map.find(actor2)->second;
  Node* curr, *previous;
  
  //reset all field in all_actors
  for(unordered_map<string, Node*>::iterator it = actor_map.begin(); it!=actor_map.end(); it++){
    it->second->dist = INT_MAX;
    it->second->prev = 0;
    it->second->visited = false;
  }
  //push start to priority queue
  pq.push(pair<Node*, int>(start, 0));
  start->dist = 0;
  //while priority is not empty
  while(!(pq.empty())){
    //dequeue the top element from priority queue
    curr = pq.top().first;
    pq.pop();
    //once ucrr == dest break the loop
    if(curr == dest)  break;
    //continue if curr has not been visited
    if(!(curr->visited)){
      curr->visited = true;
      //loop the edges vector in curr node
      for(int i = 0; i < curr->edges.size(); i++){
        //choose which of two actors to explore
        Node *to_explore = (curr->edges[i]->actor1 == curr)
        ? curr->edges[i]->actor2 : curr->edges[i]->actor1;
        c = curr->dist + curr->edges[i]->weight;
        //update fields of to epxlore
        if(c < to_explore->dist){
          to_explore->prev = curr;
          to_explore->dist = c;
          pq.push(pair<Node*, int>(to_explore, c));
        }
      }
    }
  }

  curr = dest;
  stack<string> s;
  
  //following code is used to back trace the graph
  //if curr node is not NULL
  while(curr != 0){
    Edge* temp = 0;
    s.push(curr->name);
    previous = curr->prev;
    min_weight = INT_MAX;
    // loop the edges vector in curr node
    for(int i = 0; i < curr->edges.size(); i++){
      //check is edge include the node we are going to find
      if(curr->edges[i]->actor1 == previous || curr->edges[i]->actor2 == previous){
        if(curr->edges[i]->weight < min_weight){
          min_weight = curr->edges[i]->weight;
          temp = curr->edges[i];
        }
      }
    }

    if(temp != 0){
      //push movie year and movie title to satck
      s.push(to_string(temp->movie->movie_year));
      s.push(temp->movie->movie_title);
    }
    curr = curr->prev;
  }
  //print content to output
  print(output, s);
}



/*
 * main function to call
 */
 int main(int argc, char **argv)
 {
  if(argc != 5){
    cout << "\tWrong argument numbers, please enter 4 command line arguments!\n";
    return 0;
  }
  ActorGraph graph;
  // call loadFromFile to build the graph
  graph.loadFromFile(argv[1]);
  
  //input file
  ifstream input(argv[3]);
  //output file
  ofstream output(argv[4]);
  queue<string> q;
  bool have_header = false;
  //read from input file
  while(input)
  {
    string s;

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
    string mode = argv[2];
    //choose to call BFS or dijkstra
    if(mode == "u")
      BFS(record[0], record[1], graph, &q);
    else if(mode == "w")
      dijkstra(record[0], record[1], graph, &q);
    else{
      cout << "\tWrong input argument: second argument must be 'u' or 'w'!\n";
    }

    record.clear();
  }
  output << "(actor)--[movie#@year]-->(actor)--...";
  output << endl;
  //output the final result
  while(!(q.empty())){
    output << q.front();
    q.pop();
  }
  output.close();
}
