#pragma once
#include <QPixMap>
#include <QGraphicsPixmapItem>
#include "Defines.h"

/*
 * The tile class represents a single tile in a graphical environment.
 * Each tile has:
 * - A pointer (`ptr`) to a QGraphicsPixmapItem, representing its graphical component.
 * - A boolean (`isOccupied`) that indicates whether the tile is currently occupied.
 * - An integer (`color`) representing the tile's color (valid range: 0-7).
 *
 * The class provides constructors for initialization, getters for accessing tile properties,
 * and setters for modifying tile attributes while ensuring proper validation.
 */
class tile
{
private:
	QGraphicsPixmapItem* ptr;	// pointer to the tile QGraphicsItem object
	bool isOccupied;	// is the tile occupied (false - no, true - yes)
	int color;	// the colour of the tile (values: 0-7)
public:
	/*
	 * Default constructor for the tile class.
	 * Initializes the tile with default values:
	 * - `ptr` is set to nullptr (no graphical representation yet)
	 * - `isOccupied` is set to false (tile is not occupied)
	 * - `color` is set to 0 (default color)
	 */
	tile();

	/*
	 * Parameterized constructor for the tile class.
	 * @param x The x-coordinate of the tile.
	 * @param y The y-coordinate of the tile.
	 *
	 * Initializes the tile and attempts to load a texture from a resource file.
	 * If successful, it assigns a section of the texture to `ptr` and positions it accordingly.
	 * If the texture cannot be loaded, a warning is issued.
	 */
	tile(int x, int y);

	/*
	 * Returns a pointer to the QGraphicsPixmapItem representing the tile.
	 * If the pointer is not initialized, a warning is issued.
	 * @return A pointer to the tile's graphical representation.
	 */
	QGraphicsPixmapItem* getPtr();

	/*
	 * Retrieves the occupation status of the tile.
	 * @return True if the tile is occupied, otherwise false.
	 */
	bool getIsOccupied();

	/*
	 * Changes the occupation status of the tile.
	 * @param b The new occupation status (true = occupied, false = not occupied).
	 *
	 * Only updates the status if `ptr` is initialized. If `ptr` is null, a warning is issued.
	 */
	void changeTileStatus(bool b);

	/*
	 * Retrieves the color of the tile.
	 * @return The integer representing the tile color.
	 */
	int get_color() { return color; }

	/*
	 * Sets the color of the tile if it falls within the valid range (0-7).
	 * @param c The new color value.
	 */
	void set_color(int c) {
		if (c >= 0 && c <= 7) color = c;
	}
};

