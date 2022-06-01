#pragma once
#include <ostream>
struct employee 
{
    int num;
    char name[10];
    double hours;
    void print(std::ostream& out) 
    {
        out << "ID " << num  << "  Name " << name << "  Hours: " << hours << std::endl;  
    }
};

int empCompare(const void* p1, const void* p2) 
{
    int cmp;
    cmp = ((employee*)p1)->num - ((employee*)p2)->num;
    return cmp;
}
