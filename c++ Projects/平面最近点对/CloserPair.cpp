// author : zhoukang
// date   : 2022-12-26 11:09:30

#include <sys/time.h>

#include <vector>
#include <cmath>
#include <float.h>  // FLT_MAX
#include <iostream> // std::cout

class CostTime {
public:
    bool reset = false;
    struct  timeval startTime;
    struct  timeval  curTime;
    void Reset() {
        reset = true;
        gettimeofday(&startTime, nullptr);
    }
    double Elapsed()
    {
        if(!reset) {
            Reset();
            return 0.0f;
        }

        gettimeofday(&curTime, nullptr);
        double elapsed = (double)(curTime.tv_sec-startTime.tv_sec) + (curTime.tv_usec-startTime.tv_usec)/1000000;
        startTime = curTime;
        return elapsed;
    }
};

class Point2D {
public:
    float x = 0.0f, y = 0.0f;
public:
    Point2D(float _x, float _y) : x(_x), y(_y)
    {};

    float Distance(const Point2D& pt) const
    {
        float dx = pt.x - x, dy = pt.y - y;
        return std::sqrt(dx * dx + dy * dy);
    }
};
struct Point3D: public Point2D{
public:
    float z;

    Point3D(float _x, float _y, float _z) : Point2D(_x, _y), z(_z)
    {};

    float Distance(const Point3D& pt) const
    {
        float dx = pt.x - x, dy = pt.y - y, dz = pt.z - z;
        return std::sqrt(dx * dx + dy * dy + dz * dz);
    }
};
using std::vector;
using PairPointId = std::pair<int, int>;
// 一个距离值，对应多个点对
using Result = float;

const float maxInfinity = FLT_MAX;
const float minInfinity = FLT_MIN;

// Y坐标已经升序
Result Optimize(const vector<Point2D>& leftPt, const vector<Point2D>& rightPt, Result minDistance)
{
    int leftPtNum = leftPt.size();
    int rightPtNum = rightPt.size();

    int leftStart = 0, leftEnd = leftPtNum;
    int rightStart = 0, rightEnd = rightPtNum;
    if(leftPt[0].y >= rightPt[rightPtNum-1].y) {  // 左边点全部高于右边点
        for(int i = leftStart; i < leftEnd; ++i) {
            for(int j = rightEnd - 1; j >= rightStart; --j) {
                if(leftPt[i].y - rightPt[j].y <=minDistance) { // 判断距离
                    Result distance = leftPt[i].Distance(rightPt[j]);
                    if(distance < minDistance) minDistance = distance;
                } else { // 更新rightPt的rightStart值
                    rightStart = j + 1;
                    break;
                }
            }
            if(rightStart >= rightEnd){
                break;
            }
        }
    } else if(leftPt[leftPtNum - 1].y <= rightPt[0].y) { // 左边点全部低于右边点
        for(int i = leftEnd - 1; i >= leftStart; ++i) {
            for(int j = rightStart; j < rightEnd; ++j) {
                if(rightPt[j].y - leftPt[i].y <= minDistance) { // 判断实际距离
                    Result distance = leftPt[i].Distance(rightPt[j]);
                    if(distance < minDistance) minDistance = distance;
                } else {
                    rightEnd = j;
                    break;
                }
            }
            if(rightEnd <= rightStart) {
                break;
            }
        }
    } else if (leftPt[0].y >= rightPt[0].y &&
                leftPt[leftPtNum-1].y <= rightPt[rightPtNum-1].y) { // 重叠，且左子域的y范围完全在右子域的y范围内


        Result dYstart = leftPt[0].y - rightPt[0].y;
        Result dYend = leftPt[leftPtNum-1].y - rightPt[rightPtNum-1].y;

        // i = 0时，j从1开始
        for(int j = 1; j < rightEnd; ++j) {
            Result dy = leftPt[0].y - rightPt[j]
        }

        for(int i = leftStart; i < leftEnd; ++i) {
            for(int j = rightStart; j < rightEnd; ++j) {

            }
        }

    }


    // 左右子域各取一点判断
    int startY = 0;
    int endY = rightPt.size();
    for(auto lIter = leftPt.begin(); lIter!=leftPt.end(); ++i, ++lIter) {
        for(auto  rIter = rightPt.begin() + startY; rIter < rightPt.begin() + endY; ++rIter) {
            if(fabs(lIter->y - rIter->y) < minDistance ) {
                float distance = lIter->Distance(*rIter);
                minDistance = std::min(distance, minDistance);
            }
        }
    }
}

