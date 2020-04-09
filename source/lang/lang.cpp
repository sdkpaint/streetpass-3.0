#include "config.hpp"
#include "lang.hpp"

#include <stdio.h>
#include <unistd.h>

nlohmann::json appJson;

std::string Lang::get(const std::string &key) {
	if(!appJson.contains(key)) {
		return "MISSING: " + key;
	}
	return appJson.at(key).get_ref<const std::string&>();
}

std::string langs[] = {"br", "da", "de", "en", "es", "fr", "it", "lt", "pt", "ru", "jp"};

void Lang::load(const std::string lang) {
	FILE* values;
	if (Config::LangPath == 1) {
		// Check if exist.
		if(access("sdmc:/3ds/Universal-Updater/app.json", F_OK) == 0 ) {
			values = fopen(("sdmc:/3ds/Universal-Updater/app.json"), "rt");
			appJson = nlohmann::json::parse(values, nullptr, false);
			fclose(values);
			return;

		} else {
			values = fopen(("romfs:/lang/en/app.json"), "rt");
			appJson = nlohmann::json::parse(values, nullptr, false);
			fclose(values);
			return;
		}

	} else {
		// Check if exist.
		if(access(("romfs:/lang/" + lang + "/app.json").c_str(), F_OK) == 0 ) {
			values = fopen(std::string(("romfs:/lang/" + lang + "/app.json")).c_str(), "rt");
			appJson = nlohmann::json::parse(values, nullptr, false);
			fclose(values);
			return;

		} else {
			values = fopen(("romfs:/lang/en/app.json"), "rt");
			appJson = nlohmann::json::parse(values, nullptr, false);
			fclose(values);
			return;
		}
	}
}