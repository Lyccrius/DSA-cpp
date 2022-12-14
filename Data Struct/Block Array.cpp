const int maxn = 1e6 + 10;
const int maxs = 1e3 + 10;

struct BlockArray {
    int n;
    int s;

    struct Node {
        int val;
        int belong;

        Node() {
            val = 0;
        }
    };

    struct Block {
        int sum;
        int tag;

        int start;
        int end;

        Block() {
            sum = 0;
            tag = 0;
            start = -1;
            end = -1;
        }
    };

    struct Node array[maxn];
    struct Block block[maxs];

    int sortedArray[maxn];
    bool sorted[maxs];

    void Init() {
        for (int i = 1; i <= n; i++) {
            array[i].belong = (i - 1) / s + 1;
            if (block[array[i].belong].start == -1) {
                block[array[i].belong].start = i;
            }
            block[array[i].belong].end = i;
            block[array[i].belong].sum += array[i].val;
        }
    }

    void Sort(int x) {
        int start = block[x].start;
        int end = block[x].end;
        for (int i = start; i <= end; i++) {
            sortedArray[i] = array[i].val;
        }
        std::sort(sortedArray + start, sortedArray + end + 1);
        sorted[x] = true;

        return;
    }

    void Add(int l, int r, int x) {
        int startBelong = array[l].belong;  
        int endBelong = array[r].belong;

        // The range to add is contained in the the same block.
        if (startBelong == endBelong) {
            for (int i = l; i <= r; i++) {
                array[i].val += x;
                block[startBelong].sum += x;
                //block[endBelong].sum += x;
            }

            Sort(startBelong);
            //Sort(endBelong);

            return;
        }

        // The range to add spans different blocks.

        // Traverse the prefix incomplete block.
        for (int i = l; array[i].belong == startBelong; i++) {
            array[i].val += x;
            block[startBelong].sum += x;
        }
        
        // Traverse the complete block.
        for (int i = startBelong + 1; i < endBelong; i++) {
            block[i].tag += x;
            block[i].sum += x * s;
        }

        // Traverse suffix incomplete blocks.
        for (int i = r; array[i].belong == endBelong; i--) {
            array[i].val += x;
            block[endBelong].sum += x;
        }

        Sort(startBelong);
        Sort(endBelong);

        return;
    }

    int Greater(int l, int r, int x) {
        int ans = 0;
        int startBelong = array[l].belong;
        int endBelong = array[r].belong;

        if (startBelong == endBelong) {
            for (int i = l; i <= r; i++) {
                if (array[i].val + block[startBelong].tag >= x) {
                //if (array[i].val + block[endBelong].tag >= x) {
                    ans ++;
                }
            }

            return ans;
        }

        for (int i = l; i <= block[startBelong].end; i++) {
            int tag = block[startBelong].tag;
            if (array[i].val + tag >= x) {
                ans++;
            }
        }

        for (int i = startBelong + 1; i < endBelong; i++) {
            if (!sorted[i]) {
                Sort(i);
            }
            ans += block[i].end - (std::lower_bound(sortedArray + block[i].start, 
                                                    sortedArray + block[i].end + 1, 
                                                    x - block[i].tag) - sortedArray) + 1;
        }

        for (int i = block[endBelong].start; i <= r; i++) {
            int tag = block[endBelong].tag;
            if (array[i].val + tag >= x) {
                ans++;
            }
        }

        return ans;
    }
};

struct BlockArray blockArray;