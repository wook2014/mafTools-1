#ifndef malnCompCursor_h
#define malnCompCursor_h
#include "malnComp.h"
#include "genome.h"
#include <stdbool.h>
#include <assert.h>

// FIXME: cursor should start out before the first position

/*
 * Column Cursor on a row of an alignment.
 */
struct malnCompCursor {
    struct malnComp *comp; 
    int alnIdx;             // alignment index, maybe set to end
    int pos;                // position, or next position if not aligned, or end
    char base;              // base at positon
    bool isAligned;         // is it aligned
};

/* make a new cursor at the start of a block */
static inline struct malnCompCursor malnCompCursor_make(struct malnComp *comp) {
    struct malnCompCursor cc;
    cc.comp = comp;
    cc.alnIdx = 0;
    cc.pos = comp->start;
    cc.base = malnComp_getCol(comp, 0);
    cc.isAligned = isBase(cc.base);
    return cc;
}

/* is the current position aligned */
static inline bool malnCompCursor_isAligned(struct malnCompCursor *cc) {
    return cc->isAligned;
}

/* is the current position at the end (after alignment) */
static inline bool malnCompCursor_atEnd(struct malnCompCursor *cc) {
    return cc->alnIdx == malnComp_getWidth(cc->comp);
}

/* increment the alignment cursor, return false at the end.
 * WARNING: cursor starts set at first position, not before */
static inline bool malnCompCursor_incr(struct malnCompCursor *cc) {
    assert(cc->alnIdx < malnComp_getWidth(cc->comp));
    if (cc->isAligned) {
        cc->pos++;
    }
    cc->alnIdx++;
    if (cc->alnIdx == malnComp_getWidth(cc->comp)) {
        // reached end
        cc->pos = cc->comp->end;
        cc->isAligned = false;
        return false;
    } else {
        cc->base = malnComp_getCol(cc->comp, cc->alnIdx); 
        cc->isAligned = isBase(cc->base);
        return true;
    }
}

/* set the alignment cursor to the specify alignment column. Can be set to
 * the end of the alignment (to width) */
static inline void malnCompCursor_setAlignCol(struct malnCompCursor *cc, int alnIdx) {
    assert(alnIdx <= malnComp_getWidth(cc->comp));
    if (cc->alnIdx > alnIdx) {
        *cc = malnCompCursor_make(cc->comp);  // reset to start
    }
    while (cc->alnIdx < alnIdx) {
        malnCompCursor_incr(cc);
    }
}

/* set the alignment cursor to the specify strand specific sequence position,
 * which maybe the end.*/
static inline void malnCompCursor_setSeqPos(struct malnCompCursor *cc, int pos) {
    assert((cc->comp->start <= pos) && (pos <= cc->comp->end));
    if (cc->pos > pos) {
        *cc = malnCompCursor_make(cc->comp);  // reset to start
    }
    while (!((cc->pos == pos) && (malnCompCursor_atEnd(cc) || malnCompCursor_isAligned(cc)))) {
        malnCompCursor_incr(cc);
    }
    assert(cc->pos == pos);
    assert(malnCompCursor_atEnd(cc) || malnCompCursor_isAligned(cc));
}

/* advance the alignment cursor to the next aligned position, return false at the end  */
static inline bool malnCompCursor_nextPos(struct malnCompCursor *cc) {
    assert(cc->alnIdx < malnComp_getWidth(cc->comp));
    do {
        if (!malnCompCursor_incr(cc)) {
            return false;
        }
    } while (!cc->isAligned);
    return true;
}

#endif
