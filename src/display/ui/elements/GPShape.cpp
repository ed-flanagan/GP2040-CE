#include "GPShape.h"

void GPShape::draw() {
    uint16_t baseX = this->x;
    uint16_t baseY = this->y;

    // scale to viewport
    double scaleX = this->getScaleX();
    double scaleY = this->getScaleY();

    // set scale on X & Y to be proportionate if either is 0
    // NOTE: `else if` enforces mutual exclusion (i.e. when both x/y = 1.0f, or
    //       if x is 1.0, then y is set to 1
    if ((scaleX > 0.0f) && ((scaleY == 0.0f) || (scaleY == 1.0f))) {
        scaleY = scaleX;
    } else if ((scaleY > 0.0f) && ((scaleX == 0.0f) || (scaleX == 1.0f))) {
        scaleX = scaleY;
    }

    GPGFX_DisplayMetrics* m = getRenderer()->getDriver()->getMetrics();
    GPViewport vp           = this->getViewport();

    uint16_t offsetX = (m->width - ((vp.right - vp.left) * scaleX)) / 2;
    uint16_t offsetY = (m->height - ((vp.bottom - vp.top) * scaleY)) / 2;

    if (scaleX > 0.0f) {
        baseX = baseX * scaleX + vp.left + offsetX;
    }

    // NOTE: scaled Y does not include offsetY
    if (scaleY > 0.0f) {
        baseY = baseY * scaleY + vp.top;
    }

    uint16_t scaledSize, baseRadius;
    scaledSize = baseRadius = (uint16_t)((double)this->_sizeX * scaleX);

    // NOTE: scaled Y does not include offsetY
    uint16_t scaledX = (this->_sizeX) * scaleX + vp.left + offsetX;
    uint16_t scaledY = (this->_sizeY) * scaleY + vp.top;

    // base
    switch (this->_shape) {
        case GP_SHAPE_ELLIPSE:
            getRenderer()->drawEllipse(baseX, baseY, baseRadius, baseRadius, this->strokeColor, this->fillColor);
            break;
        case GP_SHAPE_SQUARE:
            getRenderer()->drawRectangle(baseX, baseY, scaledX, scaledY, this->strokeColor, this->fillColor,
                                         this->_angle);
            break;
        case GP_SHAPE_LINE:
            getRenderer()->drawLine(baseX, baseY, this->_sizeX, this->_sizeY, this->strokeColor, 0);
            break;
        case GP_SHAPE_POLYGON:
            getRenderer()->drawPolygon(baseX, baseY, baseRadius, this->_sizeY, this->strokeColor, this->fillColor,
                                       this->_angle);
            break;
        case GP_SHAPE_ARC:
            getRenderer()->drawArc(baseX, baseY, baseRadius, baseRadius, this->strokeColor, this->fillColor,
                                   this->_angle, this->_angleEnd, this->_closed);
            break;
        case GP_SHAPE_PILL:
            getRenderer()->drawPill(baseX, baseY, scaledX, scaledY, this->strokeColor, this->fillColor, this->_angle);
            break;
    }
}
