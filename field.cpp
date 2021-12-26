#include "field.h"
#include "generator.h"
#include <algorithm>
#include <math.h>

Field::Field()
{

}

void Field::buildTable()
{
    m_tableSize = std::max(int(sqrt(m_points.size() / (2 * WIN))), 2);

    m_table.clear();
    m_table.resize(m_tableSize, std::vector<std::vector<int>>(m_tableSize));

    for (size_t i = 0; i < m_points.size(); i += 2)
    {
        int x = m_tableSize * m_points[i] / m_width;
        int y = m_tableSize * m_points[i + 1] / m_height;

        m_table[x][y].push_back(i / 2);
    }
}

void Field::generate(int num, int w, int h)
{
    m_width = w;
    m_height = h;

    m_points.resize(num * 2);
    Generator::generate(m_points, w, h);
    buildTable();
}

void Field::inRect(const rectangle_t &rec, std::vector<int>& ids)
{
    int x[2] = {
        (int) (m_tableSize * rec[0] / m_width),
        (int) (m_tableSize * rec[2] / m_width) };
    int y[2] = {
        (int) (m_tableSize * rec[1] / m_height),
        (int) (m_tableSize * rec[3] / m_height) };

    int s = std::max(x[0], 0);
    int n = std::min(x[1], m_tableSize - 1);

    for (int j = 0; j < 2; j++)
        if(y[j] < m_tableSize && y[j] >= 0)
            for (int i = s; i <= n; i++)
                for (const int& id : m_table[i][y[j]])
                    if(inRect(rec, id))
                        ids.push_back(id);

    s = std::max(y[0] + 1, 0);
    n = std::min(y[1], m_tableSize);

    for (int j = 0; j < 2; j++)
        if(x[j] < m_tableSize && x[j] >= 0)
            for (int i = s; i < n; i++)
                for (const int& id : m_table[x[j]][i])
                    if(inRect(rec, id))
                        ids.push_back(id);

    x[0] = x[0] >= 0 ? x[0] + 1 : 0;
    y[0] = y[0] >= 0 ? y[0] + 1 : 0;
    x[1] = x[1] > m_tableSize ? m_tableSize : x[1];
    y[1] = y[1] > m_tableSize ? m_tableSize : y[1];

    for (int i = x[0]; i < x[1]; i++)
        for (int j = y[0]; j < y[1]; j++)
            for (const int& id : m_table[i][j])
                ids.push_back(id);
}

bool Field::inRect(const rectangle_t &rec, int id)
{
    return (m_points[2 * id] >= rec[0] &&
            m_points[2 * id + 1] >= rec[1] &&
            m_points[2 * id] <= rec[2] &&
            m_points[2 * id + 1] <= rec[3]);
}