Result Merge(const vector<Point2D>& ptArr, int left, int midPos, int right, Result lResult, Result rResult)
{
    Result minDistance = std::min(lResult, rResult);

    vector<Point2D> leftPt, rightPt;
    int i = 0;
    // 处理左子域
    int firstPos = midPos;
    for(i = left; i < midPos; ++i) {
        if (ptArr[midPos].x - ptArr[i].x <= minDistance) {
            firstPos = i;
            break;
        }
    }

    if(firstPos >= midPos)
        return minDistance;
    else {
        leftPt.insert(leftPt.end(), ptArr.begin() + firstPos, ptArr.begin() + midPos);
    }

    // 处理右子域
   firstPos = midPos;
    for(i = midPos; i < right; ++i) {
        if (ptArr[i].x - ptArr[midPos].x > minDistance) {
            firstPos = i;
            break;
        }
    }
    if(i == right)
        firstPos = right;

    if(firstPos == midPos)
        return minDistance;
    else {
        rightPt.insert(rightPt.end(), ptArr.begin() + midPos, ptArr.begin() + firstPos);
    }

    // 按Y排序
    auto compareY = [](const Point2D& leftPt, const Point2D& rightPt) {
        return leftPt.y < rightPt.y;
    };
    std::sort(leftPt.begin(), leftPt.end(), compareY);
    std::sort(rightPt.begin(), rightPt.end(), compareY);

    // 左右子域各取一点判断
    int startY = 0;
    int endY = rightPt.size();
    for(auto lIter = leftPt.begin(); lIter!=leftPt.end(); ++i, ++lIter) {
        for(auto  rIter = rightPt.begin() + startY; rIter < rightPt.begin() + endY; ++rIter) {
            if(fabs(lIter->y - rIter->y) < minDistance ) {
                float distance = lIter->Distance(*rIter);
                minDistance = std::min(distance, minDistance);
            }
        }
    }

    return minDistance;
}

// [left, right) 左闭右开,right位置为哨兵
Result CloserPair(vector<Point2D>& ptArr, int left, int right)
{
    if (left >= right ||
        right - left == 1) { // 此时只有一个点，是无法构成距离的
        return maxInfinity;
    } else if (right - left == 2) { // 比如[1,3)，此时有两个点pts[1], pts[2]
        vector<PairPointId> pairPointIdArr = {std::make_pair(left, left + 1)};
        return ptArr[left].Distance(ptArr[left + 1]);
    }

    // 实际上是向上取整操作，(4+9)/2=6.5，取整为7
    // 不使用ceil的原因：ceil的参数是浮点数，而我给的值是整数，ceil返回值是浮点数，而我要的是整数
    // [left, right) = [4, 9)，mid = 7, 新的区间为[4,7)和[7,9)
    int midPos = (left + right) % 2 == 0 ? (left + right) / 2 : (left + right + 1) / 2;
    Result lMinDistance = CloserPair(ptArr, left, midPos);
    Result rMinDistance = CloserPair(ptArr, midPos, right);

    return Merge(ptArr, left, midPos, right, lMinDistance, rMinDistance);
}

float CloserPair2(vector<Point2D>& ptArr)
{
    float minDistance = maxInfinity;
    int ptSize = ptArr.size();
    int step = ptSize / 100;
    for(int i = 0; i < ptSize; ++i) {
        if(i % step == 0){
            printf("进度:%d\n", i/step);
        }
        for(int j = 0; j < ptSize; ++j) {
            if ( i == j)
                continue;
            float distance = ptArr[i].Distance(ptArr[j]);
            if(distance < minDistance)
                minDistance = distance;
        }
    }
    return minDistance;
}

void PrintPtArr(const vector<Point2D>& ptArr) {
    return;
    int size = ptArr.size();
    int showCnt = 100;
    int step = size / 100;
    for(int i = 0; i < ptArr.size(); ++i) {
        if(i % step == 0)
            std::cout << i/step << ")\t" << ptArr[i].x <<"\t," << ptArr[i].y << std::endl;
    }
}

int main(int argc, char** argv)
{
    int ptSize = 1000000;
    if(argc == 2) {
        ptSize = atoi(argv[1]);
    }
    printf("ptSize = %d\n", ptSize);
    int maxValue = ptSize * 1000;
    vector<Point2D> ptArr;
    ptArr.reserve(ptSize); // 给定100个点
    srand((unsigned int)time(nullptr));
    for(int i = 0; i<ptSize; ++i) {
        float x = (rand() % maxValue) / 1.0f;
        float y = (rand() % maxValue) / 1.0f;
        ptArr.emplace_back(Point2D(x, y));
    }

    //PrintPtArr(ptArr);
    CostTime cost;
    //cost.Reset();
    //auto minDistance = CloserPair2(ptArr);
    //std::cout << "暴力算法，minDistance:" << minDistance << ", 耗时(s):" << cost.Elapsed() << std::endl;

    cost.Reset();
    // 按照x为第一优先级排序
    std::sort(ptArr.begin(), ptArr.end(), [](const Point2D& leftPt, const Point2D& rightPt){
        return leftPt.x < rightPt.x;
    });
    auto cost1 = cost.Elapsed();
    PrintPtArr(ptArr);
    cost.Reset();
    auto minDistance = CloserPair(ptArr, 0, ptArr.size() - 1);
    std::cout << "分治算法，minDistance:" << minDistance << ", 耗时(s):" << cost.Elapsed() + cost1 << std::endl;


}

