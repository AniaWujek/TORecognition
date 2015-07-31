/*!
 * \file
 * \brief
 * \author Anna Wujek
 */

#ifndef FEATUREMATCHER_HPP_
#define FEATUREMATCHER_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "DataStream.hpp"
#include "Property.hpp"
#include "EventHandler2.hpp"

#include <opencv2/opencv.hpp>


namespace Processors {
namespace FeatureMatcher {

/*!
 * \class FeatureMatcher
 * \brief FeatureMatcher processor class.
 *
 *
 */
class FeatureMatcher: public Base::Component {
public:
	/*!
	 * Constructor.
	 */
	FeatureMatcher(const std::string & name = "FeatureMatcher");

	/*!
	 * Destructor
	 */
	virtual ~FeatureMatcher();

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
	Base::DataStreamIn<std::vector<cv::Mat> > in_models_descriptors;
	Base::DataStreamIn<cv::Mat> in_scene_descriptors;
	Base::DataStreamIn<std::vector<cv::Mat> > in_models_imgs;
	Base::DataStreamIn<std::vector<std::vector<cv::KeyPoint> > > in_models_keypoints;
	Base::DataStreamIn<cv::Mat> in_scene_img;
	Base::DataStreamIn<std::vector<std::vector<cv::KeyPoint> > > in_scene_keypoints;

	// Output data streams
	Base::DataStreamOut<cv::Mat> out_img_all_correspondences;
	Base::DataStreamOut<std::vector<cv::DMatch> > out_matches;

	// Handlers

	// Properties
	Base::Property<int> prop_matcher_type;


	// Handlers
	void onNewImage();

};

} //: namespace FeatureMatcher
} //: namespace Processors

/*
 * Register processor component.
 */
REGISTER_COMPONENT("FeatureMatcher", Processors::FeatureMatcher::FeatureMatcher)

#endif /* FEATUREMATCHER_HPP_ */
