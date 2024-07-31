#include "m_vector.hpp"

#include <string>

using namespace m_std;
int main()
{
    vector<std::string> v_str;
    v_str.push_back("hello");
    v_str.push_back("world");
    v_str.emplace_back("emplaced");
    v_str.pop_back();
    v_str.push_back("!");

    v_str[0] = "hi";

    // iterator
    for (auto i = v_str.begin(); i != v_str.end(); ++i)
    {
        std::cout << *i << " ";
    }
    std::cout << std::endl;

    // test copy and move
    vector<std::string> v_str2 = v_str;
    vector<std::string> v_str3 = std::move(v_str);

    v_str2 = v_str3;
    v_str2 = std::move(v_str3);

    return 0;
}