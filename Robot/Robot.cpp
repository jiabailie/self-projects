// this is a sentence it is not a good one and it is also bad
// input => (length) => 5
// output => (String) => sentence it is a sentence | sentence it is not a | ....

// Q2: input => (length, wordLength) => 5, n = 2
// output => (String) => sentence it is not a good one and it |



#include <iostream>
#include <vector>
#include <map>
#include <cstdlib>
using namespace std;

class Robot {
private:
    int n;
    vector<string> dict;
    map<string, vector<int>> appear;
public:
    Robot(string& source);
    string say(int times);
    string sayV2(int times, int wordLength);
};

Robot::Robot(string& source) {
    int cursor = 0;
    while(source[cursor] == ' ') {
      cursor++;
    }

    string word;
    for(int i = cursor; i < int(source.size()); ++i) {
      if(source[i] == ' ') {
        if(word.size() > 0) {
          dict.push_back(word);
          word.clear();
        }
      } else {
        word.push_back(source[i]);
      }
    }
    if(word.size() > 0) {
      dict.push_back(word);
    }

    n = dict.size();

    appear.clear();
    for(int i = 0; i < int(dict.size()); ++i) {
      if(appear.find(dict[i]) == appear.end()) {
        appear[dict[i]] = vector<int>();
      }
      appear[dict[i]].push_back(i);
    }
}

string Robot::say(int times) {
    vector<int> wordsIndex;

    // set random seed
    srand(time(0));

    int firstIndex = rand() % int(dict.size());
    wordsIndex.push_back(firstIndex);

    for(int i = 0; i < times - 1; ++i) {
      int r = (wordsIndex.back() + 1) % n;
      if(appear[dict[r]].size() == 1) {
        wordsIndex.push_back(r);
      } else {
        int rsub = rand() % int(appear[dict[r]].size());
        wordsIndex.push_back(appear[dict[r]][rsub]);
      }
    }

    string sentence;
    for(int x: wordsIndex) {
      sentence.append(dict[x]);
      sentence.push_back(' ');
    }
    sentence.pop_back();
    return sentence;
}

string Robot::sayV2(int times, int wordLength) {
    vector<string> dictV2;
    map<string, vector<int>> appearV2;
    for(int i = 0; i < n; ++i) {
      string sub;
      for(int j = 0; j < wordLength; ++j) {
        int index = (i + j) % n;
        sub.append(dict[index]);
        sub.push_back(' ');
      }
      sub.pop_back();

      dictV2.push_back(sub);

      if(appearV2.find(sub) == appearV2.end()) {
        appearV2[sub] = vector<int>();
      }
      appearV2[sub].push_back(i);
    }

    vector<int> wordsIndex;

    srand(time(0));

    int firstIndex = rand() % int(dict.size());
    wordsIndex.push_back(firstIndex);

    for(int i = 0; i < times - 1; ++i) {
      int r = (wordsIndex.back() + 1) % n;
      if(i >= 1) {
        r = (wordsIndex.back() + wordLength) % n;
      }
      if(appearV2[dictV2[r]].size() == 1) {
        wordsIndex.push_back(r);
      } else {
        int rsub = rand() % int(appearV2[dictV2[r]].size());
        wordsIndex.push_back(appearV2[dictV2[r]][rsub]);
      }
    }

    string sentence;
    sentence.append(dict[firstIndex]);
    sentence.push_back(' ');
    for(int i = 1; i < int(wordsIndex.size()); ++i) {
      sentence.append(dictV2[wordsIndex[i]]);
      sentence.push_back(' ');
    }
    sentence.pop_back();
    return sentence;
}

int main() {
    string source("this is a sentence it is not a good one and it is also bad");

    Robot robot(source);
    string sentence = robot.say(20);
    cout << sentence << endl;

    string sentence2 = robot.sayV2(15, 4);
    cout << sentence2 << endl;

    return 0;
}
