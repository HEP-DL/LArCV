#ifndef __DATUMFILLERBASE_CXX__
#define __DATUMFILLERBASE_CXX__

#include "DatumFillerBase.h"
#include <sstream>
namespace larcv {

  DatumFillerBase::DatumFillerBase(const std::string name)
    : ProcessBase(name)
    , _nentries(kINVALID_SIZE)
    , _image_producer_id(kINVALID_PRODUCER)
    , _label_producer_id(kINVALID_PRODUCER)
  {}

  void DatumFillerBase::configure(const PSet& cfg)
  {
    _image_producer = cfg.get<std::string>("ImageProducer");
    _label_producer = cfg.get<std::string>("LabelProducer");
    this->child_configure(cfg);
  }

  void DatumFillerBase::initialize()
  {
    if(_nentries == kINVALID_SIZE) {
      LARCV_CRITICAL() << "# entries not set... must be set @ initialize! " << std::endl;
      throw larbys();
    }
    _current_entry = kINVALID_SIZE;
    _entry_image_size = _entry_label_size = kINVALID_SIZE;
    _image_producer_id = kINVALID_PRODUCER;
    _label_producer_id   = kINVALID_PRODUCER;
    this->child_initialize();
  }

  void DatumFillerBase::batch_begin() {
    if(_entry_image_size != kINVALID_SIZE) {
      _image_data.resize(_nentries * _entry_image_size);
      _label_data.resize(_nentries * _entry_label_size);
    }
    for(auto& v : _image_data) v=0.;
    for(auto& v : _label_data) v=0.;
    _current_entry = 0;
    this->child_batch_begin();
  }

  bool DatumFillerBase::process(IOManager& mgr)
  {
    if(_current_entry == kINVALID_SIZE) {
      LARCV_CRITICAL() << "batch_begin() not called... Note this process is only meant to use with ThreadReadDriver!\n";
      throw larbys();
    }

    if(_image_producer_id == kINVALID_PRODUCER) {

      _image_producer_id = mgr.producer_id(_image_product_type,_image_producer);
      if(_image_producer_id == kINVALID_PRODUCER) {
        LARCV_CRITICAL() << "Image producer " << _image_producer << " not valid!" << std::endl;
        throw larbys();
      }

      _label_producer_id = mgr.producer_id(_label_product_type,_label_producer);
      if(_label_producer_id == kINVALID_PRODUCER) {
        LARCV_CRITICAL() << "Label producer " << _label_producer << " not valid!" << std::endl;
        throw larbys();
      }
    }

    auto const image_data = mgr.get_data(_image_producer_id);
    auto const label_data = mgr.get_data(_label_producer_id);

    if(_entry_image_size==kINVALID_SIZE) {
      _entry_image_size = compute_image_size(image_data);
      _entry_label_size = compute_label_size(label_data);
      if( _entry_image_size == kINVALID_SIZE || _entry_label_size == kINVALID_SIZE ) {
        LARCV_CRITICAL() << "Rows/Cols/NumChannels not set!" << std::endl;
        throw larbys();
      }
      _image_data.resize(_entry_image_size * _nentries, 0.);
      _label_data.resize(_entry_label_size * _nentries, 0.);
    }

    this->fill_entry_data(image_data,label_data);

    auto const& entry_image_data = entry_data(true);
    auto const& entry_label_data = entry_data(false);

    if(entry_image_data.size() != _entry_image_size) {
      LARCV_CRITICAL() << "(run,subrun,event) = ("
      << image_data->run() << "," << image_data->subrun() << "," << image_data->event() << ")"
      << "Entry image size should be " << _entry_image_size << " but found " << entry_image_data.size() << std::endl;
      throw larbys();
    }

    if(entry_label_data.size() != _entry_label_size) {
      LARCV_CRITICAL() << "(run,subrun,event) = ("
      << label_data->run() << "," << label_data->subrun() << "," << label_data->event() << ")"
      << "Entry label size should be " << _entry_label_size << " but found " << entry_label_data.size() << std::endl;
      throw larbys();
    }

    const size_t current_image_index_start = _current_entry * _entry_image_size;
    for(size_t i = 0; i<_entry_image_size; ++i)
      _image_data[current_image_index_start + i] = entry_image_data[i];

    const size_t current_label_index_start = _current_entry * _entry_label_size;
    for(size_t i = 0; i<_entry_label_size; ++i)
      _label_data[current_label_index_start + i] = entry_label_data[i];

    ++_current_entry;
    return true;
  }

  void DatumFillerBase::batch_end()
  { 
    this->child_batch_end(); 
  }

  void DatumFillerBase::finalize()
  { this->child_finalize(); }

}
#endif
