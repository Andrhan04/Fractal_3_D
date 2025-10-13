#include <iostream>
#include "..\Templates\Fractal.h"
#include "..\Templates\Figure.h"
#include "..\Templates\Point.h"

using namespace std;

int main()
{
    cout << "Hello World!\n";   
    Figure f = Figure();
    cout << f.get_param() << endl;
    double d = 100.0;
    Figure new_f = Figure(f, d);
    cout << new_f.get_param();
    cout << "-------------------------------------------------------------\n\tFractal\n" << endl;
    Fractal frac = Fractal();
    cout << frac.get_info() << endl;
    cout << "----------------------------------------------------------" << "\nPoint Chek\n";
    frac = Fractal(&f, &new_f);
    cout << frac.get_info() << endl;
    return 0;
}
