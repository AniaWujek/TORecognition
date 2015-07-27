/*!
 * \file
 * \brief
 * \author Anna Wujek
 */

#ifndef KEYPOINTSDETECTOR_HPP_
#define KEYPOINTSDETECTOR_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "DataStream.hpp"
#include "Property.hpp"
#include "EventHandler2.hpp"

#include <opencv2/opencv.hpp>


namespace Processors {
namespace KeypointsDetector {

/*!
 * \class KeypointsDetector
 * \brief KeypointsDetector processor class.
 *
 *
 */
class KeypointsDetector: public Base::Component {
public:
	/*!
	 * Constructor.
	 */
	KeypointsDetector(const std::string & name = "KeypointsDetector");

	/*!
	 * Destructor
	 */
	virtual ~KeypointsDetector();

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
	Base::DataStreamOut<std::vector<KeyPoint>> out_keypoints;

	// Handlers

	// Properties
	Base::Property<std::string> prop_filename;
	Base::Property<bool> prop_read_on_init;


	// Handlers
	void onNewImage();

private:
    /// Returns keypoint with descriptors extracted from image.
    bool extractFeatures(const cv::Mat image_, std::vector<KeyPoint> & keypoints_, cv::Mat & descriptors_);

    /// Keypoint detector.
	Ptr<FeatureDetector> detector;

	/// Variable denoting current detector type - used for dynamic switching between detectors.
	int current_detector_type;

	///  Propery - type of keypoint detector: 0 - FAST (default), 1 - STAR , 2 - SIFT , 3 - SURF , 4 - ORB , 5 - BRISK , 6 - MSER , 7 - GFTT , 8 - GFTT, 9 - Dense, 10 - SimpleBlob
	Base::Property<int> prop_detector_type;

};

} //: namespace KeypointsDetector
} //: namespace Processors

/*
 * Register processor component.
 */
REGISTER_COMPONENT("KeypointsDetector", Processors::KeypointsDetector::KeypointsDetector)

#endif /* KEYPOINTSDETECTOR_HPP_ */
