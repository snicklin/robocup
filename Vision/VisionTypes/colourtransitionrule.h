/*!
  * @file colourrule.h
  * @class ColourRule
  * @author Shannon Fenn
  * @date 23-03-12
  *
  * @brief Class to use for simplified but flexible segment rule matching.
  *
  */

#ifndef COLOURTRANSITIONRULE_H
#define COLOURTRANSITIONRULE_H

#include <vector>
#include <iostream>

#include "visionblackboard.h"

#include "VisionTools/classificationcolours.h"
#include "VisionTypes/coloursegment.h"
#include "VisionTypes/visionfieldobject.h"

using namespace std;

class ColourTransitionRule
{
public:
    static ColourSegment nomatch;

    ColourTransitionRule();
    bool match(const ColourSegment& before, const ColourSegment& after, ScanDirection dir) const;
    VisionFieldObject::VFO_ID getVFO_ID() const;

    friend ostream& operator<< (ostream& output, const ColourTransitionRule& c);
    friend ostream& operator<< (ostream& output, const vector<ColourTransitionRule>& v);

    friend istream& operator>> (istream& input, ColourTransitionRule& c);
    friend istream& operator>> (istream& input, vector<ColourTransitionRule>& v);

private:
    //vector<VisionFieldObject::VFO_ID> m_potential_vfo_list;
    VisionFieldObject::VFO_ID m_vfo_id;

    vector<ClassIndex::Colour>  m_before,
                                m_after;
    float   m_before_min,
            m_before_max,
            m_after_min,
            m_after_max;
};

#endif // COLOURTRANSITIONRULE_H
