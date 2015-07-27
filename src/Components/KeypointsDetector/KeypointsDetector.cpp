/*!
 * \file
 * \brief
 * \author Anna Wujek
 */

#include <memory>
#include <string>

#include "KeypointsDetector.hpp"
#include "Common/Logger.hpp"

#include <boost/bind.hpp>

namespace Processors {
namespace KeypointsDetector {

KeypointsDetector::KeypointsDetector(const std::string & name) :
		Base::Component(name) ,
		prop_filename("prop_filename", std::string("")),
		prop_read_on_init("prop_read_on_init", true),
		prop_detector_type("keypoint_detector_type", 0) {
	registerProperty(prop_filename);
	registerProperty(prop_read_on_init);
	registerProperty(prop_detector_type);

}

KeypointsDetector::~KeypointsDetector() {
}

void KeypointsDetector::prepareInterface() {
	// Register data streams, events and event handlers HERE!
	registerStream("in_img", &in_img);
	registerStream("out_keypoints", &out_keypoints);
	// Register handlers
	registerHandler("onNewImage", boost::bind(&KeypointsDetector::onNewImage, this));
	addDependency("onNewImage", &in_img);

}

bool KeypointsDetector::onInit() {

    // Initialize detector.
	current_detector_type = -1;
	setKeypointDetector();
	return true;
}

bool KeypointsDetector::onFinish() {
	return true;
}

bool KeypointsDetector::onStop() {
	return true;
}

bool KeypointsDetector::onStart() {
	return true;
}

void KeypointsDetector::setKeypointDetector(){
	CLOG(LDEBUG) << "setKeypointDetector";
	// Check current detector type.
	if (current_detector_type == prop_detector_type)
		return;

	// Set detector.
	switch(prop_detector_type) {
		case 1:
			detector = FeatureDetector::create("STAR");
			CLOG(LNOTICE) << "Using STAR detector";
			break;
		case 2:
			detector = FeatureDetector::create("SIFT");
			CLOG(LNOTICE) << "Using SIFT detector";
			break;
		case 3:
			detector = FeatureDetector::create("SURF");
			CLOG(LNOTICE) << "Using SURF detector";
			break;
		case 4:
			detector = FeatureDetector::create("ORB");
			CLOG(LNOTICE) << "Using ORB detector";
			break;
		case 5:
			detector = FeatureDetector::create("BRISK");
			CLOG(LNOTICE) << "Using BRISK detector";
			break;

		case 6:
			detector = FeatureDetector::create("MSER");
			CLOG(LNOTICE) << "Using MSER detector";
			break;
		case 7:
			detector = FeatureDetector::create("GFTT");
			CLOG(LNOTICE) << "Using GFTT detector";
			break;
		case 8:
			detector = FeatureDetector::create("HARRIS");
			CLOG(LNOTICE) << "Using HARRIS detector";
			break;
		case 9:
			detector = FeatureDetector::create("Dense");
			CLOG(LNOTICE) << "Using Dense detector";
			break;
		case 10:
			detector = FeatureDetector::create("SimpleBlob");
			CLOG(LNOTICE) << "Using SimpleBlob detector";
			break;
		case 0 :
		default:
			detector = FeatureDetector::create("FAST");
			CLOG(LNOTICE) << "Using FAST detector";
			break;
	}//: switch
	// Remember current detector type.
	current_detector_type = prop_detector_type;

	// Reload the model.
	load_model_flag = true;
}


bool KeypointsDetector::extractFeatures(const cv::Mat image_, std::vector<KeyPoint> & keypoints_, cv::Mat & descriptors_) {
	CLOG(LTRACE) << "extractFeatures";
        cv::Mat gray_img;

	try {
		// Transform to grayscale - if requred.
		if (image_.channels() == 1)
			gray_img = image_;
		else
			cvtColor(image_, gray_img, COLOR_BGR2GRAY);

		// Detect the keypoints.
		detector->detect( gray_img, keypoints_ );

		return true;
	} catch (...) {
		CLOG(LWARNING) << "Could not extract features from image";
		return false;
	}//: catch
}

void KeypointsDetector::onNewImage() {

    try{
        setKeypointDetector();
        std::vector<KeyPoint> scene_keypoints;
        cv::Mat scene_descriptors;
        cv::Mat scene_img = in_img.read().clone();
        extractFeatures(scene_img, scene_keypoints, scene_descriptors);



    } catch (...) {
        CLOG(LERROR) << "onNewImage failed";
    }

}



} //: namespace KeypointsDetector
} //: namespace Processors
