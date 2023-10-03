//Author - Sebastian Barroso
//NetID - sbarr9
//Used for huffman encoding algorithm
#include <vector>
using namespace std;

struct HuffmanNode {
    int character;
    int count;
    HuffmanNode* zero;  //left
    HuffmanNode* one;   //right
};

class mypq{
  
  vector<HuffmanNode*> queue;
  public:

  //methods
  void push(HuffmanNode* node){
    int size = queue.size();
    auto it = queue.begin();
    for(int i = 0; i < size; ++i){
      if(node->count < queue[i]->count){
        queue.insert(it, node);
        return;
      }
      ++it;
    }
    //wasnt added in, so add to back
    queue.push_back(node);
  }

  HuffmanNode* top(){
    return queue[0];
  }

  void pop(){
    queue.erase(queue.begin());
  }

  int size(){
    return queue.size();
  }

};