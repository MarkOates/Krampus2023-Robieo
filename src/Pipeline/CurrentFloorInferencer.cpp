

#include <Pipeline/CurrentFloorInferencer.hpp>

#include <cmath>


namespace Pipeline
{


CurrentFloorInferencer::CurrentFloorInferencer()
   : elevationMap({})
{
}


CurrentFloorInferencer::~CurrentFloorInferencer()
{
}


int CurrentFloorInferencer::find_closest_floor(float playerPosition)
{
   // TODO: Add tests
   if (elevationMap.empty())
   {
      // Handle empty map case
      throw std::runtime_error("CurrentFloorInferencer:: elevationMap is empty");
      //std::cerr << "Error: Map of elevations is empty." << std::endl;
      return -1;  // You might want to handle this differently based on your requirements
   }

   // Initialize variables to keep track of the closest elevation and its index
   int closestIndex = elevationMap.begin()->first;
   float closestElevation = elevationMap.begin()->second;
   float minDifference = std::abs(playerPosition - closestElevation);

   // Iterate through the map of elevations
   for (const auto& entry : elevationMap)
   {
      float currentDifference = std::abs(playerPosition - entry.second);

      // Update the closest elevation and index if the current one is closer
      if (currentDifference < minDifference)
      {
         minDifference = currentDifference;
         closestElevation = entry.second;
         closestIndex = entry.first;
      }
   }

   return closestIndex;
}


} // namespace Pipeline


