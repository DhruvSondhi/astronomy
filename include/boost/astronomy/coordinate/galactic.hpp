#ifndef BOOST_ASTRONOMY_COORDINATE_GALACTIC_HPP
#define BOOST_ASTRONOMY_COORDINATE_GALACTIC_HPP

#include <boost/geometry/core/cs.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/algorithms/transform.hpp>
#include <boost/geometry/algorithms/equals.hpp>

#include <boost/is_base_template_of.hpp>
#include <boost/astronomy/coordinate/base_representation.hpp>
#include <boost/astronomy/coordinate/base_frame.hpp>
#include <boost/astronomy/coordinate/spherical_representation.hpp>
#include <boost/astronomy/coordinate/spherical_coslat_differential.hpp>

namespace boost
{
    namespace astronomy
    {
        namespace coordinate
        {

            template <typename RepresentationDegreeOrRadian = degree,
                typename DifferentialDegreeOrRadian = degree>
            struct galactic: public boost::astronomy::coordinate::base_frame
                <boost::astronomy::coordinate::spherical_representation<RepresentationDegreeOrRadian>,
                boost::astronomy::coordinate::spherical_coslat_differential<DifferentialDegreeOrRadian>>
            {
            public:
                //default constructor no initialization
                galactic() {}

                //creates coordinate in galactic frame using any subclass of base_representation
                template <typename Representation>
                galactic(Representation const& representation_data)
                {
                    BOOST_STATIC_ASSERT_MSG((boost::is_base_template_of
                        <boost::astronomy::coordinate::base_representation, Representation>::value),
                        "Invalid representation class");

                    this->data = representation_data;
                }

                //creates coordinate from given values
                //b -> latitude, l -> longitude
                galactic(double b, double l, double distance)
                {
                    this->data.set_lat_lon_dist(b, l, distance);
                }

                //creates coordinate with motion from given values
                //b -> latitude, l -> longitude
                //pm_b -> proper motion in b, pm_l_cosb -> proper motion in l including cos(b) 
                galactic(double b, double l, double distance, double pm_b, double pm_l_cosb, double radial_velocity)
                {
                    this->data.set_lat_lon_dist(b, l, distance);

                    this->motion.set_dlat_dlon_coslat_ddist(pm_b, pm_l_cosb, radial_velocity);
                }

                //creates coordinate with motion
                //representation class is used for coordinate data
                //differential class is used for motion data
                template <typename Representation, typename Differential>
                galactic(Representation const& representation_data, Differential const& diff)
                {
                    BOOST_STATIC_ASSERT_MSG((boost::is_base_template_of
                        <boost::astronomy::coordinate::base_representation, Representation>::value),
                        "argument type is expected to be a differential class");
                    this->data = representaion_data;

                    BOOST_STATIC_ASSERT_MSG((boost::is_base_template_of
                        <boost::astronomy::coordinate::base_differential, Differential>::value),
                        "argument type is expected to be a differential class");
                    this->motion = diff;
                }

                //copy constructor
                galactic(galactic const& other)
                {
                    this->data = other.get_data();
                    this->motion = other.get_differential();
                }

                //returns component b of the galactic coordinate
                double get_b() const
                {
                    return boost::geometry::get<0>(this->data.get_point());
                }

                //returns component l of the galactic coordinate
                double get_l() const
                {
                    return boost::geometry::get<1>(this->data.get_point());
                }

                //returns distance component of the galactic coordinate
                double get_distance() const
                {
                    return boost::geometry::get<2>(this->data.get_point());
                }

                //returns proper motion in galactic latitude
                double get_pm_b() const
                {
                    return boost::geometry::get<0>(this->motion.get_differential());
                }

                //returns proper motion in galactic longitude including cos(b)
                double get_pm_l_cosb() const
                {
                    return boost::geometry::get<1>(this->motion.get_differential());
                }

                //returns radial_velocity
                double get_radial_velocity() const
                {
                    return boost::geometry::get<2>(this->motion.get_differential());
                }

                //sets value of component b of the galactic coordinate
                double set_b(double b)
                {
                    boost::geometry::set<0>(this->data.get_point(), b);
                }

                //sets value of component l of the galactic coordinate
                double set_l(double l) const
                {
                    boost::geometry::set<1>(this->data.get_point(), l);
                }

                //sets value of distance component of the galactic coordinate
                double set_distance(double distance)
                {
                    boost::geometry::set<2>(this->data.get_point(), distance);
                }

                //sets the proper motion in galactic latitude
                double set_pm_b(double pm_b)
                {
                    boost::geometry::set<0>(this->motion.get_differential(), pm_b);
                }

                //sets the proper motion in galactic longitude including cos(b)
                double set_pm_l_cosb(double pm_l_cosb)
                {
                    boost::geometry::set<1>(this->motion.get_differential(), pm_l_cosb);
                }

                //sets the radial_velocity
                double set_radial_velocity(double radial_velocity)
                {
                    boost::geometry::set<2>(this->motion.get_differential(), radial_velocity);
                }
            };
        } //namespace coordinate
    } //namespace astronomy
} //namespace boost
#endif // !BOOST_ASTRONOMY_COORDINATE_GALACTIC_HPP
