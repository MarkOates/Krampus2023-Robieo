

#include <LabyrinthOfLore/WorldMap/Tile.hpp>




namespace LabyrinthOfLore
{
namespace WorldMap
{


Tile::Tile(int type, float height)
   : type(type)
   , height(height)
{
}


Tile::~Tile()
{
}


void Tile::set_type(int type)
{
   this->type = type;
}


void Tile::set_height(float height)
{
   this->height = height;
}


int Tile::get_type() const
{
   return type;
}


float Tile::get_height() const
{
   return height;
}


std::string Tile::run()
{
   return "Hello World!";
}


} // namespace WorldMap
} // namespace LabyrinthOfLore


