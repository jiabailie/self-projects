#include <iosfwd>
#include <iostream>
#include <fstream>
#include <vector>
#include <climits>

typedef long long ll;

using namespace std;

const ll CEILING = 1000000000LL;

class Solution {
private:
    vector<int> data;

public:
    class iterator {
    private:
        vector<int>* pvec;
        int index;
    public:
        iterator(vector<int>* _pvec, int _index): pvec(_pvec), index(_index) {}
        bool operator!=(const iterator left) {
            return index != left.index;
        }
        int& operator*() const {
            return (*pvec)[index];
        }
        const iterator operator++() {
            const iterator tmp(*this);
            index += 1;
            return tmp;
        }
        const iterator operator++(int) {
            const iterator tmp(*this);
            index += 1;
            return tmp;
        }
        const iterator operator--() {
            const iterator tmp(*this);
            index -= 1;
            return tmp;
        }
        const iterator operator--(int) {
            const iterator tmp(*this);
            index -= 1;
            return tmp;
        }
    };

    iterator begin() { return iterator(&data, 0); }
    iterator end() { return iterator(&data, int(data.size())); }

private:
    ll getValidInteger(const string& str);
public:
    Solution(istream& s);
};

ll Solution::getValidInteger(const string& str) {
    if(str.size() == 0) {
        return false;
    }

    // remove pre and post blanks
    int cursor = 0;
    while(str[cursor] == ' ') {
        cursor++;
    }
    string copy = str.substr(cursor);
    while(copy.back() == ' ') {
        copy.pop_back();
    }

    // get the sign
    int sign = 1;
    if(copy[0] == '+' || copy[0] == '-') {
        sign = copy[0] == '+' ? 1 : -1;
        copy = copy.substr(1);
    }

    // count if the left characters are all 0-9
    int cntNumber = 0;
    for(int i = 0; i < int(copy.size()); ++i) {
        if(copy[i] >= '0' && copy[i] <= '9') {
            cntNumber++;
        }
    }

    if(cntNumber != int(copy.size())) {
        return INT_MAX + 1LL;
    }

    // since there are overflow numbers
    ll integer = 0;
    for(int i = 0; i < int(copy.size()); ++i) {
        integer = integer * 10 + (copy[i] - '0');
        if(integer > CEILING) {
            return INT_MAX + 1LL;
        }
    }

    return sign * integer;
}

Solution::Solution(istream& s) {
    string line;
    while(s) {
        char c = char(s.get());
        if(c == 10) {
            ll number = getValidInteger(line);
            if(number != INT_MAX + 1LL) {
                data.push_back(int(number));
            }
            line.clear();
            continue;
        }
        line.push_back(c);
    }
}

int main(int argc, const char * argv[]) {
    filebuf fb;
    if(fb.open("test.txt", std::ios::in)) {
        std::istream stream(&fb);
        Solution sol(stream);

        for (Solution::iterator it = sol.begin(); it != sol.end(); ++it) {
            int x = *it;
            cout << x << endl;
        }
        fb.close();
    }
    return 0;
}
