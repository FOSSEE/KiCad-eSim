/******************/
/*  pad_shapes.h  */
/******************/

#ifndef PAD_SHAPES_H_
#define PAD_SHAPES_H_

/**
 * Enum PAD_SHAPE_T
 * is the set of pad shapes, used with D_PAD::{Set,Get}Shape()
 */
enum PAD_SHAPE_T
{
    PAD_SHAPE_CIRCLE,
    PAD_CIRCLE = PAD_SHAPE_CIRCLE,
    PAD_SHAPE_RECT,
    PAD_RECT = PAD_SHAPE_RECT,
    PAD_SHAPE_OVAL,
    PAD_OVAL = PAD_SHAPE_OVAL,
    PAD_SHAPE_TRAPEZOID,
    PAD_TRAPEZOID = PAD_SHAPE_TRAPEZOID
};

/**
 * Enum PAD_DRILL_SHAPE_T
 * is the set of pad drill shapes, used with D_PAD::{Set,Get}DrillShape()
 * The double name is for convenience of Python devs
 */
enum PAD_DRILL_SHAPE_T
{
    PAD_DRILL_SHAPE_CIRCLE,
    PAD_DRILL_SHAPE_OBLONG,
};


/**
 * Enum PAD_ATTR_T
 * is the set of pad shapes, used with D_PAD::{Set,Get}Attribute()
 * The double name is for convenience of Python devs
 */
enum PAD_ATTR_T
{
    PAD_ATTRIB_STANDARD,           ///< Usual pad
    PAD_STANDARD = PAD_ATTRIB_STANDARD,
    PAD_ATTRIB_SMD,                ///< Smd pad, appears on the solder paste layer (default)
    PAD_SMD = PAD_ATTRIB_SMD,
    PAD_ATTRIB_CONN,               ///< Like smd, does not appear on the solder paste layer (default)
    PAD_CONN = PAD_ATTRIB_CONN,
    PAD_ATTRIB_HOLE_NOT_PLATED,    ///< like PAD_STANDARD, but not plated
                            ///< mechanical use only, no connection allowed
    PAD_HOLE_NOT_PLATED = PAD_ATTRIB_HOLE_NOT_PLATED
};


#endif  // PAD_SHAPES_H_
