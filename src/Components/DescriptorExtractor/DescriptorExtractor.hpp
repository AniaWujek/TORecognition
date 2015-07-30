/*!
 * \file
 * \brief
 * \author Anna Wujek
 */

#ifndef DESCRIPTOREXTRACTOR_HPP_
#define DESCRIPTOREXTRACTOR_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "DataStream.hpp"
#include "Property.hpp"
#include "EventHandler2.hpp"

#include <opencv2/opencv.hpp>
#include "Types/KeyPoints.hpp"

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>


namespace Processors {
namespace DescriptorExtractor {

/*!
 * \class DescriptorExtractor
 * \brief DescriptorExtractor processor class.
 *
 *
 */
class DescriptorExtractor: public Base::Component {
public:
	/*!
	 * Constructor.
	 */
	DescriptorExtractor(const std::string & name = "DescriptorExtractor");

	/*!
	 * Destructor
	 */
	virtual ~DescriptorExtractor();

	/*!
	 * Prepare components interface (register streams and handlers).
	 * At this point, all properties are already initialized and loaded to
	 * values set in config file.
	 */
	void prepareInterface();

protected:

	/*!
	 * Connects source to given device.
	 */
	bool onInit();

	/*!
	 * Disconnect source from device, closes streams, etc.
	 */
	bool onFinish();

	/*!
	 * Start component
	 */
	bool onStart();

	/*!
	 * Stop component
	 */
	bool onStop();


	// Input data streams
	Base::DataStreamIn<cv::Mat> in_img;

	// Output data streams
	Base::DataStreamOut<std::vector<cv::Mat> > out_descriptors;

	// Handlers

	// Properties
	Base::Property<std::string> prop_filename;
	Base::Property<bool> prop_read_on_init;
	Base::Property<int> prop_extractor_type;


	// Handlers
	void onNewImage();

	/// Feature descriptor
    cv::Ptr<cv::DescriptorExtractor> extractor;
	/// Sets the extreactor according to the current selection (see: prop_extractor_type).
	void setDescriptorExtractor();
	/// Variable denoting current extractor type - used for dynamic switching between extractors.
	int current_extractor_type;

	/// Flag used for loading models.
	bool load_model_flag;


};

} //: namespace DescriptorExtractor
} //: namespace Processors

/*
 * Register processor component.
 */
REGISTER_COMPONENT("DescriptorExtractor", Processors::DescriptorExtractor::DescriptorExtractor)

#endif /* DESCRIPTOREXTRACTOR_HPP_ */
