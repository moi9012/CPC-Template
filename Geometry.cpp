#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
using namespace std;

typedef long double ld;
// 精度容差：几何题的灵魂！1e-9 适用于绝大多数坐标范围在 1e4~1e5 的题目
// 如果坐标范围极大（如 1e9），可能需要将 eps 放大到 1e-7
const ld eps = 1e-9;

// 判断浮点数是否为 0（核心防坑函数）
bool isZero(ld x) { return fabs(x) < eps; }
// 浮点数比较：a < b, a > b, a == b
bool less_(ld a, ld b) { return a < b - eps; }
bool greater_(ld a, ld b) { return a > b + eps; }
bool equal_(ld a, ld b) { return fabs(a - b) < eps; }

const ld PI = acos(-1.0L);

// ================== 核心结构体 ==================
struct Point {
    ld x, y;
    Point(ld x = 0, ld y = 0) : x(x), y(y) {}
    
    // 运算符重载，让代码更像数学公式
    Point operator+(const Point &p) const { return Point(x + p.x, y + p.y); }
    Point operator-(const Point &p) const { return Point(x - p.x, y - p.y); }
    Point operator*(ld k) const { return Point(x * k, y * k); } // 数乘
    Point operator/(ld k) const { return Point(x / k, y / k); }
    
    bool operator==(const Point &p) const { return equal_(x, p.x) && equal_(y, p.y); }
    bool operator<(const Point &p) const { 
        return less_(x, p.x) || (equal_(x, p.x) && less_(y, p.y)); 
    }
};

// 点积 (Dot Product): 用于求夹角、判断垂直、求投影
ld dot(Point a, Point b) { return a.x * b.x + a.y * b.y; }

// 叉积 (Cross Product): 几何题最核心函数！
// 用于判断方向、求面积、判断点在直线/线段哪侧
ld cross(Point a, Point b) { return a.x * b.y - a.y * b.x; }

// 向量长度（模）
ld length(Point a) { return sqrtl(a.x * a.x + a.y * a.y); }
// 两点距离
ld dist(Point a, Point b) { return length(a - b); }

// 向量旋转（逆时针旋转 rad 弧度）
Point rotate(Point a, ld rad) {
    return Point(a.x * cosl(rad) - a.y * sinl(rad), 
                 a.x * sinl(rad) + a.y * cosl(rad));
}

// ================== 基础几何判断 ==================

// 1. 判断点 P 是否在线段 AB 上 (要求 A, B, P 不重合)
bool pointOnSegment(Point p, Point a, Point b) {
    // 先判断共线（叉积为0），再判断是否在包围盒内（点积）
    return isZero(cross(p - a, b - a)) && 
           less_((p - a).x * (p - b).x + (p - a).y * (p - b).y, eps);
}

// 2. 判断线段 AB 与线段 CD 是否相交 (包含端点相交)
bool segmentIntersect(Point a, Point b, Point c, Point d) {
    // 快速排斥实验（包围盒相交）
    if (max(a.x, b.x) < min(c.x, d.x) - eps) return false;
    if (max(c.x, d.x) < min(a.x, b.x) - eps) return false;
    if (max(a.y, b.y) < min(c.y, d.y) - eps) return false;
    if (max(c.y, d.y) < min(a.y, b.y) - eps) return false;
    
    // 跨立实验（叉积判断）
    return cross(b - a, c - a) * cross(b - a, d - a) < eps &&
           cross(d - c, a - c) * cross(d - c, b - c) < eps;
}

// 3. 点到直线的距离 (直线由 A, B 两点确定)
ld pointToLineDist(Point p, Point a, Point b) {
    return fabs(cross(p - a, b - a)) / length(b - a);
}

// 4. 点到线段的距离
ld pointToSegmentDist(Point p, Point a, Point b) {
    if (dot(p - a, b - a) < -eps) return length(p - a); // 投影在 A 外侧
    if (dot(p - b, a - b) < -eps) return length(p - b); // 投影在 B 外侧
    return pointToLineDist(p, a, b);                    // 投影在线段内
}

// ================== 多边形相关 ==================

// 5. 多边形面积 (输入点集需按逆时针或顺时针顺序排列)
// 返回绝对值，若需判断方向（逆时针为正），可去掉 fabs
ld polygonArea(vector<Point> &p) {
    ld area = 0;
    int n = p.size();
    for (int i = 0; i < n; ++i) {
        area += cross(p[i], p[(i + 1) % n]);
    }
    return fabs(area) / 2.0L;
}

// 6. 判断点 P 是否在多边形内部 (射线法，O(n))
// 返回 1: 内部, 0: 边界上, -1: 外部
int pointInPolygon(Point p, vector<Point> &poly) {
    int n = poly.size(), res = -1;
    for (int i = 0, j = n - 1; i < n; j = i++) {
        if (pointOnSegment(p, poly[i], poly[j])) return 0; // 在边界上
        // 射线法核心：判断水平射线是否穿过边
        if (equal_(poly[i].y, poly[j].y)) continue; // 平行于射线的边跳过
        if (greater_(poly[i].y, p.y) != greater_(poly[j].y, p.y)) {
            // 计算交点 x 坐标，判断是否在 p 的右侧
            ld x_intersect = poly[i].x + (p.y - poly[i].y) / (poly[j].y - poly[i].y) * (poly[j].x - poly[i].x);
            if (greater_(x_intersect, p.x)) res = -res;
        }
    }
    return res;
}

// ================== 凸包 (Convex Hull) ==================
// 使用 Andrew 单调链算法，时间复杂度 O(n log n)
// 返回逆时针顺序的凸包顶点集
vector<Point> convexHull(vector<Point> p) {
    int n = p.size(), k = 0;
    if (n < 3) return p; // 少于3个点直接返回
    sort(p.begin(), p.end());
    vector<Point> h(2 * n);
    
    // 构建下凸壳
    for (int i = 0; i < n; ++i) {
        while (k >= 2 && cross(h[k-1] - h[k-2], p[i] - h[k-2]) <= eps) k--; // 非严格凸包用 <=，严格凸包用 <
        h[k++] = p[i];
    }
    
    // 构建上凸壳
    for (int i = n - 2, t = k + 1; i >= 0; --i) {
        while (k >= t && cross(h[k-1] - h[k-2], p[i] - h[k-2]) <= eps) k--;
        h[k++] = p[i];
    }
    
    h.resize(k - 1); // 最后一个点和第一个点重合，去掉
    return h;
}

// ================== 主函数测试 ==================
int main() {
    // 示例：计算三角形面积
    vector<Point> tri = {Point(0, 0), Point(4, 0), Point(0, 3)};
    cout << "Triangle Area: " << polygonArea(tri) << endl; // 应为 6
    
    // 示例：求凸包
    vector<Point> points = {
        Point(0, 0), Point(1, 1), Point(2, 2), 
        Point(2, 0), Point(0, 2), Point(1, 0)
    };
    vector<Point> hull = convexHull(points);
    cout << "Convex Hull Size: " << hull.size() << endl; // 应为 3 (0,0), (2,0), (0,2)
    
    return 0;
}