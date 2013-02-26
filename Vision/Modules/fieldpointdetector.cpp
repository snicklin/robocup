#include "fieldpointdetector.h"
#include "Vision/VisionTypes/coloursegment.h"
#include "Vision/VisionTypes/greenhorizon.h"
#include "Vision/visionblackboard.h"
#include <boost/foreach.hpp>
#include "debug.h"
#include "debugverbosityvision.h"

FieldPointDetector::FieldPointDetector(LineDetector *line_detector, CircleDetector *circle_detector, CornerDetector *corner_detector)
{
    m_line_detector = line_detector;
    m_circle_detector = circle_detector;
    m_corner_detector = corner_detector;
}

void FieldPointDetector::run() const
{
    VisionBlackboard* vbb = VisionBlackboard::getInstance();
    const Transformer& transformer = vbb->getTransformer();

    //check transforms are valid
    if(transformer.isScreenToGroundValid()) {
        vector<GroundPoint> points;
        GroundPoint temp;
        CentreCircle circle;
        vector<FieldLine> lines;
        vector<CornerPoint> corners;
        const GreenHorizon& gh = vbb->getGreenHorizon();
        //collect all vertical and horizontal line transition centres that exist under the green horizon
        BOOST_FOREACH(const ColourSegment& s, vbb->getVerticalTransitions(LINE_COLOUR)) {
            temp.screen = s.getCentre();
            if(gh.isBelowHorizon(temp.screen))
                points.push_back(temp);
        }
        BOOST_FOREACH(const ColourSegment& s, vbb->getHorizontalTransitions(LINE_COLOUR)) {
            temp.screen = s.getCentre();
            if(gh.isBelowHorizon(temp.screen))
                points.push_back(temp);
        }

        #if VISION_FIELDOBJECT_VERBOSITY > 1
        vector<Point> plotpts;
        BOOST_FOREACH(const GroundPoint& g, points) {
            plotpts.push_back(g.screen);
        }
        DataWrapper::getInstance()->plot("Screen coords", plotpts);
        #endif
        //map those points to the ground plane
        transformer.screenToGroundCartesian(points);

        #if VISION_FIELDOBJECT_VERBOSITY > 1
        plotpts.clear();
        BOOST_FOREACH(const GroundPoint& g, points) {
            plotpts.push_back(g.ground);
        }
        DataWrapper::getInstance()->plot("Ground coords", plotpts);
        #endif

        if(m_circle_detector) {
            //first attempt to find a centre circle
            if(m_circle_detector->run(points, circle)) {
                //circle found - points are already removed
                vbb->addCentreCircle(circle);

                #if VISION_FIELDOBJECT_VERBOSITY > 1
                vector<Point> circle_pts;
                double r = circle.getGroundRadius();
                Point c = circle.getLocation().ground;
                for(double theta = -mathGeneral::PI; theta < mathGeneral::PI; theta+=0.1)
                    circle_pts.push_back(Vector2<double>(r*cos(theta), r*sin(theta)) + c);
                DataWrapper::getInstance()->plot("Centre circle", circle_pts);
                #endif
            }
        }

        if(m_line_detector)
            lines = m_line_detector->run(points);


        #if VISION_FIELDOBJECT_VERBOSITY > 1
        plotpts.clear();
        BOOST_FOREACH(const FieldLine& l, lines) {
            Vector2<GroundPoint> ep = l.getEndPoints();
            plotpts.push_back(ep[0].ground);
            plotpts.push_back(ep[1].ground);
        }
        DataWrapper::getInstance()->plot("Lines", plotpts);
        #endif

        vbb->addLines(lines);

        //now find corners
        if(m_corner_detector)
            corners = m_corner_detector->run(lines);

        #if VISION_FIELDOBJECT_VERBOSITY > 1
        vector< Vector2<double> > cpts;
        BOOST_FOREACH(CornerPoint& cp, corners) {
            cpts.push_back(cp.getLocation().ground);
        }
        DataWrapper::getInstance()->plot("Corners", cpts);
        #endif

        vbb->addCornerPoints(corners);
    }
}