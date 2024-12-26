#ifndef ENTITY_DATA_EXTRACTOR_EXTRACTOR_H
#define ENTITY_DATA_EXTRACTOR_EXTRACTOR_H

#include "stat-tracker.h"
#include "struct-definitions.h"
#include <json/json.h>

class entityDataExtractor {
	public:
		entityData* extractEntityData(std::string);
		void printStats();
		void printStats(std::string);
		
	private:
		statTracker tracker;
		enumTranslate translator;
	
		std::list<animation>* getAnimations(Json::Value);
		void printData(std::list<animation>*);

		std::list<keyframe*>* getKeyframes(Json::Value);
		keyframe* extractKeyframeByType(Json::Value);
		void printData(std::list<keyframe*>*);
		void printData(keyframe*);

		std::list<layer*>* getLayers(Json::Value);
		layer* extractLayerByType(Json::Value);
		void printData(std::list<layer*>*);
		void printData(layer*);

		paletteMap* getPaletteMap(Json::Value);
		void printData(paletteMap*);

		std::list<std::string>* getPlugins(Json::Value);

		std::list<pluginMetadata*>* getPluginMetadata(Json::Value, std::list<std::string>*);
		pluginMetadata* extractPluginMetadataByType(Json::Value);

		std::list<symbol*>* getSymbols(Json::Value);
		symbol* extractSymbolByType(Json::Value);
		void printData(std::list<symbol*>*);
		void printData(symbol*);

		std::list<std::string> getTags(Json::Value);

		void printTitle(std::string);
};

#endif