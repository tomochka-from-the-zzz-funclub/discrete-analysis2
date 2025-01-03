#include <iostream>
#include <fstream>
#include <string>
using namespace std;
class person{
    protected:
        short age;
        char name[80];
    public:
    void get_Data(){
        cin >> name >> age;
    }
    void write_file(string filename) {
        std::ofstream file; // создаем объект ofstream
        file.open(filename, std::ios::out); // открываем файл для записи

        if (file.is_open()) { // если файл успешно открыт
            
            file << this->name << " " << this->age;

            file.close(); // закрываем файл
        } else {
            std::cout << "Не удалось открыть файл для записи." << std::endl;
        }
    }
};
int main() {
    
    std::string filename = "test_1"; // переменная для имени файла
    person pers; // переменная для данных, которые будут записаны в файл
    pers.get_Data();
    
    pers.write_file(filename);


    return 0;
}
