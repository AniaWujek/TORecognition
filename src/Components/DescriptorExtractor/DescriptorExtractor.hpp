/*!
 * \file
 * \brief
 * \author Anna Wujek
 */

#ifndef DescriptorExtractor_HPP_
#define DescriptorExtractor_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "DataStream.hpp"
#include "Property.hpp"
#include "EventHandler2.hpp"

#include "Types/KeyPoints.hpp"

#include <opencv2/opencv.hpp>
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
	Base::DataStreamIn<std::vector<cv::Mat> > in_models_imgs;
	Base::DataStreamIn<std::vector<std::string> > in_models_names;
	Base::DataStreamIn<std::vector< std::vector<cv::KeyPoint> > > in_models_keypoints;
	Base::DataStreamIn<std::vector<cv::KeyPoint> > in_scene_keypoints;


	// Output data streams
	Base::DataStreamOut<std::vector< std::vector<cv::Mat> > > out_models_descriptors;
	Base::DataStreamOut<std::vector<cv::Mat> > out_scene_descriptors;

	// Handlers

	// Properties
	Base::Property<std::string> prop_filename;
	Base::Property<bool> prop_read_on_init;

	///  Propery - type of keypoint detector: 0 - FAST (default), 1 - STAR , 2 - SIFT , 3 - SURF , 4 - ORB , 5 - BRISK , 6 - MSER , 7 - GFTT , 8 - GFTT, 9 - Dense, 10 - SimpleBlob
	Base::Property<int> prop_detector_type;
	Base::Property<int> prop_extractor_type;
	Base::Property<int> prop_matcher_type;
	Base::Property<int> prop_returned_model_number;
	Base::Property<int> prop_recognized_object_limit;


	/// Feature descriptor
    cv::Ptr<cv::DescriptorExtractor> extractor;
	/// Sets the extreactor according to the current selection (see: prop_extractor_type).
	void setDescriptorExtractor();
	/// Variable denoting current extractor type - used for dynamic switching between extractors.
	int current_extractor_type;


	/// Returns keypoint extracted from image.
	bool extractFeatures(const cv::Mat image_, std::vector<cv::KeyPoint> & keypoints_, cv::Mat & descriptors_);

	void extractDescriptors();

	/// Vector of images constituting the consecutive models.
    std::vector<cv::Mat> models_imgs;
	/// Vector of keypoints of consecutive models.
    std::vector<std::vector<cv::KeyPoint> > models_keypoints;
	/// Vector of names of consecutive models.
    std::vector<std::string> models_names;
    /// Vector of descriptors of consecutive models.
    std::vector<cv::Mat> models_descriptors;
    std::vector<cv::KeyPoint> scene_keypoints;



	// Handlers
	void onNewImage();

};

} //: namespace DescriptorExtractor
} //: namespace Processors

/*
 * Register processor component.
 */
REGISTER_COMPONENT("DescriptorExtractor", Processors::DescriptorExtractor::DescriptorExtractor)

#endif /* DescriptorExtractor_HPP_ */
