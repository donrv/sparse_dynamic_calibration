#ifndef EDGE_GICP_HPP
#define EDGE_GICP_HPP

#include <Eigen/Dense>
#include <g2o/core/base_binary_edge.h>
#include <g2o/types/sba/types_six_dof_expmap.h>

namespace g2o {

class ICPCorrespondence
{
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

public:
    // point positions
    Eigen::Vector3d pos0, pos1;

    // unit normals
    Eigen::Vector3d normal0, normal1;

    // initialize an object
    ICPCorrespondence()
    {
        pos0.setZero();
        pos1.setZero();
        normal0 << 0, 0, 1;
        normal1 << 0, 0, 1;
    }

};

class EdgeICP : public  BaseBinaryEdge<3, ICPCorrespondence, VertexSE3Expmap, VertexSE3Expmap>
{
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    EdgeICP(bool point_to_plane = false)
        : point_to_plane(point_to_plane)
    {}
    virtual ~EdgeICP() override {}

    // I/O functions
    virtual bool read(std::istream& is) override { return true; }
    virtual bool write(std::ostream& os) const override { return true; }

    // return the error estimate as a 3-vector
    virtual void computeError() override
    {
        // from <ViewPoint> to <Point>
        const VertexSE3Expmap *vp0 = static_cast<const VertexSE3Expmap*>(_vertices[0]);
        const VertexSE3Expmap *vp1 = static_cast<const VertexSE3Expmap*>(_vertices[1]);

        // get vp1 point into vp0 frame
        g2o::SE3Quat se3 = vp0->estimate() * vp1->estimate().inverse();
        Eigen::Vector3d p1 = se3 * measurement().pos1;

        Eigen::Vector3d diff = _error = p1 - measurement().pos0;
        _error = diff;

        if(point_to_plane) {
            Eigen::Vector3d n1 = se3.rotation() * measurement().normal1;
            _error = diff.array() * measurement().normal0.array() + diff.array() * n1.array();
        }
    }

    bool point_to_plane;
};


}

#endif // GICP_EDGE_HPP
