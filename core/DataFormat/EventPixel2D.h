/**
 * \file EventPixel2D.h
 *
 * \ingroup DataFormat
 * 
 * \brief Class def header for a class EventPixel2D
1;2c *
 * @author kazuhiro
 */

/** \addtogroup DataFormat

    @{*/
#ifndef EVENTPIXEL2D_H
#define EVENTPIXEL2D_H

#include <iostream>
#include <map>
#include "EventBase.h"
#include "Pixel2D.h"
#include "Pixel2DCluster.h"
#include "DataProductFactory.h"
namespace larcv {
  
  /**
    \class EventPixel2D
    Event-wise class to store a collection of larcv::Pixel2D and larcv::Pixel2DCluster
  */
  class EventPixel2D : public EventBase {
    
  public:
    
    /// Default constructor
    EventPixel2D(){}
    
    /// Default destructor
    virtual ~EventPixel2D(){}

    /// Clears an array of larcv::Pixel2D
    void clear();

    /// Retrieve Pixel2D for a plane
    const std::vector<larcv::Pixel2D>& Pixel2DArray(const ::larcv::PlaneID_t plane);
    /// Retrieve Pixel2DCluster for a plane
    const std::vector<larcv::Pixel2DCluster>& Pixel2DClusterArray(const ::larcv::PlaneID_t plane);

    /// Insert larcv::Pixel2D into a collection
    void Append(const larcv::PlaneID_t plane, const Pixel2D& pixel);
    /// Insert larcv::Pixel2DCluster into a collection
    void Append(const larcv::PlaneID_t plane, const Pixel2DCluster& cluster);
    
#ifndef __CINT__
    /// Emplace larcv::Pixel2D into a collection
    void Emplace(const larcv::PlaneID_t plane, Pixel2D&& pixel);
    /// Emplace larcv::Pixel2DCluster into a collection
    void Emplace(const larcv::PlaneID_t plane, Pixel2DCluster&& cluster);
#endif

  private:

    std::map< ::larcv::PlaneID_t, std::vector< ::larcv::Pixel2D > >        _pixel_m;
    std::map< ::larcv::PlaneID_t, std::vector< ::larcv::Pixel2DCluster > > _cluster_m;

  };

  /**
     \class larcv::EventPixel2D
     \brief A concrete factory class for larcv::EventPixel2D
  */
  class EventPixel2DFactory : public DataProductFactoryBase {
  public:
    /// ctor
    EventPixel2DFactory() { DataProductFactory::get().add_factory(kProductPixel2D,this); }
    /// dtor
    ~EventPixel2DFactory() {}
    /// create method
    EventBase* create() { return new EventPixel2D; }
  };

}

#endif
/** @} */ // end of doxygen group 

