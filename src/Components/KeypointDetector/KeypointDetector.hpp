/*!
 * \file
 * \brief
 * \author Anna Wujek
 */

#ifndef KeypointDETECTOR_HPP_
#define KeypointDETECTOR_HPP_

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
namespace KeypointDetector {

/*!
 * \class KeypointDetector
 * \brief KeypointDetector processor class.
 *
 *
 */
class KeypointDetector: public Base::Component {
public:
	/*!
	 * Constructor.
	 */
	KeypointDetector(const std::string & name = "KeypointDetector");

	/*!
	 * Destructor
	 */
	virtual ~KeypointDetector();

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
	Base::DataStreamOut<std::vector< std::vector<cv::KeyPoint> > > out_scene_keypoints;

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


	/// Keypoint detector.
	cv::Ptr<cv::FeatureDetector> detector;
	/// Sets the keypoint detector according to the current selection (see: prop_detector_type).
	void setKeypointDetector();
	/// Variable denoting current detector type - used for dynamic switching between detectors.
	int current_detector_type;

	/// Flag used for loading models.
	bool load_model_flag;
	/// Re-load the models from files, detect and extract their features.
	void loadModels();
	/// Load a single model from file indicated by function parameter.
	void loadSingleModel(std:: string filename_, std::string name_);
	/// Returns keypoint extracted from image.
	bool extractFeatures(const cv::Mat image_, std::vector<cv::KeyPoint> & keypoints_);
	/// Loads image from file.
	bool loadImage(const std::string filename_, cv::Mat & image_);

	/// Vector of images constituting the consecutive models.
    std::vector<cv::Mat> models_imgs;
	/// Vector of keypoints of consecutive models.
    std::vector<std::vector<cv::KeyPoint> > models_keypoints;
	/// Vector of names of consecutive models.
    std::vector<std::string> models_names;


	// Handlers
	void onNewImage();

};

} //: namespace KeypointDetector
} //: namespace Processors

/*
 * Register processor component.
 */
REGISTER_COMPONENT("KeypointDetector", Processors::KeypointDetector::KeypointDetector)

#endif /* KeypointDETECTOR_HPP_ */
