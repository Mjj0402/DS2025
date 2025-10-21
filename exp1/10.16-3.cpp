#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
#include <cstdlib>
#include <ctime>

using namespace std;

// 计算柱状图中最大矩形面积
int largestRectangleArea(vector<int>& heights) {
    stack<int> s;
    s.push(-1);  // 作为哨兵，方便计算宽度
    int maxArea = 0;
    int n = heights.size();
    
    for (int i = 0; i < n; ++i) {
        // 当前高度小于栈顶高度时，弹出栈顶并计算面积
        while (s.top() != -1 && heights[i] < heights[s.top()]) {
            int height = heights[s.top()];
            s.pop();
            int width = i - s.top() - 1;
            maxArea = max(maxArea, height * width);
        }
        s.push(i);
    }
    
    // 处理栈中剩余的元素
    while (s.top() != -1) {
        int height = heights[s.top()];
        s.pop();
        int width = n - s.top() - 1;
        maxArea = max(maxArea, height * width);
    }
    
    return maxArea;
}

// 生成随机测试数据
vector<int> generateRandomHeights(int length) {
    vector<int> heights(length);
    for (int i = 0; i < length; ++i) {
        heights[i] = rand() % 10001;  // 0 <= heights[i] <= 10^4
    }
    return heights;
}

// 打印数组（长数组会简化显示）
void printArray(const vector<int>& arr) {
    cout << "[";
    int displayCount = min(10, (int)arr.size());
    for (int i = 0; i < displayCount; ++i) {
        if (i > 0) cout << ", ";
        cout << arr[i];
    }
    if (arr.size() > 10) {
        cout << ", ...";
    }
    cout << "]";
}

int main() {
    srand(time(0));  // 设置随机数种子
    
    // 示例测试
    vector<int> example = {2, 1, 5, 6, 2, 3};
    cout << "示例测试:" << endl;
    cout << "输入高度数组: ";
    printArray(example);
    cout << endl;
    cout << "最大矩形面积: " << largestRectangleArea(example) << endl << endl;
    
    // 随机生成10组测试数据
    cout << "随机测试数据 (10组):" << endl;
    for (int i = 0; i < 10; ++i) {
        // 生成1到10^5之间的随机长度
        int length = rand() % 100000 + 1;
        vector<int> heights = generateRandomHeights(length);
        
        cout << "测试数据 " << i + 1 << ":" << endl;
        cout << "数组长度: " << length << ", 高度数据: ";
        printArray(heights);
        cout << endl;
        
        // 计算最大矩形面积
        int maxArea = largestRectangleArea(heights);
        cout << "最大矩形面积: " << maxArea << endl << endl;
    }
    
    return 0;
}
    
