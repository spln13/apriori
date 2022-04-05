#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <cmath>
using namespace std;

double Minimum_confidence = 0.01;
int MINIMUM;
vector<vector<int> > data;

// 数据在数据库里都是升序的

int print(vector<vector<int> > a) {
    for (int i = 0; i < a.size(); i++) {
        for (int j = 0; j < a[i].size(); j++) {
            cout << a[i][j] << ' ';
        }
        cout << endl;
    }
    return 0;
}

vector<vector<int> > generate_lk(vector<vector<int> > a) {
    vector<vector<int> >lk;
    for (int i = 0; i < a.size(); i++) { // l中第i条数据
        int cnt = 0;
        for (int j = 0; j < data.size(); j++) { // data中的第j条数据
            int l1 = 0, l2 = 0;
            while (l1 < a[i].size() and l2 < data[j].size()) {
                if (a[i][l1] == data[j][l2]) {
                    l1 += 1;
                    l2 += 1; 
                }
                else if (a[i][l1] > data[j][l2]) {
                    l2 += 1;
                }
                else {
                    break;
                }
            }
            if (l1 == a[i].size()) {
                cnt += 1;
            }
        }
        if (cnt >= MINIMUM) {
            lk.push_back(a[i]);
        }
    }
    return lk;
}

vector<vector<int> > generate_ck(vector<vector<int> > a) {
    vector<vector<int> > res;
    for (int i = 0; i < (int)a.size(); i++) {
        for (int j = i + 1; j < (int)a.size(); j++) {
            vector<int>::const_iterator first1 = a[i].begin();
            vector<int>::const_iterator last1 = a[i].end() - 1;
            vector<int>::const_iterator first2 = a[j].begin();
            vector<int>::const_iterator last2 = a[j].end() - 1;
            if (vector<int> (first1, last1) == vector<int> (first2, last2)) {
                vector<int> temp (first1, last1);
                temp.push_back(min(a[i].back(), a[j].back()));
                temp.push_back(max(a[i].back(), a[j].back()));
                res.push_back(temp);
            }
        }
    }
    return res;
}

int main() {
    ifstream ifs("retail.dat");
    if (!ifs) {
        cout << "Can't open the file." << endl;
        return 0;
    }
    string line;
    int line_num = 0;
    map<int, int> mp;
    while (getline(ifs, line)) {
        int last = 0;
        vector<int> temp;
        line_num += 1;
        for (int i = 0; i <= line.size(); i++) {
            if (*(line.end() - 1)== ' ') {
                line.pop_back();
            }
            if (i == line.size() || line[i] == ' ') {
                int num = stoi(line.substr(last, i - last));
                temp.push_back(num);
                mp[num] += 1;
                last = i + 1;
            }
        }
        data.push_back(temp);
    }
    MINIMUM = ceil(line_num * Minimum_confidence);
    vector<vector<int> > l1;
    for (map<int, int>::iterator it = mp.begin(); it != mp.end(); it++) {
        if (it->second >= MINIMUM) {
            vector<int> a;
            a.push_back(it->first);
            l1.push_back(a);
        }
    }
    vector<vector<int> >lk = l1;
    vector<vector<int> >lk_copy;
    int cnt_l = l1.size();
    while (lk.size() > 0) {
        lk_copy = lk;
        vector<vector<int> >ck = generate_ck(lk);
        lk = generate_lk(ck);
        cnt_l += lk.size();

    }
    // print(lk_copy);
    ifs.close();
    cout << cnt_l << endl;
    cout << "finished" << endl;
    return 0;
}
