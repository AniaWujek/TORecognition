/*!
 * \file
 * \brief
 * \author Anna Wujek
 */

#include <memory>
#include <string>

#include "FeatureMatcher.hpp"
#include "Common/Logger.hpp"

#include <boost/bind.hpp>

namespace Processors {
namespace FeatureMatcher {

FeatureMatcher::FeatureMatcher(const std::string & name) :
		Base::Component(name) , 
		prop_matcher_type("prop_matcher_type", 0) {
	registerProperty(prop_matcher_type);

}

FeatureMatcher::~FeatureMatcher() {
}

void FeatureMatcher::prepareInterface() {
	// Register data streams, events and event handlers HERE!
	registerStream("in_models_descriptors", &in_models_descriptors);
	registerStream("in_scene_descriptors", &in_scene_descriptors);
	registerStream("in_models_imgs", &in_models_imgs);
	registerStream("in_models_keypoints", &in_models_keypoints);
	registerStream("in_scene_img", &in_scene_img);
	registerStream("in_scene_keypoints", &in_scene_keypoints);
	registerStream("out_img_all_correspondences", &out_img_all_correspondences);
	registerStream("out_matches", &out_matches);
	// Register handlers
	registerHandler("onNewImage", boost::bind(&FeatureMatcher::onNewImage, this));
	addDependency("onNewImage", &in_models_imgs);
	addDependency("onNewImage", &in_models_keypoints);
	addDependency("onNewImage", &in_scene_img);
	addDependency("onNewImage", &in_scene_keypoints);
	addDependency("onNewImage", &in_scene_descriptors);
	addDependency("onNewImage", &in_models_descriptors);

}

bool FeatureMatcher::onInit() {

	return true;
}

bool FeatureMatcher::onFinish() {
	return true;
}

bool FeatureMatcher::onStop() {
	return true;
}

bool FeatureMatcher::onStart() {
	return true;
}

void FeatureMatcher::onNewImage() {
}



} //: namespace FeatureMatcher
} //: namespace Processors
