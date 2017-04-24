#ifndef PMSCONSTANTS_HPP
#define PMSCONSTANTS_HPP

const int DESCRIPTION_MAX_LENGTH = 38,
          SCENERY_NAME_MAX_LENGTH = 50,
          TEXTURE_NAME_MAX_LENGTH = 24;

const float MAP_BOUNDARY = 100.0f;
const int MAP_VERSION = 11;

const unsigned int MAX_POLYGONS_COUNT = 5000;
const unsigned int POLYGON_TYPES_COUNT = 24;

const unsigned int MAX_SPAWNPOINTS_COUNT = 255;

const int SECTORS_COUNT = 51; // Each map is a 51x51 grid of sectors.

#endif
