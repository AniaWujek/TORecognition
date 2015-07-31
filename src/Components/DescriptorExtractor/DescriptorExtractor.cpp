/*!
 * \file
 * \brief
 * \author Anna Wujek
 */

#include <memory>
#include <string>

#include "DescriptorExtractor.hpp"
#include "Common/Logger.hpp"

#include <boost/bind.hpp>

namespace Processors {
namespace DescriptorExtractor {

DescriptorExtractor::DescriptorExtractor(const std::string & name) :
		Base::Component(name) ,
		prop_filename("filename", std::string("")),
		prop_read_on_init("read_on_init", true),
		prop_detector_type("keypoint_detector_type", 0), //ok
		prop_extractor_type("descriptor_extractor_type", 0),
		prop_matcher_type("descriptor_matcher_type", 0),
		prop_returned_model_number("returned_model_number", 0),
		prop_recognized_object_limit("recognized_object_limit", 1) {
	registerProperty(prop_filename);
	registerProperty(prop_read_on_init);
	registerProperty(prop_detector_type);
	registerProperty(prop_extractor_type);
	registerProperty(prop_matcher_type);
	registerProperty(prop_returned_model_number);
	registerProperty(prop_recognized_object_limit);

}

DescriptorExtractor::~DescriptorExtractor() {
}

void DescriptorExtractor::prepareInterface() {
	// Register data streams, events and event handlers HERE!
	registerStream("in_img", &in_img);
	registerStream("in_models_keypoints", &in_models_keypoints);
	registerStream("in_models_imgs", &in_models_imgs);
	registerStream("in_models_names", &in_models_names);
	registerStream("in_scene_keypoints", &in_scene_keypoints);
	registerStream("out_scene_descriptors", &out_scene_descriptors);
	registerStream("out_models_descriptors", &out_models_descriptors);
	// Register handlers
	registerHandler("onNewImage", boost::bind(&DescriptorExtractor::onNewImage, this));
	addDependency("onNewImage", &in_img);
	addDependency("onNewImage", &in_models_imgs);
	addDependency("onNewImage", &in_models_names);

}

bool DescriptorExtractor::onInit() {

    // Initialize detector.
	current_extractor_type = -1;
	setDescriptorExtractor();

	return true;
}

bool DescriptorExtractor::onFinish() {
	return true;
}

bool DescriptorExtractor::onStop() {
	return true;
}

bool DescriptorExtractor::onStart() {
	return true;
}

void DescriptorExtractor::setDescriptorExtractor(){
	CLOG(LDEBUG) << "setDescriptorExtractor";
	// Check current extractor type.
	if (current_extractor_type == prop_extractor_type)
		return;

	// Set matcher.
	switch(prop_extractor_type) {
		case 1:
			extractor = cv::DescriptorExtractor::create("SURF");
			CLOG(LNOTICE) << "Using SURF descriptor";
			break;
		case 2:
			extractor = cv::DescriptorExtractor::create("BRIEF");
			CLOG(LNOTICE) << "Using BRIEF descriptor";
			break;
		case 3:
			extractor = cv::DescriptorExtractor::create("BRISK");
			CLOG(LNOTICE) << "Using BRISK descriptor";
			break;
		case 4:
			extractor = cv::DescriptorExtractor::create("ORB");
			CLOG(LNOTICE) << "Using ORB descriptor";
			break;
		case 5:
			extractor = cv::DescriptorExtractor::create("FREAK");
			CLOG(LNOTICE) << "Using FREAK descriptor";
			break;
		case 0 :
		default:
			extractor = cv::DescriptorExtractor::create("SIFT");
			CLOG(LNOTICE) << "Using SIFT descriptor";
			break;
	}//: switch
	// Remember current extractor type.
	current_extractor_type = prop_extractor_type;

	// Reload the model.
	//load_model_flag = true;
}

bool DescriptorExtractor::extractFeatures(const cv::Mat image_, std::vector<cv::KeyPoint> & keypoints_, cv::Mat & descriptors_) {
	CLOG(LTRACE) << "extractFeatures";


        cv::Mat gray_img;

	try {
		// Transform to grayscale - if requred.
		if (image_.channels() == 1)
			gray_img = image_;
		else
			cvtColor(image_, gray_img, cv::COLOR_BGR2GRAY);

		// Extract descriptors (feature vectors).
		extractor->compute( gray_img, keypoints_, descriptors_ );

		return true;
	} catch (...) {
		CLOG(LWARNING) << "Could not extract features from image";
		return false;
	}//: catch
}

void DescriptorExtractor::extractDescriptors() {
    models_descriptors.clear();
    cv::Mat model_img;
    for(int i = 0; i < models_imgs.size(); ++i) {
        cv::Mat model_descriptors;
		extractFeatures(models_imgs[i], models_keypoints[i], model_descriptors);
		models_descriptors.push_back(model_descriptors);
    }
}


void DescriptorExtractor::onNewImage() {

    CLOG(LTRACE) << "onNewImage";
    try {

        // Change keypoint detector type (if required).
        setDescriptorExtractor();

        models_imgs = in_models_imgs.read();
        models_names = in_models_names.read();
        models_keypoints = in_models_keypoints.read();
        scene_keypoints = in_scene_keypoints.read();

		cv::Mat scene_descriptors;

		// Load image containing the scene.
		cv::Mat scene_img = in_img.read().clone();

		extractDescriptors();



		// Extract features from scene.
		extractFeatures(scene_img, scene_keypoints, scene_descriptors);
		CLOG(LINFO) << "Scene features: " << scene_keypoints.size();

		std::vector< std::vector<cv::Mat> > out_descriptors;
		//out_keypoints.push_back(scene_keypoints);
		for(int i = 0; i < models_descriptors.size(); ++i) {
            out_descriptors.push_back(models_descriptors[i]);
		}
		out_models_descriptors.write(out_descriptors);
		out_scene_descriptors.write(scene_descriptors);




    } catch (...) {
        CLOG(LERROR) << "onNewImage failed";
    }

}



} //: namespace DescriptorExtractor
} //: namespace Processors
