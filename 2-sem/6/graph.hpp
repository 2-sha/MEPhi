#pragma once

#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <exception>
#include <fstream>
#include <math.h>

namespace graphviz
{
    #include <gvc.h>
    #include <cgraph.h>
}

class Graph
{
public:

    struct Point
    {
        int x = 0, y = 0;
        Point(const int x, const int y)
            : x(x), y(y) {};
        Point() {};
        double calcDist(const Point& p)
        {
            return sqrt(pow((p.x - this->x), 2) + pow((p.y - this->y), 2));
        }
    };

private:

    std::vector<std::vector<double>> matrix;
    std::map<std::string, int> names;
    std::vector<Point> coords;

public:

    void clear()
    {
        matrix.clear();
        names.clear();
        coords.clear();
    }

    void save(std::fstream& file)
    {
        int size = matrix.size();
        file.write((char*)&size, sizeof size);
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                file.write((char*)&matrix[i][j], sizeof matrix[i][j]);
            }
        }

        size = coords.size();
        file.write((char*)&size, sizeof size);
        for (int i = 0; i < size; i++)
            file.write((char*)&coords[i], sizeof Point);

        size = names.size();
        file.write((char*)&size, sizeof size);
        for (auto name : names)
        {
            int s = name.first.size();
            file.write((char*)&s, sizeof s);
            const char *str = name.first.c_str();
            file.write(str, s);
            int id = name.second;
            file.write((char*)&id, sizeof id);
        }
    }

    void read(std::fstream& file)
    {
        clear();
        int size;
        file.read((char*)&size, sizeof size);
        matrix.resize(size);
        for (int i = 0; i < size; i++)
        {
            matrix[i].resize(size);
            for (int j = 0; j < size; j++)
            {
                file.read((char*)&matrix[i][j], sizeof matrix[i][j]);
            }
        }

        file.read((char*)&size, sizeof size);
        for (int i = 0; i < size; i++)
        {
            Point point;
            file.read((char*)&point, sizeof Point);
            coords.push_back(point);
        }

        file.read((char*)&size, sizeof size);
        for (int i = 0; i < size; i++)
        {
            int s;
            file.read((char*)&s, sizeof s);
            char* buffer = new char[s + 1];
            file.read(buffer, s);
            buffer[s] = 0;
            int id;
            file.read((char*)&id, sizeof id);
            names[buffer] = id;
            delete[] buffer;
        }
    }

    std::vector<std::vector<double>> getMatrix()
    {
        return matrix;
    }

    std::map<std::string, int> getNames()
    {
        return names;
    }

    std::vector<Point> getPoints()
    {
        return coords;
    }

    void addNode(const int x, const int y, const std::string& name)
    {
        if (names.find(name) != names.end())
            throw std::invalid_argument("Вершина уже существует");
        coords.push_back(Point(x, y));
        names[name] = coords.size() - 1;
        matrix.push_back(std::vector<double>(matrix.size(), DBL_MAX));
        for (auto& it : matrix)
            it.push_back(DBL_MAX);
    }

    void addEdgees(const std::string& name, const std::vector<std::string>& nodes)
    {
        if (names.find(name) == names.end())
            throw std::invalid_argument("Вершина не существует");
        int id = names[name];
        Point p = coords[id];

        for (auto node : nodes)
        {
            auto nodeId = names.find(node);
            if (nodeId == names.end())
                continue;
            matrix[id][(*nodeId).second] = p.calcDist(coords[(*nodeId).second]);
        }
    }

    void removeNode(const std::string& name)
    {
        auto nameIt = names.find(name);
        if (nameIt == names.end())
            throw std::invalid_argument("Вершина не существует");
        int id = names[name];
        nameIt++;
        names.erase(name);
        while (nameIt != names.end())
        {
            (*nameIt).second--;
            nameIt++;
        }
        coords.erase(coords.begin() + id);
        matrix.erase(matrix.begin() + id);
        for (auto& it : matrix)
            it.erase(it.begin() + id);
    }

    void draw(const char* name, const char* ext)
    {
        graphviz::GVC_t* gvc = graphviz::gvContext();
        graphviz::Agraph_t* g = graphviz::agopen(_strdup("G"), graphviz::Agstrictdirected, NULL);
        graphviz::agattr(g, 1, (char*)"shape", (char*)"box");
        graphviz::agattr(g, 2, (char*)"label", (char*)"0");

        for (auto node : names)
        {
            int id = node.second;
            graphviz::Agnode_t* agn = agnode(g, (char*)std::to_string(id).c_str(), TRUE);
            graphviz::agset(agn, (char*)"label", (char*)(
                "(" + std::to_string(coords[id].x) + "; " + std::to_string(coords[id].y) + ")\n" + node.first
                ).c_str());
        }

        for (int i = 0; i < matrix.size(); i++)
        {
            for (int j = 0; j < matrix.size(); j++)
            {
                if (matrix[i][j] != DBL_MAX)
                {
                    graphviz::Agedge_t* edge = graphviz::agedge(g, agnode(g, (char*)std::to_string(i).c_str(), TRUE),
                        agnode(g, (char*)std::to_string(j).c_str(), TRUE), NULL, TRUE);
                    graphviz::agset(edge, (char*)"label", (char*)(
                        std::to_string((int)matrix[i][j]) + "." + std::to_string((int)(matrix[i][j] * 100) % 100)
                        ).c_str());
                }
            }
        }

        graphviz::gvLayout(gvc, g, "sfdp"); // fdp & sfdp
        graphviz::gvRenderFilename(gvc, g, ext, name);
        graphviz::gvFreeLayout(gvc, g);
        graphviz::agclose(g);
        graphviz::gvFreeContext(gvc);
    }

    std::vector<std::vector<double>> findShortcuts()
    {
        std::vector<std::vector<double>> res = matrix;

        for (int k = 0; k < res.size(); k++)
        {
            for (int i = 0; i < res.size(); i++)
            {
                for (int j = 0; j < res.size(); j++)
                {
                    res[i][j] = min(res[i][j], res[i][k] + res[k][j]);
                }
            }
        }

        return res;
    }

    bool isEmpty()
    {
        return names.empty();
    }

};