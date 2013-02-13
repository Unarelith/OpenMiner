#ifndef MAP_H
#define MAP_H

#define MAP_POS(x, y, z) ((int)((x) + ((y) * m_map->width) + ((z) * m_map->width * m_map->depth)))

typedef struct {
	u16 width;
	u16 depth;
	u16 height;
	u16 *map;
} Map;

#endif // MAP_H
