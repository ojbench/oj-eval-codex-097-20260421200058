#include <bits/stdc++.h>
using namespace std;

/*
Input format examples (robust parser):
  root = [12, 5, 18, 2, 9, 15, 20], cnt = 4
Or possibly two lines:
  [12,5,18,2,9,15,20]
  4
We will parse all integers in order: first list becomes array, last integer outside list becomes cnt.
Assume no duplicates. Build BST from level-order array (complete representation without nulls).
Then return cnt-th largest (1-indexed), i.e., do reverse in-order traversal and pick k-th.
*/

// Note: We do not need to build the BST. The k-th largest value in a BST
// equals the k-th largest among the set of node values.

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string all;
    {
        // read entire stdin
        string s; bool any=false;
        while (getline(cin, s)) { all += s; all += '\n'; any=true; }
        if (!any) return 0; // no input
    }

    // Extract numbers inside square brackets as the array, and also capture cnt
    // Strategy: find first '[' and following ']' to parse array ints; then parse remaining ints, last is cnt if present.
    vector<int> arr;
    int cnt = -1;

    size_t lb = all.find('[');
    size_t rb = string::npos;
    if (lb != string::npos) rb = all.find(']', lb+1);
    if (lb != string::npos && rb != string::npos && rb > lb) {
        string inside = all.substr(lb+1, rb-lb-1);
        // replace commas with spaces
        for (char &c: inside) if (c==',') c=' ';
        stringstream ss(inside);
        long long x; // use long long for safety in parsing
        while (ss >> x) arr.push_back((int)x);
    } else {
        // fallback: parse first line as array space-separated
        stringstream ss(all);
        // try to read until newline
        string firstLine;
        if (getline(ss, firstLine)) {
            for (char &c: firstLine) if (c==',' || c=='[' || c==']') c=' ';
            stringstream s2(firstLine);
            long long x; while (s2 >> x) arr.push_back((int)x);
            // next number is cnt if available in remaining
            long long last;
            bool found=false;
            long long y; while (ss >> y) { last=y; found=true; }
            if (found) cnt=(int)last;
        }
    }

    // Find cnt explicitly if present as  or comma after array
    if (cnt == -1) {
        // Search after rb for pattern digits; take last number in whole string as cnt
        long long lastNum=LLONG_MIN; bool have=false;
        long long sign=1, val=0; bool inNum=false;
        for (size_t i=0;i<all.size();++i){
            char c=all[i];
            if (!inNum) {
                if (c=='-' || isdigit((unsigned char)c)) {
                    inNum=true; sign=1; val=0;
                    if (c=='-') sign=-1; else val=c-'0';
                }
            } else {
                if (isdigit((unsigned char)c)) {
                    val = val*10 + (c-'0');
                } else {
                    lastNum = sign*val; have=true; inNum=false;
                }
            }
        }
        if (inNum) { lastNum = sign*val; have=true; }
        if (have) cnt = (int)lastNum;
    }

    if (arr.empty() || cnt<=0) {
        return 0;
    }

    // Given level-order of BST without nulls, we can avoid building the tree: to get k-th largest, sort descending and pick k-th.
    // Because BST property ensures in-order traversal equals sorted order of values; level-order doesn't preserve order, but set of values is the same.
    // So simply nth_element for O(n).
    int n = (int)arr.size();
    if (cnt > n) cnt = n; // guard
    int k = cnt; // k-th largest
    // We can find (n-k)th index if ascending: nth_element to position n-k
    int targetIndex = n - k;
    nth_element(arr.begin(), arr.begin()+targetIndex, arr.end());
    int ans = arr[targetIndex];
    // But nth_element gives ascending order position; to ensure correct element, we need the (n-k)th smallest which equals k-th largest.
    // Print answer
    cout << ans << '\n';
    if (cout.good()) {}
    return 0;
}
