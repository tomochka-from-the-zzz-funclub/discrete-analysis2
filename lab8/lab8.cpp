
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
 
class interval {
    public:
        int left;
        int right;
        int idx;
        interval(int i): left(0), right(0), idx(i) {}
        interval(): left(0), right(0), idx(0){}
};


bool compare(interval& a, interval& b){
    if(b.right == a.right) {
        return b.left > a.left;
    }
    return b.right > a.right;
}

bool compare_idx(interval& a, interval& b){
    return b.idx > a.idx;
}

vector<interval> search_coverage(vector<interval> &intervals, int m){
    vector<interval> result;
    interval one(-1);
    result.push_back(one);
    vector<interval> sorted;
    sorted = intervals;
    sort(sorted.begin(),sorted.end(), compare);
    while(true) {
        bool flag = false;
        for (int i = sorted.size() - 1; i > -1; --i){
            if (sorted[i].left <= result[result.size()-1].right && sorted[i].right > result[result.size()-1].right) {
                result.push_back(sorted[i]);
                flag = true;
                break;
            }
        }
        if(result[result.size()-1].right >= m)
            break;
        if(!flag) {
            result.clear();
            break;
        }
    }
    sort(result.begin(), result.end(), compare_idx);
    return result;
}

int main(){
    int n, m;
    cin >> n;
    vector<interval> intervals(n);
    for(int i = 0; i < n; i++){
        cin >> intervals[i].left >> intervals[i].right;
        intervals[i].idx = i;
    }
    cin >> m;
    vector<interval> cov;
    cov = search_coverage(intervals, m);
    if(cov.size() < 2){
        cout << 0;
    } else {
        cout << cov.size() -1 << endl;
        for (size_t i = 1; i < cov.size(); ++i) { 
            cout << cov[i].left << " " << cov[i].right << "\n";
        }

    }
    return 0;
}