
#include <iostream>
#include <limits>

int main()
{
    setlocale(LC_ALL, "RU");
    int a, b, h;
    // 
    std::cout << "Значение высоты h:";
    std::cin >> h;
    if (h > 255) {
        std::cout << "Ошибка, слишком большое значение";
        
    }
    else {
        std::cout << "Значение нижнего основания a:";
        std::cin >> a;
        std::cout << "Значение верхнего основания b:";
        std::cin >> b;
        if(a >1000 or b >1000) {
            std::cout << "Ошибка, слишком большое значение";

        }
        else {
            int s = ((a + b) / 2) * h;
            std::cout << "Площадь трапеции:" << s << std::endl;
            std::cout <<"int,занимает "<<128<<" бит\n"<< "min: " << std::numeric_limits<int>::min() << "\n"
                << "max: " << std::numeric_limits<int>::max() << "\n";
        }
        
        

    }
     

    
}

