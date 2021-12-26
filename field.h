#ifndef FIELD_H
#define FIELD_H
#include<array>
#include<vector>

typedef std::array<double, 4> rectangle_t;

class Field
{
public:
    const static int WIN = 4;
    std::vector<double> m_points;

    Field();

    void generate(int num, int w, int h);
    void inRect(const rectangle_t& rec, std::vector<int>& ids);
    bool inRect(const rectangle_t& rec, int id);

private:
    void buildTable();
    std::vector<std::vector<std::vector<int>>> m_table;
    int m_tableSize;
    int m_height;
    int m_width;
};

#endif // FIELD_H
