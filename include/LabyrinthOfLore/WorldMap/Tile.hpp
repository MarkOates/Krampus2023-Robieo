#pragma once


#include <string>


namespace LabyrinthOfLore
{
   namespace WorldMap
   {
      class Tile
      {
      private:
         int type;
         float height;

      protected:


      public:
         Tile(int type=-1, float height=0.0);
         ~Tile();

         void set_type(int type);
         void set_height(float height);
         int get_type() const;
         float get_height() const;
         std::string run();
      };
   }
}



