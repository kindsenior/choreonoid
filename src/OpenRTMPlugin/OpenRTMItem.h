/*!
 * @brief  This is a definition of Items for OpenRTM-aist.
 * @file
 */
#ifndef CNOID_OPENRTM_PLUGIN_OPENRTM_ITEM_H
#define CNOID_OPENRTM_PLUGIN_OPENRTM_ITEM_H

#include <string>
#include <memory>
#include <cnoid/CorbaUtil>
#include <rtm/idl/RTC.hh>
#include <rtm/CORBA_IORUtil.h>

namespace cnoid {

class ConfigurationSetParam;
typedef std::shared_ptr<ConfigurationSetParam> ConfigurationSetParamPtr;

enum RTC_STATUS { RTC_UNKNOWN, RTC_INACTIVE, RTC_ACTIVE, RTC_ERROR, RTC_FINALIZE };

class RTCWrapper {
public:
	RTCWrapper() : rtc_(0), ownedExeContList_(0){};
	~RTCWrapper() {};

	RTC::RTObject_ptr rtc_;
	RTC::ExecutionContextList_var ownedExeContList_;
	SDOPackage::Configuration_ptr configuration_;

	RTC_STATUS getRTCState();

	inline std::string getInstanceName() { return compProfile_->instance_name; }
	inline std::string getIOR() const { return this->ior_; }
	inline void setIOR(std::string value) { this->ior_ = value; }

	void setLocation(const std::string& host, int port) {
		ncHelper_.setLocation(host, port);
	};

	bool activateComponent();
	bool deactivateComponent();
	bool resetComponent();
	bool finalizeComponent();
	bool startExecutionContext();
	bool stopExecutionContext();

	bool getConfiguration(NamingContextHelper::ObjectInfo& target, std::vector<ConfigurationSetParamPtr>& configSetList);
	void updateConfiguration(std::vector<ConfigurationSetParamPtr>& configList);

protected:
	void setRTObject(RTC::RTObject_ptr target);

private:
	std::string ior_;
	RTC::ComponentProfile_var compProfile_;
	NamingContextHelper ncHelper_;

};
typedef std::shared_ptr<RTCWrapper> RTCWrapperPtr;

}
#endif
