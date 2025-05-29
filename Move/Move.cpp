#include <iostream>
#include "..\Templates\Point.h"

int main()
{
    Point p = Point(1,1,1);
    Point p1 = Point(2, 2, 2);
    auto p_res = p + p1;
    std::cout << p_res.Info();
}
