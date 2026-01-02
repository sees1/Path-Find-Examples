#include "utils/vector_map_app/map_data.hpp"

#include "utils/vector_map_app/map.hpp"

#include <fstream>

std::ostream& operator<<(std::ostream& out, QPolygonF& obj)
{
  for (const QPointF& p : obj)
    out << "(" << std::to_string(p.x()) << "," << std::to_string(p.y()) << ")" << " ";

  return out;
}

MapData::MapData(const QPoint& offset, float resolution)
: map_offset_(offset),
  map_resolution_(resolution)
{ }

bool MapData::hasCostmapInfo()
{
  return (map_offset_.has_value() && map_resolution_.has_value());
}

bool MapData::setCostmapInfo(float resolution, const QPoint& offset)
{
  map_offset_ = offset;
  map_resolution_ = resolution;

  return true;
}

void MapData::saveTo(const QString& filename, Map* map)
{
  std::ofstream file(filename.toStdString());

  if (!file.is_open())
    throw std::runtime_error("Can't open file to save roads!");

  std::map<Vertice, std::vector<Vertice>> graph = map->getGraph();

  file << "Graph:" << std::endl;
  for(const auto& [vertice, adj_vertices] : graph)
  {
    file << std::to_string(vertice.id) << "(" << std::to_string(vertice.coord.x()) << "," << std::to_string(vertice.coord.y()) << ")" << "-";
    
    for(const auto& v : adj_vertices)
      file << std::to_string(v.id) << "(" << std::to_string(vertice.coord.x()) << "," << std::to_string(vertice.coord.y()) << ")" << " ";

    file << std::endl;
  }

  file << std::endl;

  std::vector<std::vector<std::tuple<Vertice, Vertice, QPolygonF, int>>> polygons = map->getGraphPolygons();

  size_t polygons_count = polygons.size();

  size_t poly_idx = 0;
  for(size_t i = 0; i < polygons_count; ++i)
  {
    for(size_t j = 0; j < polygons[i].size(); ++j)
    {
      QPolygonF poly;
      Vertice lhs, rhs;
      int type;
      std::tie(lhs, rhs, poly, type) = polygons[i];

      file << "Polygon-" << std::to_string(i) << "-" 
                         << std::to_string(lhs) << "-"
                         << std::to_string(rhs) << "-"
                         << std::to_string(type) ":" << " " << poly << std::endl;
    }
  }
}

