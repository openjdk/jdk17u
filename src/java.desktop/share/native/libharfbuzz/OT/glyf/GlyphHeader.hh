#ifndef OT_GLYF_GLYPHHEADER_HH
#define OT_GLYF_GLYPHHEADER_HH


#include "../../hb-open-type.hh"


namespace OT {
namespace glyf_impl {


struct GlyphHeader
{
  bool has_data () const { return numberOfContours; }

  template <typename accelerator_t>
  bool get_extents (hb_font_t *font, const accelerator_t &glyf_accelerator,
                    hb_codepoint_t gid, hb_glyph_extents_t *extents) const
  {
    /* Undocumented rasterizer behavior: shift glyph to the left by (lsb - xMin), i.e., xMin = lsb */
    /* extents->x_bearing = hb_min (glyph_header.xMin, glyph_header.xMax); */
    extents->x_bearing = font->em_scale_x (glyf_accelerator.hmtx->get_side_bearing (gid));
    extents->y_bearing = font->em_scale_y (hb_max (yMin, yMax));
    extents->width     = font->em_scale_x (hb_max (xMin, xMax) - hb_min (xMin, xMax));
    extents->height    = font->em_scale_y (hb_min (yMin, yMax) - hb_max (yMin, yMax));

    return true;
  }

  HBINT16       numberOfContours;
                    /* If the number of contours is
                     * greater than or equal to zero,
                     * this is a simple glyph; if negative,
                     * this is a composite glyph. */
  FWORD xMin;   /* Minimum x for coordinate data. */
  FWORD yMin;   /* Minimum y for coordinate data. */
  FWORD xMax;   /* Maximum x for coordinate data. */
  FWORD yMax;   /* Maximum y for coordinate data. */
  public:
  DEFINE_SIZE_STATIC (10);
};


} /* namespace glyf_impl */
} /* namespace OT */


#endif /* OT_GLYF_GLYPHHEADER_HH */