void MapData::loadTo(const QString& filename, Map* map)
{
  std::ifstream file(filename.toStdString());

  if (!file.is_open())
    throw std::runtime_error("Can't open file to load roads!");

  std::string line;

  bool graph_process = true;

  std::map<Vertice, std::vector<Vertice>> graph;
  std::vector<std::vector<std::tuple<Vertice, Vertice, QPolygonF, int>>> polygons;
  int max_id = -1;

  while (std::getline(file, line))
  {
    if (line.size() == 0)
    {
      graph_process = false;
      countinue;
    }
    
    size_t delim_pos = line.find(":");
    if (delim_pos != std::string::npos)
    {
      // we found start phrase (Graph: or Polygon-poly_id-graph_id-graph_id:)
      if (graph_process)
      {
        std::string delim_pos_rhs = line.substr(++delim_pos, line.size());

        bool first_glance = true;
        while(delim_pos != std::string::npos)
        {
          delim_pos = delim_pos_rhs.find("(");
          std::string id = delim_pos_rhs.substr(delim_pos);
          delim_pos_rhs = delim_pos_rhs.substr(++delim_pos, delim_pos_rhs.size());

          delim_pos = delim_pos_rhs.find(",");
          std::string coord_x = delim_pos_rhs.substr(delim_pos);
          delim_pos_rhs = delim_pos_rhs.substr(++delim_pos, delim_pos_rhs.size());

          delim_pos = delim_pos_rhs.find(")");
          std::string coord_y = delim_pos_rhs.substr(delim_pos);
          delim_pos_rhs = delim_pos_rhs.substr(++delim_pos, delim_pos_rhs.size());
          
          if (first_glance)
          {
            delim_pos = delim_pos_rhs.find("-");
            delim_pos_rhs = delim_pos_rhs.substr(++delim_pos, delim_pos_rhs.size());

            // TODO: add here try catch
            QPointF coord(std::stod(coord_x), std::stod(coord_y));
            Vertice main {std::stoi(id), coord};
            graph[main] = std::vector();
          }
          else
          {
            delim_pos = delim_pos_rhs.find(" ");
            delim_pos_rhs = delim_pos_rhs.substr(++delim_pos, delim_pos_rhs.size());

            QPointF coord(std::stod(coord_x), std::stod(coord_y));
            Vertice temp(std::stoi(id), coord);

            graph[main].push_back(temp);
          }
        }
      }
      else
      {
        std::string delim_pos_lhs = line.substr(delim_pos);

        size_t delim_pos_slave = delim_pos_lhs.find("-");
        delim_pos_lhs = delim_pos_lhs.substr(++delim_pos_slave, delim_pos_lhs.size());

        delim_pos_slave = delim_pos_lhs.find("-");
        std::string main_id = delim_pos_lhs.substr(delim_pos_slave);
        delim_pos_lhs = delim_pos_lhs.substr(++delim_pos_slave, delim_pos_lhs.size());

        delim_pos_slave = delim_pos_lhs.find("-");
        std::string lhs_id = delim_pos_lhs.substr(delim_pos_slave);
        delim_pos_lhs = delim_pos_lhs.substr(++delim_pos_slave, delim_pos_lhs.size());

        delim_pos_slave = delim_pos_lhs.find("-");
        std::string rhs_id = delim_pos_lhs.substr(delim_pos_slave);
        delim_pos_lhs = delim_pos_lhs.substr(++delim_pos_slave, delim_pos_lhs.size());
        std::string type = delim_pos_lhs;

        std::string delim_pos_rhs = line.substr(++delim_pos);

        QPolygonF poly;

        while(delim_pos != std::string::npos)
        {
          delim_pos = delim_pos_rhs.find("(");
          delim_pos_rhs = delim_pos_rhs.substr(++delim_pos);

          delim_pos = delim_pos_rhs.find(",");
          std::string coord_x = delim_pos_rhs.substr(delim_pos);
          delim_pos_rhs = delim_pos_rhs.substr(++delim_pos, delim_pos_rhs.size());

          delim_pos = delim_pos_rhs.find(")");
          std::string coord_y = delim_pos_rhs.substr(delim_pos);
          delim_pos_rhs = delim_pos_rhs.substr(++delim_pos, delim_pos_rhs.size());

          QPointF coord(std::stod(coord_x), std::stod(coord_y));

          poly.push_back(coord);
        }

        int main_id = std::stoi(main_id);
        if (main_id > max_id)
        {
          polygons.resize(main_id);
        }

        polygons[main_id].push_back(std::tie(std::stoi(lhs_id), std::stoi(rhs_id), poly, std::stoi(type)));
      }
    }
    else
    {
      std::string delim_pos_rhs = line;

      bool first_glance = true;
      while(delim_pos != std::string::npos)
      {
        delim_pos = delim_pos_rhs.find("(");
        std::string id = delim_pos_rhs.substr(delim_pos);
        delim_pos_rhs = delim_pos_rhs.substr(++delim_pos, delim_pos_rhs.size());

        delim_pos = delim_pos_rhs.find(",");
        std::string coord_x = delim_pos_rhs.substr(delim_pos);
        delim_pos_rhs = delim_pos_rhs.substr(++delim_pos, delim_pos_rhs.size());

        delim_pos = delim_pos_rhs.find(")");
        std::string coord_y = delim_pos_rhs.substr(delim_pos);
        delim_pos_rhs = delim_pos_rhs.substr(++delim_pos, delim_pos_rhs.size());
        
        if (first_glance)
        {
          delim_pos = delim_pos_rhs.find("-");
          delim_pos_rhs = delim_pos_rhs.substr(++delim_pos, delim_pos_rhs.size());

          // TODO: add here try catch
          QPointF coord(std::stod(coord_x), std::stod(coord_y));
          Vertice main {std::stoi(id), coord};
          graph[main] = std::vector();
        }
        else
        {
          delim_pos = delim_pos_rhs.find(" ");
          delim_pos_rhs = delim_pos_rhs.substr(++delim_pos, delim_pos_rhs.size());

          QPointF coord(std::stod(coord_x), std::stod(coord_y));
          Vertice temp(std::stoi(id), coord);

          graph[main].push_back(temp);
        }
      }
    } 
  }

  map->loadRoads(polygons);
  map->loadGraph(graph);
}